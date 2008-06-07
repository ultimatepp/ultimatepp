#ifndef _PieTest_PieTest_h
#define _PieTest_PieTest_h

#include <Scatter/Pie.h>
#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

#define LAYOUTFILE <PieTest/PieTest.lay>
#include <CtrlCore/lay.h>



class PieTest : public WithPieTestLayout<TopWindow> {
public:
	typedef PieTest CLASSNAME;
	PieTest();
	void EditCategory();
	void AddCategory();
	void RemoveCategory();
	void PrintPreview();
	void ExportPNG();
	#ifdef PLATFORM_WIN32
	void ExportEMF();
	#endif
	void Help();	
};

#endif

