#include "ImageMT.h"



ImageMT::ImageMT()
{
	CtrlLayout(*this, "Window title");

	buttonLoad <<= THISBACK(LoadImage);

	m_fPower.SetData(2.3);

	buttonSquare <<= THISBACK(Square);


}

ImageMT::~ImageMT()
{
	for(int i=0;i<m_pLayers.GetCount();  )
	{
		Layer* pLayer = m_pLayers[0];
		delete pLayer;
		m_pLayers.Remove(0);
	}

}



void ImageMT::LoadImage()
{
	FileSel dlg;
	dlg.Type("Image files","*.bmp *.png *.tif *.jpg");
	dlg.Type("Image: JPEG","*.jpg");
	dlg.Type("Image: Portable Nework Graphics","*.png");
	dlg.Type("Image: Tagged Image Format","*.tif");
	dlg.Type("Image: Windows Bitmap (24bit)","*.bmp");
	dlg.ActiveDir(GetCurrentDirectory());

	if(!dlg.ExecuteOpen("Open Image"))return;

	SetCurrentDirectory(dlg.GetActiveDir());

	String path = ~dlg;

	FileIn in(path);

	if(in.IsError())
	{
		String s = Format("Error opening image file %s",path);
		PromptOK(s);
		return;
	}

	One<StreamRaster> r = StreamRaster::OpenAny(in);

	if(!r)
	{
		String s = Format("Error opening streamraster file %s",path);
		PromptOK(s);
		return;
	}

	Layer* pLayer = new Layer;

	Size sz = GetSize();

	pLayer->m_img = r->GetImage();
	pLayer->m_rc.left = 0;
	pLayer->m_rc.right = 1;
	pLayer->m_rc.top = 0;
	pLayer->m_rc.bottom = 1;

	m_pLayers.Add(pLayer);
}

void ImageMT::Paint(Draw& w)
{
	Size sz = GetSize();

	if(m_pLayers.GetCount()<=0)
		w.DrawRect(sz,SWhite);

	for(int i=0;i<m_pLayers.GetCount();i++)
	{
		Rect rc;
		rc.left = m_pLayers[i]->m_rc.left * sz.cx;
		rc.right = m_pLayers[i]->m_rc.right * sz.cx;
		rc.top = m_pLayers[i]->m_rc.top * sz.cy;
		rc.bottom = m_pLayers[i]->m_rc.bottom * sz.cy;
		w.DrawImage(rc,m_pLayers[i]->m_img);
	}
}

void ImageMT::Square()
{
	if(m_pLayers.GetCount()<=0)
		return;

	Layer* pLayer = m_pLayers[0];

	double f = m_fPower.GetData();

	AtomicInc(threads);
	Thread().Run(callback4(&Layer::MakeLayerAsThread,pLayer,f,this,0));
}

Layer::Layer(Layer* pLayer,double op,void* pMain)
{
	Size szi = pLayer->m_img.GetSize();

	ImageMT* ptr = (ImageMT*)pMain;

	Size sz = ptr->GetSize();

	ImageBuffer ib(szi);

	PostCallback(callback1(ptr,&ImageMT::StartProg,szi.cy));

	for(int i=0;i<szi.cy;i++)
	{
//		const RGBA* pOrg = pLayer->m_img[i];
		const RGBA* pOrg = pLayer->GetImage(i);
		RGBA* pNew = ib[i];

		for(int j=0;j<szi.cx;j++)
		{
			pNew[j].r = (byte)pow(pOrg[j].r,op);
			pNew[j].g = (byte)pow(pOrg[j].g,op);
			pNew[j].b = (byte)pow(pOrg[j].b,op);
			for(int k=0;k<100;k++)
			{
				double f = sqrt(j*32456.23145);
			}
		}

		PostCallback(callback(ptr,&ImageMT::Step));
	}

	PostCallback(callback(ptr,&ImageMT::EndProg));

	m_img = ib;

	m_rc.left = rand()/(double)RAND_MAX;
	m_rc.right = rand()/(double)RAND_MAX;
	m_rc.top = rand()/(double)RAND_MAX;
	m_rc.bottom = rand()/(double)RAND_MAX;

}

void Layer::MakeLayerAsThread(Layer* pLayer,double op,void* pMain,int n)
{
	ImageMT* ptr = (ImageMT*)pMain;

	Layer* pNewLayer = new Layer(pLayer,op,pMain);

	ptr->AddLayer(pNewLayer);
	ptr->Refresh();
	AtomicDec(ptr->threads); // the last thing this thread does is clean up
}





GUI_APP_MAIN
{
	ImageMT().Sizeable().Zoomable().Run();
}
