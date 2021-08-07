#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#include <vector>
#include <string>
#include "MessageStream.h"

using namespace std;

class CommandParser {
public:


	string parse(long lparam, string command);

	/*
	 * Discovered on web by Brian Rodenborn at
	 * http://groups.google.com/group/comp.lang.c++/msg/e19335f1c396d288
	 */
	static vector<string> stringSplit(string str, string delim)
	{
	   vector<string> returnVector;
	   string::size_type start = 0;
	   string::size_type end = 0;

	   while ((end=str.find (delim, start)) != string::npos)
	   {
		  returnVector.push_back (str.substr (start, end-start));
		  start = end+delim.size();
	   }

	   returnVector.push_back (str.substr (start));

	   return returnVector;
	}

	static string uCase(string inString)
	{
		for (unsigned int j = 0; j < inString.length(); j++)
		{
			inString[j]=toupper(inString[j]);
		}
		return inString;
	}

	static bool doubleFromString(string token, double &result) {
		if (token.empty()) return false;

		MessageStream strm;
		strm << token;

		return ! (strm >> result).fail();
	}

	static bool intFromString(string token, int &result) {
		if (token.empty()) return false;

		MessageStream strm;
		strm << token;

		return ! (strm >> result).fail();
	}

	static bool boolFromString(string token, bool &result) {
		if (token.empty()) return false;

		result = "1" == token || "Y" == token || "T" == token;

		return true;
	}

	static string boolToString(bool value) {
		return value ? "1" : "0";
	}
};

#endif