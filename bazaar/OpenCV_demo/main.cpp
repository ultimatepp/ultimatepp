#include <OpenCV/OpenCV.h>

void demo(Image &img) {
	IplImage *src = ImageToCv(img);
    
    IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
    IplImage* color_dst = cvCreateImage(cvGetSize(src), 8, 3);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    int i;
    cvCanny(src, dst, 50, 200, 3);
    cvCvtColor(dst, color_dst, CV_GRAY2BGR);

    lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80, 30, 10);
    for(i = 0; i < lines->total; i++) {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8);
    }

    cvNamedWindow("Source", 1);
    cvShowImage("Source", src);

    cvNamedWindow("Hough", 1);
    cvShowImage("Hough", color_dst);

    cvWaitKey(0);
    
    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    cvReleaseImage(&color_dst);
    
    cvDestroyWindow("Source");
	cvDestroyWindow("Hough");
}


GUI_APP_MAIN
{
	ImageWindow img("Image");
	
	String fileName = GetDataFile("building.jpg");
	if (!FileExists(fileName)) {
		Exclamation("File building.jpg not found. Please check source");
		return;
	}
	img.image.Set(fileName);
	
	demo(img.image.Get());
	
	img.Run();
}

