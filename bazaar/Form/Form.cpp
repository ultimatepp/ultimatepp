#include "Form.hpp"
#include "IniConfig.hpp"

static int tempFormsCount = 0;

Form:: Form() : _Current(-1) {}
Form::~Form() { Clear(); }

bool Form::Load(const String& file)
{
	Clear();

	if (!FileExists(file))
		return false;

	if (GetFileExt(file) == ".fz")
	{
		String s = ZDecompress(LoadFile(file));
		if (!LoadFromXML(*this, s))
			return false;
		_File = file;
		return true;
	}

	if (!LoadFromXMLFile(*this, file))
		return false;
	_File = file;
	return true;
}

bool Form::Layout(const String& layout, Font font)
{
	_Current = -1;
	for (int i = 0; i < _Layouts.GetCount(); ++i)
		if (_Layouts[i].Get("Form.Name") == layout)
		{
			_Current = i;
			return Generate(font);
		}
	return false;
}

bool Form::Generate(Font font)
{
	if (!IsLayout())
		return false;

	Clear(false);

	Size sz = _Layouts[_Current].GetFormSize();
	SetRect( Rect(Point(0, 0), Size(HorzLayoutZoom(sz.cx), VertLayoutZoom(sz.cy))) );

	Vector<FormObject>* p = &_Layouts[_Current].GetObjects();

	if (_Layouts[_Current].GetBool("Form.MinimizeBox", false)) MinimizeBox();
	if (_Layouts[_Current].GetBool("Form.MaximizeBox", false)) MaximizeBox();

#ifdef PLATFORM_WIN32
	if (_Layouts[_Current].GetBool("Form.ToolWindow", false)) ToolWindow();
#endif

	if (_Layouts[_Current].GetBool("Form.Sizeable", false)) Sizeable();

	Title(_Layouts[_Current].Get("Form.Title"));

	for (int i = 0; i < p->GetCount(); ++i)
	{
		Font objFont = font;
		int h = (*p)[i].GetNumber("Font.Height", 0);
		if (h != 0) objFont.Height( VertLayoutZoom(h) );
		if (font.GetHeight() == 0) objFont.Height( StdFont().GetHeight() );
		Ctrl *c = NULL;

		if ((*p)[i].Get("Type") == "Button")
		{
			Button *b = &_Ctrls.Create<Button>((*p)[i].Get("Variable"));
			b->SetFont(objFont);
			b->SetLabel((*p)[i].Get("Label"));
			b->Tip((*p)[i].Get("Tip"));
			*b <<= THISBACK1(OnAction, (*p)[i].Get("Action"));
			c = b;
		}

		if ((*p)[i].Get("Type") == "EditField")
		{
			EditField *e = &_Ctrls.Create<EditField>((*p)[i].Get("Variable"));
			e->SetFont(objFont);
			if ((*p)[i].Get("Style") == "Password")
				e->Password();
			e->Tip((*p)[i].Get("Tip"));
			if ((*p)[i].Get("TextAlign") == "Right")
				e->AlignRight();

			c = e;
		}

		if ((*p)[i].Get("Type") == "Label")
		{
			Label *e = &_Ctrls.Create<Label>((*p)[i].Get("Variable"));
			e->SetFont(objFont);
			Color fontColor = DefaultInk();
			LoadFromString(fontColor, Decode64((*p)[i].Get("Font.Color",
				StoreAsString(fontColor))));
			e->SetInk(fontColor);
			String align = (*p)[i].Get("Text.Align");
			if (align == "Center") e->SetAlign(ALIGN_CENTER);
			if (align == "Right") e->SetAlign(ALIGN_RIGHT);
			if (align == "Left") e->SetAlign(ALIGN_LEFT);
			e->SetLabel((*p)[i].Get("Label"));
			c = e;
		}

		if ((*p)[i].Get("Type") == "DropDate")
		{
			DropDate *e = &_Ctrls.Create<DropDate>((*p)[i].Get("Variable"));
			e->SetFont(objFont);
			c = e;
		}

		if ((*p)[i].Get("Type") == "ProgressBar")
		{
			ProgressIndicator *e = &_Ctrls.Create<ProgressIndicator>((*p)[i].Get("Variable"));
			e->Set(
				(*p)[i].GetNumber("ProgressBar.Initial", 0),
				(*p)[i].GetNumber("ProgressBar.Total", 100));
			c = e;
		}

		if ((*p)[i].Get("Type") == "EditInt")
		{
			EditInt *e = &_Ctrls.Create<EditInt>((*p)[i].Get("Variable"));
			e->SetFont(objFont);
			e->Min((*p)[i].GetNumber("Min", INT_MIN, INT_MAX, INT_MIN));
			e->Max((*p)[i].GetNumber("Max", INT_MIN, INT_MAX, INT_MAX));
			c = e;
		}

		if ((*p)[i].Get("Type") == "GridCtrl")
		{
			GridCtrl *e = &_Ctrls.Create<GridCtrl>((*p)[i].Get("Variable"));
			e->Chameleon();
			c = e;

			String src = (*p)[i].Get("Grid.Columns");
			ReplaceString(src, ";", "\r\n");
			StringStream s;
			s.Open(src);
			IniFile f;
			f.Load(s);

			Vector<String> names = f.EnumNames("Columns");

			for (int j = 0; j < names.GetCount(); ++j)
			{
				int n = ScanInt(names[j]);

				if (AsString(n) != names[j])
					continue;

				Vector<String> values = f.GetArray("Columns", names[j]);
				if (values.GetCount() != 3)
					continue;
				if (values[1] == "Left") e->AddColumn(values[0]).HeaderAlignCenterLeft();
				else if (values[1] == "Right") e->AddColumn(values[0]).HeaderAlignCenterRight();
				else e->AddColumn(values[0]).HeaderAlignCenter();
			}
		}

		if ((*p)[i].Get("Type") == "TabCtrl")
		{
			TabCtrl *e = &_Ctrls.Create<TabCtrl>((*p)[i].Get("Variable"));
			TabCtrl::Style& style = e->StyleDefault().Write();
			style.font = objFont;
			style.tabheight = objFont.GetHeight() + VertLayoutZoom(10);
			e->SetStyle(style);

			String src = (*p)[i].Get("Tab.Content");
			ReplaceString(src, ";", "\r\n");
			StringStream s;
			s.Open(src);
			IniFile f;
			f.Load(s);

			Vector<String> names = f.EnumNames("Tabs");
			VectorMap<int, Vector<String> > cache;

			for (int j = 0; j < names.GetCount(); ++j)
			{
				int n = ScanInt(names[j]);

				if (AsString(n) != names[j])
					continue;

				Vector<String> values = f.GetArray("Tabs", names[j]);
				if (values.GetCount() != 3)
					continue;

				if (values[0] == t_("Current form"))
					values[0] = _File;

				Container *cont = &_Ctrls.Create<Container>("TemporaryContainer"
					+ AsString(tempFormsCount));

				Form *f = &_Ctrls.Create<Form>("TemporaryForm" + AsString(tempFormsCount++));

				if (values[0] != _File)
				{
					if (!f->Load(GetFileDirectory(_File) + "\\" + values[0]))
						continue;
				}
				else
				{
					int lay = HasLayout(values[1]);
					if (lay < 0)
						continue;
					f->GetLayouts().Add(_Layouts[lay]);
				}

				if (!f->Layout(values[1], objFont))
					continue;

				cont->Set(*f, f->GetSize());
				cont->SizePos();
				e->Add(*cont, values[2]);
			}

			if (e->GetCount())
			{
				int active = (*p)[i].GetNumber("Tab.Active", -1, -1);

				if (active < 0)
					active = 0;

				if (active >= e->GetCount())
					active  = e->GetCount() - 1;

				e->Set(active);
			}

			c = e;
		}

		if ((*p)[i].Get("Type") == "Form")
		{
			Form *f = &_Ctrls.Create<Form>((*p)[i].Get("Variable"));
			f->SignalHandler = SignalHandler;
			String path = (*p)[i].Get("Form.Path");
			(*p)[i].Get("Form.PathType") == "Relative"
				? f->Load(::GetFileDirectory(_File) + "\\" + path)
				: f->Load(path);
			f->Layout((*p)[i].Get("Form.Layout"), objFont);
			f->Script = Script;
			c = f;
		}

		if (!c) continue;

		switch((*p)[i].GetHAlign())
		{
			case Ctrl::LEFT:
				c->LeftPosZ((*p)[i].GetRect().left, (*p)[i].GetRect().Width());
				break;
			case Ctrl::RIGHT: 
				c->RightPosZ(sz.cx - (*p)[i].GetRect().left - (*p)[i].GetRect().Width(), (*p)[i].GetRect().Width());
				break;
			case Ctrl::SIZE:
				c->HSizePosZ((*p)[i].GetRect().left, sz.cx - (*p)[i].GetRect().left - (*p)[i].GetRect().Width());
				break;
			case Ctrl::CENTER:
				c->HCenterPosZ((*p)[i].GetRect().Width());
		}

		switch((*p)[i].GetVAlign())
		{
			case Ctrl::TOP:
				c->TopPosZ((*p)[i].GetRect().top, (*p)[i].GetRect().Height());
				break;
			case Ctrl::BOTTOM: 
				c->BottomPosZ(sz.cy - (*p)[i].GetRect().top - (*p)[i].GetRect().Height(), (*p)[i].GetRect().Height());
				break;
			case Ctrl::SIZE:
				c->VSizePosZ((*p)[i].GetRect().top, sz.cy - (*p)[i].GetRect().top - (*p)[i].GetRect().Height());
				break;
			case Ctrl::CENTER:
				c->VCenterPosZ((*p)[i].GetRect().Height());
		}

		String frame = (*p)[i].Get("Frame");

		if (frame == "Null frame")             c->SetFrame(NullFrame());
		if (frame == "Field frame")            c->SetFrame(FieldFrame());
		if (frame == "Inset frame")            c->SetFrame(InsetFrame());
		if (frame == "Outset frame")           c->SetFrame(OutsetFrame());
		if (frame == "Thin inset frame")       c->SetFrame(ThinInsetFrame());
		if (frame == "Thin outset frame")      c->SetFrame(ThinOutsetFrame());
		if (frame == "Black frame")            c->SetFrame(BlackFrame());
		if (frame == "Button frame")           c->SetFrame(ButtonFrame());
		if (frame == "Top separator frame")    c->SetFrame(TopSeparatorFrame());
		if (frame == "Left separator frame")   c->SetFrame(LeftSeparatorFrame());
		if (frame == "Right separator frame")  c->SetFrame(RightSeparatorFrame());
		if (frame == "Bottom separator frame") c->SetFrame(BottomSeparatorFrame());

		AddChild(c);
	}

	return true;
}

