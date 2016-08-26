#ifndef _ide_MethodsCtrls_h_
#define _ide_MethodsCtrls_h_

#include <CtrlLib/CtrlLib.h>

namespace Upp {

class TextOption : public Option {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

class TextSwitch : public Switch {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

}

#endif
