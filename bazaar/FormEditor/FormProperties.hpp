#ifndef FORM_PROPERTIES_HPP
#define FORM_PROPERTIES_HPP

class OptionDisplay : public GridDisplay
{
public:
	OptionDisplay()
	{
		StaticRect wnd;
		wnd.Color(::SColorPaper());
		Option f;

		wnd.SetRect( Rect(Point(0, 0), f.GetStdSize()) );
		wnd.Add(f.SizePos());
		f.Set(0);

		ImageDraw w1(wnd.GetSize());
		wnd.DrawCtrl(w1);
		ImageBuffer buf1(w1);
		_Option[0] = buf1;

		f.Set(1);

		ImageDraw w2(wnd.GetSize());
		wnd.DrawCtrl(w2);
		ImageBuffer buf2(w2);
		_Option[1] = buf2;

		wnd.RemoveChild(&f);

		_Button.EdgeStyle();
		_Button.SetLabel(t_("Change tabs..."));
	}

private:
	virtual void Paint(Draw& w, int x, int y, int cx, int cy, const Value& val, dword style,
		Color& fg, Color& bg, Font& fnt, bool found = false, int fs = 0, int fe = 0)
	{
		if (val.GetType() == COLOR_V)
		{
			Color b = val;
			GridDisplay::Paint(w, x, y, cx, cy, Value(), style, fg, b, fnt, found,
				fs, fe);
			return;
		}
		bool button = false;
		if (val.ToString().Left(6) == "[Tabs]")
		{
			button = true;
			_Button.SetLabel(t_("Change tabs..."));
		}

		if (val.ToString().Left(9) == "[Columns]")
		{
			button = true;
			_Button.SetLabel(t_("Change columns..."));
		}

		if (button)
		{
			StaticRect wnd;
			wnd.Add(_Button.SizePos());
			wnd.SetRect( Rect(Point(0, 0), Size(cx, cy)) );
			ImageDraw w3(Size(cx, cy));
			wnd.DrawCtrl(w3);
			ImageBuffer buf(w3);
			w.DrawImage(x, y, buf);
		}
		else
			GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, fnt, found, fs, fe);
	}

	Image _Option[2];
	Button _Button;
};

class TypeDisplay : public GridDisplay
{
	virtual void Paint(Draw& w, int x, int y, int cx, int cy, const Value& val, dword style,
		Color& fg, Color& bg, Font& fnt, bool found = false, int fs = 0, int fe = 0)
	{
		bg = SColorPaper();
		fg = SColorText();
		GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, fnt, found, fs, fe);
	}
};

class PropertiesWindow : public TopWindow
{
	typedef PropertiesWindow CLASSNAME;

public:
	PropertiesWindow();

	void Generate(FormObject* pI, int index);

	void Property(const String& id, const String& label, const String& type,
		const Array<String>& prop);

	void Property(const String& id, const String& label, const String& type,
		const Color& c);

	void Clear()
	{
		_Headers.EndEdit();
		_Options.EndEdit();
		_Headers.Clear();
		_Options.Clear();
		_Index = -1;
		_Item  = NULL;
		_Properties.Clear();
	}

	void OnStartEdit();
	void OnEndEdit(int mode);

	Callback WhenChildZ;
	ExGridCtrl _Headers;
	ExGridCtrl _Options;
	OptionDisplay _Display;
	TypeDisplay _TypeDisplay;
	DropList _DropList;

protected:
	ArrayMap<String, Ctrl> _Properties;
	FormObject* _Item;
	int _Index;
};

#endif // .. FORM_PROPERTIES_HPP
