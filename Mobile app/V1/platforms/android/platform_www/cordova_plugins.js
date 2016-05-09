cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/cordova-plugin-whitelist/whitelist.js",
        "id": "cordova-plugin-whitelist.whitelist",
        "pluginId": "cordova-plugin-whitelist",
        "runs": true
    },
    {
        "file": "plugins/com.appshed.ioioplugin/ioio.js",
        "id": "com.appshed.ioioplugin.IOIO",
        "pluginId": "com.appshed.ioioplugin",
        "clobbers": [
            "cordova.plugins.IOIO"
        ]
    },
    {
        "file": "plugins/cordova-plugin-bluetooth-serial/www/bluetoothSerial.js",
        "id": "cordova-plugin-bluetooth-serial.bluetoothSerial",
        "pluginId": "cordova-plugin-bluetooth-serial",
        "clobbers": [
            "window.bluetoothSerial"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.2.0",
    "com.appshed.ioioplugin": "0.0.3",
    "cordova-plugin-bluetooth-serial": "0.4.5"
}
// BOTTOM OF METADATA
});