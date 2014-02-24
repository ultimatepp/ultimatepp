#include "TextDiffCtrl.h"

namespace Upp {

void PatchDiff::Copy(FileIn& in, FileIn& oin, int& l, int ln, int n)
{
	if(ln < l)
		throw CParser::Error("");
	while(l < ln) {
		if(oin.IsEof())
			throw CParser::Error("");
		extfile << oin.GetLine() << "\r\n";
		l++;
	}
	l += n;
	while(n--)
		oin.GetLine();
}

void PatchDiff::LoadDiff(const char *fn)
{
	try {
		FileIn in(fn);
		FileIn oin(editfile);
		extfile.Clear();
		int l = 1;
		String s = in.GetLine();
		if(IsDigit(*s)) {
			in.Seek(0);
			while(!in.IsEof()) {
				s = in.GetLine();
				if(IsDigit(*s)) {
					CParser p(s);
					int ln = p.ReadNumber();
					int n = 0;
					if(p.Char('a'))
						ln++;
					else {
						n = p.Char(',') ? p.ReadNumber() - ln + 1 : 1;
						if(!p.Char('c'))
							p.PassChar('d');
					}
					Copy(in, oin, l, ln, n);
				}
				else
				if(*s == '>') {
					if(s[1] != ' ')
						throw CParser::Error("");
					extfile << s.Mid(2) << "\r\n";
				}
				else
				if(*s != '<' && *s != '-')
					throw CParser::Error("");
			}
		}
		else {
			for(;;) {
				if(in.IsEof())
					throw CParser::Error("");
				if(in.GetLine().Mid(0, 4) == "+++ ")
					break;
			}
			while(!in.IsEof()) {
				String s = in.GetLine();
				if(*s == '@') {
					CParser p(s);
					p.PassChar2('@', '@');
					p.PassChar('-');
					int ln = p.ReadNumber();
					int n = 1;
					if(p.Char(','))
						n = p.ReadNumber();
					Copy(in, oin, l, ln, n);
				}
				else
				if(*s == '+' || *s == ' ')
					extfile << s.Mid(1) << "\r\n";
				else
				if(*s != '-')
					throw CParser::Error("");
			}
		}
		while(!oin.IsEof())
			extfile << oin.GetLine() << "\r\n";
	}
	catch(CParser::Error&) {
		Exclamation("Invalid file format!");
		extfile = LoadFile(fn);
	}
}

void PatchDiff::Open()
{
	if(!DiffFs().ExecuteOpen())
		return;
	String f = ~DiffFs();
	r <<= f;
	LoadDiff(f);
	diff.Set(LoadFile(editfile), extfile);
}

};