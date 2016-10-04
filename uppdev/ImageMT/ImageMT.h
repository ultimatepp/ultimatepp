#ifndef _ImageMT_ImageMT_h
#define _ImageMT_ImageMT_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ImageMT/ImageMT.lay>
#include <CtrlCore/lay.h>


class Layer
{
public:
	Layer(){}
	Layer(Layer* pSource,double op,void* pMain);

	static void MakeLayerAsThread(Layer* pLayer,double op,void* pMain,int n);


	const RGBA* GetImage(int i){return m_img[i];}


	Rectf m_rc;


	Image m_img;

};


class ImageMT : public WithImageMTLayout<TopWindow> {
public:
	typedef ImageMT CLASSNAME;
	ImageMT();
	~ImageMT();

	Atomic threads;

	static void SquareImage();

	void LoadImage();
	void Square();

	void AddLayer(Layer* pLayer){m_pLayers.Add(pLayer);}

	void StartProg(int Steps)	{m_prog.SetTotal(Steps);}
	void Step()					{m_prog++;}
	void EndProg()				{m_prog.Set(0);}

protected:
	void Paint(Draw& w);

	Array<Layer*> m_pLayers;


};

#endif
