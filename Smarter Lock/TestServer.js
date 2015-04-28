var net = require('net');
 
var server = net.createServer(function(socket) {
    console.log("Connected");

    socket.on('data', function(chunk) {
        // Just test. chuck != everything
        console.log('chunk');
        console.log(chunk);
        chunk[3] = 1;
        console.log('Response: ');
        console.log(chunk);
        socket.write(chunk);
    });
}).listen(2333);