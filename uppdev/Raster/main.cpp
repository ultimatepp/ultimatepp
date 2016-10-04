#include <CtrlLib/CtrlLib.h>

class SimpleImageViewer : public TopWindow {

	void MainMenu(Bar& bar) {
	    bar.Add("File",THISBACK(FileMenu));
	}
  void FileMenu(Bar& bar) {
    bar.Add("Open image...",THISBACK(OpenImage));
    bar.Add("Exit",THISBACK(Close));
  }
  void OpenImage() {
    FileSel	fs;
    fs.Type("Image files","*.jpg,*.png,*.bmp");
    fs.AllFilesType();
    fs.ActiveDir(GetFileDirectory(GetDataFile("x")));
    if (fs.ExecuteOpen("Select image to load")) {
      String filename = fs.GetFile(0);
      status = Format("Loading [%s]",filename);
      Image im = StreamRaster::LoadFileAny(filename, Progress("Loading image"));
      if (im.IsNullInstance()) {
        status = Format("Failed to load [%s]",filename);
      } else {
        status = Format("Loaded [%s]",filename);
      }
      image_display.SetImage(im);
    }
  }

public:
	typedef SimpleImageViewer CLASSNAME; // Necessary for THISBACK()

	MenuBar menu;
	StatusBar status;
	Label image_display;

	SimpleImageViewer() {
		Zoomable().Sizeable().Title("Simple Image Viewer");
		AddFrame(menu);
		menu.Set(THISBACK(MainMenu));
		AddFrame(status);

		image_display.SetPaintRect(PaintRect(ImageDisplay()));
		image_display.AddFrame(ThinInsetFrame());
		Add(image_display.HSizePosZ(5,5).VSizePosZ(5,5));
	}

};

GUI_APP_MAIN
{
	SimpleImageViewer().Run();
}