#include "MathTools.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <MathTools/MathTools.iml>
#include <Draw/iml_source.h>


struct Example {
	Tool* (*ctrl)();
	String name;
};

Array<Example>& Examples() {
	static Array<Example> x;
	return x;
};

void RegisterExample(const char *name, Tool* (*ctrl)(), String fileName) {
	Example& x = Examples().Add();
	x.name = name;
	x.ctrl = ctrl;
}

bool CompareExamples(Example &a, Example &b) {return a.name < b.name;}

GUI_APP_MAIN
{
	Sort(Examples(), CompareExamples);
	
	MathTools tools;
	
	tools.Run();
	
	tools.End();
}

MathTools::MathTools() {
	CtrlLayout(*this, "MathTools");

	for (int i = 0; i < Examples().GetCount(); ++i)
		Examples()[i].ctrl()->Init();
			
	examplesList.NoHorzGrid().NoVertGrid();
	examplesList.AddColumn("Tool");
	for (int i = 0; i < Examples().GetCount(); ++i) {
		examplesList.Add(Examples()[i].name);
		Add((*(Examples()[i].ctrl())).HSizePosZ(130, 4).VSizePosZ(4, 8));
		examplesRects.Add(Examples()[i].ctrl());
	}
		
	for (int i = 0; i < examplesRects.GetCount(); ++i)
		examplesRects[i]->Hide();
		
	examplesList.WhenSel = THISBACK(OnSel);
	examplesList.Select(0, false);
	OnSel();

	Sizeable().Zoomable().Icon(MyImages::i1());
}

void MathTools::End() {
	for (int i = 0; i < Examples().GetCount(); ++i)
		Examples()[i].ctrl()->End();
}

void MathTools::OnSel() {
	for (int i = 0; i < examplesRects.GetCount(); ++i)
		examplesRects[i]->Hide();		
	int id = examplesList.GetCursor();
	if (id < 0)
		return;
	examplesRects[id]->Show();
	examplesRects[id]->SetFocus();
}
