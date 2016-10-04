#include "paintbox.h"


PaintBox::PaintBox( void ) : Ctrl()
{
}

void PaintBox::Paint( Draw &w ) 
{	
	
	/*ImageDraw	dw(GetSize());
	onPaint(dw);
	w.DrawImage(0, 0, dw);	*/
	w.DrawRect(0,0,this->GetSize().cx,this->GetSize().cy, White);
	onPaint(w);
}
void PaintBox::MouseMove( Point p, dword keyflags )
{
	onMouseMove(p, keyflags);
}
void PaintBox::LeftUp( Point p, dword keyflags )
{
	onLeftUp(p, keyflags );
}
void PaintBox::LeftDown(Point p, dword keyflags) 
{
	onLeftDown(p, keyflags);
}
void PaintBox::MouseWheel(Point p, int zdelta, dword keyflags)
{
	onMouseWheel(p,zdelta, keyflags);
}
bool PaintBox::Key( dword key, int count )
{
	onKeyPress(key);
	return false;
}

