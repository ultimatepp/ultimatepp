#include "LayDes.h"

struct EditStringNE : EditString {
	virtual bool Key(dword key, int rep) {
		return key == K_ENTER ? true : EditString::Key(key, rep);
	}
};

struct LTProperty : public ItemProperty, public Convert {
	EditStringNE           lid;
	ButtonOption           context, id;
	byte                   charset;

	void Id();
	void Context();
	void EditAction();
	void SyncLid();

	virtual void     SetCharset(byte charset);
	virtual void     Read(CParser& p);
	virtual String   Save() const;
	virtual void     Set(const WString& text) = 0;
	virtual WString  Get() const = 0;
	virtual int      Filter(int chr) const;
	virtual Value    Format(const Value& q) const      { return q; }
	virtual Value    Scan(const Value& text) const     { return text; }

	typedef LTProperty CLASSNAME;

	LTProperty();
};

int FilterOutCr(int c)
{
	return c == '\r' ? 0 : c;
}

String NoCr(const String& s)
{
	return Filter(s, FilterOutCr);
}

int  LTProperty::Filter(int chr) const
{
	return charset == CHARSET_UTF8 ? chr : FromUnicode(chr, charset) == DEFAULTCHAR ? 0 : chr;
}

void LTProperty::EditAction()
{
	UpdateActionRefresh();
}

LTProperty::LTProperty()
{
	context = LayImg::Context();
	context.WhenAction = THISBACK(Context);
	id = LayImg::Id();
	id.WhenAction = THISBACK(Id);
	charset = CHARSET_UTF8;
}

void LTProperty::SyncLid()
{
	lid.Enable(context || id);
}

void LTProperty::Id()
{
	if(id)
		context = false;
	SyncLid();
	if(lid.IsEnabled())
		lid.SetFocus();
}

void LTProperty::Context()
{
	if(context)
		id = false;
	SyncLid();
	if(lid.IsEnabled())
		lid.SetFocus();
}

void LTProperty::SetCharset(byte cs)
{
	charset = cs;
}

void LTProperty::Read(CParser& p)
{
	lid <<= Null;
	if(p.Id("t_")) {
		p.PassChar('(');
		String q = p.ReadString();
		p.PassChar(')');
		for(const char *s = ~q; *s; s++) {
			if(*s == '\v' && s[1] != '\v') {
				context = true;
				lid <<= String(~q, s);
				Set(ToUnicode(s + 1, charset));
				SyncLid();
				return;
			}
			if(*s == '\a' && s[1] != '\a') {
				id = true;
				lid <<= String(~q, s);
				Set(ToUnicode(s + 1, charset));
				SyncLid();
				return;
			}
		}
		Set(ToUnicode(q, charset));
	}
	else
	if(p.IsString())
		Set(ToUnicode(NoCr(p.ReadString()), CHARSET_WIN1250));
	else
		Set(Null);
	SyncLid();
}

String LTProperty::Save() const
{
	String px = ~lid;
	String txt = FromUnicode(Get(), charset);
	if(id)
		return "t_(" + AsCString(px + "\a" + txt) + ')';
	else
	if(context)
		return "t_(" + AsCString(px + "\v" + txt) + ')';
	else
		return "t_(" + AsCString(txt) + ')';
}

template <class Editor>
struct TextEditProperty : public LTProperty {
	virtual Value    GetData() const                { return Get(); }
	virtual WString  Get() const                    { return ~editor; }
	virtual void     Set(const WString& text)       { editor <<= text; }

	virtual bool     PlaceFocus(dword k, int c)     { editor.SetFocus(); return editor.Key(k, c); }

protected:
	Editor     editor;
	FrameBottom<ParentCtrl> p;

	TextEditProperty() {
		editor.WhenAction = callback(this, &TextEditProperty::EditAction);
		int c = EditField::GetStdHeight();
		p.Height(c);
		editor.AddFrame(p);
		p.Add(lid.HSizePos(0, 2 * c).TopPos(0, c));
		p.Add(id.TopPos(0, c).RightPos(0, c));
		p.Add(context.TopPos(0, c).RightPos(c, c));
	}
};

