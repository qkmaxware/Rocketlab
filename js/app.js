const electron = require('electron');
const path = require('path');
const url = require('url');
const $ = require("jquery");
const rocketcore = require('./../build/Release/rocketcore.node');

function Application(){

    var windows = {
        main: null
    };

    var pages = {
        main: "index.html"
    }

    this.showDebugMenu = function(){
        windows.main.webContents.openDevTools();
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
        electron.app.quit();
    }

    return this;
}

function Page (){

    this.drawLineGraph = function(config){
        var base = {
            el: document.getElementById("canvas"),
            xlabel: "x",
            xaxis: "x",
            ylabel: "y",
            yaxis: "y",
            data: [],
            title: "",
            xgap: 25,
            ygap: 25,
            lineColour: "#ffffff",//f442b0
            lineWidth: 1
        };
        $.extend(base, config);
    
        if(base.el == null)
            return;
        
        var el = base.el;
        var ctx = el.getContext("2d");
    
        //Get data
        ctx.beginPath();
        ctx.fillStyle = 'black';
        if(base.data && base.data.length > 0){
            var data = base.data.slice();
            data.sort(function (a,b){
                if(a[base.xaxis] > b[base.xaxis]){
                    return 1;
                }
                if(a[base.xaxis] < b[base.xaxis]){
                    return -1;
                }
                return 0;
            });
            
            //Get max and min values
            var minx; var maxx; var miny; var maxy; var first = true;
            for(var i = 0; i < data.length; i++){
                var x = data[i][base.xaxis];
                var y = data[i][base.yaxis];
                if(first){
                    minx = x; maxx = x;
                    miny = y; maxy = y;
                    first = false;
                }
                
                if(x < minx)
                    minx = x;
                if(x > maxx)
                    maxx = x;
                if(y < miny)
                    miny = y;
                if(y > maxy)
                    maxy = y;
            }
            
            //Draw data
            var drawWidth = el.width - 25;
            var drawHeight = el.height - 25;
            first = true;
            for(var i = 0; i < data.length; i++){
                var x = data[i][base.xaxis];
                var y = data[i][base.yaxis];
        
                var screenX = drawWidth * ((x - minx) / (maxx - minx)) + 25;
                var screenY = drawHeight - drawHeight * ((y - miny) / (maxy - miny));
                if(first){
                    ctx.moveTo(screenX, screenY);
                    first = false;
                }else{
                    ctx.lineTo(screenX,screenY);
                }
            }
            ctx.strokeStyle = "#f442b0";
            ctx.lineWidth = 2;
        }
        ctx.stroke();
        ctx.closePath();

        //Axis lines
        ctx.beginPath();
        ctx.moveTo(0, el.height - 25);
        ctx.lineTo(el.width, el.height - 25);
        ctx.strokeStyle = "#000000";
        ctx.lineWidth = 1;
        ctx.stroke();
        ctx.closePath();

        ctx.beginPath(); 
        ctx.moveTo(25, 0);
        ctx.lineTo(25, el.height);
        ctx.stroke();
        ctx.closePath();

        //Axis text
        ctx.fillText(base.xlabel, el.width / 2, el.height - 6);
        for(var i = 0; i < drawWidth; i += base.xgap){
            var t = i / drawWidth;
            var lerp = Math.round(((1 - t) * minx + (t) * maxx) * 10) / 10;
            ctx.fillText(lerp + "", 25 + i, el.height - 15);
        }
        
        ctx.save();
        ctx.translate(10, el.height / 2);
        ctx.rotate(-Math.PI / 2);
        ctx.fillText(base.ylabel, 0, 0);
        ctx.restore();
        ctx.save();
        ctx.translate(18, el.height - 2);
        ctx.rotate(-Math.PI / 2);
        for(var i = 0; i < drawHeight; i += base.ygap){
            var t = i / drawHeight;
            var lerp = Math.round(((1 - t) * miny + (t) * maxy) * 10) / 10;
            ctx.fillText(lerp + "",  25 + i, 5);
        }
        ctx.restore();
        
        //Title text
        ctx.textAlign = 'center'
        ctx.fillText(base.title, el.width / 2 , 15);   
    };

    this.simulate = function(simdata){
        var output = rocketcore.simulate(simdata);
        showModal("Simulation complete", "File saved to: " + output, true);
    };

    this.browseFiles = function(filters){
        var filename = electron.remote.dialog.showOpenDialog({
            filters: filters,
            properties: ['openFile', 'multiSelections']
        });
        return filename;
    };

    this.openTab = function(tab){
        var x = document.getElementsByClassName("tab-pane");
        for (var i = 0; i < x.length; i++) {
            $(x[i]).hide();
        }
        $(document.getElementById(tab)).show(); 
    };

    return this;
}

module.exports = { app: Application, page: Page }