#include "e011.h"

void App::SetupPath() {
	EditString 	es;
	FrameRight<Button> fr1;
	DirSel(es, fr1);

	WithsettingFoldersLay<TopWindow> form;
	CtrlLayoutOKCancel(form, t_("Paths setting"));
	form.Icon(commonImg::folder_1_16());
	form.percorsi.AddColumn(t_("Folder"), 128);
	form.percorsi.AddColumn(t_("Path"), 256).Edit(es);
	form.percorsi.Set(0, 0, t_("Maps Folder"));
	form.percorsi.Set(0, 1, paths.At(0));

	if(form.Run() == IDOK) {
		paths.At(0) = form.percorsi.Get(0, 1);
	}
	return;
}
