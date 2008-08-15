#include <RichEdit/RichEdit.h>

struct DashEdit : public RichEdit {
	virtual void SetData(const Value& data);
	virtual Value GetData() const;

	Button dash;

	void Dash();

	typedef DashEdit CLASSNAME;

	DashEdit();
};

void DashEdit::Dash()
{
	RichTxt::FormatInfo f = GetFormatInfo();
	f.charvalid = RichText::DASHED;
	f.dashed = !f.dashed;
	ApplyFormatInfo(f);
}

DashEdit::DashEdit()
{
	dash <<= THISBACK(Dash);
	NoRuler();
}

void DashEdit::SetData(const Value& data)
{
	RichText text;
	String d = data;
	RichPara para;
	RichPara::Format f;
	for(const char *s = d; *s; s++)
		if(*s == '\n') {
			text.Cat(para);
			para.part.Clear();
		}
		else
		if(*s == '~')
			f.dashed = !f.dashed;
		else
		if((byte)*s >= ' ')
			para.Cat(String(*s, 1).ToWString(), f);
	text.Cat(para);
	Pick(text);
}

Value DashEdit::GetData() const
{
	const RichText& txt = Get();
	String r;
	for(int i = 0; i < txt.GetPartCount(); i++) {
		if(i)
			r << "\r\n";
		if(txt.IsPara(i)) {
			RichPara p = txt.Get(i);
			for(int j = 0; j < p.GetCount(); j++)
				if(p.part[j].format.dashed) {
					r << '~';
					r << p.part[j].text;
					r << '~';
				}
				else
					r << p.part[j].text;
		}
	}
	return r;
}

GUI_APP_MAIN
{
	TopWindow win;
	DashEdit edit;
	win.Add(edit.SizePos());
	edit.Add(edit.dash.RightPos(0, 20).BottomPos(0, 20));
	edit <<= "This is just a ~test~\nof ~dashing\nelements";
	win.Run();
	DUMP(edit.GetData());
}
