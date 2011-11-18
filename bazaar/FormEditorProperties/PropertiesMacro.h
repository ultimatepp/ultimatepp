#ifndef FORM_EDITOR_PROPERTIES__PROPERTIES_MACRO_H
#define FORM_EDITOR_PROPERTIES__PROPERTIES_MACRO_H

///////////////////////////////////////////////////////////////////////////////////////////////
// Properties declaration macros
///////////////////////////////////////////////////////////////////////////////////////////////

#define PROPS_INIT_PROPERTIES virtual void InitProperties() \
	{

#define PROPS_INIT_PROPERTIES_END \
	}

#define PROPS_DEFINE_PROPERTY(MethodSet, MethodGet) \
	GetProperties().Add().SetCalls(THISBACK(PropSet##MethodSet), THISBACK(PropGet##MethodGet)) \
		PROPS_SET_NAME(#MethodSet)PROPS_GET_NAME(#MethodGet)

// Meta info for property
#define PROPS_SET_NAME(Name)         .AddMetaInfo("SetMethodName", Name)
#define PROPS_GET_NAME(Name)         .AddMetaInfo("GetMethodName", Name)
#define PROPS_GROUP(Group)           .AddMetaInfo("Group", Group)
#define PROPS_NAME(Name)             .AddMetaInfo("Name", Name)
#define PROPS_HINT(Hint)             .AddMetaInfo("Hint", Hint)
#define PROPS_DEFAULT(Default)       .SetDefault(Default)
#define PROPS_TYPE(EditType, Notify) .SetEdit(new EditType, Notify)

// Ends property description
#define PROPS_PROPERTY_END \
	;

///////////////////////////////////////////////////////////////////////////////////////////////
// Properties implementation macros
///////////////////////////////////////////////////////////////////////////////////////////////

#define PROPS_METHOD_SET(MethodName, ...) \
	void PropSet##MethodName(const Vector<Value>& args) \
	{ \
		SProperty c(args); \
		PropertyCaller<CLASSNAME, __VA_ARGS__>::call(*this, &CLASSNAME::MethodName, c); \
	}

#define PROPS_METHOD_GET(MethodName, ...) \
	void PropGet##MethodName(Value& v, const Vector<Value>& args) \
	{ \
		SProperty c(args); \
		PropertyCaller<CLASSNAME, __VA_ARGS__>::call(*this, &CLASSNAME::MethodName, c); \
		v = c.Return; \
	}

#define PROPS_CLASS_METHOD_SET(ClassName, MethodName, ...) \
	void PropSet##MethodName(const Vector<Value>& args) \
	{ \
		SProperty c(args); \
		PropertyCaller<ClassName, __VA_ARGS__>::call(GetCtrl(), &ClassName::MethodName, c); \
	}

#define PROPS_CLASS_METHOD_GET(ClassName, MethodName, ...) \
	void PropGet##MethodName(Value& v, const Vector<Value>& args) \
	{ \
		SProperty c(args); \
		PropertyCaller<ClassName, __VA_ARGS__>::call(GetCtrl(), &ClassName::MethodName, c); \
		v = c.Return; \
	}

#endif
