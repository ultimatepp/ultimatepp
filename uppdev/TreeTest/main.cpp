#include "TreeTest.h"



TreeTest::TreeTest()
{
//	CtrlLayout(*this, "Window title");
	
	this->AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	
	this->AddFrame(TopSeparatorFrame());

	this->AddFrame(tool);
//	tool.Set(THISBACK(FillToolBar));
	
	this->AddFrame(status);
	status.AddFrame(m_val.Right(125));
	m_val.Set(" ");


	Size sz = GetSize();

	m_bPan = false;
	
	m_tree.SetRoot(CtrlImg::Dir,"Workspace");

//	m_tree.SetMain(this);
	
	m_tree.SizePos();
	

	AddFrame(m_sf.Left(m_tree.SizePos(), 250));

	this->AddFrame(InsetFrame());	
	
	m_rcBnds = Rect(0,10000,10000,0);
	
	BackPaint();
}

void TreeTest::MainMenu(Bar& bar) {

/*	bar.Add("File", THISBACK(SubMenuFile));
	bar.Add("Settings", THISBACK(SubMenuSettings));
	bar.Add("View", THISBACK(SubMenuView));
	bar.Add("Operations", THISBACK(SubMenuOperations));
	bar.Add("Help", THISBACK(SubMenuHelp));
*/
}
/*
void TreeTest::SubMenuFile(Bar& bar)
{
	
	
	
}
*/

void TreeTest::LeftDown(Point p, dword keyflags)
{
	m_ptPan = LPtoMP(p);
	m_ptPanLP = p;
	m_bPan = true;
}

void TreeTest::LeftUp(Point p, dword keyflags)
{
	m_bPan = false;
	Refresh();
}

void TreeTest::ShowCoords(Point pos)
{
	Point pt = LPtoMP(pos);
	String text = Format("[%d:%d]", pt.x, pt.y);
	m_pos.Set(0,text,100);
}


void TreeTest::MouseMove(Point pos, dword flags)
{	
	// always show cursor position
	ShowCoords(pos);
//	Layer* pLayer;

	if(m_bPan)
	{
		Pointf pt = LPtoMP(pos) - m_ptPan;
		m_rcBnds -= pt;
		
		ViewDraw w(this);
		w.DrawRect(GetSize(),SWhite);
		w.DrawImage(pos.x-m_ptPanLP.x,pos.y-m_ptPanLP.y,m_view);

//		if(m_optionGrid.GetData())
//		{
			DrawGrid(w);	
//		}
		
	}
}

void TreeTest::Paint(Draw& w) 
{
//	Ctrl::ShowRepaint(50);

	// set the stage
	m_rcClient = GetSize();	
	
	//#################################
	DrawView(w);
	//#################################

//	if(m_optionGrid.GetData())
//	{
		DrawGrid(w);	
//	}

	// draw currently edited layer here instead of into the m_view
//	if(m_nMode==EDIT && m_pEditing)
//	{
//		PostCallback(callback(this,&OpenWind::DrawEdited));
//	}
}

void TreeTest::DrawView(Draw& w)
{
	// square the bounds of the window with the bounds of the data
/*	if(IsMapTall())
	{	
		m_rcBnds.right = m_rcBnds.left +
					 m_rcClient.Width()*((double)(m_rcBnds.top-m_rcBnds.bottom)
					 /((double)m_rcClient.Height()));
	}
	else
	{
		m_rcBnds.bottom = m_rcBnds.top - 
							m_rcClient.Height()*((double)(m_rcBnds.Width())
							 /((double)m_rcClient.Width()));
	}
*/	
	m_Scale = GetMapScale();
	
	ImageBuffer ibBk(m_rcClient.Size()); // background drawing buffer
	
	memset(~ibBk, 255, 4 * ibBk.GetLength());
	
	// clear background
//	GraphNix::SetColor(ibBk,SWhite);

	// draw layers - most of the data in the app
//	int i,n = m_pLayers.GetCount();
//	for(i=n-1;i>=0;i--)
//	{
//		if(m_pLayers[i]->m_option) // if it is selected as visible in the tree view
//		{
//			m_pLayers[i]->DrawLayer(w, ibBk,(void*)this);	
//		}
//	}

/*	for()
	{
		
		
		
	}
*/
	m_view = Image(ibBk);

	w.DrawImage(m_rcClient,m_view);

}

