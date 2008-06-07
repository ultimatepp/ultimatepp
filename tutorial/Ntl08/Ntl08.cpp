#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Index<String> ndx;
	ndx.Add("alfa");
	ndx.Add("beta");
	ndx.Add("gamma");
	ndx.Add("delta");
	ndx.Add("kappa");
	DUMPC(ndx);

	DUMP(ndx.Find("beta"));
	DUMP(ndx.Find("something"));

	ndx.Set(0, "delta");
	DUMPC(ndx);

	int fi = ndx.Find("delta");
	while(fi >= 0) {
		DUMP(fi);
		fi = ndx.FindNext(fi);
	}

	DUMP(ndx.FindAdd("one"));
	DUMP(ndx.FindAdd("two"));
	DUMP(ndx.FindAdd("three"));
	DUMP(ndx.FindAdd("two"));
	DUMP(ndx.FindAdd("three"));
	DUMP(ndx.FindAdd("one"));

	ndx.Unlink(2);
	ndx.UnlinkKey("kappa");
	DUMP(ndx.Find(ndx[2]));
	DUMP(ndx.Find("kappa"));

	DUMP(ndx.IsUnlinked(1));
	DUMP(ndx.IsUnlinked(2));

	ndx.Put("foo");
	DUMP(ndx.Find("foo"));

	ndx.Sweep();
	DUMPC(ndx);

	ndx.Remove(1);
	DUMPC(ndx);

	ndx.RemoveKey("two");
	DUMPC(ndx);

	ndx.Insert(0, "insert");
	DUMPC(ndx);

	Vector<String> d = ndx.PickKeys();
	Sort(d);
	DUMPC(d);
}
