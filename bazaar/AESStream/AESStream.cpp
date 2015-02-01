#include <Core/Core.h>

#undef _WIN32
#include <openssl/rand.h>
#include "AESStream.h"

void AESInit();

// 
// Код исходит из того, что AES_BLOCK_SIZE == 2^n
// Code has an assumption AES_BLOCK_SIZE == 2^n bytes 
//----------------------------------------------------------------------------------------------
String AESRandomString(int length)
{
	AESInit();
	
	static bool sslRandInit = false;
	if (!sslRandInit)
	{
		sslRandInit = true;
	}
	
	StringBuffer ident(length);
	RAND_bytes((unsigned char *) ~ident, length);
	return ident;
}

String AESPadString(const String &s, int l)
{
	AESInit();
	 
	int addL = s.GetLength() % l;
	if (!addL)
		return s;
	
	String outS(s);
	outS << AESRandomString(l - addL);
	return outS;
}

dword rdtsc()
{
	return Random();
}

//----------------------------------------------------------------------------------------------
bool aesInitDone = false;
void AESInit()
{
	if (aesInitDone)
		return;
	aesInitDone = true;
	 
	if (RAND_status())
		return;
	 
	qword qw = (((qword) GetTickCount()) << 16) ^ ((qword) (GetSysTime().Get())) ^ Random();
	RAND_seed(&qw, sizeof(qw));
	
	#ifdef PLATFORM_WIN32
	RAND_screen();
	#endif

	while (!RAND_status())
	{
		dword dw = Random();
		RAND_seed(&dw, sizeof(dword));
		dw = rdtsc();
		RAND_seed(&dw, sizeof(dword));
		
		static int hops = 0;
		if (hops > 1000000)
			throw "AESInit: Random generator init error";//Ошибка инициализации генератора случайных чисел";
	}
}

//----------------------------------------------------------------------------------------------
AESEncoderStream::AESEncoderStream(qword _size, const String &_key) throw (const char *)
	: size       (_size)
	, blocks     (_size / AES_BLOCK_SIZE)
	, blocksDone (0)
	, iv0        (AESRandomString(AES_BLOCK_SIZE))
	, encodedHeader (false)
{
	AESInit();
	
	*((dword *) ~iv0)       ^= (Random() ^ rdtsc());
	*(((dword *) ~iv0) + 1) ^= (Random() ^ GetTickCount());
	if (_size & (AES_BLOCK_SIZE-1))
		++blocks;
	
	iv.Cat(iv0);
	
	if (_key.GetLength() != 16 && _key.GetLength() != 24 && _key.GetLength() != 32)
		throw "AESEncoderStream::ctor: Key must be 16, 24 or 32 bytes long";//Неверная длина ключа. Нужно 16, 24 или 32 байт";
	if (AES_set_encrypt_key((const unsigned char *) ~_key, _key.GetLength() << 3, &key))
		throw "AESEncoderStream::ctor: Could not setup AES key. OpenSSL problem?";//Не установился ключ для AES. Проблема в OpenSSL?";
}

int AESEncoderStream::AddData(const String &s)
{
	int sl = s.GetLength();
	
	if (blocksDone == blocks)
		return 0;
	
	qword sizeLeft = size - blocksDone*AES_BLOCK_SIZE;
	if (((qword) s.GetLength()) <= sizeLeft)
		data << s;
	else
	{
		sl = (int) sizeLeft;
		data << s.Left(sl);
	}
	
	if (blocksDone*AES_BLOCK_SIZE+data.GetLength() == size)
	{
		//padding last bytes to fit into AES_BLOCK_SIZE
		//последние данные всегда добавляются шумом до размера, кратного блоку
		int extraLength = size % AES_BLOCK_SIZE;
		if (extraLength)
			data << AESRandomString(AES_BLOCK_SIZE - extraLength);
	}
	return sl;
} 

String AESEncoderStream::GetHeader()
{
	if (encodedHeader)
		return "";
	
	String header;
	
	dword magic1, magic2;
	String magicS = AESRandomString(sizeof(qword));
	memcpy(&magic1, ~magicS, sizeof(dword));
	memcpy(&magic2, (~magicS) + sizeof(dword), sizeof(dword));
	dword magicDelta = AES_CONTAINER_DWORD_HEADER - (magic1 + magic2);
	magic2 += magicDelta;
	header.Cat((const char *)&magic1, sizeof(dword));
	header.Cat((const char *)&magic2, sizeof(dword));
	
	header << iv0;
	
	qword sze = (qword) size;
	header.Cat((const char *) &sze, sizeof(sze));
	header = AESPadString(header, AES_BLOCK_SIZE);
	
	StringBuffer outBuffer(header.GetLength());
	
	for (int offs = 0; offs+AES_BLOCK_SIZE <= header.GetLength(); offs += AES_BLOCK_SIZE)
		AES_ecb_encrypt(((const unsigned char *) ~header)+offs, ((unsigned char *) ~outBuffer)+offs, &key, true);

	encodedHeader = true;
	return outBuffer;
}

