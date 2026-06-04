#include "JsonValidator.h"

#include <plugin/pcre/Pcre.h>

void JsonSchemaChecker::InvalidSchema()
{
	Error("invalid schema");
}

void JsonSchemaChecker::Error(const String& error)
{
	if(logics)
		throw FailedBranch();
	else
		WhenError(error);
}

void JsonSchemaChecker::ArrayFilters(Value schema, Value data, int depth)
{
	int i = 0;
	Value pfx = schema["prefixItems"];
	for(; i < min(pfx.GetCount(), data.GetCount()); i++) {
		Path __(this, "prefixItems");
		schema_path << i;
		data_path << i;
		Check(pfx[i], data[i], depth + 1);
	}
	Value items = schema["items"];
	if(!items.IsVoid()) {
		if(items.Is<bool>()) {
			if(!(bool)items && i < data.GetCount())
				Error("additional array items are not allowed");
		}
		else {
			if(!IsValueMap(items))
				InvalidSchema();
			if(items.GetCount())
				for(; i < data.GetCount(); i++) {
					Path __(this, "items");
					data_path << i;
					Check(items, data[i], depth + 1);
				}
		}
	}
}

void JsonSchemaChecker::ObjectFilters(Value schema, Value data, int depth)
{
	ValueMap properties = schema["properties"];
	ValueMap patterns = schema["patternProperties"];
	if(properties.GetCount() || patterns.GetCount()) {
		if(!data.Is<ValueMap>())
			Error("object required");
		ValueMap object = data;
		Index<String> ids;
		Value additionalProperties = schema["additionalProperties"];
		if(!(IsVoid(additionalProperties) || additionalProperties.Is<ValueMap>() || additionalProperties.Is<bool>()))
			InvalidSchema();

		bool noAdditionalProperties = additionalProperties.Is<bool>() ? !(bool)additionalProperties : false;
		Array<RegExp> regexp;
		for(int i = 0; i < patterns.GetCount(); i++)
			regexp.Create(~patterns.GetKey(i));
		for(int i = 0; i < object.GetCount(); i++) {
			String id = ~object.GetKey(i);
			Value val = object.GetValue(i);
			bool found = false;
			for(int j = 0; j < properties.GetCount(); j++)
				if(properties.GetKey(j) == id) {
					Path __(this, "properties");
					data_path << id;
					schema_path << id;
					Check(properties.GetValue(j), val, depth + 1);
					found = true;
					break;
				}
			
			if(!found)
				for(int j = 0; j < patterns.GetCount(); j++)
					if(regexp[j].Match(id)) {
						Path __(this, "patternProperties");
						data_path << id;
						schema_path << ~patterns.GetKey(i);
						Check(patterns.GetValue(j), val, depth + 1);
						found = true;
						break;
					}
			
			if(!found) {
				if(noAdditionalProperties)
					Error("unknown property " + id);
				if(additionalProperties.Is<ValueMap>()) {
					Path __(this, "additionalProperties");
					data_path << id;
					Check(additionalProperties, val, depth + 1);
				}
			}
			ids.FindAdd(id);
		}
		for(Value v : schema["required"]) {
			String id = ~v;
			if(ids.Find(id) < 0)
				Error("required property " + id);
		}
		ValueMap m = schema["dependentRequired"];
		for(int i = 0; i < m.GetCount(); i++) {
			String id = ~m.GetKey(i);
			if(ids.Find(id) >= 0) {
				for(Value v : m.GetValue(i))
					if(ids.Find(~v) < 0)
						Error(String() << "required dependent property " << v << ", depends on " << id);
						
			}
		}
	}
}

void JsonSchemaChecker::NumberFilters(Value schema, Value data)
{
	double v = data;
	double m = schema["multipleOf"];
	if(!IsNull(m) && abs(roundf(v / m) * m - v) > abs(v) * 1e-14)
		Error("number must be multiple of " + AsString(m));
	m = schema["minimum"];
	if(!IsNull(m) && v < m)
		Error("number must be >= " + AsString(m));
	m = schema["exclusiveMinimum"];
	if(!IsNull(m) && v <= m)
		Error("number must be > " + AsString(m));
	m = schema["maximum"];
	if(!IsNull(m) && v > m)
		Error("number must be <= " + AsString(m));
	m = schema["exclusiveMaximum"];
	if(!IsNull(m) && v >= m)
		Error("number must be < " + AsString(m));
}