String Form::ExecuteForm()
{
	if (!IsLayout())
		return "Error";

	WhenClose = TopWindow::Breaker(0);
	int result;

	for (;;)
	{
		result = TopWindow::Execute();

		if (result == 0)
			return "Close";

		if (result < 0)
			return _Rejectors[ abs(result) - 1 ];

		Vector<FormObject>* p = &_Layouts[_Current].GetObjects();
		bool null = false;
		for (int i = 0; i < p->GetCount(); ++i)
		{
			if ((*p)[i].GetBool("NotNull"))
				if (IsNull(~_Ctrls[i]))
				{
					PromptOK("Необходимо заполнить поле: " + (*p)[i].Get("Variable"));
					null = true;
					break;
				}
		}
		if (null)
			continue;

		break;
	}

	return _Acceptors[ result - 1 ];
}

void Form::Clear(bool all)
{
	if (all) _Layouts.Clear();
	_Rejectors.Clear();
	_Acceptors.Clear();
	_Ctrls.Clear();
	Script.Clear();
}

int Form::HasLayout(const String& layout)
{
	for (int i = 0; i < _Layouts.GetCount(); ++i)
		if (_Layouts[i].Get("Form.Name") == layout)
			return i;
	return -1;
}

void Form::OnAction(const String& action)
{
	SignalHandler.Execute(Script, "OnAction", action);
}

