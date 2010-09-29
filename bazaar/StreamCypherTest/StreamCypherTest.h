#ifndef _Snow2CypherTest_Snow2CypherTest_h
#define _Snow2CypherTest_Snow2CypherTest_h

#include <CtrlLib/CtrlLib.h>
#include <Controls4U/Controls4U.h>

using namespace Upp;

#define LAYOUTFILE <StreamCypherTest/StreamCypherTest.lay>
#include <CtrlCore/lay.h>

class StreamCypherTest : public WithStreamCypherTestLayout<TopWindow>
{
	private:
		void onCypherSwitch(void);
		void onGo(void);
		
	public:
		typedef StreamCypherTest CLASSNAME;

		StreamCypherTest();
};

#endif

