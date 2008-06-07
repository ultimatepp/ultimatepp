#include "Docedit.h"

bool DocKey::operator==(const DocKey& b) const
{
	return nameing == b.nameing && nesting == b.nesting && item == b.item && lang == b.lang;
}

dword GetHashValue(const DocKey& k)
{
	return CombineHash(GetHashValue(k.nameing), GetHashValue(k.nesting),
	                   GetHashValue(k.item)) << k.lang;
}

int CharFilterNameing(int c)
{
	return c == ':' ? '_' : c;
}

String DocFile(const String& package, const String& name)
{
	return AppendFileName(SourcePath(package, "doc.dpp"), name);
}

Time   GetFileTime(const char *path)
{
	FindFile ff(path);
	return ff ? Time(ff.GetLastWriteTime()) : Null;
}

void CreateDocDir(const String& package)
{
	::CreateDirectory(SourcePath(package, "doc.dpp"), 0);
}

String AsCode(const DocKey& k)
{
	return AsCString(k.nameing) + ", " + AsCString(k.nesting) + ", " +
	       AsCString(k.item) + ", " + AsCString(LNGAsText(k.lang));
}

bool ReadCode(CParser& p, DocKey& key)
{
	if(!p.IsString()) return false;
	key.nameing = p.ReadString();
	if(!p.Char(',') || !p.IsString()) return false;
	key.nesting = p.ReadString();
	if(!p.Char(',') || !p.IsString()) return false;
	key.item = p.ReadString();
	if(!p.Char(',') || !p.IsString()) return false;
	key.lang = LNGFromText(p.ReadString());
	return true;
}

void DocDir::SaveLinks(const String& package) const
{
	CreateDocDir(package);
	FileOut out(DocFile(package, "links"));
	int q = dir.Find(package);
	if(q >= 0) {
		const ArrayMap<DocKey, Entry>& p = dir[q];
		for(int i = 0; i < p.GetCount(); i++) {
			const Entry& w = p[i];
			const DocKey& k = p.GetKey(i);
			if(w.type == LINK)
				out << "LINK(" << AsCode(k) << ", " << AsCString(w.text) << ")\r\n";
			if(w.type == IGNORED)
				out << "IGNORED(" << AsCode(k) << ")\r\n";
		}
	}
}

void DocDir::SaveDir(const String& package) const
{
	CreateDocDir(package);
	FileOut out(DocFile(package, "dir.h"));
	int q = dir.Find(package);
	if(q >= 0) {
		const ArrayMap<DocKey, Entry>& p = dir[q];
		for(int i = 0; i < p.GetCount(); i++) {
			const Entry& w = p[i];
			if(w.type == NORMAL || w.type == EXTERNAL) {
				const DocKey& k = p.GetKey(i);
				out << "#ifdef INCLUDE_NAMESPACE_" <<
				       Filter(k.nameing, CharFilterNameing) << "\r\n";
				out << "//" << k.item << "\r\n";
				out << "//" << k.nesting << "\r\n";
				out << "//" << k.nameing << "\r\n";
				out << "#include \"doc.dpp/";
				out << w.text;
				out << "\" //";
				if(w.type == EXTERNAL)
					out << "*";
				out << LNGAsText(k.lang) << "\r\n";
				out << "#endif\r\n\r\n";
			}
		}
	}
	SaveLinks(package);
}

bool DocDir::LoadLinks(const String& package)
{
	String f = LoadFile(DocFile(package, "links"));
	CParser p(f);
	ArrayMap<DocKey, Entry>& pk = dir.GetAdd(package);
	try {
		while(!p.IsEof()) {
			if(p.Id("LINK")) {
				DocKey key;
				p.PassChar('(');
				if(!ReadCode(p, key)) return false;
				p.PassChar(',');
				String link = p.ReadString();
				p.PassChar(')');
				Entry& e = pk.GetAdd(key);
				e.text = link;
				e.type = LINK;
			}
			if(p.Id("IGNORED")) {
				DocKey key;
				p.PassChar('(');
				if(!ReadCode(p, key)) return false;
				p.PassChar(')');
				Entry& e = pk.GetAdd(key);
				e.text.Clear();
				e.type = IGNORED;
			}
		}
		return true;
	}
	catch(CParser::Error) {
		return false;
	}
}

