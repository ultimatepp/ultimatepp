#ifdef flagGUI

#include <CtrlLib/CtrlLib.h>
//#include <GridCtrl/GridCtrl.h>

#include "Functions4U.h"

Image Rotate180(const Image& orig) {
	Size sz = orig.GetSize();
	ImageBuffer dest(sz);
	for(int rw = 0; rw < sz.cy; rw++)
		for(int cl = 0; cl < sz.cx; cl++)
			dest[rw][cl] = orig[sz.cy - rw - 1][sz.cx - cl - 1];
	return dest;
}

Image GetRect(const Image& orig, const Rect &r) {
	if(r.IsEmpty())
		return Image();
	ImageBuffer ib(r.GetSize());
	for(int y = r.top; y < r.bottom; y++) {
		const RGBA *s = orig[y] + r.left;
		const RGBA *e = orig[y] + r.right;
		RGBA *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			t++;
			s++;
		}
	}
	return ib;
}

Image NativePathIconX(const char *path, bool folder, int flags)
{
	if (!(flags & BROWSE_LINKS))
		return NativePathIcon(path, folder);
	if (!IsSymLink(path))
		return NativePathIcon(path, folder);
	
	String linkPath = GetSymLinkPath(path);
	if (linkPath.IsEmpty())
		linkPath = path;
	
	static Image ilink = Null;
	
	if (!ilink) {
		ImageDraw drw(8, 8);

		Rect r(0, 0, 8, 8);
	
		drw.DrawRect(r, White());
		drw.DrawImage(r, CtrlImg::smallright);
		drw.DrawLine(r.left, r.top, r.right-1, r.top, 1, GrayColor(100));
		drw.DrawLine(r.right-1, r.top, r.right-1, r.bottom-1, 1, GrayColor(100));
		drw.DrawLine(r.right-1, r.bottom-1, r.left, r.bottom-1, 1, GrayColor(100));
		drw.DrawLine(r.left, r.bottom-1, r.left, r.top, 1, GrayColor(100));
	
		ilink = drw;
	}

	Image img = NativePathIcon(linkPath, DirectoryExists(linkPath));
	int w = img.GetWidth();
	int h = img.GetHeight();
	
	ImageDraw drw(w, h);

	drw.Alpha().DrawImage(0, 0, img, GrayColor(255));
	drw.DrawImage(0, 0, img);
	
	int ypos = img.GetHeight() - ilink.GetHeight();
	drw.Alpha().DrawImage(0, ypos, ilink, GrayColor(255));
	drw.DrawImage(0, ypos, ilink);
	
	return drw;
}

/*
Vector<Value> ReadCol(GridCtrl& grid, int col, int begin, int end) 
{
	if (begin < 0 || end >= grid.GetRowCount() || col < 0 || col >= grid.GetColumnCount())
		throw Exc(t_("Wrong param. in ReadCol"));
	
	Vector<Value> v;
	
	for(int i = begin; i <= end; i++)
		v.Add(grid(i, col));
	return v;
}

Vector<Vector<Value> > GetGridData(GridCtrl& grid) {
	Vector<Vector<Value> > data;
	
	for (int row = 0; row < grid.GetRowCount()+1; ++row) 
		data.Add(grid.ReadRow(row));
	
	return data;
}
void SetGridData(GridCtrl& grid, Vector<Vector<Value> > &data) {
	grid.Clear(true);
	if (!data.IsEmpty()) { 
		int nrow = data.GetCount();
		int ncol = data[0].GetCount();
		for (int col = 0; col < data[0].GetCount(); ++col) 
			grid.AddColumn(data[0][col]);
		grid.SetRowCount(data.GetCount()-1);
		for (int row = 0; row < grid.GetRowCount(); ++row) 
			for (int col = 0; col < grid.GetColumnCount(); ++col) 
				grid(row, col) = data[row+1][col];
	}
}
*/
#endif
