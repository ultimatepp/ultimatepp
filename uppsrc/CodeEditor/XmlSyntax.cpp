#include "CodeEditor.h"

NAMESPACE_UPP

void XmlSyntax::Clear()
{
	hl_ink = INK_NORMAL;
	hl_paper = PAPER_NORMAL;
}

void XmlSyntax::Put0(int ink, int n, int paper)
{
	if(hout)
		hout->Put(n, hl_style[ink], hl_style[paper]);
}

inline static bool IsXmlNameChar(int c)
{
	return IsAlNum(c) || c == '.' || c == '-' || c == '_' || c == ':';
}

const wchar *XmlSyntax::Spaces(const wchar *s, const wchar *e)
{
	while(s < e && IsSpace(*s)) {
		s++;
		Put();
	}
	return s;
}

void XmlSyntax::DoScript(const wchar *s, const wchar *e)
{
	if(hout)
		script.Highlight(s, e, *hout, NULL, 0, 0);
	else
		script.ScanSyntax(s, e, 0, 0);
}

void XmlSyntax::Do(const wchar *s, const wchar *e)
{
doscript:
	if(status == SCRIPT) { // TODO: Improve ending detection...
		static WString h1 = "</script>";
		static WString h2 = "</style>";
		int q = find(s, e - s, ~h1, h1.GetLength(), 0);
		if(q < 0)
			q = find(s, e - s, ~h2, h2.GetLength(), 0);
		if(q < 0) {
			DoScript(s, e);
			return;
		}
		else {
			DoScript(s, s + q);
			s += q;
			status = TEXT;
			Set(INK_NORMAL);
		}
	}
	while(s < e) {
		s = Spaces(s, e);
		if(s >= e)
			break;
		if(status == COMMENT) {
			if(s + 2 < e && s[0] == '-' && s[1] == '-' && s[2] == '>') {
				SetPut(INK_COMMENT, 3);
				s += 3;
				status = TEXT;
				Set(INK_NORMAL);
			}
			else {
				Put();
				s++;
			}
		}
		else
		if(*s == '&') {
			s++;
			Put0(INK_CONST_STRING, 1);
			while(s < e && *s != ';') {
				Put0(INK_CONST_STRING, 1);
				s++;
			}
		}
		else
		if(*s == '<') {
			tagname.Clear();
			s++;
			if(s + 2 < e && s[0] == '!' && s[1] == '-' && s[2] == '-') {
				SetPut(INK_COMMENT, 4);
				s += 3;
				status = COMMENT;
			}
			else
			if(*s == '!') {
				status = DECL;
				s++;
				SetPut(INK_MACRO, 2, PAPER_MACRO);
			}
			else
			if(*s == '?') {
				status = PI;
				s++;
				SetPut(INK_IFDEF, 2, PAPER_IFDEF);
			}
			else {
				status = TAG0;
				SetPut(INK_KEYWORD);
			}
		}
		else
		if(*s == '>') {
			s++;
			Put();
			Set(INK_NORMAL);
			status = TEXT;
			if(tagname == "script") {
				script.SetHighlight(CSyntax::HIGHLIGHT_JAVASCRIPT);
				status = SCRIPT;
				goto doscript;
			}
			if(tagname == "style") {
				script.SetHighlight(CSyntax::HIGHLIGHT_CSS);
				status = SCRIPT;
				goto doscript;
			}
		}
		else
		if(status == TAG0) {
			tagname.Clear();
			while(s < e && !IsSpace(*s) && *s != '>') {
				tagname.Cat(*s++);
				Put();
			}
			if(*tagname != '/')
				status = TAG;
		}
		else
		if(status == TAG) {
			int n = 0;
			while(s < e && !IsSpace(*s) && *s != '>' && *s != '=') {
				n++;
				s++;
			}
			Put0(INK_UPP, n);
			status = ATTR;
		}
		else
		if(status == ATTR) {
			if(*s == '=') {
				s++;
				Put();
			}
			s = Spaces(s, e);
			int c = *s;
			if(c == '\"' || c == '\'') {
				int n = 1;
				s++;
				while(s < e) {
					n++;
					if(*s++ == c)
						break;
				}
				Put0(INK_CONST_STRING, n);
			}
			else {
				int n = 0;
				while(s < e && !IsSpace(*s) && *s != '>') {
					s++;
					n++;
				}
				Put0(INK_CONST_FLOAT, n);
			}
			status = TAG;
		}
		else {
			s++;
			Put();
		}
	}
}

void XmlSyntax::ScanSyntax(const wchar *s, const wchar *e, int, int)
{
	Do(s, e);
}

void XmlSyntax::Highlight(const wchar *s, const wchar *end, HighlightOutput& hls, CodeEditor *editor, int line, int pos)
{
	hout = &hls;
	Do(s, end);
	hout = NULL;
}

void XmlSyntax::Serialize(Stream& s)
{
	s % hl_ink
	  % hl_paper
	  % status
	  % tagname
	  % script;
}

END_UPP_NAMESPACE
