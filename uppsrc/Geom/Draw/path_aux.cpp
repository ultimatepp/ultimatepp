//////////////////////////////////////////////////////////////////////
// path_aux: support functions for fast line element generation.

#include "GeomDraw.h"

namespace Upp {

static void PathDraw_Output_1_1(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_1_1");
	out.AddFirst(a);
	out.AddNext(b);
}

static void PathDraw_Output_1_2(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_1_2");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 0;
				p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 0;
				p[3].y = b.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 0;
				p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 0;
				p[3].y = b.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 0;
				p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 0;
				p[3].y = b.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 0;
				p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 0;
				p[3].y = b.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_1_3(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_1_3");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 1);
				p[2].x = p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 1;
				p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 1);
				p[2].y = p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 1;
				p[1].y = b.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 1);
				p[2].x = p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 1;
				p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 1);
				p[2].y = p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 1;
				p[1].y = b.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 1);
				p[2].x = p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 1;
				p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 1);
				p[2].y = p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 1;
				p[1].y = b.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 1);
				p[2].x = p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 1;
				p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 1);
				p[2].y = p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 1;
				p[1].y = b.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_1_4(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_1_4");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 1;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 1;
				p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 1;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 1;
				p[3].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + -1;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 1;
				p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 1;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 1;
				p[3].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 1;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 1;
				p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + -1;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 1;
				p[3].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 0;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = a.y - 0;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + -1;
				p[2].y = a.y + 1;
				p[2].x = a.x + 0;
				p[3].y = b.y + 1;
				p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 0;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = a.x - 0;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + -1;
				p[2].x = a.x + 1;
				p[2].y = a.y + 0;
				p[3].x = b.x + 1;
				p[3].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_1_5(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_1_5");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = b.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = b.x + 1;
				p[4].y = p[1].y = a.y - 0;
				p[4].x = p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 2;
				p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = b.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = b.y + 1;
				p[4].x = p[1].x = a.x - 0;
				p[4].y = p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 2;
				p[1].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = b.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = b.x + -1;
				p[4].y = p[1].y = a.y - 0;
				p[4].x = p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 2;
				p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = b.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = b.y + 1;
				p[4].x = p[1].x = a.x - 0;
				p[4].y = p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 2;
				p[1].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = b.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = b.x + 1;
				p[4].y = p[1].y = a.y - 0;
				p[4].x = p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 2;
				p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = b.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = b.y + -1;
				p[4].x = p[1].x = a.x - 0;
				p[4].y = p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 2;
				p[1].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = b.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = b.x + -1;
				p[4].y = p[1].y = a.y - 0;
				p[4].x = p[1].x = a.x + 0;
				p = out.AddSeg(2);
				p[0].y = a.y + 1;
				p[0].x = a.x + 0;
				p[1].y = b.y + 2;
				p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = b.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = b.y + -1;
				p[4].x = p[1].x = a.x - 0;
				p[4].y = p[1].y = a.y + 0;
				p = out.AddSeg(2);
				p[0].x = a.x + 1;
				p[0].y = a.y + 0;
				p[1].x = b.x + 2;
				p[1].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_2_1(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_2_1");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 0;
				p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 0;
				p[3].y = a.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 0;
				p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 0;
				p[3].y = a.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 0;
				p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 0;
				p[3].y = a.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 0;
				p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 0;
				p[3].y = a.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_2_2(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_2_2");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + -1);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + -1);
				p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + -1);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + -1);
				p[3].y = p[1].y = b.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + -1);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + -1);
				p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + -1);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + -1);
				p[3].y = p[1].y = b.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + -1);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + -1);
				p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + -1);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + -1);
				p[3].y = p[1].y = b.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + -1);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + -1);
				p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + -1);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + -1);
				p[3].y = p[1].y = b.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_2_3(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_2_3");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 0);
				p[5].x = p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 0);
				p[5].y = p[3].y = b.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 0);
				p[5].x = p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 0);
				p[5].y = p[3].y = b.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 0);
				p[5].x = p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 0);
				p[5].y = p[3].y = b.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 0);
				p[5].x = p[3].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 0);
				p[5].y = p[3].y = b.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_2_4(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_2_4");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + 0);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + 0);
				p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + 0);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + 0);
				p[3].y = p[1].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + 0);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + 0);
				p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + 0);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + 0);
				p[3].y = p[1].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + 0);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + 0);
				p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + 0);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + 0);
				p[3].y = p[1].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = a.y + 0);
				p[2].x = p[0].x = a.x + 0;
				p[3].y = 1 + (p[1].y = b.y + 0);
				p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = a.x + 0);
				p[2].y = p[0].y = a.y + 0;
				p[3].x = 1 + (p[1].x = b.x + 0);
				p[3].y = p[1].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_2_5(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_2_5");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 1;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 1);
				p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 1;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 1);
				p[5].y = p[3].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + -1;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 1);
				p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 1;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 1);
				p[5].y = p[3].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 1;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 1);
				p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + -1;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 1);
				p[5].y = p[3].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + -1;
				p[4].y = 1 + (p[2].y = a.y + 0);
				p[4].x = p[2].x = a.x + 0;
				p[5].y = 1 + (p[3].y = b.y + 1);
				p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + -1;
				p[4].x = 1 + (p[2].x = a.x + 0);
				p[4].y = p[2].y = a.y + 0;
				p[5].x = 1 + (p[3].x = b.x + 1);
				p[5].y = p[3].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_3_1(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_3_1");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 1);
				p[2].x = p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 1;
				p[1].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 1);
				p[2].y = p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 1;
				p[1].y = a.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 1);
				p[2].x = p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 1;
				p[1].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 1);
				p[2].y = p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 1;
				p[1].y = a.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 1);
				p[2].x = p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 1;
				p[1].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 1);
				p[2].y = p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 1;
				p[1].y = a.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 1);
				p[2].x = p[0].x = a.x + 0;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 1;
				p[1].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 1);
				p[2].y = p[0].y = a.y + 0;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 1;
				p[1].y = a.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_3_2(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_3_2");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 0);
				p[5].x = p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 0);
				p[5].y = p[3].y = a.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 0);
				p[5].x = p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 0);
				p[5].y = p[3].y = a.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 0);
				p[5].x = p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 0);
				p[5].y = p[3].y = a.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 0);
				p[5].x = p[3].x = a.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 0);
				p[5].y = p[3].y = a.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_3_3(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_3_3");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -1));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -1));
				p[5].x = p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -1));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -1));
				p[5].y = p[3].y = p[1].y = b.y + 0;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -1));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -1));
				p[5].x = p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -1));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -1));
				p[5].y = p[3].y = p[1].y = b.y + 0;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -1));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -1));
				p[5].x = p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -1));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -1));
				p[5].y = p[3].y = p[1].y = b.y + 0;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -1));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -1));
				p[5].x = p[3].x = p[1].x = b.x + 0;
			}
			else
			{
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -1));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -1));
				p[5].y = p[3].y = p[1].y = b.y + 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_3_4(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_3_4");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(4, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 2;
				p[1].x = b.x + 1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + 0));
				p[6].x = p[4].x = p[2].x = a.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1));
				p[7].x = p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 2;
				p[1].y = b.y + 1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + 0));
				p[6].y = p[4].y = p[2].y = a.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1));
				p[7].y = p[5].y = p[3].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(4, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 2;
				p[1].x = b.x + -1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + 0));
				p[6].x = p[4].x = p[2].x = a.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1));
				p[7].x = p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 2;
				p[1].y = b.y + 1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + 0));
				p[6].y = p[4].y = p[2].y = a.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1));
				p[7].y = p[5].y = p[3].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(4, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 2;
				p[1].x = b.x + 1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + 0));
				p[6].x = p[4].x = p[2].x = a.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1));
				p[7].x = p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 2;
				p[1].y = b.y + -1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + 0));
				p[6].y = p[4].y = p[2].y = a.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1));
				p[7].y = p[5].y = p[3].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(4, 2);
				p[0].y = a.y - 1;
				p[0].x = a.x + 0;
				p[1].y = b.y - 2;
				p[1].x = b.x + -1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + 0));
				p[6].x = p[4].x = p[2].x = a.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1));
				p[7].x = p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = a.x - 1;
				p[0].y = a.y + 0;
				p[1].x = b.x - 2;
				p[1].y = b.y + -1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + 0));
				p[6].y = p[4].y = p[2].y = a.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1));
				p[7].y = p[5].y = p[3].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_3_5(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_3_5");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + 0));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + 0));
				p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + 0));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + 0));
				p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + 0));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + 0));
				p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + 1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + 0));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + 0));
				p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + 1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + 0));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + 0));
				p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + 0));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + 0));
				p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = b.y - 2);
				p[2].x = p[0].x = b.x + -1;
				p[1].y = a.y - 1;
				p[1].x = a.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + 0));
				p[4].x = p[2].x = p[0].x = a.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + 0));
				p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = b.x - 2);
				p[2].y = p[0].y = b.y + -1;
				p[1].x = a.x - 1;
				p[1].y = a.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + 0));
				p[4].y = p[2].y = p[0].y = a.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + 0));
				p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_4_1(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_4_1");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + -1;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 1;
				p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + -1;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 1;
				p[3].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 1;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 1;
				p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + -1;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 1;
				p[3].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + -1;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 1;
				p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 1;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 1;
				p[3].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 0;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[0].y = b.y - 0;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 1;
				p[2].y = b.y + 1;
				p[2].x = b.x + 0;
				p[3].y = a.y + 1;
				p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 0;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[0].x = b.x - 0;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 1;
				p[2].x = b.x + 1;
				p[2].y = b.y + 0;
				p[3].x = a.x + 1;
				p[3].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_4_2(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_4_2");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = b.y + 0);
				p[2].x = p[0].x = b.x + 0;
				p[3].y = 1 + (p[1].y = a.y + 0);
				p[3].x = p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = b.x + 0);
				p[2].y = p[0].y = b.y + 0;
				p[3].x = 1 + (p[1].x = a.x + 0);
				p[3].y = p[1].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = b.y + 0);
				p[2].x = p[0].x = b.x + 0;
				p[3].y = 1 + (p[1].y = a.y + 0);
				p[3].x = p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = b.x + 0);
				p[2].y = p[0].y = b.y + 0;
				p[3].x = 1 + (p[1].x = a.x + 0);
				p[3].y = p[1].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = b.y + 0);
				p[2].x = p[0].x = b.x + 0;
				p[3].y = 1 + (p[1].y = a.y + 0);
				p[3].x = p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = b.x + 0);
				p[2].y = p[0].y = b.y + 0;
				p[3].x = 1 + (p[1].x = a.x + 0);
				p[3].y = p[1].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(2, 2);
				p[2].y = 1 + (p[0].y = b.y + 0);
				p[2].x = p[0].x = b.x + 0;
				p[3].y = 1 + (p[1].y = a.y + 0);
				p[3].x = p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(2, 2);
				p[2].x = 1 + (p[0].x = b.x + 0);
				p[2].y = p[0].y = b.y + 0;
				p[3].x = 1 + (p[1].x = a.x + 0);
				p[3].y = p[1].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_4_3(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_4_3");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(4, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 2;
				p[1].x = a.x + -1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + 0));
				p[6].x = p[4].x = p[2].x = b.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1));
				p[7].x = p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 2;
				p[1].y = a.y + -1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + 0));
				p[6].y = p[4].y = p[2].y = b.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1));
				p[7].y = p[5].y = p[3].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(4, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 2;
				p[1].x = a.x + 1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + 0));
				p[6].x = p[4].x = p[2].x = b.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1));
				p[7].x = p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 2;
				p[1].y = a.y + -1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + 0));
				p[6].y = p[4].y = p[2].y = b.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1));
				p[7].y = p[5].y = p[3].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(4, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 2;
				p[1].x = a.x + -1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + 0));
				p[6].x = p[4].x = p[2].x = b.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1));
				p[7].x = p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 2;
				p[1].y = a.y + 1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + 0));
				p[6].y = p[4].y = p[2].y = b.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1));
				p[7].y = p[5].y = p[3].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(4, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 2;
				p[1].x = a.x + 1;
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + 0));
				p[6].x = p[4].x = p[2].x = b.x + 0;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1));
				p[7].x = p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 2;
				p[1].y = a.y + 1;
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + 0));
				p[6].y = p[4].y = p[2].y = b.y + 0;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1));
				p[7].y = p[5].y = p[3].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_4_4(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_4_4");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(4, 2);
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2)));
				p[6].x = p[4].x = p[2].x = p[0].x = a.x + -1;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2)));
				p[7].x = p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2)));
				p[6].y = p[4].y = p[2].y = p[0].y = a.y + -1;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2)));
				p[7].y = p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(4, 2);
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2)));
				p[6].x = p[4].x = p[2].x = p[0].x = a.x + 1;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2)));
				p[7].x = p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2)));
				p[6].y = p[4].y = p[2].y = p[0].y = a.y + -1;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2)));
				p[7].y = p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(4, 2);
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2)));
				p[6].x = p[4].x = p[2].x = p[0].x = a.x + -1;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2)));
				p[7].x = p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2)));
				p[6].y = p[4].y = p[2].y = p[0].y = a.y + 1;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2)));
				p[7].y = p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(4, 2);
				p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2)));
				p[6].x = p[4].x = p[2].x = p[0].x = a.x + 1;
				p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2)));
				p[7].x = p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(4, 2);
				p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2)));
				p[6].y = p[4].y = p[2].y = p[0].y = a.y + 1;
				p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2)));
				p[7].y = p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_4_5(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_4_5");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(5, 2);
				p[0].y = a.y - 2;
				p[0].x = a.x + -1;
				p[1].y = b.y - 2;
				p[1].x = b.x + 1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = a.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = a.x - 2;
				p[0].y = a.y + -1;
				p[1].x = b.x - 2;
				p[1].y = b.y + 1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = a.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(5, 2);
				p[0].y = a.y - 2;
				p[0].x = a.x + 1;
				p[1].y = b.y - 2;
				p[1].x = b.x + -1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = a.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = a.x - 2;
				p[0].y = a.y + -1;
				p[1].x = b.x - 2;
				p[1].y = b.y + 1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = a.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(5, 2);
				p[0].y = a.y - 2;
				p[0].x = a.x + -1;
				p[1].y = b.y - 2;
				p[1].x = b.x + 1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = a.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = a.x - 2;
				p[0].y = a.y + 1;
				p[1].x = b.x - 2;
				p[1].y = b.y + -1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = a.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(5, 2);
				p[0].y = a.y - 2;
				p[0].x = a.x + 1;
				p[1].y = b.y - 2;
				p[1].x = b.x + -1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = a.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = a.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = b.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = a.x - 2;
				p[0].y = a.y + 1;
				p[1].x = b.x - 2;
				p[1].y = b.y + -1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = a.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = a.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = b.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_5_1(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_5_1");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = a.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = a.x + -1;
				p[4].y = p[1].y = b.y - 0;
				p[4].x = p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 2;
				p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = a.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = a.y + -1;
				p[4].x = p[1].x = b.x - 0;
				p[4].y = p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 2;
				p[1].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = a.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = a.x + 1;
				p[4].y = p[1].y = b.y - 0;
				p[4].x = p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 2;
				p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = a.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = a.y + -1;
				p[4].x = p[1].x = b.x - 0;
				p[4].y = p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 2;
				p[1].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = a.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = a.x + -1;
				p[4].y = p[1].y = b.y - 0;
				p[4].x = p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 2;
				p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = a.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = a.y + 1;
				p[4].x = p[1].x = b.x - 0;
				p[4].y = p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 2;
				p[1].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(2, 3);
				p[5].y = 1 + (p[3].y = 1 + (p[2].y = 1 + (p[0].y = a.y - 2)));
				p[3].x = p[5].x = p[2].x = p[0].x = a.x + 1;
				p[4].y = p[1].y = b.y - 0;
				p[4].x = p[1].x = b.x + 0;
				p = out.AddSeg(2);
				p[0].y = b.y + 1;
				p[0].x = b.x + 0;
				p[1].y = a.y + 2;
				p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(2, 3);
				p[5].x = 1 + (p[3].x = 1 + (p[2].x = 1 + (p[0].x = a.x - 2)));
				p[3].y = p[5].y = p[2].y = p[0].y = a.y + 1;
				p[4].x = p[1].x = b.x - 0;
				p[4].y = p[1].y = b.y + 0;
				p = out.AddSeg(2);
				p[0].x = b.x + 1;
				p[0].y = b.y + 0;
				p[1].x = a.x + 2;
				p[1].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_5_2(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_5_2");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + -1;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 1);
				p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + -1;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 1);
				p[5].y = p[3].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 1;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 1);
				p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + -1;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 1);
				p[5].y = p[3].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + -1;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 1);
				p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 1;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 1);
				p[5].y = p[3].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[0].y = b.y - 1;
				p[0].x = b.x + 0;
				p[1].y = a.y - 0;
				p[1].x = a.x + 1;
				p[4].y = 1 + (p[2].y = b.y + 0);
				p[4].x = p[2].x = b.x + 0;
				p[5].y = 1 + (p[3].y = a.y + 1);
				p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[0].x = b.x - 1;
				p[0].y = b.y + 0;
				p[1].x = a.x - 0;
				p[1].y = a.y + 1;
				p[4].x = 1 + (p[2].x = b.x + 0);
				p[4].y = p[2].y = b.y + 0;
				p[5].x = 1 + (p[3].x = a.x + 1);
				p[5].y = p[3].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_5_3(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_5_3");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = b.y + 0));
				p[4].x = p[2].x = p[0].x = b.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = a.y + 0));
				p[5].x = p[3].x = p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = b.x + 0));
				p[4].y = p[2].y = p[0].y = b.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = a.x + 0));
				p[5].y = p[3].y = p[1].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = b.y + 0));
				p[4].x = p[2].x = p[0].x = b.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = a.y + 0));
				p[5].x = p[3].x = p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + -1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = b.x + 0));
				p[4].y = p[2].y = p[0].y = b.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = a.x + 0));
				p[5].y = p[3].y = p[1].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + -1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = b.y + 0));
				p[4].x = p[2].x = p[0].x = b.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = a.y + 0));
				p[5].x = p[3].x = p[1].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = b.x + 0));
				p[4].y = p[2].y = p[0].y = b.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = a.x + 0));
				p[5].y = p[3].y = p[1].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(3);
				p[2].y = 1 + (p[0].y = a.y - 2);
				p[2].x = p[0].x = a.x + 1;
				p[1].y = b.y - 1;
				p[1].x = b.x + 0;
				p = out.AddSeg(3, 2);
				p[4].y = 1 + (p[2].y = 1 + (p[0].y = b.y + 0));
				p[4].x = p[2].x = p[0].x = b.x + 0;
				p[5].y = 1 + (p[3].y = 1 + (p[1].y = a.y + 0));
				p[5].x = p[3].x = p[1].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(3);
				p[2].x = 1 + (p[0].x = a.x - 2);
				p[2].y = p[0].y = a.y + 1;
				p[1].x = b.x - 1;
				p[1].y = b.y + 0;
				p = out.AddSeg(3, 2);
				p[4].x = 1 + (p[2].x = 1 + (p[0].x = b.x + 0));
				p[4].y = p[2].y = p[0].y = b.y + 0;
				p[5].x = 1 + (p[3].x = 1 + (p[1].x = a.x + 0));
				p[5].y = p[3].y = p[1].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_5_4(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_5_4");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(5, 2);
				p[0].y = b.y - 2;
				p[0].x = b.x + 1;
				p[1].y = a.y - 2;
				p[1].x = a.x + -1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = b.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = b.x - 2;
				p[0].y = b.y + 1;
				p[1].x = a.x - 2;
				p[1].y = a.y + -1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = b.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = a.y + -1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(5, 2);
				p[0].y = b.y - 2;
				p[0].x = b.x + -1;
				p[1].y = a.y - 2;
				p[1].x = a.x + 1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = b.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = b.x - 2;
				p[0].y = b.y + 1;
				p[1].x = a.x - 2;
				p[1].y = a.y + -1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = b.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = a.y + -1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(5, 2);
				p[0].y = b.y - 2;
				p[0].x = b.x + 1;
				p[1].y = a.y - 2;
				p[1].x = a.x + -1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = b.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = a.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = b.x - 2;
				p[0].y = b.y + -1;
				p[1].x = a.x - 2;
				p[1].y = a.y + 1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = b.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = a.y + 1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(5, 2);
				p[0].y = b.y - 2;
				p[0].x = b.x + -1;
				p[1].y = a.y - 2;
				p[1].x = a.x + 1;
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = b.y + -1)));
				p[8].x = p[6].x = p[4].x = p[2].x = b.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = a.y + -1)));
				p[9].x = p[7].x = p[5].x = p[3].x = a.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[0].x = b.x - 2;
				p[0].y = b.y + -1;
				p[1].x = a.x - 2;
				p[1].y = a.y + 1;
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = b.x + -1)));
				p[8].y = p[6].y = p[4].y = p[2].y = b.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = a.x + -1)));
				p[9].y = p[7].y = p[5].y = p[3].y = a.y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

