const electron = require('electron');
const program = require("./js/app.js");

var app = new program.app();
global.app = app;

global.debug = true;

electron.app.on('window-all-closed', () => {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit();
    }
});
  
electron.app.on('activate', () => {
    app.showMainWindow();
});

electron.app.on('ready', function(){
    app.showMainWindow();
    if(global.debug)
        app.showDebugMenu();
});