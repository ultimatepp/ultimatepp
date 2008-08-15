#include "e011.h"

void DirSel(EditField& f, FrameRight<Button>& b) {
	f.AddFrame(b);
	b <<= callback1(&sSetFolder, &f);
	b.SetImage(CtrlImg::smallright()).NoWantFocus();
}

static void sSetFolder(EditField *f) {
	//FileSelector sSD;
	FileSel sSD;
	sSD.ActiveDir((const char*)~AsString(f->GetText()));
	if(!sSD.ExecuteSelectDir()) return;
	*f <<= ~sSD;
}
