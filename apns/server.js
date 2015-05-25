var net = require('net');

const RSA_FILE = './apnserver_private.pem';

var notify = require('./notify');

var glue = require('./glue');

var filenameBuf = new Buffer(RSA_FILE);
if (!glue.setRSA(filenameBuf, 0)) {
    console.log("Error: Unable to open RSA file.");
    process.exit(1);
}

console.log("RSA loaded");

var server = net.createServer(function(socket) {
    console.log("Connected");

    var buffer = new Buffer(0);

    socket.on('data', function(chunk) {
        console.log('Chunk');
        buffer = Buffer.concat([buffer, chunk]);

        var len = glue.getLength(buffer, buffer.length);

        if (len > 0) {
            // full packet

            var tokBuffer = new Buffer(64);

            if (glue.getToken(buffer, tokBuffer) == 1) {

                var token = tokBuffer.toString();

                console.log('Sending token to '+token+' ...');
                notify(token);
                console.log('Done\n');
            }

            buffer = buffer.slice(len);
        }
    }).on('end', function() {
        console.log('Client ended.');
    });
}).listen(2303);

console.log("Server started");