#include "PieCtrl.h"

#define IMAGECLASS PieImg
#define IMAGEFILE <ScatterCtrl/ScatterCtrl.iml>
#include <Draw/iml.h>

namespace Upp {
	
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
		wmfd.Create(copyRatio*GetSize().cx, copyRatio*GetSize().cy, "Pie", "PieChart");
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

void PieCtrl::RightDown(Point, dword) {
	MenuBar::Execute([=](Bar& bar){ContextMenu(bar);});
}

void PieCtrl::ContextMenu(Bar& bar)
{
	bar.Add(t_("Copy"), PieImg::Copy(), [=] {SaveToClipboard(false);}).Key(K_CTRL_C)
									.Help(t_("Copy image to clipboard"));
	bar.Add(t_("Save to file"), PieImg::Save(), [=] {SaveToFile(Null);}).Key(K_CTRL_S)
									.Help(t_("Save image to file"));
}

void PieCtrl::OnFileToSave() {
	String name = ~fileToSave;
	int ext = fileToSave.GetActiveType();
	if (ext == 0) 
		fileToSave.file = ForceExt(name, ".jpg");
	else
		fileToSave.file = ForceExt(name, ".png");
}

void PieCtrl::SaveToFile(String fileName)
{
	GuiLock __;
	if (IsNull(fileName)) {
		String name = GetTitle();
		if (name.IsEmpty())
			name = t_("Pie plot");
		fileToSave.PreSelect(ForceExt(name, ".jpg"));
		fileToSave.ClearTypes();
		fileToSave.Type(Format(t_("%s file"), "JPEG"), "*.jpg");
		fileToSave.Type(Format(t_("%s file"), "PNG"), "*.png");
		fileToSave.type.WhenAction = [=] {OnFileToSave();};
	    if(!fileToSave.ExecuteSaveAs(t_("Saving plot to PNG or JPEG file"))) {
	        Exclamation(t_("Plot has not been saved"));
	        return;
	    }
        fileName = ~fileToSave;
	} 
	if (GetFileExt(fileName) == ".png") {
		PNGEncoder encoder;
		encoder.SaveFile(fileName, GetImage(copyRatio));
	} else if (GetFileExt(fileName) == ".jpg") {	
		JPGEncoder encoder(90);
		encoder.SaveFile(fileName, GetImage(copyRatio));		
	} else
		Exclamation(Format(t_("File format \"%s\" not found"), GetFileExt(fileName)));
}

}