var net = require('net');

var host = '127.0.0.1';
var port = 10001;
var HOST = '127.0.0.1';
var PORT = 11001;
var server = new net.Socket();
var sock;

var msg = "GET / HTTP/1.1\r\n"
         + "Host: " + host + ":" + port + "\r\n"
         + "User-Agent: Grupo 3 (Luiz, Larissa, Raphael)\r\n\r\n";

net.createServer(function(sock) {
    console.log('Client connected: ' + sock.remoteAddress +':'+ sock.remotePort + "\n");
    // Handles user query
    sock.on('data', function(data) {

        console.log("[STATUS]Data received from application\n");
        //console.log(data.toString());
        console.log(msg.toString());

        //Pass data to the server
        server.connect(PORT,HOST,function(){
            server.write(msg);
            console.log("[STATUS]Data sent to server\n");
        });

        //Receives data from the server
        server.on('data',function(data){
            console.log("[STATUS]Data received from server\n");
            sock.write(data);
            console.log("[STATUS]Data sent back to application\n");
            sock.destroy();
        });
    });
    sock.on('error', function () {});
    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('[STATUS]Closed connection\n');
    });
}).listen(port, host);

console.log("[STATUS]Client listening on " + host + ":" + port + "\n");

// client.connect(port, host, function() {
//     console.log('\n[STATUS]Connected to: ' + host + ':' + port + "\n");
//     msg = "GET / HTTP/1.1\r\n"
//         + "Host: " + host + ":" + port + "\r\n"
//         + "Accept: */*\r\n"
//         + "Content-Type: application/x-www-form-urlencoded\r\n"
//         + "Content-Length: 0\r\n"
//         + "User-Agent: Grupo 3 (Luiz, Larissa, Raphael)\r\n\r\n";
//     client.write(msg);
//     console.log("\n[STATUS]Data sent\n");
// });
