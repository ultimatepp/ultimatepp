#include "Core.h"

bool IsUppValueChar(int c)
{
	return c > ' ' && c != ',' && c != ';';
}

String ReadValue(CParser& p)
{
	p.Spaces();
	if(p.IsString())
		return p.ReadOneString();
	StringBuffer v;
	while(IsUppValueChar(p.PeekChar()))
		v.Cat(p.GetChar());
	p.Spaces();
	return v;
}

static bool sMatchOr(CParser& p, const Vector<String>& flag);

static bool sMatchFlag(CParser& p, const Vector<String>& flag)
{
	if(p.Char('!'))
		return !sMatchFlag(p, flag);
	if(p.Char('(')) {
		bool b = sMatchOr(p, flag);
		p.PassChar(')');
		return b;
	}
	if(p.IsEof())
		return true;
	return FindIndex(flag, p.ReadId()) >= 0;
}

static bool sMatchAnd(CParser& p, const Vector<String>& flag)
{
	bool b = sMatchFlag(p, flag);
	while(p.IsId() || p.IsChar('!') || p.IsChar('(') || p.Char2('&', '&') || p.Char('&'))
		b = sMatchFlag(p, flag) && b;
	return b;
}

static bool sMatchOr(CParser& p, const Vector<String>& flag)
{
	bool b = sMatchAnd(p, flag);
	while(p.Char2('|', '|') || p.Char('|'))
		b = sMatchFlag(p, flag) || b;
	return b;
}

bool MatchWhen_X(const String& when, const Vector<String>& flag)
{
	CParser p(when);
	bool b = sMatchOr(p, flag);
	if(!p.IsEof())
		p.ThrowError("expected end of expression");
	return b;
}

bool MatchWhen(const String& when, const Vector<String>& flag)
{
	try {
		return MatchWhen_X(when, flag);
	}
	catch(CParser::Error e) {
		PutConsole(String().Cat()
		           << "Invalid When expression: " << AsCString(when) << ": " << e);
		return false;
	}
}

String ReadWhen(CParser& p) {
	String when;
	if(p.Char('(')) {
		if(p.IsString())
			when = p.ReadString();
		else {
			const char *b = p.GetPtr();
			int lvl = 0;
			for(;;) {
				if(p.IsEof() || lvl == 0 && p.IsChar(')'))
					break;
				if(p.Char('('))
					lvl++;
				else
				if(p.Char(')'))
					lvl--;
				else
					p.SkipTerm();
			}
			when = String(b, p.GetPtr());
			p.Char(')');
		}
	}
	return when;
}

String AsStringWhen(const String& when) {
	String out;
	try {
		Vector<String> x;
		MatchWhen_X(when, x);
		out << '(' << when << ')';
	}
	catch(CParser::Error) {
		out << '(' << AsCString(when) << ')';
	}
	return out;
}

String CustomStep::AsString() const {
	return "custom" + AsStringWhen(when) + ' ' + AsCString(ext) + ",\n\t" +
					  AsCString(command, 70, "\t") + ",\n\t" +
					  AsCString(output, 70, "\t") + ";\n\n";
}

void CustomStep::Load(CParser& p) throw(CParser::Error) {
	when = ReadWhen(p);
	ext = p.ReadString();
	p.PassChar(',');
	command = p.ReadString();
	p.PassChar(',');
	output = p.ReadString();
	p.PassChar(';');
}

String CustomStep::GetExt() const {
	return ext[0] != '.' ? "." + ToLower(ext) : ToLower(ext);
}

bool   CustomStep::MatchExt(const char *fn) const {
	return ToLower(GetFileExt(fn)) == GetExt();
}

bool LoadOpt(CParser& p, const char *key, Array<OptItem>& v) {
	if(p.Id(key)) {
		String when = ReadWhen(p);
		do {
			OptItem& m = v.Add();
			m.when = when;
			m.text = ReadValue(p);
		}
		while(p.Char(','));
		return true;
	}
	return false;
}

bool LoadFOpt(CParser& p, const char *key, Array<OptItem>& v) {
	if(p.Id(key)) {
		OptItem& m = v.Add();
		m.when = ReadWhen(p);
		m.text = ReadValue(p);
		return true;
	}
	return false;
}

void Package::Reset()
{
	charset = 0;
	optimize_speed = false;
	noblitz = nowarnings = false;
	bold = italic = false;
	ink = Null;
}

Package::Package()
{
	Reset();
}

bool StdResolver(const String& error, const String& path, int line)
{
	PutConsole("Invalid package: " + path);
	exit(1);
	return false;
}

static bool (*sResolve)(const String& error, const String& path, int line) = StdResolver;

