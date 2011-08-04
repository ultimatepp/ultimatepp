#ifndef _DispatcherTest_DispatcherTest_h
#define _DispatcherTest_DispatcherTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DispatcherTest3/DispatcherTest.lay>
#include <CtrlCore/lay.h>

#include <Dispatcher/Dispatcher.hpp>

template<class C, class T>
class WithDispatch
	: public C
	, public Dispatchable<T>

{
public:
	typedef WithDispatch<C, T> CLASSNAME;

	virtual void Dispatch(const T& o)
	{
		C::SetData(o);	
	}	
};

template<class T>
class ProgressIndicatorWithDispatch
	: public ProgressIndicator
	, public Dispatchable<T>

{
public:
	typedef ProgressIndicatorWithDispatch<T> CLASSNAME;

	virtual void Dispatch(const T& o)
	{
		Set(o);
	}	
};


class DispatcherTest : public WithDispatcherTestLayout<TopWindow> 
{
public:
	typedef DispatcherTest CLASSNAME;
	DispatcherTest();
	
	void sliderCB();
	void slideriCB();

	Dispatcher<Value> disp;
	Dispatcher<int> dispi;

	Array<Ctrl> ctrls;
	Splitter splitv1, splitv2, splith;
};

#endif

