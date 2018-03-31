#ifndef _ASYNC_TPL
#define _ASYNC_TPL

#include <nan.h>

using namespace Nan;
using namespace v8;

class Worker : public AsyncWorker {
public:
    const Nan::FunctionCallbackInfo<v8::Value>& info;
    const Callback* callback;
    void (*fn)(Worker&);
    v8::Local<v8::Value> result;

    Worker(void (*fn)(Worker&), const Nan::FunctionCallbackInfo<v8::Value>& info, Callback* callback): fn(fn), info(info), callback(callback), AsyncWorker(callback) {
        result = Null();
    }

    void Execute () {
        //Call function with the info 
        fn(*this);
    }

    void HandleOKCallback () {
        Nan::HandleScope scope;

        Local<Value> argv[] = {
            Null(), //Error
            result  //Data
        };

        callback->Call(2, argv, async_resource);
    }

    void HandleErrorCallback() {
        Nan::HandleScope scope;

        //Get here with this->SetErrorMessage()
        v8::Local<v8::Value> argv[] = {
            Nan::New(this->ErrorMessage()).ToLocalChecked(), // return error message
            Nan::Null()
        };

        callback->Call(2, argv, async_resource);
    }

};

void runAsyncMethod(void (*fn)(Worker&),int callbackIdx, const Nan::FunctionCallbackInfo<v8::Value>& info){
    Callback* callback = new Callback(To<Function>(info[callbackIdx]).ToLocalChecked());

    AsyncQueueWorker(new Worker(fn, info, callback));
}

#endif
