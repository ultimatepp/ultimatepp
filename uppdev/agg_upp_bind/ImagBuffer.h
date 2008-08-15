//#ifndef _agg_upp_bind__ImagBuffer_h_
//#define _agg_upp_bind__ImagBuffer_h_

//#include <CtrlLib/CtrlLib.h>

//NAMESPACE_UPP

class ImagBuffer : public ImageBuffer {

public:
	RGBA GetPixel(int x, int y) const;
	void SetPixel(int x, int y, RGBA color);
	void SetPixel(int x, int y, Color color, int alpha = 255);
	void SetLineV(int x, int y, int height, RGBA rgba);

	bool IsEqualColumn(int x, int y, int height, RGBA rgba);
	Image CreateCopy(int x, int y, int width, int height);

	void Rectangalize(Draw& w, bool norect=true); //showrect for debug
	
	typedef ImagBuffer CLASSNAME;
//	void DrawRect(int x, int y, int cx, int cy, Color color, int alpha = 255);
//	ImagBuffer() {;}
};

//END_UPP_NAMESPACE

//#endif
