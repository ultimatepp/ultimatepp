#include "LayDes.h"

VectorMap<String, LayoutType>& LayoutTypes()
{
	static VectorMap<String, LayoutType> q;
	return q;
}

VectorMap<String, VectorMap<String, String> >& LayoutEnums()
{
	static VectorMap<String, VectorMap<String, String> > q;
	return q;
}

void ReadClass(CParser& p, int kind)
{
	LayoutType& m = LayoutTypes().GetAdd(p.ReadId());
	m.kind = kind;
	p.PassChar('{');
	while(!p.Char('}')) {
		if(p.Char('>')) {
			String n = p.ReadId();
			m.property.Add().name = n;
			if(IsNull(m.group)) {
				int q = LayoutTypes().Find(n);
				if(q >= 0)
					m.group = LayoutTypes()[q].group;
			}
			p.PassChar(';');
		}
		else
		if(p.Id("group")) {
			m.group = p.ReadString();
			p.PassChar(';');
		}
		else {
			String id = p.ReadId();
			if(p.IsChar('('))
				m.methods.GetAdd(id) = ReadLambda(p);
			else {
				TypeProperty& r = m.property.Add();
				r.type = id;
				r.name = p.ReadId();
				if(p.Char('=')) {
					const char *b = p.GetPtr();
					while(!p.IsChar(';') && !p.IsChar('?') && !p.IsChar('@'))
						p.SkipTerm();
					r.defval = String(b, p.GetPtr());
				}
				if(p.Char('@'))
					r.level = p.ReadInt();
				if(p.Char('?'))
					r.help = p.ReadString();
				p.PassChar(';');
			}
		}
	}
	p.Char(';');
}

void  LayLib();

void LayUscClean()
{
	LayoutEnums().Clear();
	LayoutTypes().Clear();
	LayLib();
}

bool LayUscParse(CParser& p) throw(CParser::Error)
{
	if(p.Id("ctrl"))
		ReadClass(p, LAYOUT_CTRL);
	else
	if(p.Id("subctrl"))
		ReadClass(p, LAYOUT_SUBCTRL);
	else
	if(p.Id("template"))
		ReadClass(p, LAYOUT_TEMPLATE);
	else
	if(p.Id("enum_property")) {
		VectorMap<String, String>& e = LayoutEnums().GetAdd(p.ReadId());
		p.PassChar('{');
		for(;;) {
			String key = p.IsString() ? p.ReadString() : p.ReadId();
			String text = key;
			if(p.Char(':'))
				text = p.IsString() ? p.ReadString() : p.ReadId();
			e.Add(key, text);
			if(p.Char('}'))
				break;
			p.PassChar(',');
		}
		p.Char(';');
	}
	else
		return false;
	return true;
}
