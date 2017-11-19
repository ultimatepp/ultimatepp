#include "Tutorial.h"

void Stream()
{
	/// .Streams basics

	/// U++ stream working with files is `FileStream`. It has 3 derived classes, `FileIn`,
	/// `FileOut` and `FileAppend`, for the most common uses.

	FileIn in(GetDataFile("test.txt"));
	if(!in) {
		LOG("Failed to open the file");
		return;
	}
	
	/// The most basic operations of streams are Put and Get. Get works in the same ways as
	/// good old C getc - it returns negative number on eof or error:

	String h;
	int c;
	while((c = in.Get()) >= 0)
		h.Cat(c);
	DUMP(h);

	/// U++ streams provide no formatting capabilities (that is deferred to text utilities),
	/// but they have some unique features. U++ does not distinguish between 'text' and
	/// 'binary' mode streams, methods are well suited to work with both in common mode.
	
	/// GetLine returns String of single line read (lines separators being '\n', '\r' are
	/// ignored):

	in.Seek(0);
	while(!in.IsEof())
		DUMP(in.GetLine());

	/// Get method reads at most specified number of bytes from the stream and returns them as
	/// String:

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