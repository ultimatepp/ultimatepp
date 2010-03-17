#include "TestScatter2.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>
#include <plugin/jpg/jpg.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <TestScatter2/TestScatter2.iml>
#include <Draw/iml_source.h>

TestScatter2::TestScatter2()
{
	CtrlLayout(*this, "Scatter Test");
	
	tc1.Add(tab1, "Basic");	
	
	tc1.Add(tab2, "Functions");
	
	tc1.Add(tab3, "Parametric Functions");
	
	tc1.Add(tab4, "Formatting");
	
	tc1.Add(tab5, "Dynamic");
	
	tc1.Add(tab6, "Log");
	
	tc1.Add(tab7, "Operations");	
	
	tc1.Add(tab8, "Secondary Y");
	
	b1 <<= THISBACK(Preview);
	b2 <<= THISBACK(SavePNG);
	b3 <<= THISBACK(SaveJPG);
	#ifdef PLATFORM_WIN32
	b4 <<= THISBACK(SaveEMF);
	#endif
	b5 <<= THISBACK(SaveClipboard);
	Sizeable().Zoomable().Icon(MyImages::i1());
}

void TestScatter2::Preview()
{
	Report r;	
	int ntab=tc1.Get();
	
	switch (ntab)
	{
		case 0 :	
			r.DrawDrawing(300,300,tab1.scatter1.GetDrawing().GetSize().cx,tab1.scatter1.GetDrawing().GetSize().cy, tab1.scatter1.GetDrawing());
			break;	
		case 1:
			r.DrawDrawing(300,300,tab2.scatter2.GetDrawing().GetSize().cx,tab2.scatter2.GetDrawing().GetSize().cy, tab2.scatter2.GetDrawing());
			break;
		case 2:
			r.DrawDrawing(300,300,tab3.scatter3.GetDrawing().GetSize().cx,tab3.scatter3.GetDrawing().GetSize().cy, tab3.scatter3.GetDrawing());
			break;
		case 3:
			r.DrawDrawing(300,300,tab4.scatter4.GetDrawing().GetSize().cx,tab4.scatter4.GetDrawing().GetSize().cy, tab4.scatter4.GetDrawing());
			break;
		case 4:
			r.DrawDrawing(300,300,tab5.scatter5.GetDrawing().GetSize().cx,tab5.scatter5.GetDrawing().GetSize().cy, tab5.scatter5.GetDrawing());
			break;
		case 5:
			r.DrawDrawing(300,300,tab6.scatter6.GetDrawing().GetSize().cx,tab6.scatter6.GetDrawing().GetSize().cy, tab6.scatter6.GetDrawing());
			break;
		case 6:
			r.DrawDrawing(300,300,tab7.scatter7.GetDrawing().GetSize().cx,tab7.scatter7.GetDrawing().GetSize().cy, tab7.scatter7.GetDrawing());
			break;							
		case 7:
			r.DrawDrawing(300,300,tab8.scatter8.GetDrawing().GetSize().cx,tab8.scatter8.GetDrawing().GetSize().cy, tab8.scatter8.GetDrawing());
			break;					
	}
	
	Perform(r);
}

void TestScatter2::SavePNG()
{
	PNGEncoder encoder;
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			encoder.SaveFile("scatter1.png",tab1.scatter1.GetImage(3));
			break;
		case 1:	
			encoder.SaveFile("scatter2.png",tab2.scatter2.GetImage(3));
			break;
		case 2:
			encoder.SaveFile("scatter3.png",tab3.scatter3.GetImage(3));
			break;
		case 3:
			encoder.SaveFile("scatter4.png",tab4.scatter4.GetImage(3));
			break;
		case 4:
			encoder.SaveFile("scatter5.png",tab5.scatter5.GetImage(3));
			break;
		case 5:
			encoder.SaveFile("scatter6.png",tab6.scatter6.GetImage(3));
			break;
		case 6:
			encoder.SaveFile("scatter7.png",tab7.scatter7.GetImage(3));
			break;				
		case 7:
			encoder.SaveFile("scatter8.png",tab8.scatter8.GetImage(3));
			break;
	}			
	
}

void TestScatter2::SaveJPG()
{
	JPGEncoder encoder(90);
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			encoder.SaveFile("scatter1.jpg",tab1.scatter1.GetImage(3));
			break;
		case 1:
			encoder.SaveFile("scatter2.jpg",tab2.scatter2.GetImage(3));
			break;
		case 2:
			encoder.SaveFile("scatter3.jpg",tab3.scatter3.GetImage(3));
			break;
		case 3:
			encoder.SaveFile("scatter4.jpg",tab4.scatter4.GetImage(3));
			break;
		case 4:
			encoder.SaveFile("scatter5.jpg",tab5.scatter5.GetImage(3));
			break;
		case 5:
			encoder.SaveFile("scatter6.jpg",tab6.scatter6.GetImage(3));
			break;
		case 6:
			encoder.SaveFile("scatter7.jpg",tab7.scatter7.GetImage(3));
			break;				
		case 7:
			encoder.SaveFile("scatter8.jpg",tab8.scatter8.GetImage(3));
			break;		
	}			
	
}

#ifdef PLATFORM_WIN32
void TestScatter2::SaveEMF()
{
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			tab1.scatter1.SaveAsMetafile("scatter1.emf");
			break;
		case 1:
			tab2.scatter2.SaveAsMetafile("scatter2.emf");
			break;
		case 2:
			tab3.scatter3.SaveAsMetafile("scatter3.emf");
			break;
		case 3:
			tab4.scatter4.SaveAsMetafile("scatter4.emf");
			break;
		case 4:
			tab5.scatter5.SaveAsMetafile("scatter5.emf");
			break;
		case 5:
			tab6.scatter6.SaveAsMetafile("scatter6.emf");
			break;
		case 6:
			tab7.scatter7.SaveAsMetafile("scatter7.emf");
			break;				
		case 7:
			tab8.scatter8.SaveAsMetafile("scatter8.emf");
			break;			
	}	
}
#endif

void TestScatter2::SaveClipboard()
{
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			tab1.scatter1.SaveToClipboard();
			break;
		case 1:
			tab2.scatter2.SaveToClipboard();
			break;
		case 2:
			tab3.scatter3.SaveToClipboard();
			break;
		case 3:
			tab4.scatter4.SaveToClipboard();
			break;
		case 4:
			tab5.scatter5.SaveToClipboard();
			break;
		case 5:
			tab6.scatter6.SaveToClipboard();
			break;
		case 6:
			tab7.scatter7.SaveToClipboard();
			break;				
		case 7:
			tab8.scatter8.SaveToClipboard();
			break;
	}		
}

GUI_APP_MAIN
{
	TestScatter2().Run();
}

