#ifndef _JSONSchema_JSONSchema_h_
#define _JSONSchema_JSONSchema_h_

#include <Core/Core.h>

using namespace Upp;

class JsonSchemaChecker {
	void NumberFilters(Value schema, Value data);
	void StringFilters(Value schema, Value data);
	void ArrayFilters(Value schema, Value data, int depth);
	void ObjectFilters(Value schema, Value data, int depth);

	void Check(Value schema, Value data, int depth);
	void Check0(Value schema, Value data, int depth);
	
	void InvalidSchema();
	void Error(const String& error);

	struct Path {
		int sl, dl;
		JsonSchemaChecker *cr;
		
		Path(JsonSchemaChecker *ch) {
			cr = ch;
			sl = cr->schema_path.GetCount();
			dl = cr->data_path.GetCount();
		}
	
		Path(JsonSchemaChecker *ch, const char *s) : Path(ch) {
			cr->schema_path << s;
		}
		
		~Path() {
			cr->schema_path.Trim(sl);
			cr->data_path.Trim(dl);
		}
	};
	
	int logics;
	
	struct FailedBranch {};

public:
	VectorMap<String, Value> refs;
	Function<Value(String)>  Ref;
	Vector<Value>            schema_path;
	Vector<Value>            data_path;

	struct JsonSchemaError : Exc {
		JsonSchemaError(const char *e) : Exc(e) {}
	};
	
	Event<const String&> WhenError = [](const String& s) { throw JsonSchemaError(s); };

	void Validate(Value schema, Value data);
};

#endif
