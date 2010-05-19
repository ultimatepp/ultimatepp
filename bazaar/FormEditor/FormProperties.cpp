#include "FormEdit.hpp"
#include "FormProperties.hpp"
#include "EditTabs.hpp"
#include "EditColumns.hpp"

void FormEdit::OpenObjectProperties(const Vector<int>& indexes)
{
	if (!_View.IsLayout())
		return;

	String temp = _TempObjectName;
	_TempObjectName.Clear();
	_ItemList.EndEdit(false, false, false);
	int row = _ItemList.GetCurrentRow();
	if (row >= 0 && !temp.IsEmpty())
	{
		_View.GetCurrentLayout()->GetObjects()[row].Set("Variable", temp);
		_ItemList.Set(row, 1, temp);
	}
	_LayoutList.EndEdit();

	if (indexes.GetCount() == 1)
	{
		FormObject* pI = _View.GetObject(indexes[0]);
		if (!pI) return;

		_ItemProperties._Options.EndEdit();
		_ItemProperties.Generate(pI, indexes[0]);
	}

	if (indexes.GetCount() == 0)
	{
		_ItemProperties._Options.EndEdit();
		_ItemProperties._Headers.Clear();
		_ItemProperties._Options.Clear();
	}

	UpdateItemList();
}

PropertiesWindow::PropertiesWindow()
{
	Sizeable().MaximizeBox();
	_Item  = NULL;
	_Index = -1;

	Add(_Headers.HSizePosZ().TopPos(0, _Headers.GD_ROW_HEIGHT));
	_Headers.SetFrame(NullFrame());
	_Headers.Editing().OneClickEdit();
	_Headers.AddColumn(t_("Property")).SetDisplay(_TypeDisplay);
	_Headers.AddColumn(t_("Value")).SetDisplay(_TypeDisplay).Edit(_DropList);
	_Headers.Sorting(false);
	_Headers.HideRow(0);
	_Headers.FullColResizing(false);

	_DropList.Add("Button");
	_DropList.Add("Label");
	_DropList.Add("EditField");
	_DropList.Add("EditInt");
	_DropList.Add("DropDate");
	_DropList.Add("TabCtrl");
	_DropList.Add("GridCtrl");
	_DropList.Add("ProgressBar");
	_DropList.Add("Form");

	_DropList.WhenAction = THISBACK1(OnEndEdit, 1);

	Add(_Options.HSizePosZ().VSizePos(_Headers.GD_ROW_HEIGHT, 0));
	_Options.SetFrame(NullFrame());
	_Options.Editing().OneClickEdit();
	_Options.AddColumn(t_("Property"));
	_Options.AddColumn(t_("Value")).SetDisplay(_Display);
	_Options.SetInfo(t_("No properties"));
	_Options.HideRow(0);
	_Options.SetInfoOffset(_Options.GD_HDR_HEIGHT);
	_Options.FullColResizing(false);

	_Options.WhenChangeRow = THISBACK(OnStartEdit);
	_Options.WhenUpdateCell = THISBACK1(OnEndEdit, 0);
}

void PropertiesWindow::OnStartEdit()
{
	int row = _Options.GetCurrentRow();
	if (row < 0) { _Options.SetCursor(1); return; }

	if (row < _Properties.GetCount())
		_Options.GetColumn(1).Edit(_Properties[row]);
}

void PropertiesWindow::OnEndEdit(int mode)
{
	if (_Item)
	{
		for (int i = 0; i < _Properties.GetCount(); ++i)
		{
			String param = _Properties.GetKey(i);
			if (param.Find("Color") >= 0) // if color found
			{
				Color r = _Properties[i].GetData();
				_Item->Set(param, Encode64(StoreAsString(r)));
				continue;
			}
			String value = AsString(_Properties[i].GetData());
			_Item->Set(param, value);
		}

		if (mode)
		{
			_Options.EndEdit();
			_Options.ClearCursor();
			_Item->Set("Type", _Headers.Get(0, 1));
			Generate(_Item, _Index);
		}

		WhenChildZ.Execute();
	}
}

class EditIntSpinEx : public EditIntSpin
{
public:
	virtual bool Key(dword key, int count) { return false; }
};

void PropertiesWindow::Property(const String& id, const String& label, const String& type,
	const Array<String>& prop)
{
	if (type == "Option")
	{
		EditIntSpinEx* p = &_Properties.Create<EditIntSpinEx>(id);
		p->Min(0).Max(1);

		if (prop.GetCount())
		{
			p->SetData(prop[0]);
			_Options.AddRow(label, AsString(prop[0]));
		}
	}

	if (type == "EditInt")
	{
		EditInt* p = &_Properties.Create<EditInt>(id);
		if (prop.GetCount())
		{
			p->SetData(prop[0]);
			_Options.AddRow(label, AsString(prop[0]));
		}
	}

	if (type == "EditField")
	{
		EditField* p = &_Properties.Create<EditField>(id);
		if (prop.GetCount())
		{
			p->SetData(prop[0]);
			_Options.AddRow(label, AsString(prop[0]));
		}
	}

	if (type == "EditTabs")
	{
		EditTabs* p = &_Properties.Create<EditTabs>(id);
		p->WhenEndEdit = THISBACK1(OnEndEdit, 0);
		if (prop.GetCount())
		{
			p->SetLabel(t_("Change tabs..."));
			p->SetData(prop[0]);
			_Options.AddRow(label, AsString(prop[0]));
		}
	}

	if (type == "EditColumns")
	{
		EditColumns* p = &_Properties.Create<EditColumns>(id);
		p->WhenEndEdit = THISBACK1(OnEndEdit, 0);
		if (prop.GetCount())
		{
			p->SetLabel(t_("Change columns..."));
			p->SetData(prop[0]);
			_Options.AddRow(label, AsString(prop[0]));
		}
	}

	if (type == "DropList")
	{
		if (prop.GetCount() < 2) return;

		DropList* p = &_Properties.Create<DropList>(id);
		for (int i = 1; i < prop.GetCount(); ++i)
			p->Add(prop[i]);

		if (prop.GetCount())
		{
			String option = prop[ prop[0].IsEmpty() ? 1 : 0 ];
			_Options.AddRow(label, option);
			p->SetData(option);
		}
	}
}

