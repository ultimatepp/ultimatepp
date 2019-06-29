#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SetVppLogSizeLimit(INT_MAX);

	FileIn in("C:/upp/heap.log");
	FileOut out("C:/xxx/heap.bog");
	
	Index<int> th;
	Index<int64> ps;
	
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
	RDUMP(ps.GetCount());
	RDUMP(th.GetCount());
}