static void PathDraw_Output_5_5(PathDraw::Output& out, Point a, Point b)
{
	//RTIMING("PathDraw_Output_5_5");
	Point *p;
	int dx = b.x - a.x, dy = b.y - a.y;
	if(dy >= 0)
	{
		if(dx >= 0)
		{
			if(dx >= dy)
			{
				p = out.AddSeg(5, 2);
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2))));
				p[8].x = p[6].x = p[4].x = p[2].x = p[0].x = a.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2))));
				p[9].x = p[7].x = p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2))));
				p[8].y = p[6].y = p[4].y = p[2].y = p[0].y = a.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2))));
				p[9].y = p[7].y = p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
		else
		{
			if(0 >= dx + dy)
			{
				p = out.AddSeg(5, 2);
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2))));
				p[8].x = p[6].x = p[4].x = p[2].x = p[0].x = a.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2))));
				p[9].x = p[7].x = p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2))));
				p[8].y = p[6].y = p[4].y = p[2].y = p[0].y = a.y + -1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2))));
				p[9].y = p[7].y = p[5].y = p[3].y = p[1].y = b.y + 1;
			}
		}
	}
	else
	{
		if(dx >= 0)
		{
			if(dx + dy >= 0)
			{
				p = out.AddSeg(5, 2);
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2))));
				p[8].x = p[6].x = p[4].x = p[2].x = p[0].x = a.x + -1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2))));
				p[9].x = p[7].x = p[5].x = p[3].x = p[1].x = b.x + 1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2))));
				p[8].y = p[6].y = p[4].y = p[2].y = p[0].y = a.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2))));
				p[9].y = p[7].y = p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
		else
		{
			if(dy >= dx)
			{
				p = out.AddSeg(5, 2);
				p[8].y = 1 + (p[6].y = 1 + (p[4].y = 1 + (p[2].y = 1 + (p[0].y = a.y + -2))));
				p[8].x = p[6].x = p[4].x = p[2].x = p[0].x = a.x + 1;
				p[9].y = 1 + (p[7].y = 1 + (p[5].y = 1 + (p[3].y = 1 + (p[1].y = b.y + -2))));
				p[9].x = p[7].x = p[5].x = p[3].x = p[1].x = b.x + -1;
			}
			else
			{
				p = out.AddSeg(5, 2);
				p[8].x = 1 + (p[6].x = 1 + (p[4].x = 1 + (p[2].x = 1 + (p[0].x = a.x + -2))));
				p[8].y = p[6].y = p[4].y = p[2].y = p[0].y = a.y + 1;
				p[9].x = 1 + (p[7].x = 1 + (p[5].x = 1 + (p[3].x = 1 + (p[1].x = b.x + -2))));
				p[9].y = p[7].y = p[5].y = p[3].y = p[1].y = b.y + -1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

void (*PathDraw_Output_Thick[5][5])(PathDraw::Output& out, Point a, Point b) =
{
	{
		PathDraw_Output_1_1,
		PathDraw_Output_1_2,
		PathDraw_Output_1_3,
		PathDraw_Output_1_4,
		PathDraw_Output_1_5,
	},
	{
		PathDraw_Output_2_1,
		PathDraw_Output_2_2,
		PathDraw_Output_2_3,
		PathDraw_Output_2_4,
		PathDraw_Output_2_5,
	},
	{
		PathDraw_Output_3_1,
		PathDraw_Output_3_2,
		PathDraw_Output_3_3,
		PathDraw_Output_3_4,
		PathDraw_Output_3_5,
	},
	{
		PathDraw_Output_4_1,
		PathDraw_Output_4_2,
		PathDraw_Output_4_3,
		PathDraw_Output_4_4,
		PathDraw_Output_4_5,
	},
	{
		PathDraw_Output_5_1,
		PathDraw_Output_5_2,
		PathDraw_Output_5_3,
		PathDraw_Output_5_4,
		PathDraw_Output_5_5,
	},
};

}
