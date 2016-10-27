#include <RichEdit/RichEdit.h>

using namespace Upp;

struct MyRichObjectType : public RichObjectType
{
	virtual String GetTypeName(const Value&) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual bool   IsText() const;
	virtual void   Menu(Bar& bar, RichObject& ex, void *context) const;
	virtual void   DefaultAction(RichObject& ex) const;
	
	void Edit(RichObject& ex) const;
	
	typedef MyRichObjectType CLASSNAME;
};

bool MyRichObjectType::IsText() const
{
	return true;
}

String MyRichObjectType::GetTypeName(const Value&) const
{
	return "mytype";
}

void MyRichObjectType::Paint(const Value& data, Draw& w, Size sz) const
{
	w.DrawRect(sz, White);
	Font fnt = Roman(max(2, sz.cy - 2)).Bold();
	w.DrawText(2, -2, (String)data, fnt, SColorDisabled());
	w.DrawText(0, 0, (String)data, fnt, SColorText());
}

void MyRichObjectType::Edit(RichObject& ex) const
{
	String txt = ex.GetData();
	if(EditText(txt, "Edit MyRichObject", "Text"))
		ex.SetData(txt);
}

void MyRichObjectType::DefaultAction(RichObject& ex) const
{
	Edit(ex);
}

void MyRichObjectType::Menu(Bar& bar, RichObject& ex, void *) const
{
	bar.Add("Edit object..", [=, &ex] { Edit(ex); });
}

INITBLOCK {
	RichObject::Register("mytype", &Single<MyRichObjectType>());
};

String FileName()
{
	return ConfigFile("test.qtf");
}

class MyRichEdit : public RichEdit {
	ToolBar  toolbar;
	bool     extended;
	void RefreshBar();
	void InsertMy();

public:
	typedef MyRichEdit CLASSNAME;

	MyRichEdit();
};

void MyRichEdit::InsertMy()
{
	String txt;
	if(EditText(txt, "New MyRichObject", "Text")) {
		RichObject obj(&Single<MyRichObjectType>(), txt);
		obj.SetSize(1000, 300);
		obj.KeepRatio(false);
		PasteText(AsRichText(obj));
	}
}

void MyRichEdit::RefreshBar()
{
	toolbar.Set([=](Bar& bar) {
		DefaultBar(bar, false);
		bar.Add(!IsReadOnly(), "Insert new MyRichObject", CtrlImg::Plus(), THISFN(InsertMy));
	});
}

MyRichEdit::MyRichEdit()
{
	InsertFrame(0, toolbar);
	WhenRefreshBar = [=] { RefreshBar(); };
}

GUI_APP_MAIN
{
	MyRichEdit editor;

	String file = LoadFile(FileName());
	if(file.IsEmpty())
		editor.SetQTF("Some line&Here comes the RichObject: @@mytype:1200*200`Hello world!`&Next line");
	else
		editor.Pick(ParseQTF(LoadFile(FileName())));

	TopWindow w;
	w.SetRect(0, 0, 700, 500);
	w.Sizeable().Zoomable();
	w.Add(editor.SizePos());
	w.Run();

	SaveFile(FileName(), editor.GetQTF());
}
