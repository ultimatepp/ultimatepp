#ifndef _SSLTest_AESStream_h_
#define _SSLTest_AESStream_h_

#include <Core/Core.h>

using namespace Upp;

#include <openssl/aes.h>

//----------------------------------------------------------------------------------------------
String AESRandomString(int length);
String AESPadString(const String &s, int l);

//----------------------------------------------------------------------------------------------
class AESEncoderStream
{
public:
	AESEncoderStream(qword size, const String &key); // Deprecated in C++11: throw (const char *);
	
	int AddData(const String &);
	
	String GetEncryptedData();
	
private:
	String GetHeader();
	
	qword        size,
	             blocks,
	             blocksDone;
	AES_KEY      key;
	String       data;
	const String iv0;
	StringBuffer iv;
	bool         encodedHeader;
};

inline String & operator << (String &s, AESEncoderStream &stream)
{
	s << stream.GetEncryptedData();
	return s;
}

inline AESEncoderStream & operator << (AESEncoderStream &stream, const String &s)
{
	stream.AddData(s);
	return stream;
}

//----------------------------------------------------------------------------------------------
class AESDecoderStream
{
public:
	AESDecoderStream(const String &key); // Deprecated in C++11:  throw (const char *);
	
	int AddData(const String &);
	String GetDecryptedData(); // Deprecated in C++11:  throw (const char *); //выброс исключения если ключ неверный
	
	qword GetSize() {return size;}

private:
	qword        size,
	             sizeDone;
	AES_KEY      key;
	String       data;
	StringBuffer iv;
	bool         decodedHeader;
};

inline String & operator << (String &s, AESDecoderStream &stream)
{
	s << stream.GetDecryptedData();
	return s;
}

inline AESDecoderStream & operator << (AESDecoderStream &stream, const String &s)
{
	stream.AddData(s);
	return stream;
}

//----------------------------------------------------------------------------------------------
extern const dword AES_CONTAINER_DWORD_HEADER;

String SHA256Bin(const char *key);
	
#endif
