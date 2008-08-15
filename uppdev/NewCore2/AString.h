#ifndef _NewCore_AString_h_
#define _NewCore_AString_h_

template <class B, class tchar>
class AString : public B {
public:
	void Clear()                     { B::Free(); B::Zero(); }
	int  GetLength() const           { return GetCount(); }

	int operator[](int i) const      { ASSERT(i >= 0 && i <= GetCount()); return Begin()[i]; }

	operator const tchar *() const   { return Begin(); }
	const tchar *operator~() const   { return Begin(); }
};

#endif
