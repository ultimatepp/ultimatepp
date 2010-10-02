#ifndef _CypherTest_CypherTest_h
#define _CypherTest_CypherTest_h

#include <CtrlLib/CtrlLib.h>
#include <Controls4U/Controls4U.h>

using namespace Upp;

#define LAYOUTFILE <CypherTest/CypherTest.lay>
#include <CtrlCore/lay.h>

class CypherTest : public WithCypherTestLayout<TopWindow>
{
	private:
		void onCypherSwitch(void);
		void onGo(void);
		
	public:
		typedef CypherTest CLASSNAME;

		CypherTest();
};

#endif

