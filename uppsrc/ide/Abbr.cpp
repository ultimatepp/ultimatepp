#include "ide.h"

class AbbreviationsDlg : public WithAbbreviationsLayout<TopWindow> {
	typedef AbbreviationsDlg CLASSNAME;
	EditString keyword;
	
	void Add();
	void Remove();
	void Edit();
	void Sync();
	void Menu(Bar& bar);
	void Finish(const String& s);

public:
	AbbreviationsDlg(const String& hlStyles);
};

void AbbreviationsDlg::Sync()
{
	edit.Enable(abbr.IsCursor());
	remove.Enable(abbr.IsCursor());
}

void AbbreviationsDlg::Menu(Bar& bar)
{
	bar.Add("New..", THISBACK(Add));
	bar.Add(abbr.IsCursor(), "Edit..", THISBACK(Edit));
	bar.Add(abbr.IsCursor(), "Delete", THISBACK(Remove));
}

void AbbreviationsDlg::Finish(const String& s)
{
	abbr.Sort();
	Sync();
	abbr.FindSetCursor(s);
}

void AbbreviationsDlg::Add()
{
	String s;
	if(EditText(s, "New abbreviation", "Keyword", CharFilterAlpha)) {
		abbr.Add(s, Null);
		Finish(s);
	}
}

void AbbreviationsDlg::Remove()
{
	if(abbr.IsCursor() && PromptYesNo("Remove abbreviation?"))
		abbr.Remove(abbr.GetCursor());
	Sync();
}

void AbbreviationsDlg::Edit()
{
	if(!abbr.IsCursor())
		return;
	String s = abbr.GetKey();
	if(EditText(s, "Edit keyword", "Keyword", CharFilterAlpha)) {
		abbr.Set(0, s);
		Finish(s);
	}
}

AbbreviationsDlg::AbbreviationsDlg(const String& hlStyles)
{
	CtrlLayoutOKCancel(*this, "Abbreviations");
	abbr.AddColumn("Keyword").Edit(keyword);
	abbr.AddCtrl(code);
	abbr.WhenBar = THISBACK(Menu);
	add <<= THISBACK(Add);
	remove <<= THISBACK(Remove);
	edit <<= THISBACK(Edit);
	code.Highlight("cpp");
	code.ShowTabs();
	code.LoadHlStyles(hlStyles);
	Sync();
}

void Ide::Abbreviations()
{
	AbbreviationsDlg dlg(editor.StoreHlStyles());
	for(int i = 0; i < abbr.GetCount(); i++)
		dlg.abbr.Add(abbr.GetKey(i), abbr[i]);
	if(dlg.Execute() != IDOK)
		return;
	abbr.Clear();
	for(int i = 0; i < dlg.abbr.GetCount(); i++)
		abbr.Add(dlg.abbr.Get(i, 0), dlg.abbr.Get(i, 1));
	SaveAbbr();
}

void Ide::LoadAbbr()
{
	abbr.Clear();
	String s = LoadFile(ConfigFile("ide.abbrs"));
	try {
		CParser p(s);
		while(!p.IsEof()) {
			String a = p.ReadString();
			p.Char('=');
			String b = p.ReadString();
			p.Char(';');
			abbr.Add(a, b);
		}
	}
	catch(CParser::Error) {}
	if(abbr.GetCount() == 0) {
		abbr.Add("c", "case @:\r\n\tbreak;");
		abbr.Add("d", "do\r\n\t@;\r\nwhile();");
		abbr.Add("db", "do {\r\n\t@;\r\n}\r\nwhile();");
		abbr.Add("e", "else\r\n\t;");
		abbr.Add("eb", "else {\r\n\t;\r\n}");
		abbr.Add("ei", "else\r\nif(@)\r\n\t;");
		abbr.Add("eib", "else\r\nif(@) {\r\n\t;\r\n}");
		abbr.Add("f", "for(@;;)\r\n\t;");
		abbr.Add("fb", "for(@;;) {\r\n\t\r\n}");
		abbr.Add("fi", "for(int i = 0; i < @.GetCount(); i++)");
		abbr.Add("fib", "for(int i = 0; i < @.GetCount(); i++) {\r\n\t\r\n}");
		abbr.Add("fij", "for(int i = 0; i < @.GetCount(); i++)\tfor(int j = 0; j < .GetCount(); j++)");
		abbr.Add("fj", "for(int j = 0; j < @.GetCount(); j++)");
		abbr.Add("fjb", "for(int j = 0; j < @.GetCount(); j++) {\r\n\t\r\n}");
		abbr.Add("fk", "for(int k = 0; k < @.GetCount(); k++)");
		abbr.Add("fkb", "for(int k = 0; k < @.GetCount(); k++) {\r\n\t\r\n}");
		abbr.Add("i", "if(@)\r\n\t;");
		abbr.Add("ib", "if(@) {\r\n\t\r\n}");
		abbr.Add("ie", "if(@)\r\n\t;\r\nelse\r\n\t;");
		abbr.Add("ieb", "if(@) {\r\n\t\r\n}\r\nelse {\r\n\t\r\n}");
		abbr.Add("sw", "switch(@) {\r\ncase :\r\n\tbreak;\r\ndefault:\r\n\tbreak;\r\n}");
		abbr.Add("w", "while(@)\r\n\t;");
		abbr.Add("wb", "while(@) {\r\n\t\r\n}");
		abbr.Add("ff", "for(FindFile ff(@); ff; ff.Next())");
		abbr.Add("ffb", "for(FindFile ff(@); ff; ff.Next()) {\r\n\t\r\n}");
	}
}

void Ide::SaveAbbr()
{
	String r;
	for(int i = 0; i < abbr.GetCount(); i++)
		r << AsCString(abbr.GetKey(i)) << '=' << AsCString(abbr[i]) << ";\r\n";
	UPP::SaveFile(ConfigFile("ide.abbrs"), r);
}
