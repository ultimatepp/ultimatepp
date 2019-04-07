#include "ide.h"

class PrinterDlg : public WithPrintLayout<TopWindow> {
	typedef PrinterDlg CLASSNAME;

public:
	PrinterDlg();
};

PrinterDlg::PrinterDlg()
{
	CtrlLayoutOKCancel(*this, "Print");
	columns <<= 1;
	orientation <<= 0;
	line <<= 100;
}

INITBLOCK {
	RegisterGlobalConfig("PrinterDlg");
}

struct Printer {
	Draw& w;
	Size  page;
	int   cols;
	int   line;
	int   columncx;
	int   cx;
	Font  fnt;
	int   fcx;
	int   fcy;
	int   lines;
	int   tabs;
	int   coli;
	int   y;
	
	void NextLine() {
		y++;
		if(y >= lines) {
			y = 0;
			coli++;
			if(coli > cols) {
				coli = 0;
				w.EndPage();
				w.StartPage();
			}
		}
	}
	
	void PrintLine(const WString& s) {
		int x = 0;
		for(int i = 0; i < s.GetCount(); i++) {
			wchar ch = s[i];
			if(x > line) {
				NextLine();
				x = 0;
			}
			if(ch == '\t')
				x = (x + tabs) / tabs * tabs;
			else {
				if(ch != ' ')
					w.DrawText(x * fcx + coli * columncx, y * fcy, &ch, fnt, Black, 1);
				x++;
			}
		}
		NextLine();
	}
	
	Printer(PrinterDlg& dlg, Draw& w, int tabs) : w(w), tabs(tabs) {
		cols = ~dlg.columns;
		line = ~dlg.line;
#ifdef SYSTEMDRAW
		page = w.GetPageSize();
#else
		page = w.GetPagePixels();
#endif
		columncx = page.cx / cols;
		cx = page.cx / cols - 100;
		int l = 0;
		int h = 1000;
		while(l < h) {
			int fh = (l + h) / 2;
			if(cx / Courier(fh).Info().GetWidth('x') > line)
				l = fh + 1;
			else
				h = fh;
		}
		fnt = Courier(h);
		FontInfo fi = fnt.Info();
		fcx = fi.GetWidth('x');
		fcy = fi.GetHeight();
		lines = page.cy / fcy;
		y = coli = 0;
		w.StartPage();
	}
	~Printer() {
		w.EndPage();
	}
};

void Ide::Print()
{
	if(designer)
		return;
	PrinterDlg dlg;
	LoadFromGlobal(dlg, "PrinterDlg");
	int c = dlg.Execute();
	StoreToGlobal(dlg, "PrinterDlg");
	if(c != IDOK)
		return;
	PrinterJob job;
	job.Landscape(dlg.orientation);
	if(!job.Execute())
		return;
	int l, h;
	if(editor.GetSelection(l, h)) {
		l = editor.GetLine(l);
		h = editor.GetLine(h);
	}
	else {
		l = 0;
		h = editor.GetLineCount() - 1;
	}
	Printer p(dlg, job, editor.GetTabSize());
	for(int i = l; i <= h; i++)
		p.PrintLine(editor.GetWLine(i));
}
