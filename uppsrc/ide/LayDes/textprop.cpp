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
	return charset == CHARSET_UNICODE ? chr : FromUnicode(chr, charset) == DEFAULTCHAR ? 0 : chr;
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
	charset = CHARSET_UNICODE; //!! not good, but better than a crash; TRC 06/04/10//TODO
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
	String txt = FromUnicode((WString)Get(), charset);
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
};

struct TextProperty : public SmartTextEditProperty<EditString>
{
	virtual int      GetHeight() const {
		return 2 * EditField::GetStdHeight() + 6;
	}

	TextProperty() {
		Add(editor.HSizePosZ(100, 2).TopPos(2));
		editor.SetConvert(*this);
	}

	static ItemProperty *Create() { return new TextProperty; }
};

struct DocProperty : public SmartTextEditProperty<DocEdit>
{
	Button  large;

	virtual int      GetHeight() const {
		return EditField::GetStdHeight() + 6 + 120 + 1;
	}

	void LargeEdit();

	typedef DocProperty CLASSNAME;

	DocProperty() {
		editor.UpDownLeave();
		Add(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, 120));
		Add(large.RightPosZ(2, DPI(16)).TopPos(2, DPI(16)));
		large.SetLabel("...");
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
}

struct QtfProperty : public TextEditProperty<RichTextView>
{
	Button  large;

	virtual int    GetHeight() const           { return EditField::GetStdHeight() + 6 + 120 + 1; }

	void LargeEdit();

	typedef QtfProperty CLASSNAME;

	QtfProperty() {
		Add(editor.HSizePosZ(2, 2).TopPos(EditField::GetStdHeight() + 2, Zy(120)));
		Add(large.RightPosZ(2, 16).TopPos(2, 16));
		large.SetLabel("...");
		large <<= THISBACK(LargeEdit);
		editor.SetZoom(Zoom(1, 7));
		defval = ~editor;
	}

	static ItemProperty *Create() { return new QtfProperty; }
};

void QtfProperty::LargeEdit()
{
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
}

void RegisterTextProperties() {
	ItemProperty::Register("Text", TextProperty::Create);
	ItemProperty::Register("Qtf", QtfProperty::Create);
	ItemProperty::Register("Doc", DocProperty::Create);
}
