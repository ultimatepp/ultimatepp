#ifdef flagGUI

#include <plugin/tif/tif.h>
#include <plugin/jpg/jpg.h>
#include <plugin/gif/gif.h>
#include <plugin/bmp/bmp.h>

#include <CtrlLib/CtrlLib.h>
#include <Functions4U/Functions4U_Gui.h>


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

bool SaveImage(const Image &img, int qualityBpp, const String &fileName, String ext) {	
	if(!img) 
		return false;
	
	if (ext == "")
		ext = GetFileExt(fileName);
	
	if (ext == ".jpg" || ext == ".jpeg") {
	    JPGEncoder jpg;	    
	    if (IsNull(qualityBpp))
	        qualityBpp = 85;
	    else {
	    	if (qualityBpp > 100)
	        	qualityBpp = 100;		// Max 100
	    }
		jpg.Quality(qualityBpp);	
		jpg.SaveFile(fileName, img);
	} else if (ext == ".gif") {
		GIFEncoder gif;	 
		gif.SaveFile(fileName, img);
	} else if (ext == ".tif" || ext == ".tiff") {
		TIFEncoder tif;
		if (IsNull(qualityBpp))
	        qualityBpp = 24;
		tif.Bpp(qualityBpp);	//1, 2, 4, 8, 24, 32
		tif.SaveFile(fileName, img);	
	} else if (ext == ".bmp") {
		BMPEncoder bmp;
		if (IsNull(qualityBpp))
	        qualityBpp = 24;
		bmp.Bpp(qualityBpp);	//1, 4, 8, 24, 32	and Bpp(4).Mono(); Bpp(8).Mono();
		bmp.SaveFile(fileName, img);	
	} else if (ext == ".png") {
		PNGEncoder png;
		if (IsNull(qualityBpp))
	        qualityBpp = 24;
		png.Bpp(qualityBpp);	//1, 4, 8, 24, 32	and only with 8	png.Interlace();
		png.SaveFile(fileName, img);
	} else
		return false;
	return true;
}

bool PrintImage(const Image &img, int x, int y, int width, int height) {
	if(!img) 
		return false;
	
	PrinterJob pd(t_("Printing document"));
	if(!pd.Execute()) 
		return false; 
	
	Draw& w = pd.GetDraw();
	w.StartPage();
	if (IsNull(width))
		w.DrawImage(x, y, img);
	else
		w.DrawImage(x, y, width, height, img);
	w.EndPage();
	
	return true;
}

void DrawRectLine(Draw& w, int x, int y, int width, int height, int lineWidth, const Color &color) {
	w.DrawLine(x, y, x+width, y, lineWidth, color);
	w.DrawLine(x+width, y, x+width, y+height, lineWidth, color);
	w.DrawLine(x+width, y+height, x, y+height, lineWidth, color);
	w.DrawLine(x, y+height, x, y, lineWidth, color);
}

void DrawRectLine(Draw& w, Point &pos, Size &s, int lineWidth, const Color &color) {
	w.DrawLine(pos.x, pos.y, pos.x+s.cx, pos.y, lineWidth, color);
	w.DrawLine(pos.x+s.cx, pos.y, pos.x+s.cx, pos.y+s.cy, lineWidth, color);
	w.DrawLine(pos.x+s.cx, pos.y+s.cy, pos.x, pos.y+s.cy, lineWidth, color);
	w.DrawLine(pos.x, pos.y+s.cy, pos.x, pos.y, lineWidth, color);
}

void DrawRectLine(Draw& w, Rect &r, int lineWidth, const Color &color) {
	w.DrawLine(r.left, r.top, r.right, r.top, lineWidth, color);
	w.DrawLine(r.right, r.top, r.right, r.bottom, lineWidth, color);
	w.DrawLine(r.right, r.bottom, r.left, r.bottom, lineWidth, color);
	w.DrawLine(r.left, r.bottom, r.left, r.top, lineWidth, color);
}

int GetEditWidth(const String str) {
	Font font = StdFont();
	
	int ret = 0;
	for (int i = 0; i < str.GetCount(); ++i)
		ret += font.GetWidth(str[i]);
	return ret;
}


bool ConsoleOutput::Init(bool forceWindow) {
#ifdef PLATFORM_WIN32	
	ownConsole = true;
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        if (GetLastError() != ERROR_ACCESS_DENIED) {
            if (!AttachConsole(GetCurrentProcessId())) {
                if (GetLastError() != ERROR_ACCESS_DENIED) {
                    ownConsole = false;
                    if (!forceWindow)
                    	return false;
                    
					if (!AllocConsole())
						return false;
					AttachConsole(GetCurrentProcessId()) ;
                }
            }
        }
    }
	freopen("CONIN$",  "r", stdin); 
	freopen("CONOUT$", "w", stdout); 
	freopen("CONOUT$", "w", stderr); 
	
	return true;
#else
	if (forceWindow)
		return false;
	return true;
#endif	
}

ConsoleOutput::~ConsoleOutput() {
#ifdef PLATFORM_WIN32
	if (ownConsole && (GetConsoleWindow() == GetForegroundWindow())) {
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0; 
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = K_ENTER; 
		input.ki.dwFlags = 0; 
		SendInput(1, &input, sizeof(INPUT));
		input.ki.dwFlags = KEYEVENTF_KEYUP; 
		SendInput(1, &input, sizeof(INPUT));
	}
#endif
}

void ArrayCtrlWhenBar(Bar &menu, ArrayCtrl &array) {
	menu.Add(t_("Select all"), Null, [&] {ArrayCtrlRowSelect(array);})
		.Key(K_CTRL_A).Help(t_("Select all rows"));
								
	int count = array.GetSelectCount();
	if (count == 0)
		menu.Add(t_("No row selected"), Null, Null).Enable(false).Bold(true);
	else {
		menu.Add(Format(t_("Selected %d rows"), count), Null, Null).Enable(false).Bold(true);
		menu.Add(t_("Copy"), Null, [&] {ArrayCtrlRowCopy(array, false);})
			.Key(K_CTRL_C).Help(t_("Copy selected rows to clipboard"));
	}
}

void ArrayCtrlRowCopy(const ArrayCtrl &array, bool header) {
	array.SetClipboard(true, header);
}

void ArrayCtrlRowSelect(ArrayCtrl &array) {
	array.Select(0, array.GetCount(), true);
}

Vector<int> ArrayCtrlGetSelected(const ArrayCtrl &array) {
	Vector<int> selected;
	for (int r = 0; r < array.GetCount(); ++r) {
		if (array.IsSel(r))	
			selected << r;
	}
	return selected;
}

#endif