#ifndef FORM_EDITOR_PROPERTIES__PROPERTIES_EDITOR_H
#define FORM_EDITOR_PROPERTIES__PROPERTIES_EDITOR_H

#include "Property.h"
#include "PropertyCaller.h"
#include <ExpandFrame/ExpandFrame.h>
#include <FormEditorCommon/FormEditorCommon.h>

#define LAYOUTFILE <FormEditorProperties/FormEditorProperties.lay>
#include <CtrlCore/lay.h>

template <class T>
class PropertiesWindowBase : public WithPropertiesLayout<TopWindow>
{
	typedef PropertiesWindowBase CLASSNAME;

protected:
	T* GetProperties() { return _Properties; }
	virtual void Cancel();
	virtual void Load();
	virtual void Save();

public:
	PropertiesWindowBase(T* p);
	virtual ~PropertiesWindowBase();
	virtual String GetObjectWidgetClass() const { return String(); }
	virtual int Execute();

private:
	T* _Properties;
	Array<StaticRect> _Panes;
};

template <class T>
PropertiesWindowBase<T>::PropertiesWindowBase(T* p)
{
	CtrlLayoutOKCancel(*this, t_("Properties"));
	ToolWindow().Sizeable();
	_Properties = p;
	_Properties->InitProperties();
	Load();
}

template <class T>
PropertiesWindowBase<T>::~PropertiesWindowBase()
{
	_Properties->ClearProperties();
}

template <class T>
int PropertiesWindowBase<T>::Execute()
{
	if (!_Properties->GetProperties().GetCount())
	{
		PromptOK(t_("No properties for object!"));
		return IDCANCEL;
	}
	int r = WithPropertiesLayout<TopWindow>::Execute();
	r == IDOK ? Save() : Cancel();
	return r;
}

template <class T>
void PropertiesWindowBase<T>::Load()
{
	Vector<String> groups = _Properties->GetPropertiesGroups();
	propGroups.Clear();
	_Panes.Clear();

	for (int i = 0; i < groups.GetCount(); ++i)
	{
		if (groups[i].IsEmpty())
			groups[i] = t_("Others");
		Array<Property*> gProps = _Properties->GetPropertiesByGroup(groups[i]);
		StaticRect& pane = _Panes.Add();
		int paneCY = 10;
		for (int j = 0; j < gProps.GetCount(); ++j)
		{
			gProps[j]->ToPane(pane, paneCY, 5);
		}
		paneCY += 15;
		propGroups.AddExpander(pane, true, paneCY).SetTitle(groups[i]);
	}
}

template <class T>
void PropertiesWindowBase<T>::Save()
{
	Array<Property>& props = _Properties->GetProperties();
	for (int i = 0; i < props.GetCount(); ++i)
		if (props[i].IsChanged())
			props[i].Apply();
	Break();
}

template <class T>
void PropertiesWindowBase<T>::Cancel()
{
	Array<Property>& props = _Properties->GetProperties();
	for (int i = 0; i < props.GetCount(); ++i)
		if (props[i].IsChanged())
			props[i].Restore();
	Break();
}

template <class T>
class PropertiesWindowHistory
{
	class HChangeProperty : public IHistoryItem
	{
		String _Name;
		Value _Last;
		Value _Next;
		int _Prop;
		T* _Ptr;

	public:
		HChangeProperty(T* obj, String name, int prop, Value last, Value v)
			: _Ptr(obj), _Name(name), _Prop(prop), _Last(last), _Next(v) {}
		virtual ~HChangeProperty() {}

		virtual String GetDesc() const;
		virtual void   Undo();
		virtual void   Do();
	};

public:
	PropertiesWindowHistory(T* p) : _Properties(p) {}

private:
	T* _Properties;

protected:
	virtual void Save();
};

template <class T>
void PropertiesWindowHistory<T>::Save()
{
	Array<Property>& props = _Properties->GetProperties();
	for (int i = 0; i < props.GetCount(); ++i)
		if (props[i].IsChanged())
		{
			T* c = _Properties->GetObject();
			if (!c) continue;
			Value v;
			props[i].Get(v, Vector<Value>());
			c->AddToHistory(new HChangeProperty(c, props[i].MetaString("Name"),
				i, props[i].GetLast(), v));
		}
}

// History: Property changes
template <class T>
void PropertiesWindowHistory<T>::HChangeProperty::Do()
{
	if (!_Ptr) return;
	_Ptr->InitProperties();
	Property& prop = _Ptr->GetProperties()[_Prop];
	prop.Set(Vector<Value>() << _Next);
	_Ptr->ClearProperties();
}

template <class T>
void PropertiesWindowHistory<T>::HChangeProperty::Undo()
{
	if (!_Ptr) return;
	_Ptr->InitProperties();
	Property& prop = _Ptr->GetProperties()[_Prop];
	prop.Set(Vector<Value>() << _Last);
	_Ptr->ClearProperties();
}

template <class T>
String PropertiesWindowHistory<T>::HChangeProperty::GetDesc() const
{
	if (!_Ptr) return String(t_("Unable to change the property: NULL-pointer."));
	return NFormat(t_("Property of the object (type \"%s\"), named \"%s\", changed to: \"%s\""),
		_Ptr->GetObjectWidgetClass(), _Name, _Next.ToString());
}

template <class T>
class PropertiesWindow : public PropertiesWindowBase<T>, public PropertiesWindowHistory<T>
{
public:
	PropertiesWindow(T* p) : PropertiesWindowBase<T>(p), PropertiesWindowHistory<T>(p)
		{ _useHistory = false; }
	virtual ~PropertiesWindow() {}

	virtual void Save()
	{
		PropertiesWindowBase<T>::Save();

		if (_useHistory)
			PropertiesWindowHistory<T>::Save();
	}

	bool UseHistory(bool flag = true) { _useHistory = flag; }

private:
	bool _useHistory;
};

#endif
