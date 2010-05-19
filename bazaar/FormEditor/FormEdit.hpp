#ifndef FORM_EDIT_HPP
#define FORM_EDIT_HPP

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <GridCtrl/GridCtrl.h>
#include <Docking/Docking.h>
#include "ScrollContainer.hpp"
#include "FormView.hpp"
#include "ExGridCtrl.hpp"

#define IMAGECLASS FormEditImg
#define IMAGEFILE <FormEditor/FormEdit.iml>
#include <Draw/iml_header.h>

#define TFILE <FormEditor/FormEdit.t>
#include <Core/t.h>

#define LAYOUTFILE <FormEditor/FormEdit.lay>
#include <CtrlCore/lay.h>

#include "FormProperties.hpp"

class CtrlContEx : public Ctrl
{
public:
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetRect(), SColorFace());
	}
};

static char sZoomText[] = "OK Cancel Exit Retry";

class FormEdit : public DockWindow
{
	typedef FormEdit CLASSNAME;
	enum ViewMode { VIEW_MODE_AS_IS, VIEW_MODE_INFO, VIEW_MODE_WIREFRAME };
	enum ZoomMode { ZOOM_MODE_STANDARD, ZOOM_MODE_SMALL, ZOOM_MODE_LARGE };

public:
	FormEdit();
	virtual void DockInit();

	void OpenLayoutProperties();

	void CreateMenuBar(Bar& bar);
	void CreateBar(Bar& bar);
	void AlignBar(Bar& bar);
	void FileBar(Bar& bar);
	void ItemBar(Bar& bar);
	void FormBar(Bar& bar);
	void ViewBar(Bar& bar);
	void WindowBar(Bar& bar);

	void CreateToolBar(Bar& bar);
	void NullCallback() {}
	void Quit() { Break(); }

	void Clear();
	Size GetFormSize() { return _View.Deoffseted(_View.GetPageRect()).GetSize(); }
	Vector<FormObject>* GetObjects() { return _View.GetObjects(); }
	
	void SetSprings(dword hAlign, dword vAlign)
	{
		_View.SetSprings(hAlign, vAlign);
	}

	void SetFlag(const String& flag, bool state)
	{
		_View.SetBool(flag, state);
		_View.Refresh();
	}

	bool GetFlag(const String& flag, bool init)
	{
		return _View.GetBool(flag, init);
	}

	void ToggleFlag(const String& flag, bool init)
	{
		SetFlag(flag, !GetFlag(flag, init));
		_ToolBar.Set(THISBACK(CreateToolBar));
		SetViewMode(_ViewMode);
	}

	void UpdateTools()
	{
		_ToolBar.Set(THISBACK(CreateToolBar));
		_MenuBar.Set(THISBACK(CreateMenuBar));
		_View.Refresh();
	}

	void TestLayout();

	void NewFile();
	void OpenFile();
	void SaveFile();
	void SaveAsFile();

	void OpenObjectProperties(const Vector<int>& indexes);
	void UpdateChildProperties(const Vector<int>& indexes);
	void UpdateChildPos(Vector<int> indexes);
	void UpdateChildCount(int count);
	void UpdateChildAllPos();
	void UpdateChildZ();

	void ToggleLayoutLock()
	{
		LockLayout( !IsLocked() );
		UpdateTools();

		for (int i = 0; i < GetDockableCtrls().GetCount(); ++i)
			GetDockableCtrls()[i]->SetFrame( IsLocked() ? FieldFrame() : NullFrame() );
	}

	void SetViewMode(ViewMode mode)
	{
		_ViewMode = mode;
		if (mode == VIEW_MODE_WIREFRAME) _View.ShowFrames();
		if (mode == VIEW_MODE_AS_IS) _View.HideInfo();
		if (mode == VIEW_MODE_INFO) _View.ShowInfo();

		if (mode == VIEW_MODE_AS_IS)
		{			
			_View.SetBool("Grid.Visible", false);
			_View.Refresh();
		}
		else
		{
			_View.GetBool("Grid.Binding")
				? _View.SetBool("Grid.Visible", true)
				: _View.SetBool("Grid.Visible", false);
			_View.Refresh();
		}
		UpdateChildZ();
		UpdateTools();
	}

	void ProjectSaved(bool flag = true)
	{
		if (flag == _Saved) return;
		_Saved = flag;
		UpdateTools();
	}

	bool IsProjectSaved() { return _Saved; }

