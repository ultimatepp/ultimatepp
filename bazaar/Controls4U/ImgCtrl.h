#ifndef _Controls4U_ImgCtrl_h_
#define _Controls4U_ImgCtrl_h_

#define MAX_STEP 0.5
#define MIN_ZOOM_FACTOR 0.1
#define MAX_ZOOM_FACTOR 5.00

class ImgCtrl : public Ctrl {
public:
	typedef ImgCtrl CLASSNAME;
	ImgCtrl();
	
	void Zoom(double m_fStep);
	void Fit();
	void RealSize();
	
	void SetStepZoomFactor(double f_step);
	void SetZoomVisible(bool b_show);
	void SetThumbVisible(bool _thumb);
	
	Image &GetImage() {return image;};	
	void SetImage(ImageBuffer img);
	void SetImage(Image img);
	void SetImage(const char *fileName);
	
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	
	int GetHeight() {return image.GetHeight();};
	int GetWidth() 	{return image.GetWidth();};
	void GetRectRep(double &left, double &top, double &width, double &height);
	void GetRepXY(double x, double y, int &repx, int &repy);
	void GetXYRep(int repx, int repy, double &x, double &y);
	
	void Load(const WString str_img_path);
	bool Save(String fileName, String ext, int qualityBpp = -1);
	void Print(int n_draw_origin_x=0,int n_draw_origin_y=0,int n_image_cx=1000,int n_image_cy=1000);
		
	bool IsEmpty();
	
	void BW();
	
	virtual void Paint(Draw& w);
	
private:
	void LocalMenu(Bar& bar);
	
	double x0, y0; 			//Coordenada origem para desenho
	double xDelta, yDelta; 	//Guarda o ultimo deslocamento em relação ao 0,0
	
	int width, height;
	
	Image   image;
	Image   imageRep;
	double	factor;
	
	bool 	moving; 
	Point 	lastPoint;
	bool    showZoom;
	bool 	thumb;
};

#endif
