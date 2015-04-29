var net = require('net');

function randSeq() {
    var set = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
    var str = '';
    for (var i = 0; i < 16; i++) {
        var rand = Math.floor(Math.random() * set.length);
        str += set[rand];
    };

    return str;
}
 
var server = net.createServer(function(socket) {
    console.log("Connected");

    socket.on('data', function(chunk) {
        // Just test. chuck != everything
        console.log('chunk');
        console.log(chunk);

        switch (chunk[3]) {
            case 8:
                chunk[3] = 1;
                console.log('Response: ');
                console.log(chunk);
                socket.write(chunk);
                break;
            case 9:
                chunk[3] = 16;
                chunk[11] = 24;
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
        }
    });
}).listen(2333);