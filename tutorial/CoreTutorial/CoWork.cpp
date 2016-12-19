#include "Tutorial.h"

void CoWorkTutorial()
{
	/// .`CoWork`

	FileIn in(GetDataFile("test.txt")); // let us open some tutorial testing data
	
	Index<String> w;
	Mutex m; // need mutex to serialize access to w

	CoWork co;
	while(!in.IsEof()) {
		String ln = in.GetLine();
		co & [ln, &w, &m] {
			for(const auto& s : Split(ln, [](int c) { return IsAlpha(c) ? 0 : c; })) {
				Mutex::Lock __(m);
				w.FindAdd(s);
			}
		};
	}
	co.Finish();
	
	DUMP(w);
	
	///
	

	in.Seek(0);
	while(!in.IsEof()) {
		String ln = in.GetLine();
		co & [ln, &w, &m] {
			Vector<String> h = Split(ln, [](int c) { return IsAlpha(c) ? 0 : c; });
			CoWork::FinLock(); // replaces the mutex, locked till the end of CoWork job
			for(const auto& s : h)
				w.FindAdd(s);
		};
	}
	
	DUMP(w);
	
	///
}
