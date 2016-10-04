#ifndef _PDF_PDF_h
#define _PDF_PDF_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <PDF/PDF.lay>
#include <CtrlCore/lay.h>



class PDF : public WithPDFLayout<TopWindow> {
public:
	typedef PDF CLASSNAME;
	PDF();
};

#endif
