#include "Designer.h"

static Point sReadAB(CParser& p) {
	Point pt;
	p.PassChar('(');
	pt.x = p.ReadInt();
	p.PassChar(',');
	pt.y = p.ReadInt();
	p.PassChar(')');
	return pt;
}

static String ReadVar(CParser& p) {
	String var;
	for(;;) {
		if(p.IsId())
			var << p.ReadId();
		else
		if(p.IsInt())
			var << p.ReadInt();
		else
		if(p.Char('.'))
			var << '.';
		else
		if(p.Char('['))
			var << '[';
		else
		if(p.Char(']'))
			var << ']';
		else
			return var;
	}
}

void LayoutDesigner::LoadCtrl(CParser& p, ItemInfo& ii, int li) {
	if(p.Id("ITEM")) {
		p.PassChar('(');
		if(p.IsId()) ii.classname = p.ReadIdt();
		p.PassChar(',');
	}
	else
	if(p.Id("UNTYPED"))
		p.PassChar('(');
	else
		p.ThrowError("Bad item.");
	if(p.IsId()) ii.variable = ReadVar(p);
	if(strncmp(ii.variable, "dv___", 5) == 0)
		ii.variable.Clear();
	p.PassChar(',');
	LogPos pos;
	do {
		if(p.Id("LeftPosZ") || p.Id("LeftPos")) {
			Point pt = sReadAB(p);
			pos.x = PosLeft(pt.x, pt.y);
		}
		else
		if(p.Id("RightPosZ") || p.Id("RightPos")) {
			Point pt = sReadAB(p);
			pos.x = PosRight(pt.x, pt.y);
		}
		else
		if(p.Id("HSizePosZ") || p.Id("HSizePos")) {
			Point pt = sReadAB(p);
			pos.x = PosSize(pt.x, pt.y);
		}
		else
		if(p.Id("HCenterPosZ") || p.Id("HCenterPos")) {
			Point pt = sReadAB(p);
			pos.x = PosCenter(pt.x, pt.y);
		}
		else
		if(p.Id("TopPosZ") || p.Id("TopPos")) {
			Point pt = sReadAB(p);
			pos.y = PosLeft(pt.x, pt.y);
		}
		else
		if(p.Id("BottomPosZ") || p.Id("BottomPos")) {
			Point pt = sReadAB(p);
			pos.y = PosRight(pt.x, pt.y);
		}
		else
		if(p.Id("VSizePosZ") || p.Id("VSizePos")) {
			Point pt = sReadAB(p);
			pos.y = PosSize(pt.x, pt.y);
		}
		else
		if(p.Id("VCenterPosZ") || p.Id("VCenterPos")) {
			Point pt = sReadAB(p);
			pos.y = PosCenter(pt.x, pt.y);
		}
		else
		if(p.Id("SetLabel")) {
			p.PassChar('(');
			if(p.IsString())
				ii.label.At(li) = p.ReadString();
			else
				ii.labelid = p.ReadId();
			p.PassChar(')');
		}
		else
		if(p.Id("HelpC") || p.Id("Help") || p.Id("Description")) {
			p.PassChar('(');
			if(p.IsString())
				ii.help.At(li) = p.ReadString();
			else
				ii.helpid = p.ReadId();
			p.PassChar(')');
		}
		else
		if(p.Id("Hide")) {
			p.PassChar('(');
			p.PassChar(')');
		}
		else
			p.ThrowError("Invalid item modifier.");
	}
	while(p.Char('.'));
	p.PassChar(')');
	ii.pos = pos;
}

void LayoutDesigner::LoadLayoutFile(const char *_filename) {
	SetLayout(-1);
	layout.Clear();
	langlist.Clear();
	try {
		filename = _filename;
		String s = LoadFile(filename);
		CParser p(s);
		if(p.Id("#ifdef"))
			p.Id("LAYOUTFILE");
		while(p.Id("LAYOUT")) {
			p.PassChar('(');
			String name = p.ReadId();
			p.PassChar(',');
			int cx = p.ReadInt();
			p.PassChar(',');
			int cy = p.ReadInt();
			p.PassChar(')');
			const char *a = p.GetPtr();
			const char *b = a;
			Form& l = layout.Add();
			l.name = name;
			l.size = Size(cx, cy);
			while(!p.IsEof() && !p.Id("END_LAYOUT"))
				LoadCtrl(p, l.item.Add());
			while(*a == '\n' || *a == '\r')
				a++;
		}
		p.Id("#endif");
		while(p.Id("#if")) {
			p.PassId("LANGUAGE");
			p.PassChar2('=', '=');
			String langid = p.ReadId();
			CParser::Pos idpos = p.GetPos();
			if(*idpos.ptr == '(')
			{
				const char *b = idpos.ptr;
				while(*idpos.ptr && *idpos.ptr++ != ')')
					;
				langid.Cat(b, idpos.ptr - b);
				p.SetPos(idpos);
			}
			langlist.Add(langlist.GetCount(), langid);
			VectorMap<String, String> map;
			while(p.Id("txt")) {
				p.PassChar('(');
				String& txt = map.Add("txt" + p.ReadId());
				p.PassChar(',');
				txt = p.ReadString();
				p.PassChar(')');
			}
			p.PassId("#endif");
			for(int i = 0; i < layout.GetCount(); i++) {
				Form& l = layout[i];
				for(int i = 0; i < l.item.GetCount(); i++) {
					ItemInfo& ii = l.item[i];
					ii.label.Add(map.Get(ii.labelid, String()));
					ii.help.Add(map.Get(ii.helpid, String()));
				}
			}
		}
	}
	catch(CParser::Error& e) {
		Exclamation(e);
		filename.Clear();
	}
	MakeCaption();
	SetupLayoutList();
	SetLayout(layout.GetCount() ? 0 : -1, 0);
}

