#include "PieCtrl.h"

NAMESPACE_UPP

PieCtrl::PieCtrl() : copyRatio(1) {
	Transparent();
	BackPaint();
}

#ifdef PLATFORM_WIN32

void PieCtrl::SaveAsMetafile(const char* file)
{
	GuiLock __;
	WinMetaFileDraw wmfd;	
	wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "Pie", "PieChart", file);
	PaintPie(wmfd, copyRatio);	
	wmfd.Close();
}
	
void PieCtrl::SaveToClipboard(bool saveAsMetafile) 
{
	GuiLock __;
	if (saveAsMetafile) {
		WinMetaFileDraw wmfd;		
		wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "ScatterCtrl", "chart");
		PaintPie(wmfd, copyRatio);	
		WinMetaFile wmf = wmfd.Close();	 
		wmf.WriteClipboard();
	} else {
		Image img = GetImage(copyRatio);
		WriteClipboardImage(img);	
	}
}

#else

void PieCtrl::SaveToClipboard(bool) 
{
	GuiLock __;
	Image img = GetImage(copyRatio);
	WriteClipboardImage(img);
}

#endif

void PieCtrl::Paint(Draw& w)
{
	PaintPie(w, 1);
}

END_UPP_NAMESPACE