void JsonSchemaChecker::StringFilters(Value schema, Value data)
{
	WString s = data;
	int m = schema["minLength"];
	if(!IsNull(m) && s.GetLength() < m)
		Error("string must have at least " + AsString(m) + " characters");
	m = schema["maxLength"];
	if(!IsNull(m) && s.GetLength() > m)
		Error("string must have at most " + AsString(m) + " characters");
	String p = schema["pattern"];
	if(!IsNull(p)) {
		RegExp exp(p);
		if(!exp.Match(~data))
			Error("failed regexp pattern " + p);
	}
}

void JsonSchemaChecker::Check(Value schema, Value data, int depth)
{
	Value allOf = schema["allOf"];
	if(!allOf.IsVoid()) {
		if(!allOf.Is<ValueArray>())
			InvalidSchema();
		for(int i = 0; i < allOf.GetCount(); i++) {
			Path __(this, "allOf");
			schema_path << i;
			Check0(allOf[i], data, depth + 1);
		}
	}
	logics++;
	auto countOf = [&](Value m, int max, int def) {
		if(m.IsVoid())
			return def;
		if(!m.Is<ValueArray>())
			InvalidSchema();
		int count = 0;
		for(int i = 0; i < m.GetCount() && count < max; i++)
			try {
				Check0(m[i], data, depth + 1);
				count++;
			}
			catch(FailedBranch) {}
		return count;
	};
	if(countOf(schema["anyOf"], 1, 1) == 0) {
		Path __(this, "anyOf");
		Error("anyOf failed");
	}
	if(countOf(schema["oneOf"], 2, 1) != 1) {
		Path __(this, "oneOf");
		Error("oneOf failed");
	}
	
	Value Not = schema["not"];
	if(!Not.IsVoid()) {
		if(!Not.Is<ValueMap>())
			InvalidSchema();
		bool ok = true;
		try {
			Path __(this, "not");
			Check(Not, data, depth + 1);
			ok = false;
		}
		catch(FailedBranch) {}
		if(!ok)
			Error("'not' failed");
	}
	logics--;
	
	String ref = schema["$ref"];
	if(ref.GetCount()) {
		String ptr;
		int q = ref.Find('#');
		if(q >= 0) {
			ptr = ref.Mid(q + 1);
			ref.Trim(q);
		}
		Value ref_schema = refs.Get(ref, Value());
		if(ref_schema.IsVoid()) {
			ref_schema = Ref(ref);
			refs.Add(ref, ref_schema);
		}
		if(ptr.GetCount())
			ref_schema = DereferenceJSONPointer(ref_schema, ptr);
		Path __(this, "$ref:" + ref);
		Check0(ref_schema, data, depth);
	}
	Check0(schema, data, depth);
}

void JsonSchemaChecker::Check0(Value schema, Value data, int depth)
{
	if(depth++ > 200) {
		Error("invalid schema - depth is too high");
		return;
	}
	Value m = schema["enum"];
	if(m.Is<ValueArray>()) {
		bool ok = false;
		for(Value v : m)
			if(IsNull(v) && IsNumber(data) && IsNull(data) || v == data) {
				ok = true;
				break;
			}
		if(!ok)
			Error("enum error");
	}
	
	Value c = schema["const"];
	if(!c.IsVoid() && !(IsNumber(c) && IsNull(c) ? IsNumber(data) && IsNull(data) : data == c))
		Error("constant mismatch");
	
	Value type = schema["type"];
	bool type_ok = type.IsVoid();
	auto CheckType = [&](const char *t) {
		if(type_ok)
			return;
		if(type == t) {
			type_ok = true;
			return;
		}
		for(Value v : type) {
			if(v == t) {
				type_ok = true;
				return;
			}
		}
	};
	
	if(IsString(data)) {
		CheckType("string");
		StringFilters(schema, data);
	}
	else
	if(IsNumber(data) && IsNull(data) && !IsVoid(data)) // ParseJSON produces int Null
		CheckType("null");
	else
	if(IsNumber(data) && !data.Is<bool>()) {
		if(FitsInInt64(data) && (int64)data == data)
			CheckType("integer");
		CheckType("number");
		NumberFilters(schema, data);
	}
	else
	if(data.Is<ValueMap>()) {
		CheckType("object");
		ObjectFilters(schema, data, depth);
	}
	else
	if(data.Is<ValueArray>()) {
		CheckType("array");
		ArrayFilters(schema, data, depth);
	}
	else
	if(data.Is<bool>())
		CheckType("boolean");
	
	if(!type_ok)
		Error("type mismatch");
	
	depth--;
}

void JsonSchemaChecker::Validate(Value schema, Value data)
{
	schema_path.Clear();
	data_path.Clear();
	logics = 0;
	refs.GetAdd(Null) = schema;
	try {
		Check(schema, data, 0);
	}
	catch(ValueTypeError e) { InvalidSchema(); }
}