String SaveText(String mod, const Vector<String>& txt, bool lang, const String& name, int ii) {
	mod = '.' + mod +  '(';
	if(lang) {
		for(int i = 0; i < txt.GetCount(); i++)
			if(!txt[i].IsEmpty())
				return mod + name + Format("_%d", ii) + ')';
		return String();
	}
	return txt.GetCount() && !txt[0].IsEmpty() ? mod + AsCString(txt[0]) + ')' : String();
}

String LayoutDesigner::SaveCtrl(const ItemInfo& ii, int i, const String& name, bool lang) const {
	String out;
	if(ii.classname.IsEmpty())
		out << "\tUNTYPED(";
	else
		out << "\tITEM(" << ii.classname << ", ";
	out << (ii.variable.IsEmpty() ? Format("dv___%d", i) : ii.variable) << ", ";
	LogPos p = ii.pos;
	const char *zfix = (qlib2_output ? "" : "Z");
	switch(p.x.GetAlign()) {
	case Ctrl::LEFT:   out << Format("LeftPos%s(%d, %d).", zfix, p.x.GetA(), p.x.GetB()); break;
	case Ctrl::RIGHT:  out << Format("RightPos%s(%d, %d).", zfix, p.x.GetA(), p.x.GetB()); break;
	case Ctrl::SIZE:   out << Format("HSizePos%s(%d, %d).", zfix, p.x.GetA(), p.x.GetB()); break;
	case Ctrl::CENTER: out << Format("HCenterPos%s(%d, %d).", zfix, p.x.GetB(), p.x.GetA()); break;
	}
	switch(p.y.GetAlign()) {
	case Ctrl::TOP:    out << Format("TopPos%s(%d, %d)", zfix, p.y.GetA(), p.y.GetB()); break;
	case Ctrl::BOTTOM: out << Format("BottomPos%s(%d, %d)", zfix, p.y.GetA(), p.y.GetB()); break;
	case Ctrl::SIZE:   out << Format("VSizePos%s(%d, %d)", zfix, p.y.GetA(), p.y.GetB()); break;
	case Ctrl::CENTER: out << Format("VCenterPos%s(%d, %d)", zfix, p.y.GetB(), p.y.GetA()); break;
	}
	out << SaveText("SetLabel", ii.label, lang, "txt" + name, i);
	out << SaveText(qlib2_output ? "HelpC" : "Description", ii.help, lang, "txt" + name + "_HLP", i);
	if(IsLayoutRef(ii.classname) && ii.variable.IsEmpty())
		out << ".Hide()";
	out << ")\n";
	return out;
}

String txtSave(int li, const Vector<String>& txt, int ii, String name) {
	for(int i = 0; i < txt.GetCount(); i++)
		if(!txt[i].IsEmpty()) {
			String s;
			if(li < txt.GetCount())
				s = txt[li];
			return "txt(" + name + Format("_%d", ii) + ", " + AsCString(s) + ")\n";
		}
	return String();
}

