const fs = require('fs');

function model(){
    this.vertices = [];
    this.normals = [];
    this.triangles = [];

    return this;
}

function engine(){
    this.name = null;
    this.diameter = null;
    this.length = null;
    this.delays = [];
    this.propellantWeight = 0;
    this.totalWeight = 0;
    this.manufacturer = null;

    this.datapoints = [];

    var self = this;

    this.parseEnv = function(filename, callback, error){
        function readFile(fname, callback, error){
            fs.readFile(fname, function (err, data) {
                if (err && error) {
                    error(err); 
                }
                if(callback)
                    callback(data);
            });
        };

        readFile(filename, 
            function(data){
                var d = data.toString();
                var lines = d.split('\n');
                var header = true;
                var e = self;
                for(var i = 0; i < lines.length; i++){
                    var line = lines[i].trim();
                    //Skip blank or comment lines
                    if(line.length == 0 || line.startsWith(';'))
                        continue;
                    //Dataline
                    if(header){
                        var head = line.split(" ");
                        e.name = head[0];
                        e.diameter = head[1];
                        e.length = head[2];
                        e.delays = head[3].split('-');
                        e.propellantWeight = head[4];
                        e.totalWeight = head[5];
                        e.manufacturer = head[6];
                        header = false;
                    }else{
                        var point = line.split(" ");
                        e.datapoints.push({time: parseFloat(point[0]), thrust: parseFloat(point[1])});
                    }
                }
    
                if(callback)
                    callback(e);
            },
            function(err){
                if(error)
                    error(err);
            }
        );
    };

    return this;
}

function stage(){
    
    this.body = new body();
    this.fins = null;
    this.engine = new engine();
    this.connector = null;
    this.nose = null;

    return this;
}

function rocket(){

    this.stages = [];

    this.newStage = function(){
        this.stages.push(new stage());
    }

    return this;
}

function fins(){

    return fins;
}

function body(){

    this.thickness = 0.1;
    this.massDensity = 0;
    this.upperRadius = 1;
    this.lowerRadius = 1;
    this.length = 1;
    var self = this;

    this.getOuterVolume = function(){
        var lr = this.lowerRadius;
        var ur = this.upperRadius;
        return (1/3) * Math.PI * (lr*lr + lr*ur + ur*ur) * this.length;
    }

    this.getInnerVolume = function(){
        var lr = this.lowerRadius - this.thickness;
        var ur = this.upperRadius - this.thickness;
        return (1/3) * Math.PI * (lr*lr + lr*ur + ur*ur) * this.length;
    }

    this.getVolume = function(){
        return this.getOuterVolume() - this.getInnerVolume();
    }

    this.getWeight = function(){
        return this.massDensity * getVolume();
    }

    this.getShape = function(){
        return {
            line: [
                {x: -self.lowerRadius, y: 0},
                {x: self.lowerRadius, y: 0},
                {x: self.lowerRadius, y: self.length},
                {x: -self.lowerRadius, y: self.length},
            ],
            closeLine: true
        }
    }

}

module.exports = { 
    engine: engine, 
    model: model, 
    body: body,
    rocket: rocket
}