template <class Editor>
struct SmartTextEditProperty : public TextEditProperty<Editor> {
	typedef TextEditProperty<Editor> B;
	virtual WString  Get() const {
		WString text = ~B::editor;
		WString r;
		for(const wchar *s = text; *s; s++) {
			if(*s == '\\' && s[1] == '1') {
				r.Cat('\1');
				s++;
			}
			else
//			if(*s == '\\' && s[1] == 'b') {
//				r.Cat('\b');
//				s++;
//			}
//			else
			if(*s == '\n' || *s >= ' ')
				r.Cat(*s);
		}
		return r;
	}
	virtual void Set(const WString& text) {
		WString r;
		for(const wchar *s = text; *s; s++) {
			if(*s == '\1')
				r.Cat("\\1");
			else
//			if(*s == '\b')
//				r.Cat("\\b");
//			else
			if(*s == '\n' || *s >= ' ')
				r.Cat(*s);
		}
		B::editor.SetCharset(B::charset);
		B::editor <<= r;
	}
	void Qtf() {
		String text = B::editor.GetData();
		if(text.TrimStart("\\1"))
			text.TrimStart("[g ");
		else
			text = "\1" + text;
		QTFEdit(text);
		B::editor.SetData("\\1[g " + text);
		B::EditAction();
	}
};

struct TextProperty : public SmartTextEditProperty<EditString>
{
	Button qtf;

	virtual int  GetHeight() const {
		return 2 * EditField::GetStdHeight() + 6;
	}
	
	virtual void     AdjustLabelWidth(int cx)   { editor.HSizePos(cx, 0); }
	virtual bool     InlineEditor() const       { return true; }

	TextProperty() {
		Add(editor.HSizePosZ(100, 0).TopPos(2));
		qtf.SetLabel("Qtf");
		editor.SetConvert(*this);

		int c = EditField::GetStdHeight();
		int w = GetTextSize("Qtf", StdFont()).cx + DPI(2);
		p.Add(lid.TopPos(0, c).HSizePos(0, 3 * w));
		p.Add(id.TopPos(0, c).RightPos(w, w));
		p.Add(context.TopPos(0, c).RightPos(2 * w, w));
		p.Add(qtf.TopPos(0, c).RightPos(0, w));
		
		qtf << [=] {
			Qtf();
		};
	}

	static ItemProperty *Create() { return new TextProperty; }
};

struct DocProperty : public SmartTextEditProperty<DocEdit>
{
	Button  large;
	Button  qtf;

	virtual int      GetHeight() const {
		return EditField::GetStdHeight() + 6 + Zy(120) + 1;
	}

	void LargeEdit();

	typedef DocProperty CLASSNAME;

	DocProperty() {
		editor.UpDownLeave();
		Add(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, Zy(120)));
		Add(large.RightPosZ(2, 24).TopPos(2, DPI(16)));
		large.SetLabel("...");
		Add(qtf.RightPosZ(26, 24).TopPos(2, DPI(16)));
		qtf.SetLabel("Qtf");
		qtf << [=] { Qtf(); };
		large <<= THISBACK(LargeEdit);
	}

	static ItemProperty *Create() { return new DocProperty; }
};

Rect s_texteditorpos;

void DocProperty::LargeEdit()
{
	editor.SetFocus();
	TopWindow w;
	w.NoCenter();
	w.Sizeable();
	w.SetRect(s_texteditorpos);
	w.Title(name.ToWString());
	DocEdit edit;
	edit.SetFont(LayFont2());
	edit <<= GetData();
	w.Add(edit.SizePos());
	w.Run();
	editor.SetData(~edit);
	s_texteditorpos = w.GetRect();
	EditAction();
}

struct QtfProperty : public TextEditProperty<RichTextView>
{
	Button  large;

	virtual int    GetHeight() const           { return EditField::GetStdHeight() + 6 + 120 + 1; }

	void LargeEdit();
	void QtfEdit();

	typedef QtfProperty CLASSNAME;

	QtfProperty() {
		Add(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, Zy(120)));
		Add(large.RightPosZ(2, 16).TopPos(2, Zy(16)));
		large.SetLabel("...");
		large <<= THISBACK(LargeEdit);
		editor.SetZoom(Zoom(1, 7));
		defval = ~editor;
	}

	static ItemProperty *Create() { return new QtfProperty; }
};

void QtfProperty::LargeEdit()
{
	String text = GetData();
	if(text.StartsWith("\\1"))
		text = text.Mid(2);
	QTFEdit(text);
	editor.SetData("\\1" + text);
	EditAction();
/*
	large.SetFocus();
	TopWindow w;
	w.NoCenter();
	w.Sizeable();
	w.Title(name.ToWString());
	w.SetRect(s_texteditorpos);
	RichEditWithToolBar edit;
	edit <<= GetData();
	w.Add(edit.SizePos());
	w.Run();
	editor.SetData(~edit);
	s_texteditorpos = w.GetRect();
*/
}

void RegisterTextProperties() {
	ItemProperty::Register("Text", TextProperty::Create);
	ItemProperty::Register("Qtf", QtfProperty::Create);
	ItemProperty::Register("Doc", DocProperty::Create);
}
