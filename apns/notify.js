var apn = require('apn');

var options = {
    "cert": "../aps_cert.pem",
    "key": "../aps_key.pem",
    "gateway": "gateway.sandbox.push.apple.com",
    "port": 2195
};

apnConnection = new apn.Connection(options);

module.exports = function(token) {
    var device = new apn.Device(token);
    var note = new apn.Notification();

    note.expiry = Math.floor(Date.now() / 1000) + 60;
    note.badge = 1;
    note.alert = 'Somebody has ringed your doorbell!';
    note.sound = 'default';
    note.payload = {};
    note.device = device;

    apnConnection.pushNotification(note, device);
};
