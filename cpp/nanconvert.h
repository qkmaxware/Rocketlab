#ifndef _ROCKETCORE_NANCONVERT_H
#define _ROCKETCORE_NANCONVERT_H

#include <nan.h>
#include <iostream>
#include <map>
#include <vector>

namespace js {

enum jstype { 
    js_null,
    js_number,
    js_boolean,
    js_string,
    js_object,
    js_array 
};

struct jsvalue {
    jstype type;
    bool nullOrUndefined;
    double number;
    bool boolean;
    std::string string;
    std::map<std::string, jsvalue> object;
    std::vector<jsvalue> array;
};

jsvalue decodeV8(v8::Local<v8::Value> value);

namespace encoder {

v8::Local<v8::Primitive> encodeNullOrUndefined(jsvalue value);

v8::Local<v8::Number> encodeNumber(jsvalue value);

v8::Local<v8::Boolean> encodeBoolean(jsvalue value);

v8::Local<v8::String> encodeString(jsvalue value);

v8::Local<v8::Array> encodeArray(jsvalue value);

v8::Local<v8::Object> encodeObject(jsvalue value);

}

v8::Local<v8::Value> encodeV8(jsvalue value);

}

#endif