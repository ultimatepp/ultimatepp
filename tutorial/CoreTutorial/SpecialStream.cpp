#include "Tutorial.h"

void SpecialStream()
{
	/// .Special streams
	SizeStream szs;
	szs << "1234567";
	DUMP(szs.GetSize());
	
	///
	
	StringStream in("123456");
	CompareStream cs(in);
	cs.Put("12345");
	DUMP(cs.IsEqual());
	cs.Put("7");
	DUMP(cs.IsEqual());
	
	///
	
	struct MyOutStream : OutStream {
		virtual  void  Out(const void *data, dword size) {
			DUMPHEX(String((const char *)data, size));
		}
	};
	
	MyOutStream os;
	os << "This is a test " << 12345;
	os.Close();
	
	///
	
	StringStream ss1;
	StringStream ss2;
	TeeStream tee(ss1, ss2);
	tee << "Tee stream test";
	tee.Close();
	DUMP(ss1.GetResult());
	DUMP(ss2.GetResult());

	///
	
	static const char s[] = "Some line\nAnother line";
	MemReadStream ms(s, sizeof(os));
	while(!ms.IsEof())
		DUMP(ms.GetLine());
	
	///
}
