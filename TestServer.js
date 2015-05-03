var net = require('net');

PACKET_HEAD_LENGTH = 16;

PACKET_OFFSET_SIGN = 0;
PACKET_OFFSET_TYPE = 4;
PACKET_OFFSET_SEQNUM = 8;
PACKET_OFFSET_PLEN = 12;
PACKET_OFFSET_PAYLOAD = 16;

function randSeq() {
    var set = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
    var str = '';
    for (var i = 0; i < 16; i++) {
        var rand = Math.floor(Math.random() * set.length);
        str += set[rand];
    }

    return str;
}
 
var server = net.createServer(function(socket) {
    console.log("Connected");

    socket.on('data', function(chunk) {
        // *** Just test. chuck != everything
        console.log('chunk');
        console.log(chunk);

        switch (chunk.readInt32BE(PACKET_OFFSET_TYPE)) {
            case 8: // UNLOCK
            case 11:
            case 12:
                chunk.writeInt32BE(1, PACKET_OFFSET_TYPE);
                console.log('Response: ');
                console.log(chunk);
                socket.write(chunk);
                break;
            case 9: // REQUEST_PASSCODE
                chunk.writeInt32BE(16, PACKET_OFFSET_TYPE); // PASSCODE
                chunk.writeInt32BE(24, PACKET_OFFSET_PLEN); // length 24
                passcode = randSeq();
                console.log("Passcode: "+passcode);
                pcBuf = new Buffer(passcode.split('').map(function (e) {
                    return e.charCodeAt(0);
                }));
                // date = (new Date()).getTime();
                dateBuf = new Buffer([0,0,0,0,0,7,0,0]);
                // dateBuf.writeInt32BE(0);
                // dateBuf.writeInt32BE(date, 4);
                
                buf = Buffer.concat([chunk, pcBuf, dateBuf]);
                // assert(buf.length == 36);

                console.log('Response: ');
                console.log(buf);
                socket.write(buf);
                break;
        }
    });
}).listen(2333);