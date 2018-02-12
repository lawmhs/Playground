# going through tutorial nodebeginner.org

var http = require("http");

function onRequest(request, response) {
	response.writeHead(200, {"Content-Type": "text/plain"});
	response.write("Hello World");
	response.end();
	}
	
https.createServer(onRequest).listen(8888);
