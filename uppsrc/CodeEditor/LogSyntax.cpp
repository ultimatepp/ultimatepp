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
		int c = *s;
		const wchar *s0 = s;
		if(IsDigit(c)) {
			bool fp = false;
			while(s < end && IsDigit(*s) || *s == '.' || *s == 'e') {
				fp = fp || *s == '.' || *s == 'e';
				s++;
			}
			hls.Put(int(s - s0), hl_style[(IsAlpha(*s) || *s == '_') && (IsAlNum(s[1]) || s[1] == '_') ? INK_NORMAL // More likely a hexdump or something
			                              : fp ? INK_CONST_FLOAT : INK_CONST_INT]);
		}
		else
		if(c == '\'' || c == '\"') {
			s++;
			for(;;) {
				if(s >= end) {
					hls.Put(1, ink);
					s = s0 + 1;
					break;
				}
				if(*s == c) {
					s++;
					hls.Put((int)(s - s0), hl_style[INK_CONST_STRING]);
					break;
				}
				s += 1 + (s[0] == '\\' && s[1] == c);
			}
		}
		else
		if(IsAlpha(c) || c == '_') {
			static Index<String> rws;
			ONCELOCK {
				rws << "error" << "errors" << "warning" << "warnings" << "failed" << "exit" << "fatal"
				    << "failure" << "rejected";
			}
			String w;
			while(s < end && IsAlNum(*s) || *s == '_')
				w.Cat(ToLower(*s++));
			bool hl = rws.Find(w) >= 0;
			hls.Put(w.GetCount(), hl ? err : ink);
			hl_line = hl_line || hl;
		}
		else {
			bool hl = findarg(c, '[', ']', '(', ')', ':', '-', '=', '{', '}', '/', '<', '>', '*', '#', '@') >= 0;
			hls.Put(1, hl ? hl_style[INK_OPERATOR] : ink);
			s++;
		}
		
	}
	if(hl_line)
		hls.SetPaper(0, hls.GetCount(), hl_style[PAPER_WARNING].color);
}

END_UPP_NAMESPACE
