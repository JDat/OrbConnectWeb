Orb::Connect Web Edition

Bringing Orbiter into the Web era!

Launch checklist:

1. Unpack the attached add-on
2. Enable OrbConnect.dll module.
3. Launch the simulation.
4. Open http://127.0.0.1:38888/ in a Web browser.
5. Try clicking around the panel

FAQ

Q: Why??? 
A: Because. Seriously, you don't see how this can be useful?

Q: What browser does it work with? 
A: I have tested with Firefox. Any modern browser should work -- 
the client-side code is not abusive in any way.

Q: Orbiter hangs when I use RCS controls. 
A: Try disabling OrbiterSound. 

Q: Firefox hangs when clicking the engine controls. 
A: It will unhang after a few seconds. 

Q: Where can I report bugs?
A: Please use this thread: http://orbiter-forum.com/showthread.php?t=33144

Q: I don't like layout of the controls! 
A: No problem, just edit www\index.html to suit your needs.

Q: I want to add more controls or make my own panel. 
A: Have a look at www\example.html to see how this is done.

Q: Does it work over the network? 
A: Of course. Just make sure that your firewall is not blocking 
incoming connections to TCP port 38888.

Q: Does it work with my phone/tablet? 
A: It does with my Android 4.0 tablet. 

Q: Can I have multiple clients? 
A: Of course.

Q: Can I have multiple panels? 
A: Sure. Just make several .html files and use browser tabs or something.

Q: Can this be used to control X? 
A: If X is accessible via Orb::Connect, then yes. See 
Doc\OrbConnect\OrbConnectUserGuide-2.1.pdf.

Q: My simpit already uses Orb::Connect server! I'm not going to rewrite 
everything to run over HTTP! 
A: There is no need. The Orb::Connect server operates the same as in 
stock version, on port 37777.

Q: How does it work? 
A: I have added an HTTP server to Orb::Connect. This server has two 
functions. First, it serves files located under "www" directory. 
Second, if the client sends a POST request to /cgi with Orb::Connect 
commands, these commands are passed to Orb::Connect core, and the result
is returned to the client. On the client side, there is a JavaScript 
library which handles communication with the server.

Q: Can you put it in normal terms? 
A: There is a web server in Orbiter. Your browser connect to the web 
server, and loads the HTML file with the panel layout. The JavaScript 
code in that file handles updates.

Q: Are you saying that I can control spacecraft using a web browser? 
A: Yes!

Q: What is the update frequency? 
A: It is set on the client, with the parameter passed to 
OrbiterClient::start(). The included examples use 5Hz.

Q: What is the impact on frame rate? 
A: I have not observed any. It should be very small, because the HTTP 
server is written in a non-blocking way.

Q: Did you write the HTTP server yourself? 
A: No. I have used mongoose.

Q: What about license? 
A: GPL. 

Q: Where is the source code? 
A: Included in the download, under Orbitersdk directory.


