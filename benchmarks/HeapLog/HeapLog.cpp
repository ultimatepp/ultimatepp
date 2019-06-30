#include <Core/Core.h>
#include <plugin/lzma/lzma.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SetVppLogSizeLimit(INT_MAX);

	FileIn in("C:/xxx/heap.log");

	Index<int> th;
	Index<int64> ps;

	StringStream out;
	while(!in.IsEof()) {
		String s = in.GetLine();
		CParser p(s);
		if(p.Char('-')) {
			int tid = p.ReadNumber(16);
			int64 ptr = p.ReadNumber64(16);
			int t = th.Find(tid);
			int q = ps.Find(ptr);
			if(q >= 0) {
				out.Put32(th.Find(tid));
				out.Put32(q);
				out.Put64(0xffffffffffffffff);
			}
		}
		else {
			int tid = p.ReadNumber(16);
			int64 sz = p.ReadNumber64(16);
			int64 ptr = p.ReadNumber64(16);
			int t = th.FindAdd(tid);
			int q = ps.FindAdd(ptr);
			out.Put32(th.Find(tid));
			out.Put32(q);
			out.Put64(sz);
		}
	}
	Cout() << "Pointers: " << ps.GetCount() << ", threads: " << th.GetCount() << '\n';
	Cout() << "Compressing\n";
	String s = LZMACompress(out.GetResult());
	Cout() << "Compressed length: " << s.GetCount() << "\n";
	SaveFile("c:/xxx/heap.lzma", s);
}