String AESEncoderStream::GetEncryptedData()
{
	if (blocksDone == blocks)
		return "";
	
	qword overallBlocks = (qword) (data.GetLength()/AES_BLOCK_SIZE);
	int doBlocks = (int) min(overallBlocks, blocks-blocksDone);
	if (!doBlocks)
		return "";
	
	int doSize = doBlocks*AES_BLOCK_SIZE;
	StringBuffer outData(doSize);

	AES_cbc_encrypt((const unsigned char *) ~data, (unsigned char *) ~outData, (const unsigned long) doSize, &key, (unsigned char *) ~iv, true);
	if (doSize < data.GetLength())
		data.Remove(0, doSize);
	else
		data.Clear();
	blocksDone += doBlocks;
	
	if (encodedHeader)
		return outData;
	else
		return (GetHeader()+String(outData));
}

//----------------------------------------------------------------------------------------------
AESDecoderStream::AESDecoderStream(const String &_key) throw (const char *)
	: size       (0L)
	, sizeDone   (0L)
	, iv         (AES_BLOCK_SIZE)
	, decodedHeader (false)
{
	AESInit();

	if (_key.GetLength() != 16 && _key.GetLength() != 24 && _key.GetLength() != 32)
		throw "AESDecoderStream::ctor: Key must be 16, 24 or 32 bytes long";//Неверная длина ключа. Нужно 16, 24 или 32 байт";
	if (AES_set_decrypt_key((const unsigned char *) ~_key, _key.GetLength() << 3, &key))
		throw "AESDecoderStream::ctor: Could not setup AES key. OpenSSL problem?";//Не установился ключ для AES. Проблема в OpenSSL?";
}

int AESDecoderStream::AddData(const String &s)
{
	if (!size)
	{
		data << s;
		int io = data.GetLength();
		return s.GetLength();
	}
	
	qword sz = size & ~((qword) (AES_BLOCK_SIZE-1));
	if (size & (AES_BLOCK_SIZE-1))
		sz += AES_BLOCK_SIZE;
	
	qword dl = (qword)data.GetLength();
	if (dl >= sz)
		return 0;
	
	qword sl = (qword) s.GetLength();
	if (dl+sl < sz)
		data << s;
	else
	{
		sl = sz - dl;
		data << s.Left((int) sl);
	}

	return (int) sl;
}

String AESDecoderStream::GetDecryptedData() throw (const char *)
{
	#pragma pack(push,1)
	struct AES_CONTAINER_HEADER
	{
		dword m1;
		dword m2;
		Upp::byte  iv[AES_BLOCK_SIZE];
		qword size;
	} header;
	#pragma pack(pop)
	
	if (!decodedHeader)
	{
		if (data.GetLength() < sizeof(header))
			return "";
		
		for (int offs = 0; offs+AES_BLOCK_SIZE <= sizeof(header); offs += AES_BLOCK_SIZE)
			AES_ecb_encrypt(((const unsigned char *) ~data)+offs, ((unsigned char *) &header)+offs, &key, false);
		
		if (header.m1+header.m2 != AES_CONTAINER_DWORD_HEADER)
			throw "Wrong key!";//"Ключ шифрования неверный";
		
		memcpy(~iv,  &(header.iv[0]), AES_BLOCK_SIZE);
		size = header.size;
		
		data.Remove(0, sizeof(header));
		decodedHeader = true;
	}
	
	dword doSize = data.GetLength();
	qword sizeLeft = size-sizeDone;
	if (((qword) doSize) > sizeLeft)
		doSize = (dword) sizeLeft;
	if (!doSize)
		return "";
	
	dword extSize = doSize & (~(dword) (AES_BLOCK_SIZE-1));
	if (extSize && ((dword) data.GetLength()) >= doSize+AES_BLOCK_SIZE)
		doSize += AES_BLOCK_SIZE;
	    
	StringBuffer outData(doSize);

	AES_cbc_encrypt((const unsigned char *) ~data, (unsigned char *) ~outData, (const unsigned long) doSize, &key, (unsigned char *) ~iv, false);
	data.Remove(0, doSize);
	sizeDone += doSize;
	return outData;
}

//----------------------------------------------------------------------------------------------
const dword AES_CONTAINER_DWORD_HEADER = 0x377FEA9F;
