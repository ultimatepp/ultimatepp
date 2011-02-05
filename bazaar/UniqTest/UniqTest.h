#ifndef _UniqTest_UniqTest_h
#define _UniqTest_UniqTest_h

#include <CtrlLib/CtrlLib.h>

#include <Uniq/Uniq.h>

using namespace Upp;

#define LAYOUTFILE <UniqTest/UniqTest.lay>
#include <CtrlCore/lay.h>

class UniqTest : public WithUniqTestLayout<TopWindow>
{
	private:
	public:
		typedef UniqTest CLASSNAME;
		UniqTest();

		// handles news app instances
		void handleInstance(Vector<String> const &v);
		
};

#endif

