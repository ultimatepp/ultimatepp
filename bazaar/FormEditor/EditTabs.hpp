#ifndef EDIT_TABS_HPP
#define EDIT_TABS_HPP

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#include <Form/IniConfig.hpp>
#include <Form/Form.hpp>

class EditTabs : public Button
{
	typedef EditTabs CLASSNAME;

public:
	EditTabs()
	{
		EdgeStyle();

#ifdef PLATFORM_WIN32
		_Tabs.Chameleon();
		_Window.ToolWindow();
#endif
		_Window.Title(t_("Tab settings"));
		_Window.Add(_Tabs.SizePos());
		_Window.Sizeable();
		_Window.SetRect( GetWorkArea().CenterRect( HorzLayoutZoom(400), VertLayoutZoom(250)) );

		_Tabs.Appending().Removing().Editing().Moving().MovingRows();
		_Tabs.AddColumn(t_("Form")).Edit(_FormEdit);
		_Tabs.AddColumn(t_("Lay")).Edit(_LayEdit);
		_Tabs.AddColumn(t_("Tab Name")).Edit(_TabEdit);

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

		Vector<String> names = f.EnumNames("Tabs");
		VectorMap<int, Vector<String> > cache;

		_Tabs.Clear();
		for (int i = 0; i < names.GetCount(); ++i)
		{
			int n = ScanInt(names[i]);

			if (AsString(n) != names[i])
				continue;

			Vector<String> values = f.GetArray("Tabs", names[i]);
			if (values.GetCount() != 3)
				continue;

			cache.GetAdd(i, values);
		}

		int max = 0;
		for (int i = cache.GetCount() - 1; i >= 0; --i)
			if (max < cache.GetKey(i))
				max = cache.GetKey(i);

		_Tabs.SetRowCount(max);

		for (int i = cache.GetCount() - 1; i >= 0; --i)
			for (int c = 0; c < 3; ++c)
				_Tabs.Set(i, c, cache[i][c]);
	}

	virtual Value GetData() const
	{
		if (!_Tabs.GetRowCount())
			return Value("[Tabs];");

		IniFile f;
		for (int i = 0; i < _Tabs.GetRowCount(); ++i)
		{
			String v1 = _Tabs.Get(i, 0).ToString();
			String v2 = _Tabs.Get(i, 1).ToString();
			String v3 = _Tabs.Get(i, 2).ToString();
			f.AddArray("Tabs", AsString(i), Array<String>()
				<< (v1.IsEmpty() ? String(t_("None")) : v1)
				<< (v2.IsEmpty() ? String(t_("None")) : v2)
				<< (v3.IsEmpty() ? String(t_("None")) : v3));
		}

		String data = f.Dump();
		ReplaceString(data, "\r\n", ";");
		return Value(data);
	}

	virtual const Value& operator<<=(const Value& v) { SetData(v); return v; }
	virtual Value operator~() const { return GetData(); }
	void OpenEditor()
	{
		for (;;)
		{
			bool error = false;
			_Window.Execute();
			for (int i = 0; i < _Tabs.GetRowCount(); ++i)
			{
				Form f;
				if (!_Tabs.Get(i, 0).ToString().IsEmpty() &&
					_Tabs.Get(i, 0).ToString() != t_("Current form"))
				{
					if (!f.Load( _Tabs.Get(i, 0).ToString() ))
					{
						PromptOK( Format(t_("Error while loading form \"%s\"!"),
							_Tabs.Get(i, 0).ToString()) );
						error = true;
						break;
					}
					if (!f.Layout( _Tabs.Get(i, 1).ToString() ))
					{
						PromptOK( Format(t_("The lay \"%s\" not found in form \"%s\""),
							_Tabs.Get(i, 1).ToString(), _Tabs.Get(i, 0).ToString()) );
						error = true;
						break;
					}
				}
				else
					_Tabs.Set(i, 0, t_("Current form"));
			}
			if (!error)
				break;
		}

		WhenEndEdit();
	}

	Callback WhenEndEdit;

private:
	EditString _FormEdit;
	EditString _TabEdit;
	EditString _LayEdit;
	TopWindow _Window;
	GridCtrl _Tabs;
};

#endif // .. EDIT_TABS_HPP
