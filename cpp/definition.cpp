#include <nan.h>
#include "simulator.h"
#include <vector> 
#include "nanconvert.h"

void invokeSimulation(const Nan::FunctionCallbackInfo<v8::Value>& info){
    //Confirm inputs
    if(info.Length() < 1){
        Nan::ThrowTypeError("Wrong number of arguments. Required (1).");
        return;
    }
        
    if(!(info[0]->IsObject())){
        Nan::ThrowTypeError("Argument 0 must be an object");
        return;
    }

    //Convert js input into c++ equivalents   
    jsvalue argvalue = parseV8(info[0]);

    simulator::atmosphere atmo;
    atmo.surfaceDensity = 1.225;
    atmo.atmosphericRadius = 700;

    simulator::planet earth;
    earth.surfaceAcceleration = 9.8;
    earth.surfaceRadius = 6371000;
    earth.atmo = atmo;
    
    simulator::thrustcurve curve;
    std::vector<jsvalue> decoded_array = argvalue.object["motor"].object["thrustcurve"].array;

    double* time = new double[decoded_array.size()];
    double* thrust = new double[decoded_array.size()];

    for(int i = 0; i < decoded_array.size(); i++){
        double t = decoded_array[i].object["time"].number;
        double f = decoded_array[i].object["thrust"].number;
        time[i] = t;
        thrust[i] = f;
    }
    curve.points = decoded_array.size();
    curve.time = time;
    curve.thrust = thrust;
    
    simulator::engine motor;
    motor.totalMass = argvalue.object["motor"].object["totalWeight"].number;
    motor.propellantMass = argvalue.object["motor"].object["propellantWeight"].number;
    motor.ignitionDelay = argvalue.object["motor"].object["delay"].number;
    motor.thrust = curve;
    
    simulator::rocket rkt;
    rkt.rocketMass = argvalue.object["rocket"].object["mass"].number;
    rkt.motor = motor;
    
    double timestep = argvalue.object["quality"].object["timestep"].number;
    
    //Call fn
    std::vector<simulator::timeslice> timeline;
    simulator::simulate(timestep, rkt, earth, timeline);

    //Create output

    //Cleanup and set output
    delete time;
    delete thrust;
    info.GetReturnValue().Set(Nan::New((int)timeline.size()));
}

void init(v8::Local<v8::Object> exports) {
    //Tell JS about my function(s)
    exports->Set(
        Nan::New("simulate").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(invokeSimulation)->GetFunction()
    );
}

//Init the Nodejs module
NODE_MODULE(rocketcore, init)