#ifndef _OpenCV_OpenCV_h_
#define _OpenCV_OpenCV_h_

#include <CtrlLib/CtrlLib.h>
#include <cv.h>
#include <highgui.h>

using namespace Upp;

#include <Controls4U/Controls4U.h>

IplImage *ImageToCv(Image &img, bool iscolor = false);
Image CvToImage(IplImage *imgCV);

class ImageWindow : public TopWindow {
typedef ImageWindow CLASSNAME;
public:
	ImageWindow(String _title);

	StaticImage image;	
	String title;
	bool waitKey;
	
	bool ShowCV(IplImage *imgCV);
	virtual void Close();
	virtual void LeftDown(Point p, dword keyflags);
	void WaitKey();
};

#endif
