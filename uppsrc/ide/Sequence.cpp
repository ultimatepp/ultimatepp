#include "ide.h"

struct InsertSequenceDlg : public WithSeqLayout<TopWindow> {
	void Sync();

	InsertSequenceDlg();
	~InsertSequenceDlg();
};

void InsertSequenceDlg::Sync()
{
	String h = ~first;
	first.Error(false);
	int64 val = 1;
	String auto_formula = "n + 1";
	int    auto_count = 8;
	int    auto_width = 0;
	int    auto_format = 0;
	auto DoBase = [&] (const String& s, int mul) {
		val = s.GetCount() ? stou64(~s, nullptr, 16) : 1;
		int bitcnt = 0;
		int last = 0;

		for(int i = 0; i < 64; i++) {
			if(((uint64)1 << i) & val) {
				last = i;
				bitcnt++;
			}
		}
		if(bitcnt == 1) {
			if(val == 1 || *s == '0') {
				auto_count = s.GetCount() * mul - last;
				auto_formula = "2 * n";
			}
			else {
				auto_count = last + 1;
				auto_formula = "n / 2";
			}
		}
		else
			auto_formula = "n + 1";
	};
	if(h.TrimStart("0x")) {
		String s = Filter(h, [](int c) { c = ToLower(c); return c >= '0' && c <= '9' || c >= 'a' && c <= 'f' ? c : 0; });
		if(s.GetCount() != h.GetCount())
			first.Error();
		if(s.GetCount() > 16) {
			s.Trim(16);
			first.Error();
		}
		auto_width = s.GetCount() ? s.GetCount() : 8;
		auto_format = 1;
		DoBase(s, 4);
	}
	else
	if(h.TrimStart("0b")) {
		String s = Filter(h, [](int c) { return findarg(c, '0', '1') >= 0 ? c : 0; });
		if(s.GetCount() != h.GetCount())
			first.Error();
		if(s.GetCount() > 64) {
			s.Trim(64);
			first.Error();
		}
		auto_width = s.GetCount() ? s.GetCount() : 8;
		auto_format = 2;
		DoBase(s, 1);
	}
	else {
		String s = Filter(h, CharFilterDigit);
		if(s.GetCount() != h.GetCount())
			first.Error();
		bool overflow = false;
		ScanInt<char, byte, uint64, int64, 10>(val, s, overflow);
		if(overflow) {
			val = 1;
			first.Error();
		}
	}
	
	next.NullText(auto_formula);
	count.NullText(AsString(auto_count));
	width.NullText(AsString(auto_width));
	format.SetValue(0, AttrText(get_i(auto_format, "dec", "hex", "bin")).Italic().NormalInk(SColorDisabled()));
	separator.Enable(line);

	String l;
	next.Error(false);
	try {
		for(int i = 0; i < Nvl((int)~count, auto_count); i++) {
			String m = ~prefix;
			switch(Nvl((int)~format, auto_format)) {
			case 1:  m << "0x" + FormatIntBase(val, 16, Nvl((int)~width, auto_width), '0', -1); break;
			case 2:  m << "0b" + FormatIntBase(val, 2, Nvl((int)~width, auto_width), '0', -1); break;
			default: m << AsString(val); break;
			}
			m << ~postfix;
			String sep = "\n";
			if(line)
				sep = ~separator;
			MergeWith(l, sep, m);
			ArrayMap<String, EscValue> vars;
			vars.Add("n") = val;
			EscValue v = Evaluatex(Nvl(~~next, auto_formula), vars);
			if(v.IsNumber())
				val = v.GetInt64();
		}
	}
	catch(CParser::Error) {
		next.Error();
	}
	list <<= l;
}

INITBLOCK {
	RegisterGlobalConfig("ins-seq");
}

InsertSequenceDlg::InsertSequenceDlg()
{
	CtrlLayoutOKCancel(*this, "Insert sequence");

	list.SetReadOnly();
	
	first.AddList("1");
	first.AddList("0x01");
	first.AddList("0x0001");
	first.AddList("0x000001");
	first.AddList("0x00000001");
	first.AddList("0x80");
	first.AddList("0x8000");
	first.AddList("0x800000");
	first.AddList("0x80000000");
	first.AddList("0b10000000");
	first.AddList("0b00000001");
	first.SetFilter([](int c) {
		int u = ToLower(c);
		return IsDigit(c) ? c : u < 128 && strchr("xabcdef", u) ? u : 0;
	});
	
	next.AddList("n + 1");
	next.AddList("n - 1");
	next.AddList("2 * n");
	next.AddList("n / 2");
	next.SetFilter([](int c) {
		int u = ToLower(c);
		return IsDigit(c) ? c : u < 128 && strchr("xabcdefn+-*/().", u) ? u : 0;
	});
	
	count.AddList(1);
	count.AddList(2);
	count.AddList(3);
	count.AddList(4);
	count.AddList(5);
	count.AddList(8);
	count.AddList(10);
	count.AddList(16);
	count.AddList(20);
	count.AddList(30);
	count.AddList(32);
	count.MinMax(1, 20000);
	
	format.Add(Null, Null);
	format.Add(0, "dec");
	format.Add(1, "hex");
	format.Add(2, "bin");
	
	width.MinMax(1, 128);
	
	separator <<= ", ";
	
	for(Ctrl& q : *this)
		q << [=] { Sync(); };
		
	copy.SetImage(CtrlImg::copy());
	copy << [=] { WriteClipboardText(~list); };
	
	clear << [=] {
		first <<= Null;
		next <<= Null;
		count <<= Null;
		prefix <<= Null;
		format <<= Null;
		width <<= Null;
		postfix <<= Null;
		line <<= false;
		separator <<= ", ";
		Sync();
	};
	
	LoadFromGlobal(*this, "ins-seq");

	Sync();
}

InsertSequenceDlg::~InsertSequenceDlg()
{
	StoreToGlobal(*this, "ins-seq");
}

void Ide::InsertSequence()
{
	InsertSequenceDlg dlg;
	if(dlg.ExecuteOK()) {
		if(dlg.line)
			editor.Paste(~dlg.list);
		else {
			int64 lp = editor.GetPos(editor.GetCurrentLine(), 0);
			WString indent = editor.GetW((int)lp, int(editor.GetCursor() - lp)); // try to maintain the indent
			indent = Filter(indent, [](int c)->int { return c == '\t' ? '\t' : ' '; });
			WString txt = Filter(dlg.list.GetW(), [](int c) { return c == '\r' ? 0 : c; });
			txt.Replace("\n", "\n" + indent);
			editor.Paste(txt);
		}
	}
}
