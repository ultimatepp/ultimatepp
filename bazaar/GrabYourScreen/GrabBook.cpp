#include "GrabYourScreen.h"

GrabBook::GrabBook() {
	CtrlLayout(*this);
}

void GrabBook::Init() {	
	initialPage = 0;
	actualPage = 0;
	left <<= 100;
	right <<= 1200;
	up <<= 100;
	down <<= 900;
	split = true;
	
	folder = AppendFileName(GetDesktopFolder(), t_("Book"));
	                                                   
	canvas.SetShowWindow(false);
	
	canvas.WhenPaint = THISBACK(OnPaint);
	reset.WhenAction = THISBACK(OnReset);
	capture.WhenAction = THISBACK(OnCapture);
	savePDF.WhenAction = THISBACK(OnSavePDF);
	timeCallback.Set(-1000, THISBACK(Timer));
	
	keyId = Ctrl::RegisterSystemHotKey(K_CTRL | K_F10, THISBACK(OnCapture));
}

GrabBook::~GrabBook() {
	Ctrl::UnregisterSystemHotKey(keyId);
}

void GrabBook::OnPaint(Painter &painter) {
	painter.Move(left, up).Line(right, up).Line(right, down).
			Line(left, down).Line(left, up).Stroke(2, Black());
	painter.Move(left, up).Line(right, down).Stroke(2, Black());
	painter.Move(right, up).Line(left, down).Stroke(2, Black());
}

void GrabBook::OnReset() {
	actualPage = 0;
	initialPage = 0;
}

void GrabBook::OnCapture() {
	Rect rPdf(left, up, right, down);
	Rect screenRect;
	Window_GetRect((int64)GetHWND(), screenRect.left, screenRect.top, screenRect.right, screenRect.bottom);
	if (rPdf.Intersects(screenRect)) {
		Exclamation(t_("Program window interferes with image"));
		return;
	}
	
	Timer();
	
	if (!DirectoryExists(folder))
		RealizeDirectory(folder);
	Image img = ::GetRect(canvas.GetBackground(), Rect(left, up, right, down));
	if (!SaveImage(img, 100, AppendFileName(folder, Format("%s.jpg", Format(t_("Pag%04d"), int(actualPage)))))) 
		Exclamation(t_("Error saving image"));
	actualPage = actualPage + 1;
}

void GrabBook::OnSavePDF() {
	Report r;
	
	int width, height;
	if (split) {
		width = 210;
		height = 297;
	} else {
		width = 297;
		height = 210;
	}
	r.SetPageSize((width-5-5)*6000/254, (height-5-5)*6000/254);
	r.Margins(5*6000/254, 5*6000/254);	

	String fileName;
		
	FileSel fs;
	
	fs.ActiveDir(GetDesktopFolder());
	fs.Type(t_("PDF files"), ".pdf");
	
	if (fs.ExecuteSaveAs(t_("Saving file"))) 
		fileName = ~fs;
	else
		return; 

	WaitCursor wait;

	ForceExt(fileName, ".pdf");

	Size psz = r.GetPageSize();
	for(int i = initialPage; i < actualPage; ++i) {
		if (i > 0)
			r.NewPage();

		String fileName = AppendFileName(folder, Format("%s.jpg", Format(t_("Pag%04d"), i)));
		Image img = StreamRaster::LoadFileAny(fileName);
		Image imgIzq, imgDer;
		if (split) {
			imgIzq = ::GetRect(img, Rect(0, 0, img.GetWidth()/2, img.GetHeight()));
			imgDer = ::GetRect(img, Rect(img.GetWidth()/2 + 1, 0, img.GetWidth(), img.GetHeight()));
		} else
			imgIzq = img;
		
		DrawingDraw dw(psz.cx, psz.cy);
		Rect rect = FitInFrame(psz, imgIzq.GetSize());
		dw.DrawImage(rect, imgIzq);
		QtfRichObject qrob(CreateDrawingObject(dw, psz.cx, psz.cy));
		String qtf;
		qtf << qrob;
		r << qtf;
		
		if (split) {
			r.NewPage();
			DrawingDraw dw(psz.cx, psz.cy);
			Rect rect = FitInFrame(psz, imgDer.GetSize());
			dw.DrawImage(rect, imgDer);
			QtfRichObject qrob(CreateDrawingObject(dw, psz.cx, psz.cy));
			String qtf;
			qtf << qrob;
			r << qtf;
		}
	}
	
	if (!SaveFile(fileName, UPP::Pdf(r))) {
		Exclamation(t_("It is not possible to save file.&Posibly .pdf file is already opened."));
		return;
	}
	LaunchFile(fileName);
}

void GrabBook::Timer() {
	NON_REENTRANT_V;
		
	canvas.SetBackground(Snap_Desktop());
}

/*
A añadir:
- Seleccion manual de ventana (usando lo de Screen)
- Paso automático de páginas
- Barrido con zoom?
*/