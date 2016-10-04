#ifndef _beta_beta_h
#define _beta_beta_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <beta/beta.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <beta/beta.iml>
#include <Draw/iml_header.h>

class beta : public WithbetaLayout<TopWindow> {
public:
	typedef beta CLASSNAME;
	beta();
};

#endif
