const fs = require('fs');

function readFile(fname, callback, error){
    fs.readFile(fname, function (err, data) {
        if (err && error) {
            error(err); 
        }
        if(callback)
            callback(data);
    });
};

function eng(){
    this.name = null;
    this.diameter = null;
    this.length = null;
    this.delays = [];
    this.propellantWeight = 0;
    this.totalWeight = 0;
    this.manufacturer = null;

    this.datapoints = [];
    return this;
}

function parseEnv(filename, callback, error){
    readFile(filename, 
        function(data){
            var d = data.toString();
            var lines = d.split('\n');
            var header = true;
            var e = new eng()
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
}

module.exports = { eng: eng, parseEnv: parseEnv }