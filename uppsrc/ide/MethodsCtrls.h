#ifndef _ide_MethodsCtrls_h_
#define _ide_MethodsCtrls_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class TextOption : public Option {
public:
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
};

END_UPP_NAMESPACE

#endif
