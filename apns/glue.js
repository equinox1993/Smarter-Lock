/**
 * Created by Edwin on 15/5/24.
 */

var ffi = require('ffi');

module.exports = ffi.Library('cppglue/build/libcppglue', {
    'setRSA': ['int', ['char*', 'int']],
    'getToken': ['int', ['char*', 'char*']],
    'getLength': ['int', ['char*', 'int']],
    'makePacket': ['int', ['char*', 'char*']]
});