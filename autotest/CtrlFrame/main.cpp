#include <CtrlLib/CtrlLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	NullFrameClass fr[50];
	for(int q = 0; q < 10000; q++) {
		DLOG("==========");
		Ctrl h;
		Vector<CtrlFrame *> h1;
		int N = Random(40) + 1;
		h1.Add(&NullFrame());
		for(int i = 0; i < N; i++) {
			int pos = Random(h.GetFrameCount() + 1);
			CtrlFrame& val = fr[Random(50)];
			h.InsertFrame(pos, val);
			h1.Insert(pos, &val);
			DDUMP(h.GetFrameCount());
			DDUMP(h1.GetCount());
			ASSERT(h1.GetCount() == h.GetFrameCount());
			for(int i = 0; i < h.GetFrameCount(); i++)
				ASSERT(&h.GetFrame(i) == h1[i]);
#if 0
			DLOG("===========");
			DDUMP(pos);
			DDUMP(val);
			DDUMP(h.GetFrameCount());
			for(int i = 0; i < h.GetFrameCount(); i++)
				DLOG(i << " " << h.GetFrame(i));
#endif
		}
		while(h.GetFrameCount() > 1) {
			int pos = Random(h.GetFrameCount());
			h.RemoveFrame(pos);
			h1.Remove(pos);
			DDUMP(h.GetFrameCount());
			DDUMP(h1.GetCount());
			ASSERT(h1.GetCount() == h.GetFrameCount());
			for(int i = 0; i < h.GetFrameCount(); i++)
				ASSERT(&h.GetFrame(i) == h1[i]);
#if 0
			DLOG("===========");
			DDUMP(pos);
			DDUMP(h.GetFrameCount());
			for(int i = 0; i < h.GetFrameCount(); i++)
				DLOG(i << " " << h.GetFrame(i));
#endif
		}
	}
	
	DLOG("================ OK");
}
