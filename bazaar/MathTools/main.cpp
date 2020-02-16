#include "MathTools.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>
#include <Theme/Theme.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <MathTools/MathTools.iml>
#include <Draw/iml_source.h>

#define TFILE <MathTools/MathTools.t>
#include <Core/t.h>

struct Example {
	Tool* (*ctrl)();
};

	
Upp::Array<Example>& Examples() {
	static Upp::Array<Example> x;
	return x;
}

void RegisterExample(Tool* (*ctrl)(), String fileName) {
	Example& x = Examples().Add();
	x.ctrl = ctrl;
}

bool CompareExamples(Example &a, Example &b) {return a.ctrl()->name < b.ctrl()->name;}


GUI_APP_MAIN
{
	SetLanguage(SetLNGCharset(GetSystemLNG(), CHARSET_UTF8));	
	
	MathTools tools;
	tools.Init();
	tools.Run();
	tools.End();
}

void MathTools::Init() {
	CtrlLayout(*this, "MathTools");
	
	Sort(Examples(), CompareExamples);
	
	for (int i = 0; i < Examples().GetCount(); ++i) {
		Examples()[i].ctrl()->Init();
		Examples()[i].ctrl()->SetFrame(ThinInsetFrame());
	}
	examplesList.NoHorzGrid().NoVertGrid();
	examplesList.AddColumn(t_("Tool"));
	for (int i = 0; i < Examples().GetCount(); ++i) {
		examplesList.Add(Examples()[i].ctrl()->name);
		Add((*(Examples()[i].ctrl())).HSizePosZ(102, 2).VSizePosZ(2, 2));
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
