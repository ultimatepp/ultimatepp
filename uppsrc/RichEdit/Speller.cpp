#include "RichEdit.h"

namespace Upp {

void RichEdit::SpellerAdd(const WString& w, int lang)
{
	Upp::SpellerAdd(w, lang);
}

int RichEdit::fixedlang;

Bits RichEdit::SpellParagraph(const RichPara& para)
{
	int len = para.GetLength();
	Buffer<wchar> text(len);
	Buffer<int> lang(len);
	wchar *s = text;
	int *g = lang;
	for(int i = 0; i < para.GetCount(); i++) {
		const RichPara::Part& p = para[i];
		if(p.IsText()) {
			int l = p.text.GetLength();
			memcpy(s, p.text, l * sizeof(wchar));
			Fill(g, g + l, fixedlang ? fixedlang : p.format.language);
			s += l;
			g += l;
		}
		else {
			*s++ = 127;
			*g++ = 0;
		}
	}
	Bits e;
	s = text;
	wchar *end = text + len;
	while(s < end) {
		if(IsLetter(*s)) {
			const wchar *q = s;
			while(s < end && IsLetter(*s) || s + 1 < end && *s == '\'' && IsLetter(s[1]))
				s++;
			if(!SpellWord(q, int(s - q), lang[q - text]))
				e.SetN(int(q - text), int(s - q));
		}
		else
			s++;
	}
	return e;
}

}
