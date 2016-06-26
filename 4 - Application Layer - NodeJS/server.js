var net = require('net');
var fs = require('fs');

var HOST = '127.0.0.1';
var PORT = 10000;

net.createServer(function(sock) {

    var requestedFile;
    var header;
    var body;

    // We have a connection - a socket object is assigned to the connection automatically
    console.log('Client connected: ' + sock.remoteAddress +':'+ sock.remotePort + "\n");

    // Add a 'data' event handler to this instance of socket
    sock.on('data', function(data) {

        console.log("[STATUS]Data received\n");
        console.log(data.toString());

        header = "HTTP/1.1 200 OK\r\n"
            + "Content-Type: text/html;charset=UTF-8\r\n"
            + "\r\n";

        body = fs.readFileSync('index.html', 'utf8');

        // Write data back to the socket
        sock.write(header + body);
        console.log("[STATUS]Data sent\n");
        sock.destroy();
    });

    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('[STATUS]Closed connection\n');
    });

}).listen(PORT, HOST);

console.log('[STATUS]Server listening on ' + HOST +':'+ PORT + "\n");
