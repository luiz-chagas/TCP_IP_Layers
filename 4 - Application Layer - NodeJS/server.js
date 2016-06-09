var net = require('net');
var fs = require('fs');

var HOST = '127.0.0.1';
var PORT = 8080;

// Create a server instance, and chain the listen function to it
// The function passed to net.createServer() becomes the event handler for the 'connection' event
// The sock object the callback function receives UNIQUE for each connection
net.createServer(function(sock) {

    var requestedFile;
    var header;
    var body;

    // We have a connection - a socket object is assigned to the connection automatically
    console.log('Client connected: ' + sock.remoteAddress +':'+ sock.remotePort + "\n\n");

    // Add a 'data' event handler to this instance of socket
    sock.on('data', function(data) {

        console.log(data.toString());

        header = "HTTP/1.1 200 OK\r\n"
            + "Content-Type: text/html;charset=UTF-8\r\n"
            + "\r\n";

        body = fs.readFileSync('index.html', 'utf8');

        // Write data back to the socket
        sock.write(header + body);
        sock.destroy();
    });

    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('Closed connection\n\n');
    });

}).listen(PORT, HOST);

console.log('Server listening on ' + HOST +':'+ PORT);
