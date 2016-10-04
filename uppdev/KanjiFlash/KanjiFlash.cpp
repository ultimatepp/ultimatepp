#include "KanjiFlash.h"

struct KanjiDisplay : Display
{
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		w.DrawText(r.left + 2, r.top, ValueTo<String>(q), StdFont().Height(16), ink);
	}
};

KanjiFlash::KanjiFlash()
{
	CtrlLayout(*this, "Window title");

	lstKanji.ItemHeight(GetTextSize("Miu", StdFont().Height(16)).cy);
	lstKanji.SetDisplay(Single<KanjiDisplay>());

	arrKanji.ItemHeight(GetTextSize("Miu", StdFont().Height(16)).cy);
	arrKanji.SetDisplay(Single<KanjiDisplay>());

	miu.ItemHeight(GetTextSize("Miu", StdFont().Height(16)).cy);
	miu.SetDisplay(Single<KanjiDisplay>());

	btnOpen <<= THISBACK(Open);
	lstKanji <<= THISBACK(Select);
	btnClear <<= THISBACK(Clear);

	treKanji <<= THISBACK(TreeClick);
}

void KanjiFlash::Open()
{
	if(!fs.ExecuteOpen()) return;
	filename = fs;
	lstKanji.Clear();
	try {
		String s = LoadFile(filename);
		XmlParser p(s);
		while(!p.IsTag())
			p.Skip();
		p.PassTag("kanjidic2");
		while(!p.End())
			if(p.Tag("header"))
				while(!p.End()) {
					if(p.Tag("file_version"))
						lblVer = p.ReadTextE();
					else
					if(p.Tag("database_version"))
						lblDBVer = p.ReadTextE();
					else
					if(p.Tag("date_of_creation"))
						lblDBDate = p.ReadTextE();
					else
						p.Skip();
				}
			else
			if(p.Tag("character")) {
				Kanji kanji;
				while(!p.End())
					if(p.Tag("literal"))
						kanji.Literal(p.ReadTextE());
					else
					if(p.Tag("misc"))
						while(!p.End()) {
							if(p.Tag("grade"))
								kanji.Grade(StrInt(p.ReadTextE()));
							else
							if(p.Tag("stroke_count"))
								kanji.StrokeCount(StrInt(p.ReadTextE()));
							else
								p.Skip();
						}
					else
					if(p.Tag("dic_number"))
						while(!p.End()) {
							if(p.Tag("dic_ref"))
							{
								kanji.SetDictIndex(0, StrInt(p.ReadTextE()));
							}
							else
								p.Skip();
						}
					else
						p.Skip();
				lstKanji.Add(kanji.Literal());
				list.Add(kanji);
			}
			else
				p.Skip();
	}
	catch(XmlError) {
		Exclamation("Error reading the input file!");
	}
	lblCount.SetText(IntStr(lstKanji.GetCount()));
	PopulateTree();
}

void KanjiFlash::PopulateTree()
{
	treKanji.SetRoot(Null, "Kanji");
	treKanji.Add(0, Null, "All");
	int jy = treKanji.Add(0, Null, "Jouyou");
	treKanji.Open(jy);
	treKanji.Add(0, Null, "Remebering the Kanji");

	for (int i = 1; i <= 6; i++)
		treKanji.Add(jy, Null, "Grade " + IntStr(i));
	treKanji.Add(jy, Null, "Superior");
	treKanji.Add(jy, Null, "Non-jouyou");

	treKanji.Open(0);
}

void KanjiFlash::TreeClick()
{
	if (treKanji.GetCursor() < 0)
		return;

	arrKanji.Clear();

	String s = treKanji.GetValue(treKanji.GetCursor());

	if (s == "Grade 1")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 1)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Grade 2")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 2)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Grade 3")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 3)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Grade 4")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 4)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Grade 5")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 5)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Grade 6")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].Grade() == 6)
				arrKanji.Add(list[i].Literal());
	}
	else if (s == "Remebering the Kanji")
	{
		for (int i = 0; i < list.GetCount(); i++)
			if (list[i].GetDictIndex(0) > 0 && list[i].GetDictIndex(0) <= 20)
				arrKanji.Add(list[i].Literal());
	}
}

void KanjiFlash::Select()
{
	if (lstKanji.GetCursor() < 0)
		return;

	Kanji& kanji = list[lstKanji.GetCursor()];

	lblLit.SetText(kanji.Literal());
	lblGrade.SetText(kanji.GradeAsString());
	lblSC.SetText(IntStr(kanji.StrokeCount()));
}

GUI_APP_MAIN
{
	KanjiFlash().Run();
}