String LayoutDesigner::GetLayoutFile() const {
	int i;
	Vector<int> layindex;
	layindex.SetCount(layout.GetCount());
	for(i = 0; i < layout.GetCount(); i++)
		layindex[i] = i;
	int maxswaps = layout.GetCount() * layout.GetCount();
	for(i = 0; maxswaps > 0 && i < layindex.GetCount(); i++)
	{
		int lx = layindex[i];
		const Form& l = layout[lx];
		int maxref = -1;
		String dummy;
		for(int c = 0; c < l.item.GetCount(); c++)
			maxref = max(maxref, FindIndex(layindex, ParseLayoutRef(l.item[c].classname, dummy)));
		if(maxref > i)
		{
			layindex.Remove(i);
			layindex.Insert(maxref, lx);
			i--;
			maxswaps--;
		}
	}

	StringStream out;
	out << "#ifdef LAYOUTFILE\n\n";
	for(i = 0; i < layindex.GetCount(); i++) {
		const Form& l = layout[layindex[i]];
		out << "LAYOUT(" << l.name << ", " << l.size.cx << ", " << l.size.cy << ")\n";
		for(int c = 0; c < l.item.GetCount(); c++)
			out << SaveCtrl(l.item[c], c, l.name, langlist.GetCount());
		out << "END_LAYOUT\n\n";
	}
	out << "#endif\n";
	for(int li = 0; li < langlist.GetCount(); li++) {
		out << "\n#if LANGUAGE == " << String(langlist.GetValue(li)) << "\n\n";
		for(int i = 0; i < layindex.GetCount(); i++) {
			const Form& l = layout[layindex[i]];
			for(int c = 0; c < l.item.GetCount(); c++) {
				out << txtSave(li, l.item[c].label, c, l.name);
				out << txtSave(li, l.item[c].help, c, l.name + "_HLP");
			}
			out << '\n';
		}
		out << "#endif\n";
	}
	return out;
}

bool LayoutDesigner::SaveLayoutFile() {
	String lf = GetLayoutFile();
	if(::LoadFile(filename) != lf && !::SaveFile(filename, lf)) {
		Exclamation("Error storing output file " + filename);
		return false;
	}
	return true;
}

const char exts[] =
	"Layout files (*.lay)|*.lay|"
	"All files (*.*)|*.*|"
;

void LayoutDesigner::MakeCaption() {
	String s;
	if(layouti >= 0) {
		s.Cat(layout[layouti].name);
		if(!s.IsEmpty())
			s.Cat(" - ");
	}
	s.Cat(filename.IsEmpty() ? "New layout file" : filename);
	frame.Title(s);
}

bool LayoutDesigner::DoSave() {
	if(filename.IsEmpty()) {
		if(!SaveLayoutFileAs()) return false;
	}
	else
		if(!SaveLayoutFile()) return false;
	return true;
}

bool LayoutDesigner::PromptSave() {
	return layout.GetCount() == 0
	|| (!filename.IsEmpty() && GetLayoutFile() == LoadFile(filename))
	|| !PromptYesNo("Save current file ?") || DoSave();
}

void LayoutDesigner::NewFile() {
	if(!PromptSave()) return;
	SetLayout(-1);
	layout.Clear();
	filename.Clear();
	frame.Refresh();
	MakeCaption();
	SetupLayoutList();
}

void LayoutDesigner::OpenFile() {
	if(!PromptSave()) return;
	if(filesel.ExecuteOpen()) {
		LoadLayoutFile(~filesel);
		LruList.NewEntry(filename);
	}
}

bool LayoutDesigner::SaveLayoutFileAs() {
	filesel = filename;
	if(!filesel.ExecuteSaveAs(NULL)) return false;
	filename = filesel;
	bool r = SaveLayoutFile();
	if(r) LruList.NewEntry(filename);
	MakeCaption();
	return r;
}

void LayoutDesigner::SaveFileAs() {
	SaveLayoutFileAs();
}

void LayoutDesigner::SaveFile() {
	DoSave();
}

void LayoutDesigner::CloseFile() {
	if(CloseLayoutFile())
	{
#ifdef flagIDERW
		SetTimeCallback(0, THISBACK(CloseDesigner), 555);
#endif
	}
}

bool LayoutDesigner::CloseLayoutFile() {
	if(layout.GetCount() && (filename.IsEmpty() || LoadFile(filename) != GetLayoutFile()))
		switch(PromptYesNoCancel("Save current file ?")) {
		case 1:
			if(!DoSave()) return false;
		case 0:
			break;
		default:
			return false;
		}
	frame.Close();
	return true;
}

void LayoutDesigner::LruOpen(const String& s) {
	if(!PromptSave()) return;
	LoadLayoutFile(s);
}

void LayoutDesigner::Serialize(Stream& s) {
	int version = 1;
	s.Magic();
	s / version;
	s % filesel;
	LruList.Serialize(s);
	splital.Serialize(s);
	if(version == 0) {
		Splitter d;
		d.Serialize(s);
	}
	itab.SerializeHeader(s);
	if(version >= 1)
		frame.SerializePlacement(s);
	else {
		Rect r = frame.GetRect();
		s % r;
		frame.SetRect(r);
	}
	s % grid.x % grid.y;
	s % ignoremin;
	s % matrix.x % matrix.y % matrix.hspace % matrix.vspace;
	s % usegrid;
	s.Magic();
}
