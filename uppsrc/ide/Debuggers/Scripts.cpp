#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LTIMESTOP(x)
#define LLOG(x)

void Pdb::LoadPrettyScripts()
{
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String filename = SourcePath(n, pk.file[i]);
			if(ToLower(GetFileExt(filename)) == ".dbg") {
				try {
					String d = LoadFile(filename);
					CParser p(d, filename);
					while(!p.IsEof()) {
						if(p.Id("fn")) {
							String id = p.ReadId();
							pretty_globals.GetAdd(id) = ReadLambda(p);
						}
						else
						if(p.Id("typename")) {
							String type;
							for(;;) {
								if(p.Char2(':', ':'))
									type << "::";
								else
								if(p.IsId())
									type << p.ReadId();
								else
									break;
							}
							pretty_scripts.GetAdd(type) = ReadLambda(p, false, "(value, template_param, from, items)");
						}
						else
						if(!p.Char(';'))
							p.ThrowError();
					}
				}
				catch(CParser::Error) {}
			}
		}
	}
	
	StdLib(pretty_globals);
	
	auto ValGet = [](EscEscape& e, const char *what) -> int64 {
		if(e[0].IsMap())
			return e[0].MapGet(what).GetInt64();
		else
		if(e[0].IsInt64())
			return e[0].GetInt64();
		else
			e.ThrowError("parameter can be either :value structure or integer");
		return 0;
	};
	
	auto Type = [=](EscEscape& e) { return (int)ValGet(e, "type"); };
	auto Adr = [=](EscEscape& e) { return ValGet(e, "address"); };
	
	Escape(pretty_globals, "SizeOf(x)", [=](EscEscape& e) {
		if(e[0].IsArray())
			e = SizeOfType((String)e[0]);
		else
			e = SizeOfType(Type(e));
	});
	Escape(pretty_globals, "TypeName(x)", [=](EscEscape& e) {
		e = GetType(Type(e)).name;
	});
	Escape(pretty_globals, "TypeNo(s)", [=](EscEscape& e) {
		e = GetTypeInfo((String)e[0]).type;
	});
	Escape(pretty_globals, "PeekPtr(x)", [=](EscEscape& e) {
		e = (int64)PeekPtr(Adr(e));
	});
	Escape(pretty_globals, "Peek8(x)", [=](EscEscape& e) {
		e = (int64)PeekByte(Adr(e));
	});
	Escape(pretty_globals, "Peek16(x)", [=](EscEscape& e) {
		e = (int64)PeekWord(Adr(e));
	});
	Escape(pretty_globals, "Peek32(x)", [=](EscEscape& e) {
		e = (int64)PeekDword(Adr(e));
	});
	Escape(pretty_globals, "Peek64(x)", [=](EscEscape& e) {
		e = (int64)Peek64(Adr(e));
	});
	Escape(pretty_globals, "Peek32f(x)", [=](EscEscape& e) {
		e = (float)PeekDword(Adr(e));
	});
	Escape(pretty_globals, "Peek64f(x)", [=](EscEscape& e) {
		e = (double)Peek64(Adr(e));
	});
	Escape(pretty_globals, "SizeOfPtr()", [=](EscEscape& e) {
		e = win64 ? 8 : 4;
	});
	Escape(pretty_globals, "LOG(x)", [=](EscEscape& e) {
		String txt;
		if(e[0].IsArray())
			txt = (String)e[0];
		else
			txt = e[0].ToString();
		PutConsole("dbg! " + txt);
	});
	Escape(pretty_globals, "DeRef(x)", [=](EscEscape& e) {
		EscValue v = e[0];
		if(!v.IsMap())
			e.ThrowError("paramater must be :value structure");
		v.MapSet("address", (int64)PeekPtr(v.MapGet("address").GetInt64()));
		e = v;
	});
	Escape(pretty_globals, "NestedType(x, id)", [=](EscEscape& e) {
		const auto& t = GetType(Type(e));
		String id = (String)e[1];
		for(int st : t.member_type)
			if(GetType(st).name == id) {
				e = st;
				return;
			}
		e = 0;
	});
	Escape(pretty_globals, "Field(x, id)", [=](EscEscape& e) {
		EscValue v = e[0];
		if(!v.IsMap())
			e.ThrowError("paramater must be :value structure");
		Pdb::Val val;
		val.type = v.MapGet("type").GetInt();
		val.address = v.MapGet("address").GetInt64();
		val = GetAttr(val, e[1]);
		SetVal(v, val);
		e = v;
	});
	Escape(pretty_globals, "Align(adr, sz)", [=](EscEscape& e) {
		e = (int64)Align(e[0].GetInt64(), e[1].GetInt());
	});
}

