#ifndef _Gen_Misc_h_
#define _Gen_Misc_h_

#include <Core/Core.h>

NAMESPACE_UPP

//some usefull interfaces

//the enable option
template<class B = EmptyClass>
class EnableOption : public B
{
public:
	EnableOption() : enabled(true) {}
	virtual void Enable(bool e)                { enabled = e; }
	inline void Disable()                      { return Enable(false); }
	virtual bool IsEnabled() const             { return enabled; }
protected:
	bool enabled;	
};

//notificator interface
template<class B = EmptyClass>
class Observable : public B
{
public:	
	Callback  operator<<=(Callback action)     { WhenAction = action; return action; }
	Callback& operator<<(Callback action)      { return WhenAction << action; }
	void Action()                              { WhenAction(); }
public:
	Callback WhenAction;
};

//get set data interface only with some helpers
template<class T, class B = EmptyClass>
class GetSetData : public B
{
public:
	virtual ~GetSetData() {} //might be used as base interface
	virtual T    GetData() const               = 0;
	virtual void SetData(const T& _v)          = 0;
//	virtual T& GetRef()                        { NEVER(); Value v; return v; }  
//	virtual const T& GetRef() const            { NEVER(); Value v; return v; }
	T            operator~() const             { return GetData(); }
	const T&     operator<<=(const T& v)       { SetData(v); return v; }
};

//get set data interface only with some helpers
template<class T, class B = EmptyClass>
class GetSetDataMulti : public B
{
public:
	virtual ~GetSetDataMulti() {} //might be used as base interface
	virtual T    GetDataMulti(int i) const     = 0;
	virtual void SetDataMulti(int i, const T& _v) = 0;
	virtual T& GetRefMulti(int i)              = 0;
	virtual const T& GetRefMulti(int i) const  = 0;
	T            operator~() const             { return GetDataMulti(0); }
	const T&     operator<<=(const T& v)       { SetDataMulti(0, v); return v; }
};

//Min Max values
template <class T, class B = EmptyClass>
class MinMaxOption : public B
{
public:
	MinMaxOption() : nn(false) {}
	MinMaxOption(const T & min, const T& max) : min(min), max(max), nn(false) {}
	virtual MinMaxOption& SetMin(const T& _min){ min = _min; return *this; }
	inline MinMaxOption& Min(const T& _min)    { return SetMin(_min); }
	T GetMin() const                           { return min; }
	inline T Min() const                       { return GetMin(); }
	virtual MinMaxOption& SetMax(const T& _max){ max = _max; return *this; }
	inline MinMaxOption& Max(const T& _max)    { return SetMax(_max); }
	T GetMax() const                           { return max; }
	inline T Max() const                       { return GetMax(); }
	virtual MinMaxOption& NotNull(bool _nn = true) { nn = _nn; return *this; }
	T IsNotNull() const                        { return nn; }
protected:
	T min, max;
	bool nn;
};

//a visiting interface
template<class T, class B = EmptyClass>
class Visiting : public B
{
public:
	typedef Visiting<T,B> CLASSNAME;
	Visiting() : pt(NULL) {}
	virtual ~Visiting() {}

	virtual void Visit(T& t) { pt = &t; Reload(); }
	virtual void Reload() { }
	virtual void Clear() { pt = NULL; }

	bool IsVisiting() const { return pt; }
	bool IsEmpty() const { return !IsVisiting(); }
	
	T& Get() const { return *pt; }

protected:
	T* pt;
};

END_UPP_NAMESPACE

#endif
