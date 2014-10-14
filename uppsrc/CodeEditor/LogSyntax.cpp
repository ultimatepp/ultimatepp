#include "CodeEditor.h"

NAMESPACE_UPP

void LogSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, CodeEditor *editor, int line, int pos)
{
	const HlStyle& ink = hl_style[INK_NORMAL];
	const HlStyle& paper = hl_style[PAPER_NORMAL];
	HlStyle err = hl_style[INK_ERROR];
	err.bold = true;
	bool hl_line = false;
	while(s < end) {
		if(IsDigit(*s)) {
			const wchar *s0 = s;
			bool fp = false;
			while(IsDigit(*s) || *s == '.' || *s == 'e') {
				fp = fp || *s == '.' || *s == 'e';
				s++;
			}
			hls.Put(int(s - s0), hl_style[fp ? INK_CONST_FLOAT : INK_CONST_INT]);
		}
		else
		if(IsAlpha(*s) || *s == '_') {
			static Index<String> rws;
			ONCELOCK {
				rws << "error" << "errors" << "warning" << "warnings" << "failed" << "exit" << "fatal"
				    << "failure" << "rejected";
			}
			String w;
			while(IsAlNum(*s) || *s == '_')
				w.Cat(ToLower(*s++));
			bool hl = rws.Find(w) >= 0;
			hls.Put(w.GetCount(), hl ? err : ink);
			hl_line = hl_line || hl;
		}
		else {
			bool hl = findarg(*s, '[', ']', '(', ')', ':', '-', '=', '{', '}', '/', '<', '>', '*', '#', '@') >= 0;
			hls.Put(1, hl ? hl_style[INK_OPERATOR] : ink);
			s++;
		}
		
	}
	if(hl_line)
		hls.SetPaper(0, hls.GetCount(), hl_style[PAPER_WARNING].color);
}

END_UPP_NAMESPACE
