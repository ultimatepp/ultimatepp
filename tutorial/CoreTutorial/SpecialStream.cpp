#include "Tutorial.h"

void SpecialStream()
{
	/// .Special streams
	
	/// `SizeStream` counts the number of bytes written to the stream:
	
	SizeStream szs;
	szs << "1234567";
	DUMP(szs.GetSize());
	
	/// `CompareStream` can be used to compare the content of some stream with data written to
	/// `CompareStream`:
	
	StringStream in("123456");
	CompareStream cs(in);
	cs.Put("12345");
	DUMP(cs.IsEqual());
	
	///
	
	cs.Put("7");
	DUMP(cs.IsEqual());
	
	/// `OutStream` buffers output data to bigger blocks, then outputs them via `Out` virtual
	/// method:
	
	struct MyOutStream : OutStream {
		virtual  void  Out(const void *data, dword size) {
			DUMPHEX(String((const char *)data, size));
		}
	};
	
	MyOutStream os;
	os << "This is a test " << 12345;
	os.Close();
	
	/// `TeeStream` sends output data to two separate streams:
	
	StringStream ss1;
	StringStream ss2;
	TeeStream tee(ss1, ss2);
	tee << "Tee stream test";
	tee.Close();
	DUMP(ss1.GetResult());
	DUMP(ss2.GetResult());

	/// `MemReadStream` can be used to convert read-only memory block to stream data:
	
	static const char s[] = "Some line\nAnother line";
	MemReadStream ms(s, sizeof(s) - 1);
	while(!ms.IsEof())
		DUMPHEX(ms.GetLine());
	
	///
}
