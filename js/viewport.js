function viewport(canvas){
    var ctx = canvas.getContext('2d');

    var centerX = 0;
    var centerY = 0;
    var scale = 10; //100 pixels to 1m
    var parts = [];

    this.setCenter = function(x, y){
        centerX = x;
        centerY = y;
    }
    this.getCenter = function(){
        return { x: centerX, y: centerY };
    }

    this.setScale = function(s){
        scale = s;
    }
    this.getScale = function(){
        return scale;
    }

    this.addPart = function(part){
        parts.push(part);
    }
    this.clearParts = function(){
        parts = [];
    }

    
    //Redraw viewport
    function redraw(){
        //Clear canvas
        ctx.fillStyle = "#000000";
        ctx.fillRect(0,0,canvas.width,canvas.height);

        //Draw model
        ctx.beginPath();
        ctx.strokeStyle = "#eacf54";
        ctx.lineWidth = 1;
        {
            for(var i = 0; i < parts.length; i++){
                var part = parts[i];
                if(!part.getShape){
                    console.log("Invalid part");
                    continue;
                }
                
                var shape = part.getShape();
                for(var j = 0; j < shape.line.length; j++){
                    //Get data
                    var point = shape.line[j];
                    var x = point.x;
                    var y = point.y;

                    //Get canvas position
                    var cx = canvas.width/2 + (x - centerX) * scale;
                    var cy = canvas.height/2 + (y - centerY) * scale;

                    //Draw
                    if(j == 0){
                        ctx.moveTo(cx, cy);
                    }else{
                        ctx.lineTo(cx, cy);
                    }
                }
                ctx.stroke();
            }
        }
        ctx.stroke();
        ctx.closePath();

        //Draw axis
        ctx.fillStyle = "#FFFFFF";
        ctx.strokeStyle = "#FFFFFF";
        ctx.lineWidth = 1;
        var x = 10; var y = canvas.height - 20;
        ctx.rect(x,y,10,10);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(x + 5, y + 5);
        ctx.lineTo(x + 5, y - 20);
        ctx.moveTo(x + 5, y + 5);
        ctx.lineTo(x + 30, y + 5);
        ctx.fillText("Y",x, y - 25);
        ctx.fillText("X",x +35, y + 10);
        ctx.closePath();
        ctx.stroke();
    }

    //Create resize function
    function resizeCanvas(){
        var viewport = document.getElementById("viewport");

        canvas.style.width ='100%';
        canvas.style.height='100%';

        canvas.width  = canvas.offsetWidth;
        canvas.height = canvas.offsetHeight;

        redraw();
    }
    resizeCanvas();

    //User callable repaint function
    this.repaint = function(){
        resizeCanvas();
    }

    //Register event listeners
    var mouseDown = false;
    window.addEventListener("mousedown", function() { 
        mouseDown = true;
    }, false);
    window.addEventListener("mouseup", function() {
        mouseDown = false;
    }, false);

    var mouseover = false;
    canvas.addEventListener("mouseover", function() {
        mouseover = true;
    }, false);
    canvas.addEventListener("mouseout", function() {
        mouseover = false;
    }, false);

    window.addEventListener('resize', resizeCanvas, false);
    window.addEventListener('mousemove', function(pos){
        var mx = pos.movementX;
        var my = pos.movementY;

        if(mouseDown && mouseover){
            centerX += -mx / scale;
            centerY += -my / scale;
            redraw();
        }
    }, false);
    return this;
}

module.exports = {
    viewport: viewport
};