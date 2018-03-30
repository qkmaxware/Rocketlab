#include <nan.h>
#include "simulator.h"


void init(v8::Local<v8::Object> exports) {
    //Tell JS about my function(s)
    
}

//Init the Nodejs module
NODE_MODULE(rocketcore, init)