#include "cyclic.h"

cyclic_space::cyclic_space()
{   
	Sizeable().Zoomable();
	SetRect(100, 100, 640, 480);
	col[0] = LtGreen;
	col[1] = LtBlue;
	col[2] = Green;
	col[3] = Red;
	col[4] = LtMagenta;
	col[5] = Yellow;
	col[6] = Cyan;
	col[7] = LtRed;
	col[8] = LtCyan;
	col[9] = Magenta;
	col[10] = White;
	col[11] = LtYellow;
	col[12] = Blue;
	col[13] = Gray;
	col[14] = LtGray;
	col[15] = Black;
	BackPaint();
	reset(16);
}
void cyclic_space::reset(int cn) // New random field
{
    int k = cn;
    n = cn-1;
//    KillTimeCallback(0);
	srand ( time(NULL) );
	for(int i=0; i<640; i++)
	{
		for(int j=0; j<480; j++)
		{
		    fd[i][j]=rand() % k;
		}
	}
/*	for(int i=0;i<1000;i++)
	{
    	step();	
	}*/
//	SetTimeCallback(-20, THISBACK(step));
}
void cyclic_space::change(int num, Color ncs[16]) // new colors from cyclic object
{
    for(int j=0; j<16; j++)
    {
       col[j] = ncs[j];
    }
    reset(num);
}
void cyclic_space::step()
{
    calculate();
    Refresh();
    ProcessEvents();
}
void cyclic_space::LeftDown(Point, dword)
{
/*	cyclic cs;
	cs.Setcols(col);
	cs.RunAppModal();*/
	step();
}
void cyclic_space::calculate() // Next cell generation
{
	int i,j;
	int id[480],od[480],ad,dat;
	    id[0]=od[0]=fd[0][0];
	    dat=id[0]<n?id[0]+1:0;
	    id[0]=fd[0][479]==dat?fd[0][479]:id[0];
	    id[0]=fd[639][0]==dat?fd[639][0]:id[0];
	    id[0]=fd[0][1]==dat?fd[0][1]:id[0];
	    id[0]=fd[1][0]==dat?fd[1][0]:id[0];
	for(j=1; j<479; j++)
	{
	    id[j]=od[j]=fd[0][j];
	    dat=id[j]<n?id[j]+1:0;
	    id[j]=fd[0][j-1]==dat?fd[0][j-1]:id[j];
	    id[j]=fd[639][j]==dat?fd[639][j]:id[j];
	    id[j]=fd[0][j+1]==dat?fd[0][j+1]:id[j];
	    id[j]=fd[1][j]==dat?fd[1][j]:id[j];
	}
	id[479]=od[479]=fd[0][479];
	dat=id[479]<n?id[479]+1:0;
	id[479]=fd[0][478]==dat?fd[0][478]:id[479];
	id[479]=fd[639][479]==dat?fd[639][479]:id[479];
	id[479]=fd[0][0]==dat?fd[0][0]:id[479];
	id[479]=fd[1][479]==dat?fd[1][479]:id[479];
	for(i=1; i<639; i++)
	{
	    ad=fd[i][0];
	    dat=ad<n?ad+1:0;
	    ad=fd[i][479]==dat?fd[i][479]:ad;
	    ad=fd[i-1][0]==dat?fd[i-1][0]:ad;
	    ad=fd[i][1]==dat?fd[i][1]:ad;
	    ad=fd[i+1][0]==dat?fd[i+1][0]:ad;
	    fd[i-1][0]=id[0];
	    id[0]=ad;
	    for(j=1; j<479; j++)
	    {
	        ad=fd[i][j];
	        dat=ad<n?ad+1:0;
	        ad=fd[i][j-1]==dat?fd[i][j-1]:ad;
	        ad=fd[i-1][j]==dat?fd[i-1][j]:ad;
	        ad=fd[i][j+1]==dat?fd[i][j+1]:ad;
	        ad=fd[i+1][j]==dat?fd[i+1][j]:ad;
	        fd[i-1][j]=id[j];
	        id[j]=ad;
	    }
	    ad=fd[i][479];
	    dat=ad<n?ad+1:0;
	    ad=fd[i][478]==dat?fd[i][478]:ad;
	    ad=fd[i-1][479]==dat?fd[i-1][479]:ad;
	    ad=fd[i][0]==dat?fd[i][0]:ad;
	    ad=fd[i+1][479]==dat?fd[i+1][479]:ad;
	    fd[i-1][479]=id[479];
	    id[479]=ad;
	}
	ad=fd[639][0];
	dat=ad<n?ad+1:0;
	ad=fd[639][479]==dat?fd[639][479]:ad;
	ad=fd[638][0]==dat?fd[638][0]:ad;
	ad=fd[639][1]==dat?fd[639][1]:ad;
	ad=od[0]==dat?od[0]:ad;
	fd[638][0]=id[0];
	id[0]=ad;
	for(j=1; j<479; j++)
	{
	    ad=fd[639][j];
	    dat=ad<n?ad+1:0;
	    ad=fd[639][j-1]==dat?fd[639][j-1]:ad;
	    ad=fd[638][j]==dat?fd[638][j]:ad;
	    ad=fd[639][j+1]==dat?fd[639][j+1]:ad;
	    ad=od[j]==dat?od[j]:ad;
	    fd[638][j]=id[j];
	    id[j]=ad;
	}
	ad=fd[639][479];
	dat=ad<n?ad+1:0;
	ad=fd[639][478]==dat?fd[639][478]:ad;
	ad=fd[638][479]==dat?fd[638][479]:ad;
	ad=fd[639][0]==dat?fd[639][0]:ad;
	ad=od[479]==dat?od[479]:ad;
	fd[638][479]=id[479];
	id[479]=ad;
	for(j=0; j<480; j++)
	{
	   fd[639][j]=id[j];
	}
}
void cyclic_space::Paint(Draw& w)
{ 
	int i, j;
	Size sz = GetSize();
	w.DrawRect(sz, White);
	for(i=0; i<640; i++)
	{
		for(j=0; j<480; j++)
		{
		    w.DrawRect(i,j,1,1,col[fd[i][j]]);
		}
	}
}

cyclic::cyclic()
{
	CtrlLayout(*this, "Színek");
	colnum.SetData(16);
	okb <<= THISBACK(Click);
	Sizeable();
}
void cyclic::Setcols(Color pc[16])
{
    for(int i=0;i<16;i++)
    {
        dc[i] = pc[i];
    }
	for(int i=0;i<16;i++)
	{
		cols[i].SetData(dc[i]);
		int x,y;
		x = i >= 8?128:28;
		y = 8 + 32 * (i % 8);
		lbl[i] = Format("%d",i+1);
		cols[i].LeftPosZ(x, 64);
		cols[i].TopPosZ(y, 24);
		lbl[i].LeftPosZ(x-20, 14);
		lbl[i].TopPosZ(y, 24);
		Add(lbl[i]);
        Add(cols[i]);
	}
}
void cyclic::Click()
{
  for(int i=0;i<16;i++)
  {
      dc[i]=cols[i].GetData();
  }
  par.change(colnum, dc);
  Break(IDOK);
}

GUI_APP_MAIN
{
	par.Run();
}

