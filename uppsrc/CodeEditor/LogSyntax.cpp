#include "CodeEditor.h"

namespace Upp {
	
#define LTIMING(x) // RTIMING(x)

inline bool Is3(const wchar *s, int c)
{
	return s[0] == c && s[1] == c && s[2] == c;
}

void LogSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, CodeEditor *editor, int line, int64 pos)
{
	LTIMING("LogSyntax::Highlight");
	const HlStyle& ink = hl_style[INK_NORMAL];
	HlStyle err = hl_style[INK_ERROR];
	err.bold = true;
	HlStyle stt = hl_style[INK_MACRO];
	bool st_line = false;
	bool hl_line = false;
	bool sep_line = false;
	while(s < end) {
		int c = *s;
		dword pair = MAKELONG(s[0], s[1]);
	#define P2(x) MAKELONG(x, x)
		if(s + 3 <= end && findarg(pair, P2('-'), P2('*'), P2('='), P2('+'), P2('#'), P2(':'), P2('%'), P2('$')) >= 0 && s[2] == c)
			sep_line = true;
		if(findarg(pair, MAKELONG('0', 'x'), MAKELONG('0', 'X'), MAKELONG('0', 'b'), MAKELONG('0', 'B')) >= 0)
			s = HighlightHexBin(hls, s, 2, thousands_separator);
		else
		if(IsDigit(c))
			s = HighlightNumber(hls, s, thousands_separator, false, false);
		else
		if(c == '\'' || c == '\"') {
			const wchar *s0 = s;
			s++;
			for(;;) {
				int c1 = *s;
				if(s >= end || c1 == c) {
					s++;
					hls.Put((int)(s - s0), hl_style[INK_CONST_STRING]);
					break;
				}
				s += 1 + (c1 == '\\' && s[1] == c);
			}
		}
		else
		if(IsAlpha(c) || c == '_') {
			static Index<String> rws, sws;
			ONCELOCK {
				rws << "error" << "errors" << "warning" << "warnings" << "warn" << "failed" << "exit"
				    << "fatal" << "failure" << "rejected";
				sws << "ok" << "success";
			}
			String w;
			while(s < end && IsAlNum(*s) || *s == '_')
				w.Cat(ToLower(*s++));
			bool hl = rws.Find(w) >= 0;
			bool st = sws.Find(w) >= 0;
			hls.Put(w.GetCount(), hl ? err : st ? stt : ink);
			hl_line = hl_line || hl;
			st_line = st_line || st;
		}
		else
		if(c == '\\' && s[1]) {
			hls.Put(2, ink);
			s += 2;
		}
		else {
			bool hl = findarg(c, '[', ']', '(', ')', ':', '-', '=', '{', '}', '/', '<', '>', '*',
			                     '#', '@', '\\', '.') >= 0;
			hls.Put(1, hl ? hl_style[INK_OPERATOR] : ink);
			s++;
		}
		
	}
	if(hl_line)
		hls.SetPaper(0, hls.GetCount(), hl_style[PAPER_WARNING].color);
	else
	if(sep_line)
		hls.SetPaper(0, hls.GetCount(), hl_style[PAPER_IFDEF].color);
	else
	if(st_line)
		hls.SetPaper(0, hls.GetCount(), hl_style[PAPER_BLOCK2].color);
}

}