bool DocDir::LoadDir(const String& package)
{
	ArrayMap<DocKey, Entry>& p = dir.GetAdd(package);
	p.Clear();
	LoadLinks(package);
	FileIn in(DocFile(package, "dir.h"));
	int q = 0;
	String d[3];
	while(!in.IsEof()) {
		String l = in.GetLine();
		if(memcmp(l, "#include ", 9) == 0) {
			const char *b = strchr(l, '/');
			if(!b) return false;
			const char *e = strchr(b + 1, '\"');
			if(!e) return false;
			String fn = String(b + 1, e);
			e = strchr(e, '/');
			if(!e || e[1] != '/') return false;
			e += 2;
			int type = NORMAL;
			if(*e == '*') {
				type = EXTERNAL;
				e++;
			}
			Entry& w = p.GetAdd(DocKey(d[2], d[1], d[0], LNGFromText(e)));
			w.text = fn;
			w.type = type;
			q = 0;
		}
		if(l[0] == '/' && l[1] == '/') {
			if(q >= 3) return false;
			d[q] = l.Mid(2);
			q++;
		}
	}
	return true;
}

int DocDir::ReadDocHeader(const char *filename, DocKey& key)
{
	FileIn in(filename);
	if(!in) return -1;
	String l = in.GetLine();
	CParser p(l);
	if(!p.Id("ITEM") || !p.Char('(') || !ReadCode(p, key)) return -1;
	p.Char(')');
	if(p.Id("EXTERNAL"))
		return EXTERNAL;
	return NORMAL;
}

void DocDir::RebuildDir(const String& package)
{
	FindFile ff(DocFile(package, "*.dpp"));
	Progress pi("Rebuilding " + package + " doc directory %d");
	ArrayMap<DocKey, Entry>& p = dir.GetAdd(package);
	p.Clear();
	LoadLinks(package);
	while(ff) {
		DocKey key;
		pi.Step();
		int q = ReadDocHeader(DocFile(package, ff.GetName()), key);
		if(q >= 0) {
			Entry& w = p.GetAdd(key);
		 	w.text = ff.GetName();
		 	w.type = q;
		}
		ff.Next();
	}
	SaveDir(package);
}

void DocDir::Refresh(const String& package)
{
	FindFile ff(DocFile(package, "dir.h"));
	if(!ff) {
		RebuildDir(package);
		return;
	}
	FileTime dirtime = ff.GetLastWriteTime();
	ff.Search(DocFile(package, "links"));
	if(ff && ff.GetLastWriteTime() > dirtime) {
		RebuildDir(package);
		return;
	}
	const ArrayMap<DocKey, Entry>& p = dir.GetAdd(package);
	Index<String> dfn;
	for(int i = 0; i < p.GetCount(); i++)
		if(p[i].type == NORMAL || p[i].type == EXTERNAL)
			dfn.Add(p[i].text);
	ff.Search(DocFile(package, "*.dpp"));
	int count = 0;
	while(ff) {
		DocKey key;
		if(dfn.Find(ff.GetName()) >= 0)
			if(ff.GetLastWriteTime() > dirtime) {
				RebuildDir(package);
				return;
			}
			else
				count++;
		else
		if(ReadDocHeader(DocFile(package, ff.GetName()), key) >= 0) {
			RebuildDir(package);
			return;
		}
		ff.Next();
	}
	if(count != dfn.GetCount())
		RebuildDir(package);
}

int CharFilterLNG_(int c)
{
	return c == '-' ? '_' : c;
}

