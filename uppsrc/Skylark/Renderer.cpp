#include "Skylark.h"

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

namespace Upp {

Renderer& Renderer::operator()(const ValueMap& map)
{
	ValueArray v = map.GetValues();
	const Index<Value>& k = map.GetKeys();
	for(int i = 0; i < map.GetCount(); i++)
		var.GetAdd(k[i]) = v[i];
	return *this;
}

Renderer& Renderer::Link(const char *id, const HandlerId& handler, const Vector<Value>& arg)
{
	var.GetAdd(id) = Raw('\"' + MakeLink(handler, arg) + '\"');
	return *this;
}

Renderer& Renderer::operator()(const char *id, const HandlerId& handler)
{
	return Link(id, handler, Vector<Value>());
}

Renderer& Renderer::operator()(const char *id, const HandlerId& handler, const Value& arg1)
{
	return Link(id, handler, Vector<Value>() << arg1);
}

Renderer& Renderer::operator()(const char *id, const HandlerId& handler, const Value& arg1, const Value& arg2)
{
	return Link(id, handler, Vector<Value>() << arg1 << arg2);
}

Renderer& Renderer::operator()(const char *id, void (*handler)(Http&))
{
	return operator()(id, HandlerId(handler));
}

Renderer& Renderer::operator()(const char *id, void (*handler)(Http&), const Value& arg1)
{
	return operator()(id, HandlerId(handler), arg1);
}

Renderer& Renderer::operator()(const char *id, void (*handler)(Http&), const Value& arg1, const Value& arg2)
{
	return operator()(id, HandlerId(handler), arg1, arg2);
}

Renderer& Renderer::SetLanguage(int lang_)
{
	lang = lang_;
	return *this;
}

StaticMutex                 template_cache_lock;
ArrayMap<String, One<Exe> > template_cache;

const One<Exe>& Renderer::GetTemplate(const char *template_name)
{
	LTIMING("GetTemplate");
	StringBuffer s;
	{
		LTIMING("MakeSignature");
		for(int i = 0; i < var.GetCount(); i++)
			s << var.GetKey(i) << ';';
		s << ':' << template_name;
	}
	if(!SkylarkApp::Config().use_caching) // Templates get overwritten is not cached, MT hazard
		s << ';' << Thread::GetCurrentId();	
	String sgn = s;
	LLOG("Trying to retrieve " << sgn << " from cache");
	Mutex::Lock __(template_cache_lock);
	int q = template_cache.Find(sgn);
	if(q >= 0 && SkylarkApp::Config().use_caching)
		return template_cache[q];
	One<Exe>& exe = q >= 0 ? template_cache[q] : template_cache.Add(sgn);
	try {
		LLOG("About to compile: " << sgn);
		LTIMING("Compile");
		exe = Compile(GetPreprocessedTemplate(template_name, lang), var.GetIndex());
	}
	catch(CParser::Error e) {
		exe = Compile(String().Cat() << "<html><body>Error in template: " << e << "</body></html>", var.GetIndex());
	}
	return exe;
	NEVER();
	return template_cache[0];
}

String Renderer::RenderString(const String& template_name)
{
	return UPP::Render(GetTemplate(template_name), this, var.GetValues());
}

Renderer& Renderer::Render(const char *id, const String& template_name)
{
	var.Add(id, Render(template_name));
	return *this;
}

Renderer::~Renderer()
{
}

};