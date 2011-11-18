#ifndef FORM_EDITOR_PROPERTIES__PROPERTIES_H
#define FORM_EDITOR_PROPERTIES__PROPERTIES_H

#include "PropertiesEditor.h"

template <class T>
class PropertiesBase : public IPartBase
{
public:
	PropertiesBase(T* c) : _This(c) {}
	virtual ~PropertiesBase() {}

	virtual void InitProperties() {}
	virtual int  OpenProperties();
	virtual void ClearProperties() { GetProperties().Clear(); }
	virtual int  ExecuteProperties(const Size& wndSize = Null);

	Array<Property>& GetProperties() { return _Properties; }
	const Array<Property>& GetProperties() const { return _Properties; }

	int GetPropertyGroupCount() const { return GetPropertiesGroups().GetCount(); }
	Vector<String> GetPropertiesGroups() const;
	Array<Property*> GetPropertiesByGroup(const String& group);

	T* GetObject() { return _This; }

private:
	T* _This;
	Array<Property> _Properties;
};

template <class T>
Vector<String> PropertiesBase<T>::GetPropertiesGroups() const
{
	const Array<Property>& props = GetProperties();
	Vector<String> result;
	for (int i = 0; i < props.GetCount(); ++i)
	{
		bool found = false;
		for (int j = 0; j < result.GetCount(); ++j)
			if (props[i].MetaString("Group") == result[j])
			{
				found = true;
				break;
			}
		if (!found)
			result << props[i].MetaString("Group");
	}
	return result;
}

template <class T>
Array<Property*> PropertiesBase<T>::GetPropertiesByGroup(const String& group)
{
	Array<Property>& props = GetProperties();
	Array<Property*> result;
	for (int i = 0; i < props.GetCount(); ++i)
	{
		if (props[i].MetaString("Group") == group)
			result.Add(&props[i]);
	}
	return result;
}

template <class T>
int PropertiesBase<T>::OpenProperties()
{
	return PropertiesWindow<PropertiesBase<T> >(this).Execute();
}

template <class T>
int PropertiesBase<T>::ExecuteProperties(const Size& wndSize)
{
	PropertiesWindow<PropertiesBase<T> > wnd(this);
	if (!IsNull(wndSize))
		wnd.GetWorkArea().CenterRect(wndSize);
	int r = wnd.Execute();
	return r;
}

#endif
