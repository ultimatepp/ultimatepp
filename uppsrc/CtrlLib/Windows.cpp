#include <CtrlLib/CtrlLib.h>

namespace Upp {

void WindowsList()
{
	WithSimpleSelectLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, t_("Activate window"));
	d.SetRect(0, 0, 400, 300);
	d.list.AddColumn();
	d.list.NoHeader().NoGrid();
	Vector<Ctrl *> w = Ctrl::GetTopWindows();
	int p = 0;
	Vector< Ptr<Ctrl> > ww;
	for(int i = 0; i < w.GetCount() && p < 10; i++) {
		TopWindow *q = dynamic_cast<TopWindow *>(w[i]);
		if(q && !q->GetOwner()) {
			d.list.Add(q->GetTitle());
			ww.Add(q);
		}
	}
	d.list.GoBegin();
	if(d.Run() == IDOK && d.list.IsCursor()) {
		Ctrl *q = ww[d.list.GetCursor()];
		if(q)
			q->SetForeground();
	}
}

void PutForeground(Ptr<Ctrl> ctrl)
{
	if(ctrl)
		ctrl->SetForeground();
}

void WindowsMenu(Bar& bar)
{
	Vector<Ctrl *> w = Ctrl::GetTopWindows();
	int p = 1;
	for(int i = 0; i < w.GetCount() && p < 10; i++) {
		TopWindow *q = dynamic_cast<TopWindow *>(w[i]);
		if(q && !q->GetOwner() && p < 10) {
			bar.Add(Format("&%d ", p++) + FromUnicode(q->GetTitle(), CHARSET_DEFAULT),
			        callback1(PutForeground, Ptr<Ctrl>(q)))
			   .Check(q->IsForeground())
			   .Help(t_("Activate this window"));
		}
	}
	if(p >= 10)
		bar.Add(t_("More windows.."), callback(WindowsList));
}

}
