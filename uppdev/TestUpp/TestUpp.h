#ifndef _TestUpp_TestUpp_h
#define _TestUpp_TestUpp_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class DocEditEx : public DocEdit
{
	virtual bool Key(dword key, int count) {
		return DocEdit::Key(key < 65536 ? ToUpper((wchar)key) : key, count);
	}
};

#define LAYOUTFILE <TestUpp/TestUpp.lay>
#include <CtrlCore/lay.h>

class TestUpp : public WithTestUppLayout<TopWindow> {
public:
	typedef TestUpp CLASSNAME;
	TestUpp();
};

#endif