void Package::SetPackageResolver(bool (*Resolve)(const String& error, const String& path, int line))
{
	sResolve = Resolve;
}

byte CharsetByNameX(const String& s)
{
	return s == "UTF-8-BOM" ? CHARSET_UTF8_BOM : CharsetByName(s);
}

void Package::Option(bool& option, const char *name)
{
	File& f = file.Top();
	for(int i = 0; i < f.option.GetCount(); i++) // Ugly BW compatibility hack
		if(f.option[i].text == name) {
			f.option.Remove(i);
			option = true;
			break;
		}
}

bool Package::Load(const char *path)
{
	for(;;) {
		Reset();
		library.Clear();
		static_library.Clear();
		target.Clear();
		flag.Clear();
		option.Clear();
		link.Clear();
		uses.Clear();
		include.Clear();
		accepts.Clear();
		file.Clear();
		config.Clear();
		custom.Clear();
		description.Clear();
		String f = LoadFile(path);
		time = FileGetTime(path);
		if(IsNull(time))
			return false;
		CParser p(f);
		try {
			while(!p.IsEof()) {
				if(!LoadOpt(p, "options", option) &&
				   !LoadOpt(p, "link", link) &&
				   !LoadOpt(p, "library", library) &&
				   !LoadOpt(p, "static_library", static_library) &&
				   !LoadOpt(p, "flags", flag) &&
				   !LoadOpt(p, "target", target) &&
				   !LoadOpt(p, "uses", uses) &&
				   !LoadOpt(p, "include", include)) {
					if(p.Id("charset"))
						charset = CharsetByNameX(p.ReadString());
					else
					if(p.Id("description")) {
						description = p.ReadString();
						const char *q = strchr(description, 255);
						ink = Null;
						bold = italic = false;
						if(q) {
							CParser p(q + 1);
							bold = p.Char('B');
							italic = p.Char('I');
							if(p.IsNumber()) {
								RGBA c = Black();
								c.r = p.ReadInt();
								p.Char(',');
								if(p.IsNumber())
									c.g = p.ReadInt();
								p.Char(',');
								if(p.IsNumber())
									c.b = p.ReadInt();
								ink = c;
							}
							description = String(~description, q);
						}
					}
					else
					if(p.Id("acceptflags")) {
						do
							accepts.Add(ReadValue(p));
						while(p.Char(','));
					}
					else
					if(p.Id("noblitz"))
					   noblitz = true;
					else
					if(p.Id("optimize_speed"))
						optimize_speed = true;
					else
					if(p.Id("optimize_size"))
						optimize_speed = false;
					else
					if(p.Id("file")) {
						do {
							File fv(ReadValue(p));
							File& f = file.Add();
							f = pick(fv);
							while(!p.IsChar(',') && !p.IsChar(';')) {
								if(!LoadFOpt(p, "options", f.option) &&
								   !LoadFOpt(p, "depends", f.depends)) {
									if(p.Id("optimize_speed"))
										f.optimize_speed = true;
									else
									if(p.Id("optimize_size"))
										f.optimize_speed = false;
									else
									if(p.Id("pch"))
										f.pch = true;
									else
									if(p.Id("nopch"))
										f.nopch = true;
									else
									if(p.Id("noblitz"))
										f.noblitz = true;
									else
									if(p.Id("readonly"))
										f.readonly = true;
									else
									if(p.Id("separator"))
										f.separator = true;
									else
									if(p.Id("charset"))
										f.charset = CharsetByNameX(p.ReadString());
									else
									if(p.Id("tabsize"))
										f.tabsize = minmax(p.ReadInt(), 1, 20);
									else
									if(p.Id("font"))
										f.font = minmax(p.ReadInt(), 0, 3);
									else
									if(p.Id("highlight"))
										f.highlight = p.ReadId();
									else
										p.SkipTerm();
								}
							}
							Option(f.pch, "PCH");
							Option(f.nopch, "NOPCH");
							Option(f.noblitz, "NOBLITZ");
						}
						while(p.Char(','));
					}
					else
					if(p.Id("mainconfig")) {
						do {
							String c = p.ReadString();
							p.Char('=');
							p.Id("external"); // Backward compatibility...
							p.Id("console");
							p.Id("remotelinux");
							p.Id("normal");
							String f = p.ReadString();
							Config& cf = config.Add();
							cf.name = c;
							cf.param = f;
						}
						while(p.Char(','));
					}
					else
					if(p.Id("custom"))
						custom.Add().Load(p);
					else
						p.SkipTerm();
				}
				p.Char(';');
			}
			for(int i = 0; i < option.GetCount(); i++)
				if(option[i].when == "BUILDER_OPTION" && option[i].text == "NOWARNINGS") {
					nowarnings = true;
					option.Remove(i);
					break;
				}
			return true;
		}
		catch(CParser::Error error) {
			if(sResolve(error, path, p.GetLine() - 1))
				return false;
			Save(path);
			return true;
		}
	}
}