bool Form::Exit(const String& action)
{
	return SetCallback(action, TopWindow::Rejector(0));
}

bool Form::Acceptor(const String& action)
{
	if (SetCallback(action, TopWindow::Acceptor( _Acceptors.GetCount() + 1 )))
	{
		_Acceptors << action;
		return true;
	}
	return false;
}

bool Form::Rejector(const String& action)
{
	if (SetCallback(action, TopWindow::Rejector( -(_Rejectors.GetCount() + 1) )))
	{
		_Rejectors << action;
		return true;
	}
	return false;
}

bool Form::SetCallback(const String& action, Callback c)
{
	if (!IsLayout())
		return false;

	Vector<FormObject>* p = &_Layouts[_Current].GetObjects();
	for (int i = 0; i < p->GetCount(); ++i)
	{
		if ((*p)[i].Get("Type") == "Button")
			if ((*p)[i].Get("Action") == action)
			{
				_Ctrls[i] <<= c;
				return true;
			}
	}

	return false;
}

Value Form::GetData(const String& var)
{
	if (!IsLayout())
		return false;

	Vector<FormObject>* p = &_Layouts[_Current].GetObjects();
	for (int i = 0; i < p->GetCount(); ++i)
	{
		if ((*p)[i].Get("Variable") == var)
			return ~_Ctrls[i];
	}
	return Value();
}

Ctrl* Form::GetCtrl(const String& var)
{
	if (!IsLayout())
		return false;

	Vector<FormObject>* p = &_Layouts[_Current].GetObjects();
	for (int i = 0; i < p->GetCount(); ++i)
	{
		if ((*p)[i].Get("Variable") == var)
			return &_Ctrls[i];
	}
	return NULL;
}
