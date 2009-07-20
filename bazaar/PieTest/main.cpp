#include "PieTest.h"

#define IMAGECLASS MyImg
#define IMAGEFILE <PieTest/PieTest.iml>
#include <Draw/iml.h>

#define TOPICFILE <PieTest/app.tpp/all.i>
#include <Core/topic_group.h>
	
void PieTest::EditCategory()
{
	WithSetData<TopWindow> setData;
	CtrlLayoutOKCancel(setData, "Edit Category");
	setData.array1.SetLineCy(21);
	setData.array1.NoCursor();
	setData.array1.AddColumn("Index",44);
	setData.array1.AddColumn("Category Name",120).Ctrls<EditString>();
	setData.array1.AddColumn("Value",48).Ctrls<EditDouble>();
	setData.array1.AddColumn("Color",46).Ctrls<ColorPusher>();	
	
	setData.array1.SetEditable();
	
	for(int i=0;i<pie1.GetCatCount();i++)
	{
		setData.array1.Set(i,0,i);
		setData.array1.Set(i,1,pie1.GetCatName(i));
		setData.array1.Set(i,2,pie1.GetCatValue(i));
		setData.array1.Set(i,3,pie1.GetCatColor(i));
	}
	if(setData.Execute() != IDOK)
		return;	
	for(int i=0;i<pie1.GetCatCount();i++)
	{
		pie1.SetCatName(i,setData.array1.Get(i,1));
		pie1.SetCatValue(i,setData.array1.Get(i,2));
		pie1.SetCatColor(i,setData.array1.Get(i,3));
	}
	
}
void PieTest::AddCategory()
{
	WithAddCategory<TopWindow> addCategory;
	CtrlLayoutOKCancel(addCategory, "Add Category");
	addCategory.cp1.WithText();
	if(addCategory.Execute() != IDOK)
		return;
	pie1.AddCategory(~addCategory.str1,~addCategory.d1,~addCategory.cp1);
}
void PieTest::RemoveCategory()
{
	WithRemoveCategory<TopWindow> removeCat;
	CtrlLayoutOKCancel(removeCat, "Remove Category");
	if(removeCat.Execute() != IDOK)
		return;
	pie1.RemoveCategory(~removeCat.int1);
}
void PieTest::PrintPreview()
{
	Report rep;	
	Drawing dd(pie1.GetDrawing());
	QtfRichObject pieQTF(CreateDrawingObject(dd));
	String qtf;
	qtf<<"[= "<<pieQTF<<"]&"
	   <<"-|[2 A pie chart is a circular chart divided into sectors, illustrating relative magnitudes or frequencies. In a pie chart, the arc length of each sector (and consequently its central angle and area), is proportional to the quantity it represents. Together, the sectors create a full disk. "
	   <<"[@B (http://en.wikipedia.org/wiki/Pie`_chart)]&&"
	   <<"[_ Warnings:] &-|`-do not use more than six categories `- otherwise, the resulting picture will be too complex to understand.&-|`-avoid pie charts when the values of each component are similar because it is difficult to see the differences between slice sizes.";
	   
 
	rep<<qtf;
	
	                                      
	Perform(rep,"Pie Chart Preview");
	
}
void PieTest::ExportPNG()
{
	PNGEncoder encoder;
	encoder.SaveFile("pie1.png",pie1.GetImage());
}
#ifdef PLATFORM_WIN32
void PieTest::ExportEMF()
{
	pie1.SaveAsMetafile("pie1.emf");
}
#endif
void PieTest::Help()
{	
	HelpWindow help;
	help.AddTree(0,MyImg::i1(),"topic://PieTest/app/page1$en-us","intro");
	help.AddTree(0,MyImg::i1(),"topic://PieTest/app/page2$en-us","page2");
	help.FinishTree();	
	
	help.GoTo("topic://PieTest/app/page1$en-us");
	help.Execute();
}
	
PieTest::PieTest()
{
	CtrlLayout(*this, "Pie Chart Demo");
	SetSkin(ChStdSkin);
	Icon(MyImg::i1()).Sizeable();
	b1.SetImage(MyImg::i2());
	b2.SetImage(MyImg::i3());
	b3.SetImage(MyImg::i4());
	b4.SetImage(MyImg::i5());
	b5.SetImage(MyImg::i6());
	b6.SetImage(MyImg::i7());
	
	b1<<=THISBACK(EditCategory);
	b2<<=THISBACK(AddCategory);
	b3<<=THISBACK(RemoveCategory);
	b4<<=THISBACK(PrintPreview);
	b5<<=THISBACK(ExportPNG);
#ifdef PLATFORM_WIN32
	b6<<=THISBACK(ExportEMF);
#else
	b6.Hide();
#endif
	help <<= THISBACK(Help);	

	pie1.AddCategory("CatA",17,Color(90,150,255));
	pie1.AddCategory("CatB",9.2,Color(90,90,255));	
}

GUI_APP_MAIN
{
	PieTest().Run();	
}

