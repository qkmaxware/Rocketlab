#include "nanconvert.h"

jsvalue parseV8(v8::Local<v8::Value> value){
    if(value->IsNull() || value->IsUndefined()){
        jsvalue r;
        r.nullOrUndefined = true;
        r.number = 0;
        r.boolean = false;
        return r;
    }
    if(value->IsBoolean()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = 1;
        r.boolean = value->BooleanValue();
    }
    if(value->IsNumber()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = value->NumberValue();
        r.boolean = r.number != 0 ? true : false;
        return r;
    }
    if(value->IsString()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = 0;
        r.boolean = true;
        r.string = std::string(*Nan::Utf8String(value));
        return r;
    }
    if(value->IsArray()){
        v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(value);
        int arrayLength = jsArray->Length();
        std::vector<jsvalue> values;

        for (int i = 0; i < arrayLength; i++){
            values.push_back(parseV8(jsArray->Get(i)));
        }

        jsvalue r;
        r.nullOrUndefined = false;
        r.number = arrayLength;
        r.boolean = true;
        r.array = values;
        return r;
    }
    if(value->IsObject()){
        v8::Local<v8::Object> obj = Nan::To<v8::Object>(value).ToLocalChecked();
        v8::Local<v8::Array> properties = obj->GetOwnPropertyNames();
        jsvalue r;
        for (unsigned int i = 0; i < properties->Length(); ++i) {
            v8::Local<v8::Value> obj_key = properties->Get(i);
            v8::Local<v8::Value> obj_value = obj->Get(obj_key);

            if (obj_key->IsString()) {
                std::string key = std::string(*Nan::Utf8String(obj_key));
                r.object[key] = parseV8(obj_value);
            } else {
                // Ignore it
            }
        }
        return r;
    }

    //Default catch all case
    jsvalue empty;
    empty.nullOrUndefined = true;
    empty.number = 0;
    empty.boolean = false;
    return empty;
} 