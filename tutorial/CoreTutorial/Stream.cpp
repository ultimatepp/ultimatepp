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
	
	/// The most basic operations of streams are `Put` and `Get`. `Get` works in the same ways
	/// as good old C getc - it returns negative number on eof or error:

	String h;
	int c;
	while((c = in.Get()) >= 0)
		h.Cat(c);
	DUMP(h);

	/// U++ streams provide no formatting capabilities (that is deferred to text utilities),
	/// but they have some unique features. U++ does not distinguish between 'text' and
	/// 'binary' mode streams, methods are well suited to work with both in common mode.
	
	/// `GetLine` returns `String` of single line read (lines separator being '\n', '\r' is
	/// ignored):

	in.Seek(0);
	while(!in.IsEof())
		DUMP(in.GetLine());
	
	/// `Peek` can be used to look at the next character without actually moving on to the next
	/// one:
	
	in.Seek(0);
	DDUMP((char)in.Peek());
	DDUMP(in.GetLine());

	/// `Get` method reads at most specified number of bytes from the stream and returns them as
	/// `String`:

	in.Seek(0);
	DUMP(in.Get(10));
	
	/// If there is not enough characters in the Stream as required by Get, everything till EOF
	/// is returned:
	
	in.Seek(0);
	DUMP(in.Get(999999).GetCount());
	
	/// In contrast, `GetAll` method fails when there is not enough characters in the Stream and
	/// returns Void `String` if Stream is not in `LoadThrowing` mode:

	in.Seek(0);
	h = in.GetAll(100);
	DUMP(h.GetCount());
	
	///
	
	h = in.GetAll(999999);
	DUMP(h.IsVoid());
	
	/// In `LoadThrowing` mode, `Stream` throws `LoadingError` exception when there is problem with
	/// input `Stream`:
	
	in.LoadThrowing();
	try {
		in.GetAll(999999);
	}
	catch(LoadingError) {
		LOG("Loading error");
	}
	
	/// Template variant of `Stream::operator<<` is using `AsString` to convert data to text:

	String fn = GetHomeDirFile("test.txt");
	FileOut out(fn);
	if(!out) {
		LOG("Failed to open the file");
		return;
	}
	out << "Some number " << 321 << " and Point " << Point(1, 2);
	out.Close();
	
	/// When writing to the `Stream`, the good way to check for errors is to write all data,
	/// close the stream and then check for `IsError`:
	
	if(out.IsError()) { // check whether file was properly written
		LOG("Error");
		return;
	}
	DUMP(LoadFile(fn));
	
	/// `FileAppend` can be used to append data to the file:
	
	FileAppend out2(fn);
	out2 << "\nSomething more";
	out2.Close();
	DUMP(LoadFile(fn));
	
	/// Important and often used type of `Stream` is `StringStream` which works with `String` as
	/// input/output.
	
	/// `Stream` also provides methods to store/load primitive types, in both
	/// little-endian and big-endian modes:
	
	StringStream ss;
	ss.Put32le(0x12345678);
	ss.Put32be(0x12345678);
	DUMPHEX(ss.GetResult());

	///

	StringStream ss2(ss.GetResult());
	DUMPHEX(ss2.Get32le());
	DUMPHEX(ss2.Get32be());

	///
}