	void StdFontZoom()
	{
		_ZoomMode = ZOOM_MODE_STANDARD;
		UpdateTools();

		StdFontZoomShort();
		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void StdFontZoomShort()
	{
		_View.SetFont(StdFont());
		Ctrl::SetZoomSize(GetTextSize(sZoomText, StdFont()), Size(99, 13));
		UpdateChildZ();
		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void SmallFontZoom()
	{
		_ZoomMode = ZOOM_MODE_SMALL;
		UpdateTools();

		_View.SetFont(StdFont().Height(14));
		Ctrl::SetZoomSize(GetTextSize(sZoomText, StdFont().Height(14)), Size(99, 13));
		UpdateChildZ();
		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void LargeFontZoom()
	{
		_ZoomMode = ZOOM_MODE_LARGE;
		UpdateTools();

		_View.SetFont(StdFont().Height(18));
		Ctrl::SetZoomSize(GetTextSize(sZoomText, StdFont().Height(18)), Size(99, 13));
		UpdateChildZ();
		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void LastFontZoom()
	{
		switch(_ZoomMode)
		{
			case ZOOM_MODE_SMALL: SmallFontZoom(); break;
			case ZOOM_MODE_LARGE: LargeFontZoom(); break;
			default: StdFontZoom();
		}
	}

	bool IsSmallZoom()    { return _ZoomMode == ZOOM_MODE_SMALL; }
	bool IsLargeZoom()    { return _ZoomMode == ZOOM_MODE_LARGE; }
	bool IsStandardZoom() { return _ZoomMode == ZOOM_MODE_STANDARD; }

	void ToggleColoring()
	{
		_View.ToggleBool("View.Coloring");
		UpdateTools();
	}

	void UpdateLayoutList()
	{
		if (!_View.IsLayout())
			return;
		_LayoutList.Clear();
		Vector<FormLayout>* p = &_View.GetLayouts();
		for (int i = 0; i < p->GetCount(); ++i)
			_LayoutList.AddRow((*p)[i].Get("Form.Name"));

		_View.SetFormSize(_View.GetCurrentLayout()->GetFormSize());
	}

	void UpdateItemList()
	{
		if (!_View.IsLayout())
		{
			_ItemList.Clear();
			return;
		}
		
		Vector<int> sel = _View.GetSelected();
		_ItemList.ClearSelection();

		for (int i = 0; i < sel.GetCount(); ++i)
			if (0 <= sel[i] && sel[i] < _ItemList.GetRowCount())
				_ItemList.Select(sel[i]);
	}

	void SelectItem()
	{
		if (!_View.IsLayout())
			return;

		if (_ItemList.IsSelected())
		{
			Vector<int> sel;
			_View.ClearSelection();
			for (int i = 0; i < _ItemList.GetRowCount(); ++i)
				if (_ItemList.IsSelected(i))
				{
					_View.AddToSelection(i);
					if (!sel.GetCount())
						sel << i;
					else if (sel.GetCount() > 0)
						sel.Clear();
				}
			OpenObjectProperties(sel);
		}
	}

	void OnAddLayout()
	{
		int row = _LayoutList.GetCurrentRow();
		if (row < 0) return;

		_View.AddLayout(_LayoutList.Get(row, 0).ToString());

		_ItemProperties.Clear();
		_View.SelectLayout(_View.GetLayoutCount() - 1);
		UpdateChildZ();

		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void OnUpdateLayout()
	{
		int row = _LayoutList.GetCurrentRow();
		if (row < 0) return;

		_View.UpdateLayoutName(row, _LayoutList.Get(row, 0).ToString());
	}

	void OnSelectLayout()
	{
		int row = _LayoutList.GetCurrentRow();
		if (row < 0) return;

		_ItemProperties.Clear();
		_View.SelectLayout(row);
		UpdateChildZ();
		OpenObjectProperties(_View.GetSelected());
		_Container.Set(_View, _View.GetPageRect().GetSize());
	}

	void OnRemoveLayout()
	{
		int row = _LayoutList.GetCurrentRow();
		if (row < 0) return;

		_ItemProperties.Clear();
		_View.RemoveLayout(row);

		OpenObjectProperties(_View.GetSelected());
		_Container.Set(_View, _View.IsLayout() ? _View.GetPageRect().GetSize() : Size());

		UpdateChildZ();
		Refresh();
	}

	void UpdateObjectType()
	{
		int row = _ItemList.GetCurrentRow();
		if (row < 0) return;

		_View.GetCurrentLayout()->GetObjects()[row].Set("Type", _ItemList.Get(row, 0));
		_ItemProperties._Headers.Set(0, 1, _ItemList.Get(row, 0));
		UpdateChildZ();
		OpenObjectProperties(_View.GetSelected());
	}

	void UpdateObjectName()
	{
		int row = _ItemList.GetCurrentRow();
		if (row < 0) return;

		_View.GetCurrentLayout()->GetObjects()[row].Set("Variable", _ItemList.Get(row, 1));
		_View.Refresh();
		OpenObjectProperties(_View.GetSelected());
	}

	void UpdateTempObjectName()
	{
		int row = _ItemList.GetCurrentRow();
		if (row < 0) return;

		_TempObjectName = _ItemList.Get(row, 1).ToString();
	}

	void CreateObjectMenu(Bar& bar)
	{
		int row = _ItemList.GetCurrentRow();
		if (row < 0) return;

		bar.Add(t_("Edit"), GridImg::Modify(), _ItemList.StdEdit);
		if (!_ItemList.IsSelected())
			return;
		bar.Separator();
		_View.CreateObjectMenu(bar, row);
	}

	void UpdateTabCtrls();

	void LoadGUILayouts(bool first = false);

	void Serialize(Stream& s)
	{
		SerializeLayout(s);
	}

	void AddGUILayout()
	{
		String name;

		TopWindow dlg;
		dlg.Title(t_("New layout"));
		dlg.SetRect( GetWorkArea().CenterRect(300, 80) );
		EditString s;
		Button ok, cancel;
		ok.SetLabel(t_("Add"));
		ok <<= dlg.Acceptor(IDOK);
		cancel <<= dlg.Rejector(IDCANCEL);
		cancel.SetLabel(t_("Cancel"));
		dlg.ToolWindow();
		dlg.Add( s.HSizePosZ(8, 8).TopPosZ(8, 18) );
		dlg.Add( ok.RightPosZ(80, 65).BottomPosZ(8, 25) );
		dlg.Add( cancel.RightPosZ(8, 65).BottomPosZ(8, 25) );
		if (dlg.Execute() == IDCANCEL)
			return;
		name = (~s).ToString();
		if (name.IsEmpty())
			name = t_("User Interface");

		name = " " + name;
		SaveLayout(name);
		DeleteFile(ConfigFile("Layouts.bin"));
		StoreToFile(*this, ConfigFile("Layouts.bin"));
		LoadGUILayouts();
		_GUILayouts <<= _GUILayouts.GetKey( _GUILayouts.GetCount() - 1 );
		UpdateTools();
	}

	void SaveGUILayout()
	{
		SaveLayout(GetLayouts().GetKey( _GUILayouts.GetIndex() ));
		DeleteFile(ConfigFile("Layouts.bin"));
		StoreToFile(*this, ConfigFile("Layouts.bin"));
	}

	void UpdateGUILayout()
	{
		int index = _GUILayouts.GetIndex();
		if (index < 0 || index >= GetLayouts().GetCount())
			return;
		LoadLayout(GetLayouts().GetKey( index ));
		_GUILayouts.GetButton(2).Enable((~_GUILayouts).ToString() != t_(" Default"));
	}

	void RemoveGUILayout()
	{
		int count = GetLayouts().GetCount();
		if (!count) return;

		DeleteLayout( GetLayouts().GetKey( _GUILayouts.GetIndex() ) );
		StoreToFile(*this, ConfigFile("Layouts.bin"));
		LoadGUILayouts();
		_GUILayouts <<= t_(" Default");
		LoadLayout(t_(" Default"));
		UpdateTools();
	}

private:
	Size _ToolSize;
	bool _Saved;
	String _File;
	ViewMode _ViewMode;
	ZoomMode _ZoomMode;

	CtrlContEx _CtrlContainer;
	ScrollContainer _Container;
	Array<Ctrl> _Temporaries;
	Array<Ctrl> _Ctrls;
	FormView _View;

	EditString _LayoutNameField;
	EditString _ObjectNameField;
	DropList _GUILayouts;
	DropList _TypeList;
	String _TempObjectName;

	MenuBar _MenuBar;
	ToolBar _ToolBar;
	ExGridCtrl _LayoutList;
	ExGridCtrl _ItemList;
	PropertiesWindow _ItemProperties;
};

#endif // .. FORM_EDIT_HPP