void Pdb::SetVal(EscValue& v, Pdb::Val val)
{
	v.MapSet("address", (int64)val.address);
	v.MapSet("type", val.type);
}

bool Pdb::PrettyScript(const String& type, Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p)
{
	int ii = pretty_scripts.Find(type);
	if(ii < 0)
		return false;
	LTIMESTOP("PrettyScript duration");
	LLOG("PrettyScript " << type << ", from: " << from << ", count: " << count);
	ArrayMap<String, EscValue> g = clone(pretty_globals);
	Vector<EscValue> args;
	SetVal(args.Add(), val);
	EscValue& v = args.Add();
	v.SetEmptyArray();
	for(int i = 0; i < tparam.GetCount(); i++)
		v.ArrayAdd(tparam[i]);
	args.Add(from);
	args.Add(count);
	bool ret = false;
	p.kind = CONTAINER;
	p.separated_types = true;
	Escape(g, "TEXT(x, color = 1)", [&](EscEscape& e) {
		ret = true;
		static Color col[] = {
            SBlack(), SRed(), SGreen(), SBrown(), SBlue(), SMagenta(), SCyan(), SGray(),
            SLtGray(), SLtRed(), SLtGreen(), SLtYellow(), SLtBlue(), SLtMagenta(),
            SLtCyan(), SYellow(), SWhiteGray(), SWhite()
		};
		Color c = col[clamp((int)e[1].GetInt(), 0, __countof(col))];
		if(e[0].IsArray())
			p.Text((String) e[0], c);
		else
		if(e[0].IsNumber())
			p.Text(AsString(e[0].GetNumber()), c);
		else
			e.ThrowError("invalid argument to 'Text'");
	});
	Escape(g, "ITEM_PTR(adr)", [&](EscEscape& e) {
		ret = true;
		p.data_ptr << e[0].GetInt64();
	});
	Escape(g, "ITEM_PTRS(adr, sz, from, items)", [&](EscEscape& e) {
		ret = true;
		adr_t adr = e[0].GetInt64();
		int64 sz = e[1].GetInt();
		int64 from = e[2].GetInt();
		int64 items = e[3].GetInt();

		for(int64 i = 0; i < items; i++)
			p.data_ptr << adr + (i + from) * sz;
	});
	Escape(g, "ITEM_DEREF_PTRS(adr, from, items)", [&](EscEscape& e) {
		ret = true;
		adr_t adr = e[0].GetInt64();
		int64 sz = win64 ? 8 : 4;
		int64 from = e[1].GetInt();
		int64 items = e[2].GetInt();

		for(int64 i = 0; i < items; i++)
			p.data_ptr << PeekPtr(adr + (i + from) * sz);
	});
	Escape(g, "ITEM_DEREF_PTRSZ(adr, sz, from, items)", [&](EscEscape& e) {
		ret = true;
		adr_t adr = e[0].GetInt64();
		int64 sz = e[1].GetInt64();
		int64 from = e[2].GetInt();
		int64 items = e[3].GetInt();

		for(int64 i = 0; i < items; i++)
			p.data_ptr << PeekPtr(adr + (i + from) * sz);
	});
	Escape(g, "ITEM_COUNT(x)", [&](EscEscape& e) {
		ret = true;
		p.data_count = e[0].GetInt64();
	});
	Escape(g, "ITEM_TYPE(x)", [&](EscEscape& e) {
		ret = true;
		p.data_type << (String)e[0];
	});
	Escape(g, "STRING()", [&](EscEscape& e) {
		p.kind = TEXT;
		ret = true;
	});
	Escape(g, "CHUNK(x)", [&](EscEscape& e) {
		p.chunk = clamp(e[0].GetInt(), 1, 10000);
		ret = true;
	});

	try {
		Execute(g, NULL, pretty_scripts[ii], args, 2000);
		if(p.data_count == 0 && p.kind == CONTAINER)
			p.kind = SINGLE_VALUE;
	}
	catch(CParser::Error e) {
		p.kind = SINGLE_VALUE;
		p.text.Clear();
		p.Text("ERR: ");
		p.Text(e);
		return true;
	}
	return ret;
}

#endif