void PropertiesWindow::Property(const String& id, const String& label, const String& type,
	const Color& c)
{
	if (type == "EditColor")
	{
		ColorPusher* p = &_Properties.Create<ColorPusher>(id);
		p->SetData(c);

		_Options.AddRow(label, c);
	}
}

void PropertiesWindow::Generate(FormObject* pI, int index)
{
	if (!pI) return;

	_Properties.Clear();
	_Options.Clear();

	_Item  = pI;
	_Index = index;

	String type = pI->Get("Type");
	if (type.IsEmpty()) return;

	Property("Variable", t_("Variable:"), "EditField", Array<String>() << pI->Get("Variable"));

	if (_Headers.GetRowCount() != 1)
	{
		_Headers.Clear();
		_Headers.AddRow(t_("Type:"), type);
	}

	Property("Font.Height", t_("Font height:"), "EditInt", Array<String>()
		<< AsString(pI->GetNumber("Font.Height", 0, 0, 500)));

	if (type == "EditField")
	{
		Property("Style", t_("Style:"), "DropList", Array<String>()
			<< pI->Get("Style") << "Text Field" << "Password");
		Property("TextAlign", t_("Text align:"), "DropList",
			Array<String>() << pI->Get("TextAlign") << "Left" << "Right");
		Property("Tip", t_("Tip:"), "EditField", Array<String>() << pI->Get("Tip"));
		Property("NotNull", t_("Not null:"), "Option", Array<String>() << pI->Get("NotNull",
			"0"));
	}

	if (type == "EditInt")
	{
		Property("Min", t_("Min:"), "EditInt", Array<String>() << pI->Get("Min"));
		Property("Max", t_("Max:"), "EditInt", Array<String>() << pI->Get("Max"));
		Property("Tip", t_("Tip:"), "EditField", Array<String>() << pI->Get("Tip"));
		Property("NotNull", t_("Not null:"), "Option", Array<String>() << pI->Get("NotNull",
			"0"));
	}

	if (type == "Label")
	{
		Color src = DefaultInk();
		LoadFromString( src, Decode64(pI->Get("Font.Color", Encode64(StoreAsString(src)))) );
		Property("Label", t_("Label:"), "EditField", Array<String>() << pI->Get("Label"));
		Property("Text.Align", t_("Text align:"), "DropList",
			Array<String>() << pI->Get("Text.Align") << "Left" << "Center" << "Right");
		Property("Font.Color", t_("Font color:"), "EditColor", src);
	}

	if (type == "Button")
	{
		Property("Label", t_("Label:"), "EditField", Array<String>() << pI->Get("Label"));
		Property("Action", t_("Action:"), "EditField", Array<String>() << pI->Get("Action"));
		Property("Tip", t_("Tip:"), "EditField", Array<String>() << pI->Get("Tip"));
	}

	if (type == "Form")
	{
		Property("Form.PathType", t_("Type of path:"), "DropList",
			Array<String>() << pI->Get("Form.PathType") << "Relative" << "Absolute");
		Property("Form.Path", t_("Path:"), "EditField", Array<String>() << pI->Get("Form.Path"));
		Property("Form.Layout", t_("Layout:"), "EditField", Array<String>() << pI->Get("Form.Layout"));
	}

	if (type == "TabCtrl")
	{
		Property("Tab.Content", t_("Tabs:"), "EditTabs", Array<String>()
			<< pI->Get("Tab.Content", "[Tabs];"));
	}

	if (type == "GridCtrl")
	{
		Property("Grid.Columns", t_("Columns:"), "EditColumns", Array<String>()
			<< pI->Get("Grid.Columns", "[Columns];"));
	}

	Property("Frame", t_("Frame:"), "DropList",
		Array<String>()
			<< pI->Get("Frame")
			<< "Default frame"
			<< "Null frame"
			<< "Top separator frame"
			<< "Left separator frame"
			<< "Right separator frame"
			<< "Bottom separator frame"
			<< "Field frame"
			<< "Inset frame"
			<< "Outset frame"
			<< "Thin inset frame"
			<< "Thin outset frame"
			<< "Black frame"
			<< "Button frame");

	_Options.HideRow(0);
	_Headers.SetCursor(0);
	_Headers.StartEdit();
	_Options.SetCursor(0);
}

void FormEdit::UpdateChildProperties(const Vector<int>& indexes)
{
	if (!_View.IsLayout())
		return;

	OpenObjectProperties(indexes);
}
