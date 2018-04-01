#include "nanconvert.h"

namespace js {

jsvalue decodeV8(v8::Local<v8::Value> value){
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
            values.push_back(decodeV8(jsArray->Get(i)));
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
                r.object[key] = decodeV8(obj_value);
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

namespace encoder {

v8::Local<v8::Primitive> encodeNullOrUndefined(jsvalue value){
    return Nan::Null();
}

v8::Local<v8::Number> encodeNumber(jsvalue value){
    return Nan::New(value.number);
}

v8::Local<v8::Boolean> encodeBoolean(jsvalue value){
    return Nan::New(value.boolean);
}

v8::Local<v8::String> encodeString(jsvalue value){
    return Nan::New(value.string).ToLocalChecked();
}

v8::Local<v8::Array> encodeArray(jsvalue value){
    v8::Local<v8::Array> array = Nan::New<v8::Array>();

    for(int i = 0; i < value.array.size(); i++){
        switch(value.array[i].type){
            case jstype::js_array:
                Nan::Set(array, i, encodeArray(value.array[i]));
                break;
            case jstype::js_boolean:
                Nan::Set(array, i, encodeBoolean(value.array[i]));
                break;
            case jstype::js_number:
                Nan::Set(array, i, encodeNumber(value.array[i]));
                break;
            case jstype::js_string:
                Nan::Set(array, i, encodeString(value.array[i]));
                break;
            case jstype::js_object:
                Nan::Set(array, i, encodeObject(value.array[i]));
                break;
            default:
                Nan::Set(array, i, encodeNullOrUndefined(value.array[i]));
                break;
        }
        
    }

    return array;
}

v8::Local<v8::Object> encodeObject(jsvalue value){
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();

    for(std::map<std::string,jsvalue>::iterator iter = value.object.begin(); iter != value.object.end(); iter++){
        v8::Local<v8::String> k = Nan::New(iter->first).ToLocalChecked();
        jsvalue v = iter->second;

        switch(v.type){
            case jstype::js_array:
                Nan::Set(obj, k, encodeArray(v));
                break;
            case jstype::js_boolean:
                Nan::Set(obj, k, encodeBoolean(v));
                break;
            case jstype::js_number:
                Nan::Set(obj, k, encodeNumber(v));
                break;
            case jstype::js_string:
                Nan::Set(obj, k, encodeString(v));
                break;
            case jstype::js_object:
                Nan::Set(obj, k, encodeObject(v));
                break;
            default:
                Nan::Set(obj, k, encodeNullOrUndefined(v));
                break;
        }
    }

    return obj;
}

}

v8::Local<v8::Value> encodeV8(jsvalue value){
    switch(value.type){
        case jstype::js_array:
            return (encoder::encodeArray(value));
        case jstype::js_boolean:
            return (encoder::encodeBoolean(value));
        case jstype::js_number:
            return (encoder::encodeNumber(value));
        case jstype::js_string:
            return (encoder::encodeString(value));
        case jstype::js_object:
            return (encoder::encodeObject(value));
        default:
            return (encoder::encodeNullOrUndefined(value));
    }
}

}