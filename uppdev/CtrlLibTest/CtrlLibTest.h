$uvs: PENDING CONFLICT
#ifndef _CtrlLibTest_CtrlLibTest_h
#define _CtrlLibTest_CtrlLibTest_h

#include <CtrlLib/CtrlLib.h>
$uvs: REPOSITORY INSERT
using namespace Upp;

template<class T>
class WithMinSize : public T
{
	Size minsize;
public:
	virtual Size GetMinSize() const 	{ return minsize; }
	void 		 SetMinSize(Size sz)	{ minsize = sz; }
};

#include <ExpandFrame/ExpandFrame.h>
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE

using namespace Upp;
$uvs: END REPOSITORY DELETE

#define LAYOUTFILE <CtrlLibTest/CtrlLibTest.lay>
#include <CtrlCore/lay.h>

$uvs: REPOSITORY INSERT
#define CNT 10
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
#define IMAGECLASS Img
#define IMAGEFILE <CtrlLibTest/img.iml>
#include <Draw/iml_header.h>

class Tools : public TopWindow
{
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
		if (message == WM_MOVE && GetOwner()) {
			Rect or = GetOwner()->GetScreenRect();
			Rect r = GetScreenRect();

			if (!or.Contains(r)) {
				Point tl = r.TopLeft();
				Point br = r.BottomRight();
				if (tl.x < or.left)
					r.OffsetHorz(or.left - tl.x);
				if (tl.y < or.top)
					r.OffsetVert(or.top - tl.y);
				if (br.x > or.bottom)
					r.OffsetHorz(br.x - or.bottom);
				if (br.y > or.right)
					r.OffsetVert(br.y - or.right);
				SetRect(r);
			}
		}
		return TopWindow::WindowProc(message, wParam, lParam);
	}
};

$uvs: END REPOSITORY DELETE
class CtrlLibTest : public WithCtrlLibTestLayout<TopWindow> {
public:
	typedef CtrlLibTest CLASSNAME;
$uvs: REPOSITORY INSERT

	ParentCtrl paneframe;
	WithTestLayout< WithMinSize<ParentCtrl> > pane[CNT];
	ExpandFrame	frame[CNT];

	CtrlLibTest() {
		CtrlLayout(*this, "CtrlLib Test App");
		Sizeable();
		for (int i = 0; i < CNT; i++) {
			CtrlLayout(pane[i]);
			WString s = Format("Pane %d", i);
			paneframe.AddFrame(frame[i].Top(pane[i]).SetTitle(s).Expand(true).SetStyle(ExpandFrame::StyleWide()));
		}
	}

	void OnButton1()
	{
		//Duration t = (Duration)~duration;
		int i = 1;
		i++;
	}
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	ImageBuffer ib;
	Image img;


	CtrlLibTest();
$uvs: END REPOSITORY DELETE
};

#endif

