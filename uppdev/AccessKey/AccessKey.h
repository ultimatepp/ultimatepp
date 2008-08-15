#ifndef _AccessKey_AccessKey_h
#define _AccessKey_AccessKey_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <AccessKey/AccessKey.lay>
#include <CtrlCore/lay.h>



class AccessKey : public WithAccessKeyLayout<TopWindow> {
public:
	typedef AccessKey CLASSNAME;
	AccessKey();
};

#endif
