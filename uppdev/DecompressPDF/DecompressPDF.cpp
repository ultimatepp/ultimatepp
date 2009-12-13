#include <Core/Core.h>
#include <plugin/z/z.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FileIn in("u:/Dokument-A.pdf");
	FileOut out("u:/out.pdf");
	while(!in.IsEof()) {
		String s = in.GetLine();
		int q = s.Find("stream");
		if(q < 0)
			out << s << "\n";
		else {
			out << s.Mid(0, q + 6);
			out << "stream";
			s = s.Mid(q + 6);
			while(!in.IsEof()) {
				s.Cat(in.Get());
				if(s.GetLength() > 9 && memcmp(s.End() - 9, "endstream", 9) == 0) {
					out << ZDecompress(s) << "\nendstream\n";
					break;
				}
			}
		}
	}
}
