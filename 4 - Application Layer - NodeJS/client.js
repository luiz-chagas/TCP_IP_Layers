var net = require('net');

var host = '127.0.0.1';
var port = 10000;
var client = new net.Socket();


client.connect(port, host, function() {
    console.log('Connected to: ' + host + ':' + port + "\n\n");
    msg = "GET / HTTP/1.1\r\n"
        + "Host: " + host + ":" + port + "\r\n"
        + "Accept: */*\r\n"
        + "Content-Type: application/x-www-form-urlencoded\r\n"
        + "Content-Length: 0\r\n"
        + "User-Agent: Grupo 3 (Luiz, Larissa, Raphael)\r\n\r\n";
    client.write(msg);
});

client.on('data', function(data) {
    console.log(data.toString());
});

client.on('close', function() {
    console.log('\n\nConnection closed');
});