String DocDir::GetAddFileName(const String& package, const DocKey& key, int type)
{
	Entry& w = dir.GetAdd(package).GetAdd(key);
	String& fn = w.text;
	w.type = type;
	if(!IsEmpty(fn)) return fn;
	String nm = key.nameing + '_' + key.nesting + '_' + key.item;
	String n;
	const char *s = nm;
	while(*s && n.GetLength() < 30)
		if(iscid(*s))
			n.Cat(*s++);
		else {
			n.Cat('_');
			while(*s && !iscid(*s))
				s++;
		}
	n << '_' << LNGAsText(key.lang);
	int i = 0;
	for(;;) {
		fn = n + FormatIntAlpha(i) + ".dpp";
		if(!FindFile(DocFile(package, fn)))
			return fn;
		i++;
	}
}

bool DocDir::GetFileName(const DocKey& key, String& fn, String& package)
{
	dword hv = GetHashValue(key);
	for(int i = 0; i < dir.GetCount(); i++) {
		const ArrayMap<DocKey, Entry>& p = dir[i];
		int q = p.Find(key, hv);
		if(q >= 0) {
			fn = p[q].text;
			package = dir.GetKey(i);
			return true;
		}
	}
	return false;
}

void DocDir::RemoveOtherKey(const DocKey& key, int t1, int t2)
{
	dword hv = GetHashValue(key);
	for(int i = 0; i < dir.GetCount(); i++) {
		int q = dir[i].Find(key, hv);
		if(q >= 0) {
			Entry& e = dir[i][q];
			if(e.type != t1 && e.type != t2) {
				if(e.type == NORMAL || e.type == EXTERNAL)
					DeleteFile(DocFile(dir.GetKey(i), dir[i][q].text));
				dir[i].Remove(q);
			}
			SaveDir(dir.GetKey(i));
			SaveLinks(dir.GetKey(i));
		}
	}
}

void DocDir::Remove(const DocKey& k)
{
	RemoveOtherKey(k, UNDOCUMENTED, UNDOCUMENTED);
}

void DocDir::SaveText(const String& package, const DocKey& k, const String& text, bool external)
{
	RemoveOtherKey(k, NORMAL, EXTERNAL);
	CreateDocDir(package);
	String fn = DocFile(package, GetAddFileName(package, k, external ? EXTERNAL : NORMAL));
	FileOut out(fn);
	out << "ITEM(" << AsCode(k) << ")";
	if(external)
		out << " EXTERNAL";
	out << "\r\n";
	int n = 0;
	for(;;) {
		int m = min(text.GetLength() - n, 2048);
		if(m == 0) break;
		out << "TEXT(\r\n\t";
		out << AsCString(~text + n, ~text + n + m, 64, "\t") << "\r\n";
		out << ")\r\n";
		n += m;
	}
	out << "END_ITEM\r\n";
	SaveDir(package);
	SaveLinks(package);
}

void DocDir::SaveLink(const String& package, const DocKey& key, const String& text)
{
	RemoveOtherKey(key, LINK);
	Entry& w = dir.GetAdd(package).GetAdd(key);
	w.text = text;
	w.type = LINK;
	SaveLinks(package);
}

void DocDir::Ignore(const String& package, const DocKey& key)
{
	RemoveOtherKey(key, IGNORED);
	Entry& w = dir.GetAdd(package).GetAdd(key);
	w.type = IGNORED;
	SaveLinks(package);
}

DocDir::Entry *DocDir::Find(const DocKey& key, String& package) const
{
	dword hv = GetHashValue(key);
	for(int i = 0; i < dir.GetCount(); i++) {
		int q = dir[i].Find(key, hv);
		if(q >= 0) {
			package = dir.GetKey(i);
			return const_cast<Entry *>(&dir[i][q]);
		}
	}
	return NULL;
}

DocDir::Entry *DocDir::Find(const DocKey& key) const
{
	String dm;
	return Find(key, dm);
}

int DocDir::GetStatus(const DocKey& key)
{
	DocDir::Entry *e = Find(key);
	return e ? e->type : UNDOCUMENTED;
}

int DocDir::GetStatus(const String& nameing, const String& nesting, const String& item,
                      int lang)
{
	return GetStatus(DocKey(nameing, nesting, item, lang));
}

String DocDir::GetFilePath(const DocKey& key) const
{
	String package;
	Entry *e = Find(key, package);
	if(!e || e->type != NORMAL && e->type != EXTERNAL) return Null;
	return DocFile(package, e->text);
}

