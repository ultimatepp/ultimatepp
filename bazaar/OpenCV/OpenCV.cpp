#include <OpenCV/OpenCV.h>

IplImage *ImageToCv(Image &img, bool iscolor) {
	CvSize sz;
	sz.width = img.GetWidth();
	sz.height = img.GetHeight();
	IplImage *src = cvCreateImage(sz, 8, 1);
	
	for (int y = 0; y < img.GetHeight(); y++) {
		uchar* ptr = (uchar*) (src->imageData + y * src->widthStep);
		for (int x = 0; x < img.GetWidth(); x++) {
			if (iscolor) {
				ptr[3*x] = img[y][x].r;
				ptr[3*x+1] = img[y][x].g;
				ptr[3*x+2] = img[y][x].b;
			} else
				ptr[x] = Grayscale(img[y][x]);
		}
	}
	return src;
}

Image CvToImage(IplImage *imgCV) {
	if (!imgCV)
		return Null;
	CvSize sz = cvGetSize(imgCV);
	
	ImageBuffer img(sz.width, sz.height);
	
	for (int y = 0; y < img.GetHeight(); y++) {
		uchar* ptr = (uchar*) (imgCV->imageData + y * imgCV->widthStep);
		for (int x = 0; x < img.GetWidth(); x++) {
			if (imgCV->nChannels > 1) {
				img[y][x].r = ptr[3*x];
				img[y][x].g = ptr[3*x+1];
				img[y][x].b = ptr[3*x+2];
			} else 
				img[y][x].r = img[y][x].g = img[y][x].b = ptr[x];
		}
	}
	return img;
}

ImageWindow::ImageWindow(String _title) {
	title = _title;
	Title(title).Sizeable().Zoomable();
	waitKey = false;
	Add(image.SizePos());
	image.IgnoreMouse();
	OpenMain();
};

bool ImageWindow::ShowCV(IplImage *imgCV) {
	Image img;
	img = CvToImage(imgCV);
	if (!IsOpen())
		OpenMain();
	return image.Set(img);
}

void ImageWindow::Close() {
	waitKey = false;
	TopWindow::Close();
}

void ImageWindow::LeftDown(Point p, dword keyflags) {
	if (waitKey)
		waitKey = false;
}

void ImageWindow::WaitKey() {
	waitKey = true;
	String title = GetTitle().ToString();
	Title(title + ". Waiting for a click...");
	while (waitKey) {
		Sleep(20);
		Ctrl::ProcessEvents();
	}
	Title(title);
}

