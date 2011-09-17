#ifndef _FTGL_Demo_FTGLCtrl_h_
#define _FTGL_Demo_FTGLCtrl_h_

#include <CtrlLib/CtrlLib.h>
#include <GLCtrl/GLCtrl.h>

#include <FTGL/ftgl.h>

#include "TrackBall.h"

using namespace Upp;

class FTGLCtrl : public GLCtrl
{
	private:
	
		TrackBall trackBall;

		typedef enum { FTGL_BITMAP, FTGL_PIXMAP, FTGL_OUTLINE, FTGL_POLYGON, FTGL_EXTRUDE, FTGL_TEXTURE, NumStyles } Styles;
		typedef enum { EDITING = 1, INTERACTIVE } Modes;

		Modes mode;

		Vector<String> fontFiles;
		int currentFont;
		
		// array containing all fonts
		Array<FTFont> fonts;
		int totalFonts;
		One<FTPixmapFont> infoFont;
		
		Array<FTLayout> layouts;
		int currentLayout;
		bool hasLayout(void) { return currentLayout >= 0 && currentLayout < layouts.GetCount(); }
		
		// displayed string
		String myString;

		void SetUpLighting();
		void SetUpFonts(int numFontFiles);
		void RenderFontmetrics();
		void RenderFontInfo();
		void SetCamera();
		void Paint0(void);
		
		int GetStyle() { return currentFont % NumStyles; }
		int GetFace()  { return currentFont / NumStyles; }

		float OX;
		float OY;
		
		float initialLineLength;

	protected:
		virtual void GLPaint();
		
		virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
		virtual bool Key(dword key, int count);
		
		virtual void Layout();
		
	public:
		typedef FTGLCtrl CLASSNAME;
		
		void SetFontFiles(Vector<String> const &files);

		FTGLCtrl();
};

#endif