void TreeTest::DrawGrid(Draw& w)
{
	// draw a coordinate grid
	// probably will be extracted into own object later
	// needs to be smarter / more configurable
	// would be nice to add a frame around the view

	int inc,mins;
	if(m_rcBnds.Width()>5000000.0)
	{
		inc = 1000000;
	}
	else if(m_rcBnds.Width()>500000.0)
	{
		inc = 100000;
	}	
	else if(m_rcBnds.Width()>50000.0)
	{
		inc = 10000;
	}
	else if(m_rcBnds.Width()>5000.0)
	{
		inc = 1000;
	}
	else
	{
		inc = 100;		
	}
	
	Color colGrid(128,128,128);
	
	// draw grid
	int x0 =  (int)(m_rcBnds.left - fmod(m_rcBnds.left,inc) +0.5);
	int y0 =  (int)(m_rcBnds.bottom - fmod(m_rcBnds.bottom,inc)+0.5);
	Pointf ptf1,ptf2;
	Point pt1,pt2;
	String s;

	ptf1.y = m_rcBnds.bottom;
	ptf2.y = m_rcBnds.top;
	
	Font font(StdFont().GetFace(),(int)(StdFont().GetHeight()));//*m_fTextSizeFactor));
	
	s = Format(" %.0f ",m_rcBnds.top);
	mins = ::GetTextSize(s,font).cx;
	
	for(int x = x0;x<m_rcBnds.right;x+=inc)
	{
		ptf1.x = ptf2.x = x;
		
		pt1 = MPtoLP(ptf1);
		pt2 = MPtoLP(ptf2);
		
		s = Format(" %.0f ",x);

		Size sz = ::GetTextSize(s,font);

		if(pt1.x < mins)//sz.cx)
			continue;

		w.DrawText(pt1.x-sz.cy/2,pt1.y,900,s,font,colGrid);
		
//		w.DrawText(pt1.x+sz.cy/2,pt1.y-sz.cx,-900,s,font,colGrid);
		
		pt1.y-=sz.cx;
		
		w.DrawLine(pt1, pt2, 1, colGrid);
	}

	ptf1.x = m_rcBnds.left;
	ptf2.x = m_rcBnds.right;

	s = Format(" %.0f ",m_rcBnds.right);
	mins = ::GetTextSize(s,font).cx;

	for(int y = y0;y<m_rcBnds.top;y+=inc)
	{
		ptf1.y = ptf2.y = y;
		
		pt1 = MPtoLP(ptf1);
		pt2 = MPtoLP(ptf2);
		
		s = Format(" %.0f ",y);

		Size sz = ::GetTextSize(s,font);

		if(pt1.y > m_rcClient.bottom - mins)//sz.cx)
			continue;

		w.DrawText(pt1.x,pt1.y-sz.cy/2,s,font,colGrid);

		pt1.x += sz.cx;

		w.DrawLine(pt1, pt2, 1, colGrid);
	}	
}

double TreeTest::GetMapScale()
{
	double hs = m_rcClient.bottom-m_rcClient.top;
	double hm = m_rcBnds.top-m_rcBnds.bottom;
	
	return hs/hm;
	
}

Point TreeTest::MPtoLP(Pointf pt)
{
	pt.x -= m_rcBnds.left;
	pt.y -= m_rcBnds.top;
	
	Point p;
	double val = (double)pt.x * m_Scale;
	p.x = (int)(val+0.5);
	val = (double)pt.y * -m_Scale;
	p.y = (int)(val+0.5);	
	
	return p;
}

Size TreeTest::MPtoLP(Sizef szf)
{
	Size sz;
	double val = (double)szf.cx * m_Scale;
	sz.cx = (int)(val+0.5);
	val = (double)szf.cy * m_Scale;//-m_Scale; // no negative sizes
	sz.cy = (int)(val+0.5);	
	
	return sz;
}

Rect TreeTest::MPtoLP(Rectf r)
{
	r.left -= m_rcBnds.left;
	r.right -= m_rcBnds.left;
	r.top -= m_rcBnds.top;
	r.bottom -= m_rcBnds.top;
	
	Rect rc;
	double val = (double)r.left * m_Scale;
	rc.left = (int)(val+0.5);
	val = (double)r.right * m_Scale;
	rc.right = (int)(val+0.5);
	val = (double)r.top * -m_Scale;
	rc.top = (int)(val+0.5);	
	val = (double)r.bottom * -m_Scale;
	rc.bottom = (int)(val+0.5);	
	
	return rc;
}


Pointf TreeTest::LPtoMP(Point pt)
{
	Pointf p;
	p.x = (double)pt.x/m_Scale;
	p.y = -(double)pt.y/m_Scale;
	
	p.x += m_rcBnds.left;
	p.y += m_rcBnds.top;

	return p;	
}

GUI_APP_MAIN
{
	TreeTest().Zoomable().Sizeable().Run();
}