String DocDir::GetPackage(const DocKey& key) const
{
	String package;
	Entry *e = Find(key, package);
	return e ? package : Null;
}

String DocDir::GetText(const DocKey& key) const
{
	String s = LoadFile(GetFilePath(key));
	if(s.IsEmpty()) return Null;
	CParser p(s);
	DocKey dummy;
	if(!p.Id("ITEM") || !p.Char('(') || !ReadCode(p, dummy)) return "Invalid file";
	p.Char(')');
	p.Id("EXTERNAL");
	String text;
	while(p.Id("TEXT")) {
		p.Char('(');
		if(p.IsString())
			text.Cat(p.ReadString());
		else
			return "Invalid file";
		p.Char(')');
	}
	return p.Id("END_ITEM") ? text : "Invalid file";
}

String DocDir::GetLink(const DocKey& key) const
{
	DocDir::Entry *e = Find(key);
	if(!e) return Null;
	return e->type == LINK ? e->text : Null;
}

bool Contains(const DocSet& r, const DocKey& key)
{
	int q = r.Find(key.nameing);
	if(q < 0) return false;
	int w = r[q].Find(key.nesting);
	if(w < 0) return false;
	return r[q][w].Find(key.item) >= 0;
}

bool Contains(const String& big, const String& part)
{
	if(part.GetLength() > big.GetLength()) return false;
	const char *s = big;
	const char *e = s + big.GetLength() - part.GetLength();
	int l = part.GetLength();
	while(s <= e) {
		if(memcmp(s, part, l) == 0)
			return true;
		s++;
	}
	return false;
}

DocSet DocDir::Select(const DocQuery& query)
{
	DocSet r;
	CppBase base;
	int i;
	for(i = 0; i < doc_base.GetCount(); i++) {
		String nameing = doc_base.GetKey(i);
		CppNamespace& mm = doc_base[i];
		for(int i = 0; i < mm.GetCount(); i++) {
			String nesting = mm.GetKey(i);
			CppNest& nn = mm[i];
			for(int i = 0; i < nn.GetCount(); i++) {
				CppItem& q = nn[i];
				String item = nn.GetKey(i);
				if((query.name.IsEmpty() || query.name == q.name) &&
				   (query.text.IsEmpty() || Contains(item, query.text)) &&
				   (query.package.IsEmpty() || q.package.CompareNoCase(query.package) == 0) &&
				   (query.header.IsEmpty() || q.file.CompareNoCase(query.header) == 0)) {
					String pk;
					const Entry *e = Find(DocKey(nameing, nesting, item, query.lang), pk);
					int st = e ? e->type : UNDOCUMENTED;
					if((query.undocumented || e) && (st != IGNORED || query.ignored)) {
						DocItem& a = r.GetAdd(nameing).GetAdd(nesting).GetAdd(item);
						a.cppitem = &q;
						a.item = item;
						a.status = st;
						a.package = e ? pk : q.package;
					}
				}
			}
		}
	}
	for(i = 0; i < dir.GetCount(); i++) {
		ArrayMap<DocKey, Entry>& pk = dir[i];
		String package = dir.GetKey(i);
		for(int j = 0; j < pk.GetCount(); j++) {
			const DocKey& k = pk.GetKey(j);
			if(k.lang == query.lang &&
			   query.name.IsEmpty() &&
			   (query.text.IsEmpty() || Contains(k.item, query.text)) &&
			   (query.package.IsEmpty() || package.CompareNoCase(query.package) == 0) &&
			   query.header.IsEmpty() &&
			   !Contains(r, k)) {
				DocItem& a = r.GetAdd(k.nameing).GetAdd(k.nesting).GetAdd(k.item);
				a.cppitem = NULL;
				a.item = k.item;
				int st = pk[j].type;
				a.status = st == NORMAL ? OBSOLETE : st == LINK ? OBSOLETELINK : st;
				a.package = package;
			}
		}
	}
	return r;
}

DocDir doc_dir;
