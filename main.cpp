#include <phpcpp.h>
#include "simdjson/singleheader/simdjson.h"
#include "simdjson/singleheader/simdjson.cpp"

Php::Value simdjson_isvalid(Php::Parameters &params)
{
    const char *p = params[0];
    ParsedJson pj = build_parsed_json(p);
    return Php::Value(pj.isValid());
}

Php::Value makeArray(ParsedJson::iterator & pjh, bool assoc)
{
    if (pjh.is_object()){
		Php::Value obj;
		if(assoc){
			obj = Php::Array();
		} else {
			obj = Php::Object();
		}
        if (pjh.down()) {
			const char *key = pjh.get_string();
			pjh.next();
			obj[key] = makeArray(pjh, assoc);
			while(pjh.next()) {
				key = pjh.get_string();
				pjh.next();
				obj[key] = makeArray(pjh, assoc);
			}
			pjh.up();
		}
		return obj;
	} else if (pjh.is_array()) {
		Php::Array arr;
		if(pjh.down()) {
			arr[0] = makeArray(pjh, assoc);
			int i = 1;
			while(pjh.next()) {
				arr[i] = makeArray(pjh, assoc);
				i++;
			}
			pjh.up();
		}
		return arr;
	} else if(pjh.is_string()){
		return pjh.get_string();
	} else if(pjh.is_double()){
		return pjh.get_double();
	} else if(pjh.is_integer()){
		return pjh.get_integer();
	} else {
		switch (pjh.get_type()) {
			case 'n':
				return nullptr;
			case 't':
				return true;
			case 'f':
				return false;
		}
	}
}

Php::Value simdjson_decode(Php::Parameters &params)
{
    const char *p = params[0];
	bool assoc = false;
	if(params.size() > 1){
		assoc = params[1];
	}
    ParsedJson pj = build_parsed_json(p);
    if(!pj.isValid()){
        throw Php::Exception("json is not valid");
    }
    ParsedJson::iterator pjh(pj);
    return makeArray(pjh, assoc);
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("simdjson", "1.0");
        
        // @todo    add your own functions, classes, namespaces to the extension
        extension.add<simdjson_isvalid>("simdjson_isvalid",{ 
        Php::ByVal("json", Php::Type::String)
    });
     extension.add<simdjson_decode>("simdjson_decode",{
        Php::ByVal("json", Php::Type::String, true),
		Php::ByVal("assoc", Php::Type::Bool, false)
    });
     // return the extension
        return extension;
    }
}
