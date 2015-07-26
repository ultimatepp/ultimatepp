#include "CppBase.h"

NAMESPACE_UPP

#ifdef _MSC_VER
#pragma inline_depth(255)
#pragma optimize("t", on)
#endif

void SLPos(SrcFile& res)
{
	res.linepos.Add(res.text.GetLength());
}

SrcFile::SrcFile() :
	preprocessorLinesRemoved(0),
	blankLinesRemoved(0),
	commentLinesRemoved(0)
{
}

SrcFile PreProcess(Stream& in, Parser& parser) // This is not really C preprocess, only removes (or processes) comment and directives
{
	SrcFile res;
	bool include = true;
	int lineno = 0;
	while(!in.IsEof()) {
		String ln = in.GetLine();
		lineno++;
		SLPos(res);
		while(*ln.Last() == '\\') {
			ln.Trim(ln.GetLength() - 1);
			ln.Cat(in.GetLine());
			SLPos(res);
		}
		const char *rm = ln;
		if(IsAlNum(*rm)) {
			const char *s = ln;
			bool islbl = false;
			bool wassemi = false;
			while(*s && iscid(*s) || findarg(*s, '\t', ' ', ':') >= 0) { // check for label, labeled lines are not grounded
				if(*s == ':' && !wassemi) {
					islbl = true;
					wassemi = true; // second ':' cancels label
				}
				else
				if(*s != '\t' && *s != ' ')
					islbl = false; // something was after the label, e.g. void Foo::Bar()
				s++;
			}
			if(!islbl)
				res.text << '\2';
		}
		else
		if(*rm == '\x1f') // line started with macro
			res.text << '\2';
		while(*rm == ' ' || *rm == '\t') rm++;
		if(*rm == '\0')
			res.blankLinesRemoved++;
		else
		if(*rm == '#')
		{
			const char *s = rm + 1;
			while(*s == ' ' || *s == '\t')
				s++;
			if(s[0] == 'd' && s[1] == 'e' && s[2] == 'f' &&
			   s[3] == 'i' && s[4] == 'n' && s[5] == 'e' && !iscid(s[6])) {
				s += 6;
				while(*s == ' ') s++;
				String macro;
				while(iscid(*s))
					macro.Cat(*s++);
				if(*s == '(') {
					while(*s != ')' && *s)
						macro.Cat(*s++);
					macro << ')';
				}
				if(include)
					parser.AddMacro(lineno, macro);
			}
			res.preprocessorLinesRemoved++;
		}
		else {
			bool lineContainsComment = false;
			bool lineContainsNonComment = false;
			String cmd;
			while(*rm) {
				if(*rm == '\"') {
					lineContainsNonComment = true;
					res.text << '\"';
					rm++;
					while((byte)*rm && *rm != '\r' && *rm != '\n') {
						if(*rm == '\"') {
							res.text << '\"';
							rm++;
							break;
						}
						if(*rm == '\\' && rm[1]) {
							if(include)
								res.text.Cat(*rm);
							rm++;
						}
						if(include)
							res.text.Cat(*rm);
						rm++;
					}
				}
				else
				if(*rm == '\\' && rm[1]) {
					lineContainsNonComment = true;
					if(include) {
						res.text.Cat(*rm++);
						res.text.Cat(*rm++);
					}
					else
						rm += 2;
				}
				else
				if(rm[0] == '/' && rm[1] == '/') {
					cmd = rm + 2;
					if(!lineContainsNonComment)
						res.commentLinesRemoved++;
					break;
				}
				else
				if(rm[0] == '/' && rm[1] == '*') {
					lineContainsComment = true;
					rm += 2;
					for(;;) {
						if(*rm == '\0') {
							if(!lineContainsNonComment)
								res.commentLinesRemoved++;
							if(in.IsEof()) break;
							SLPos(res);
							ln = in.GetLine();
							rm = ~ln;
							lineContainsNonComment = false;
						}
						if(rm[0] == '*' && rm[1] == '/') {
							rm += 2;
							break;
						}
						rm++;
					}
					if(include)
						res.text.Cat(' ');
				}
				else {
					lineContainsNonComment = true;
					if(include)
						res.text.Cat(*rm);
					rm++;
				}
			}
			if(include)
				res.text << ' ';
			if(cmd[0] == '$') {
				if(cmd[1] == '-') include = false;
				if(cmd[1] == '+') include = true;
				if(cmd[1]) {
					res.text.Cat(~cmd + 2);
					res.text.Cat(' ');
				}
			}
			if(lineContainsComment && !lineContainsNonComment)
				res.commentLinesRemoved++;
		}
	}
	return pick(res);
}

END_UPP_NAMESPACE
