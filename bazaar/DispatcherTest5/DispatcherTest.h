#ifndef _DispatcherTest_DispatcherTest_h
#define _DispatcherTest_DispatcherTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DispatcherTest5/DispatcherTest.lay>
#include <CtrlCore/lay.h>

#include <Dispatcher/Dispatcher.hpp>

class MyEditInt
	: public EditInt
	, public Dispatchable<Value>
	, public Dispatchable<int>
{
public:
	typedef MyEditInt CLASSNAME;

	virtual void Dispatch(const Value & o, unsigned param)
	{
		SetData(o);
	}	
	virtual void Dispatch(const int & o, unsigned param)
	{
		SetData(o);
	}	
};

class MyProgressIndicator
	: public ProgressIndicator
	, public Dispatchable<Value>
	, public Dispatchable<int>
{
public:
	typedef MyProgressIndicator CLASSNAME;

	virtual void Dispatch(const Value & o, unsigned param)
	{
		Set(o);
	}	
	virtual void Dispatch(const int & o, unsigned param)
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

	DispatcherGen disp;

	Array<Ctrl> ctrls;
	Splitter splitv1, splitv2, splith;
};

#endif

