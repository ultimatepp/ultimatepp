#include "Tutorial.h"

void Stream()
{
	/// .Streams basics

	FileIn in(GetDataFile("test.txt"));
	if(!in) {
		LOG("Failed to open the file");
		return;
	}

	String h;
	int c;
	while((c = in.Get()) >= 0)
		h.Cat(c);
	DUMP(h);

	in.Seek(0);
	while(!in.IsEof())
		DUMP(in.GetLine());

	in.Seek(0);
	DUMP(in.Get(10));
	
	in.Seek(0);
	DUMP(in.Get(999999).GetCount());

	in.Seek(0);
	h = in.GetAll(100);
	DUMP(h.GetCount());
	h = in.GetAll(999999);
	DUMP(h.IsVoid());
	in.LoadThrowing();
	try {
		in.GetAll(999999);
	}
	catch(LoadingError) {
		LOG("Loading error");
	}

	String fn = GetHomeDirFile("test");
	FileOut out(fn);
	if(!out) {
		LOG("Failed to open the file");
		return;
	}
	out << "Some number " << 321 << " and Point " << Point(1, 2);
	out.Close();
	if(out.IsError()) { // check whether file was properly written
		LOG("Error");
		return;
	}
	DUMP(LoadFile(fn));
	
	FileAppend out2(fn);
	out2 << "\nSomething more";
	out2.Close();
	DUMP(LoadFile(fn));
	
	StringStream ss;
	ss.Put32le(0x12345678);
	ss.Put32be(0x12345678);
	
	StringStream ss2(ss.GetResult());
	DUMP(ss.Get32le());
	DUMP(ss.Get32be());
	DUMPHEX(ss.GetResult());
	
	

	///
}