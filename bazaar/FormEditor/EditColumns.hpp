#ifndef EDIT_COLUMNS_HPP
#define EDIT_COLUMNS_HPP

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#include <Form/IniConfig.hpp>
#include <Form/Form.hpp>

class EditColumns : public Button
{
	typedef EditColumns CLASSNAME;

public:
	EditColumns()
	{
		EdgeStyle();

#ifdef PLATFORM_WIN32
		_Columns.Chameleon();
		_Window.ToolWindow();
#endif
		_Window.Title(t_("Columns"));
		_Window.Add(_Columns.SizePos());
		_Window.Sizeable();
		_Window.SetRect( GetWorkArea().CenterRect( HorzLayoutZoom(500), VertLayoutZoom(300)) );

		_Columns.Appending().Removing().Editing().Moving().MovingRows();
		_Columns.AddColumn(t_("Column name")).Edit(_ColumnName);
		_Columns.AddColumn(t_("Header aligh")).Edit(_HeaderAlign);
		_Columns.AddColumn(t_("Content aligh")).Edit(_DataAlign);

		_HeaderAlign.Add("Left");
		_HeaderAlign.Add("Center");
		_HeaderAlign.Add("Right");

		_DataAlign.Add("Left");
		_DataAlign.Add("Center");
		_DataAlign.Add("Right");

		WhenPush = THISBACK(OpenEditor);
	}

	virtual void SetData(const Value& data)
	{
		String src = data.ToString();
		ReplaceString(src, ";", "\r\n");

		StringStream s;
		s.Open(src);

		IniFile f;
		f.Load(s);

		Vector<String> names = f.EnumNames("Columns");
		VectorMap<int, Vector<String> > cache;

		_Columns.Clear();
		for (int i = 0; i < names.GetCount(); ++i)
		{
			int n = ScanInt(names[i]);

			if (AsString(n) != names[i])
				continue;

			Vector<String> values = f.GetArray("Columns", names[i]);
			if (values.GetCount() != 3)
				continue;

			cache.GetAdd(i, values);
		}

		int max = 0;
		for (int i = cache.GetCount() - 1; i >= 0; --i)
			if (max < cache.GetKey(i))
				max = cache.GetKey(i);

		_Columns.SetRowCount(max);

		for (int i = cache.GetCount() - 1; i >= 0; --i)
			for (int c = 0; c < 3; ++c)
				_Columns.Set(i, c, cache[i][c]);
	}

	virtual Value GetData() const
	{
		if (!_Columns.GetRowCount())
			return Value("[Columns];");

		IniFile f;
		for (int i = 0; i < _Columns.GetRowCount(); ++i)
		{
			String v2 = _Columns.Get(i, 1).ToString();
			String v3 = _Columns.Get(i, 2).ToString();
			f.AddArray("Columns", AsString(i), Array<String>()
				<< _Columns.Get(i, 0).ToString()
				<< (v2.IsEmpty() ? String("Left") : v2)
				<< (v3.IsEmpty() ? String("Left") : v3));
		}

		String data = f.Dump();
		ReplaceString(data, "\r\n", ";");
		return Value(data);
	}

	virtual const Value& operator<<=(const Value& v) { SetData(v); return v; }
	virtual Value operator~() const { return GetData(); }
	void OpenEditor()
	{
		_Columns.WhenAcceptedRow = WhenEndEdit;
		_Columns.WhenUpdateCell = WhenEndEdit;
		_Columns.WhenRemovedRow = WhenEndEdit;
		_Columns.WhenCreateRow = WhenEndEdit;
		_Columns.WhenLeftClick = WhenEndEdit;
		_Columns.WhenEnter = WhenEndEdit;

		_Window.Execute();
		WhenEndEdit();
	}

	Callback WhenEndEdit;

private:
	EditString _ColumnName;
	DropList _HeaderAlign;
	DropList _DataAlign;
	TopWindow _Window;
	GridCtrl _Columns;
};

#endif // .. EDIT_COLUMNS_HPP
