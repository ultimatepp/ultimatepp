#include "RichEdit.h"

namespace Upp {

struct SelectSymbolDlg : WithSelectSymbolLayout<TopWindow> {
	SelectSymbolDlg();
	
	void Sync();
	void Variants(int codepoint);
};

SelectSymbolDlg::SelectSymbolDlg()
{
	CtrlLayoutOKCancel(*this, "Insert");
	search.NullText(t_("Search"));
	search.SetFilter(CharFilterDefaultToUpperAscii);
	group <<= 0;
	search ^= group ^= [=] { Sync(); };

	symbols.NoHyperlinkDecoration();
	symbols.WhenLink << [=](const String& s) { Variants(Atoi(s)); };
}

void SelectSymbolDlg::Sync()
{
	const VectorMap<String, Vector<Tuple<int, String>>>& syms = UnicodeSymbols();

	int g = ~group;
	String s = ToLower(~~search);
	String qtf = "[A5 ";
	for(int i = 0; i < syms.GetCount(); i++)
		if(g == 0 || g == i + 1)
			for(Tuple<int, String> h : syms[i])
				if(h.b.Find(s) >= 0 && StdFont().HasChar(h.a))
					qtf << "[^" << AsString(h.a) << "^ " << WString(h.a, 1) << "] ";
	symbols.SetQTF(qtf);
}

void SelectSymbolDlg::Variants(int codepoint)
{
	String qtf = "[A5 ";
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 10);
		if(HasCodepoint(fnt, codepoint)) {
			qtf << "[!" << fnt.GetFaceName() << "! " << WString(codepoint, 1) << "] ";
		}
	}
	DDUMP(qtf);
	variants.SetQTF(qtf);
}

bool SelectUnicodeSymbol(int& codepoint, Font& font)
{
	SelectSymbolDlg dlg;
	dlg.Sync();
	dlg.Run();
	return true;
}

}