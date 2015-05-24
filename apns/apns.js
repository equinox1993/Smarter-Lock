var apn = require('apn');

var dev = 'f5ad0f90fd5a49f678a1fee89f0f87d31deeb22577759e8aeaf4a21d462d68af';

var options = {
    "cert": "../aps_cert.pem",
    "key": "../aps_key.pem",
    "gateway": "gateway.sandbox.push.apple.com",
    "port": 2195
};

apnConnection = new apn.Connection(options);
device = new apn.Device(dev);
note = new apn.Notification();

note.expiry = Math.floor(Date.now() / 1000) + 60;
note.badge = 3;
note.alert = 'test APNS ';
note.sound = 'default';
note.payload = {'messageFrom': 'Caroline'};
note.device = device;

apnConnection.pushNotification(note, device);
