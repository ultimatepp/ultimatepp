#ifndef _DispatcherTest_DispatcherTest_h
#define _DispatcherTest_DispatcherTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DispatcherTest6/DispatcherTest.lay>
#include <CtrlCore/lay.h>

#include <Dispatcher/Dispatcher.hpp>

class MyEditInt
	: public EditInt
{
public:
	typedef MyEditInt CLASSNAME;

	virtual void DispatchV(const Value& o)
	{
		SetData(o);
	}	
	virtual void DispatchI(const int& o)
	{
		SetData(o);
	}	
};

class MyProgressIndicator
	: public ProgressIndicator
{
public:
	typedef MyProgressIndicator CLASSNAME;

	virtual void DispatchV(const Value& o)
	{
		Set(o);
	}	
	virtual void DispatchI(const int& o)
	{
		Set(o);
	}	
};

class DispatcherTest : public WithDispatcherTestLayout<TopWindow> 
{
public:
	typedef DispatcherTest CLASSNAME;
	DispatcherTest();
	~DispatcherTest();
	
	void sliderCB();
	void slideriCB();
	virtual void Dispatch(const Value & o);

	DispatcherCBGen disp;

	Array<Ctrl> ctrls;
	Splitter splitv1, splitv2, splith;
};

#endif

