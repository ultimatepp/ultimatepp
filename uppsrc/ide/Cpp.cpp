#include "ide.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

#define LTIMING(x) // DTIMING(x)

String ResolveTParam(const String& type, const Vector<String>& tparam)
{
	CodeBaseLock __;
	return ResolveTParam(CodeBase(), type, tparam);
}

void ResolveTParam(Vector<String>& type, const Vector<String>& tparam)
{
	CodeBaseLock __;
	return ResolveTParam(CodeBase(), type, tparam);
}

String Qualify(const String& scope, const String& type, const String& usings)
{
	CodeBaseLock __;
	return Qualify(CodeBase(), scope, type, usings);
}

void AssistScanError(int line, const String& text)
{
#ifdef _DEBUG
	PutVerbose(String().Cat() << "(" << line << "): " << text);
#endif
}

void AssistEditor::Context(ParserContext& parser, int pos)
{
	LTIMING("Context");
	LLOG("---------- Context " << path);
	
	theide->ScanFile(true);
	
	parser = AssistParse(Get(0, pos), theide->editfile, AssistScanError,
	                     [&](String scope, String type, String usings) {
							CodeBaseLock __;
							String t = Qualify(CodeBase(), scope, type, usings);
							return CodeBase().Find(NoTemplatePars(t)) >= 0 ? t : Null;
	                     });
	inbody = parser.IsInBody();
#ifdef _DEBUG
	PutVerbose("body: " + AsString(inbody));
	PutVerbose("scope: " + AsString(parser.current_scope));
	PutVerbose("using: " + AsString(parser.context.namespace_using));
	for(int i = 0; i < parser.local.GetCount(); i++)
		PutVerbose(parser.local.GetKey(i) + ": " + parser.local[i].type);
#endif
}

Index<String> AssistEditor::EvaluateExpressionType(const ParserContext& parser, const Vector<String>& xp)
{
	CodeBaseLock __;
	return GetExpressionType(CodeBase(), parser, xp);
}
