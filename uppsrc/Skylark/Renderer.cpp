#include "Skylark.h"

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

namespace Upp {

Renderer& Renderer::operator()(const ValueMap& map)
{
	ValueArray v = map.GetValues();
	const Index<Value>& k = map.GetKeys();
	for(int i = 0; i < map.GetCount(); i++)
		var.Add(k[i], v[i]);
	return *this;
}

Renderer& Renderer::Link(const char *id, void (*view)(Http&), const Vector<Value>& arg)
{
	var.Add(id, Raw('\"' + MakeLink(view, arg) + '\"'));
	return *this;
}

Renderer& Renderer::operator()(const char *id, void (*view)(Http&))
{
	return Link(id, view, Vector<Value>());
}

Renderer& Renderer::operator()(const char *id, void (*view)(Http&), const Value& arg1)
{
	return Link(id, view, Vector<Value>() << arg1);
}

Renderer& Renderer::operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2)
{
	return Link(id, view, Vector<Value>() << arg1 << arg2);
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
	String sgn = s;
	LLOG("Trying to retrieve " << sgn << " from cache");
	Mutex::Lock __(template_cache_lock);
	int q = template_cache.Find(sgn);
	if(q >= 0 && SkylarkApp::Config().use_caching)
		return template_cache[q];
	LLOG("About to compile: " << sgn);
	LTIMING("Compile");
	One<Exe>& exe = q >= 0 ? template_cache[q] : template_cache.Add(sgn);
	exe = Compile(GetPreprocessedTemplate(template_name, lang), var.GetIndex());
	return exe;
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