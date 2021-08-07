#include "mongoose.h"
#include "OrbConnect.h"
#include <stdio.h>

#define MAX_LOG_BUFFER_SIZE 1024
#define Log(fmt, ...) \
    do {\
        char szBuf[MAX_LOG_BUFFER_SIZE], szBuf2[MAX_LOG_BUFFER_SIZE];\
        sprintf_s(szBuf, MAX_LOG_BUFFER_SIZE, fmt, __VA_ARGS__);\
		sprintf_s(szBuf2, MAX_LOG_BUFFER_SIZE, "OrbConnect::%s:%4d Thread %5d %s", __FILE__, __LINE__, GetCurrentThreadId(), szBuf);\
        oapiWriteLog(szBuf2);\
    } while(0)


struct mg_server *server;

//
// NOTE:
// This code executes in the main Orbiter thread without performance loss, 
// because Mongoose core is non-blocking (asynchronous). 
//

void send_reply(struct mg_connection *conn, string reply)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Cache: no-cache\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
		"%s",
		reply.length(), reply.c_str() );
}

string do_query(char *query)
{
	SendText_Intercept = 1;
	parseCommand(0, query);
	SendText_Intercept = 0;
	return SendText_Buf;
}

static int handler(struct mg_connection *conn)
{
	if ( strcmp(conn->request_method, "GET") == 0 ) {
		mg_printf(conn, "HTTP/1.1 405 Method Not Allowed\r\n\r\n");
		return 1;
	}

	char cmd[65536];
	mg_get_var(conn, "cmd", cmd, sizeof(cmd));

	string reply;
	char *token, *next;
	token = strtok_s (cmd, "|", &next);
	while (token != NULL)
	{
		if (strlen(token) > 0)
			reply = reply + do_query(token) + "|";

		token = strtok_s(NULL, "|", &next);
	}

	send_reply(conn, reply);

	return 1; // finished handling client
}

void http_init(void)
{
	server = mg_create_server(NULL);
	mg_set_option(server, "listening_port", "38888");
	mg_set_option(server, "document_root", "www");
	mg_set_option(server, "error_log_file", "http_error.log");
	mg_add_uri_handler(server, "/cgi", handler);
	//Log("Starting on port %s\n", mg_get_option(server, "listening_port"));
}

void http_postStep(void)
{
	mg_poll_server(server, 0);

}

void http_finish(void)
{
	mg_destroy_server(&server);
}
