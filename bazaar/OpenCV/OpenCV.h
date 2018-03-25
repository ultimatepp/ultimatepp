#ifndef _OpenCV_OpenCV_h_
#define _OpenCV_OpenCV_h_


#include <CtrlLib/CtrlLib.h>

#define int64 undef_int64
#define uint64 undef_uint64
#define CPU_SSE2 undef_CPU_SSE2

#include <opencv/cv.h>
#include <opencv/highgui.h>

#undef int64 
#undef uint64 
#undef CPU_SSE2 

#include <Controls4U/Controls4U.h>

using namespace Upp;


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
