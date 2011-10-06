#include "TestScatter2.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

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
	
	tc1.Add(tab9, "Big dataset");
	
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
		case 8:
			r.DrawDrawing(300,300,tab9.scatter9.GetDrawing().GetSize().cx,tab9.scatter9.GetDrawing().GetSize().cy, tab9.scatter9.GetDrawing());
			break;					
	}
	
	Perform(r);
}

void TestScatter2::SavePNG()
{
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			tab1.scatter1.SaveToImage("scatter1.png");
			break;
		case 1:	
			tab2.scatter2.SaveToImage("scatter2.png");
			break;
		case 2:
			tab3.scatter3.SaveToImage("scatter3.png");
			break;
		case 3:
			tab4.scatter4.SaveToImage("scatter4.png");
			break;
		case 4:
			tab5.scatter5.SaveToImage("scatter5.png");
			break;
		case 5:
			tab6.scatter6.SaveToImage("scatter6.png");
			break;
		case 6:
			tab7.scatter7.SaveToImage("scatter7.png");
			break;				
		case 7:
			tab8.scatter8.SaveToImage("scatter8.png");
			break;
		case 8:
			tab9.scatter9.SaveToImage("scatter9.png");
			break;
	}			
	
}

void TestScatter2::SaveJPG()
{
	int ntab=tc1.Get();	
	
	switch (ntab)
	{
		case 0:
			tab1.scatter1.SaveToImage("scatter1.jpg");
			break;
		case 1:
			tab2.scatter2.SaveToImage("scatter2.jpg");
			break;
		case 2:
			tab3.scatter3.SaveToImage("scatter3.jpg");
			break;
		case 3:
			tab4.scatter4.SaveToImage("scatter4.jpg");
			break;
		case 4:
			tab5.scatter5.SaveToImage("scatter5.jpg");
			break;
		case 5:
			tab6.scatter6.SaveToImage("scatter6.jpg");
			break;
		case 6:
			tab7.scatter7.SaveToImage("scatter7.jpg");
			break;				
		case 7:
			tab8.scatter8.SaveToImage("scatter8.jpg");
			break;		
		case 8:
			tab9.scatter9.SaveToImage("scatter9.jpg");
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
		case 8:
			tab9.scatter9.SaveAsMetafile("scatter9.emf");
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
		case 8:
			tab9.scatter9.SaveToClipboard();
			break;			
	}		
}

GUI_APP_MAIN
{
	TestScatter2().Run();
}

