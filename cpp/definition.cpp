#include <nan.h>
#include "async.h"
#include "simulator.h"


void runSimulation(Worker& asyncWorker){

}

void invokeSimulation(const Nan::FunctionCallbackInfo<v8::Value>& info){
    //Start the aysnc work on the runSimulation method, callback is at position 1
    //runAsyncMethod(runSimulation, 1, info);
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