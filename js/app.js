const electron = require('electron');
const path = require('path');
const url = require('url');
const $ = require("jquery");
//const rocketcore = require('./../build/Release/rocketcore.node');

function Application(){

    var windows = {
        main: null
    };

    var pages = {
        main: "index.html"
    }

    this.showDebugMenu = function(){
        windows.main.webContents.openDevTools()
    }

    this.showMainWindow = function(){
        if(windows.main == null){
            // Create the browser window.
            windows.main = new electron.BrowserWindow({width: 800, height: 600})
        
            // and load the index.html of the app.
            windows.main.loadURL(url.format({
                pathname: path.join(__dirname, "..", "pages", pages.main),
                protocol: 'file:',
                slashes: true
            }))
        
            // Emitted when the window is closed.
            windows.main.on('closed', () => {
                // Dereference the window object, usually you would store windows
                windows.main = null
            })
        };
    }

    this.quit = function(){

    }

    return this;
}

function Page (){

    this.browseFiles = function(){
        var filename = electron.remote.dialog.showOpenDialog({properties: ['openFile', 'multiSelections']});
        return filename;
    }

    this.openTab = function(tab){
        var x = document.getElementsByClassName("tab-pane");
        for (var i = 0; i < x.length; i++) {
            $(x[i]).hide();
        }
        $(document.getElementById(tab)).show(); 
    }

    return this;
}

module.exports = { app: Application, page: Page }