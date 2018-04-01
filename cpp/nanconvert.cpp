#include "nanconvert.h"

jsvalue parseV8(v8::Local<v8::Value> value){
    if(value->IsNull() || value->IsUndefined()){
        jsvalue r;
        r.nullOrUndefined = true;
        r.number = 0;
        r.boolean = false;
        r.type = jstype::js_null;
        return r;
    }
    if(value->IsBoolean()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = 1;
        r.boolean = value->BooleanValue();
        r.type = jstype::js_boolean;
        return r;
    }
    if(value->IsNumber()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = value->NumberValue();
        r.boolean = r.number != 0 ? true : false;
        r.type = jstype::js_number;
        return r;
    }
    if(value->IsString()){
        jsvalue r;
        r.nullOrUndefined = false;
        r.number = 0;
        r.boolean = true;
        r.string = std::string(*Nan::Utf8String(value));
        r.type = jstype::js_string;
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
        r.type = jstype::js_array;
        return r;
    }
    if(value->IsObject()){
        v8::Local<v8::Object> obj = Nan::To<v8::Object>(value).ToLocalChecked();
        v8::Local<v8::Array> properties = obj->GetOwnPropertyNames();
        jsvalue r;
        r.number = properties->Length();
        r.boolean = true;
        r.nullOrUndefined = false;
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
        r.type = jstype::js_object;
        return r;
    }

    //Default catch all case
    jsvalue empty;
    empty.nullOrUndefined = true;
    empty.number = 0;
    empty.boolean = false;
    empty.type = jstype::js_null;
    return empty;
} 