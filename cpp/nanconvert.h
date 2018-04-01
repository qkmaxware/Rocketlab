#ifndef _ROCKETCORE_NANCONVERT_H
#define _ROCKETCORE_NANCONVERT_H

#include <nan.h>
#include <iostream>
#include <map>
#include <vector>

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

jsvalue parseV8(v8::Local<v8::Value> value);
 

#endif