String WriteValue(const String& x) {
	for(const char *s = x; s < x.End(); s++)
		if(!IsUppValueChar(*s))
			return AsCString(x);
	return x;
}

void putopt(Stream& out, const char *key, const Array<OptItem>& m) {
	bool was = false;
	for(int i = 0; i < m.GetCount(); i++)
		if(IsNull(m[i].when)) {
			if(was)
				out << ",\n\t";
			else
				out << key << "\n\t";
			out << WriteValue(m[i].text);
			was = true;
		}
	if(was)
		out << ";\n\n";
	for(int i = 0; i < m.GetCount(); i++)
		if(!IsNull(m[i].when))
			out << key << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text) << ";\n\n";
}

void putp(Stream& out, const char *key, const Vector<String>& v)
{
	if(v.GetCount()) {
		out << key << "\n";
		for(int i = 0; i < v.GetCount(); i++) {
			if(i) out << ",\n";
			out << '\t' << WriteValue(v[i]);
		}
		out << ";\n\n";
	}
}

void putfopt(Stream& out, const char *key, const Array<OptItem>& m)
{
	for(int i = 0; i < m.GetCount(); i++)
		out << "\n\t\t" << key << AsStringWhen(m[i].when) << ' ' << WriteValue(m[i].text);
}

bool Package::Save(const char *path) const {
	StringStream out;
	if(description.GetCount() || italic || bold || !IsNull(ink)) {
		String d = description;
		d.Cat(255);
		if(bold)
			d << 'B';
		if(italic)
			d << 'I';
		if(!IsNull(ink))
			d << (int)ink.GetR() << ',' << (int)ink.GetG() << ',' << (int)ink.GetB();
		out << "description " << AsCString(d) << ";\n\n";
	}
	if(charset > 0 && charset < CharsetCount() || charset == CHARSET_UTF8)
		out << "charset " << AsCString(CharsetName(charset)) << ";\n\n";
	else
	if(charset == CHARSET_UTF8_BOM)
		out << "charset \"UTF-8-BOM\";\n\n";
	if(optimize_speed)
		out << "optimize_speed;\n\n";
	if(noblitz)
		out << "noblitz;\n\n";
	if(nowarnings)
		out << "options(BUILDER_OPTION) NOWARNINGS;\n\n";
	putp(out, "acceptflags", accepts);
	putopt(out, "flags", flag);
	putopt(out, "uses", uses);
	putopt(out, "target", target);
	putopt(out, "library", library);
	putopt(out, "static_library", static_library);
	putopt(out, "options", option);
	putopt(out, "link", link);
	putopt(out, "include", include);
	if(file.GetCount()) {
		out << "file\n";
		int i;
		for(i = 0; i < file.GetCount(); i++) {
			if(i) out << ",\n";
			const File& f = file[i];
			out << '\t' << WriteValue(f);
			if(f.readonly)
				out << " readonly";
			if(f.separator)
				out << " separator";
			if(f.tabsize > 0)
				out << " tabsize " << f.tabsize;
			if(f.font > 0)
				out << " font " << f.font;
			if(f.optimize_speed)
				out << " optimize_speed";
			if(f.pch)
				out << " options(BUILDER_OPTION) PCH";
			if(f.nopch)
				out << " options(BUILDER_OPTION) NOPCH";
			if(f.noblitz)
				out << " options(BUILDER_OPTION) NOBLITZ";
			if(f.charset > 0 && f.charset < CharsetCount() || f.charset == CHARSET_UTF8)
				out << " charset " << AsCString(CharsetName(f.charset));
			else
			if(f.charset == CHARSET_UTF8_BOM)
				out << " charset \"UTF-8-BOM\"";
			if(!IsNull(f.highlight))
				out << " highlight " << f.highlight;
			putfopt(out, "options", f.option);
			putfopt(out, "depends", f.depends);
		}
		out << ";\n\n";
	}
	if(config.GetCount()) {
		out << "mainconfig\n";
		for(int i = 0; i < config.GetCount(); i++) {
			const Config& f = config[i];
			if(i) out << ",\n";
			out << '\t' << AsCString(f.name) << " = " << AsCString(f.param);
		}
		out << ";\n\n";
	}
	for(int i = 0; i < custom.GetCount(); i++)
		out << custom[i].AsString();
	return SaveChangedFile(path, out.GetResult());
}
