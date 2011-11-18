#ifndef FORM_EDITOR_COMMON__UTILS_H
#define FORM_EDITOR_COMMON__UTILS_H

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

// Returns fullpath to the file in the exe-file directory
inline String ConfigFileEx(const char* fp)
{
	return AppendFileName( GetFileDirectory(GetExeFilePath()), String(fp) );
}

// Converts const char* text to String
inline String AsString(const char* s) { return String(s); }

// Saves Image to String
inline String AsString(Image mIcon)
{
	return StoreAsString(mIcon);
}

// Loads Image from String
inline Image ToImage(const String& icon)
{
	Image r;
	return LoadFromString(r, icon) ? r : Image();
}

// Returns the bool-value from any type of value
inline bool ToBool(const Value& v)
{
	if (v.GetType() == BOOL_V) return (bool)v;
		else return v.ToString() == "true" || ScanInt(v.ToString()) == 1;
}

// Extended VectorMap, supports adding elements in one line
template <class U, class T> class VectorMapEx : public VectorMap<U, T>
{
public:
	virtual ~VectorMapEx() {}
	VectorMapEx<U, T>& Add(const U& u, const T& t)
	{
		VectorMap<U, T>::Add(u, t);
		return *this;
	}
};

#endif
