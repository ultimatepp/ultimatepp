#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class PreviewImage : public ImageCtrl {
	void SetData(const Value& val) {
		String path = val;
		if(IsNull(path.IsEmpty()))
			SetImage(Null);
		else
			SetImage(StreamRaster::LoadFileAny(~path));
	}
};


GUI_APP_MAIN
{
	PreviewImage img;
	FileSel      fs; 
	
	fs.Type("Image file(s)", "*.jpg *.gif *.png *.bmp");
	fs.Preview(img);
	
	fs.ExecuteOpen();
}
