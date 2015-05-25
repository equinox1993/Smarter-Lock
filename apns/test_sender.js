/**
 * Created by Edwin on 15/5/24.
 */


const TEST_DEVICE = 'f5ad0f90fd5a49f678a1fee89f0f87d31deeb22577759e8aeaf4a21d462d68af';
const RSA_FILE = '../apnserver_public.pem';

var glue = require('./glue');
var net = require('net');

var rsaFileBuf = new Buffer(RSA_FILE);
if (!glue.setRSA(rsaFileBuf, 1)) {
    console.log("Error: Unable to open RSA file.");
    process.exit(1);
}
console.log("RSA loaded");

var client = net.connect({port: 2303, host: 'localhost'}, function () {
    console.log('Connected to server!');
    var devBuf = new Buffer(TEST_DEVICE);
    var outBuf = new Buffer(256);

    var packetLen = glue.makePacket(devBuf, outBuf);

    outBuf = outBuf.slice(0, packetLen);

    console.log(outBuf);
    client.write(outBuf);

    client.end();
}).on('end', function() {
    console.log('end');
});