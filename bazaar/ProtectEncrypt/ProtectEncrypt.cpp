#include <Core/Core.h>

#include <Protect/Protect.h>

using namespace Upp;

// search a buffer for a matching pattern
// returns position of pattern
byte *ProtectSearchBuf(byte *buf, byte *bufEnd, const byte *pattern, size_t patternLen)
{
	while(buf < bufEnd - patternLen)
	{
		if(*buf != *pattern)
		{
			buf++;
			continue;
		}
		if(!memcmp(buf, pattern, patternLen))
		   return buf;
		buf++;
	}
	return NULL;
}

// encrypts code inside buffer
int CryptBuf(byte *buf, byte *bufEnd, String const &key)
{
	int patches = 0;
	
	byte *bStart = buf;
	while( (bStart = ProtectSearchBuf(bStart, bufEnd, (const byte *)PROTECT_START_MARKER, strlen(PROTECT_START_MARKER))) != NULL)
	{
		// overwrite start pattern, just to fool a bit
		// symple pattern search
		for(unsigned i = 0; i < strlen(PROTECT_START_MARKER); i++)
			*bStart++ = (byte)(Random() & 0xff);
		
		// locate end pattern
		byte *bEnd = ProtectSearchBuf(bStart, bufEnd, (const byte *)PROTECT_END_MARKER, strlen(PROTECT_END_MARKER));
		if(!bEnd)
			return 0;
		
		// get size of chunk to patch
		size_t size = bEnd - bStart;

		// overwrite end pattern, just to fool a bit
		// symple pattern search
		for(unsigned i = 0; i < strlen(PROTECT_END_MARKER); i++)
			*bEnd++ = (byte)(Random() & 0xff);
		
		// crypt buffer
		RC4 rc4(key);
		rc4.Crypt(bStart, bStart, size);
		patches++;
	}
	
	return patches;
}

CONSOLE_APP_MAIN
{
	// setup exit code for errors
	SetExitCode(1);

	// command line parameters are :
	// 1) file to encrypt (full path)
	// 2) key, in hex ascii format (AABB001122CCDD....)
	if(CommandLine().GetCount() != 2)
	{
		Cerr() << "USAGE : ProtectEncrypt filename key\n";
		return;
	}
	
	// gets key, check it and convert to HEX String

	// string must contain an even number of digits
	// and they must be hax ones (0-9, A-F)
	String key0 = ToUpper(CommandLine()[1]);
	if(!key0.GetCount())
	{
		Cerr() << "Error: null KEY\n";
		return;
	}
	if(key0.GetCount() % 2)
	{
		Cerr() << "Error: KEY must contain an even number of digits\n";
		return;
	}
	String key;
	for(int i = 0; i < key0.GetCount()-1;)
	{
		byte b;
		char c = key0[i++];
		if(c >= '0' && c <= '9')
			b = c - '0';
		else if(c >= 'A' && c <= 'F')
			b = c - 'A' + 10;
		else
		{
			Cerr() << "Invalid hex digit '" << c << "' in key\n";
			return;
		}
		b <<= 4;
		c = key0[i++];
		if(c >= '0' && c <= '9')
			b += c - '0';
		else if(c >= 'A' && c <= 'F')
			b += c - 'A' + 10;
		else
		{
			Cerr() << "Invalid hex digit '" << c << "' in key\n";
			return;
		}
		key.Cat(b);
	}
	
	// loads file into buffer
	String fName = CommandLine()[0];
	if(!FileExists(fName))
	{
		Cerr() << "File '" << fName << "' not found\n";
		return;
	}
	FileIn f(fName);
	dword size = (dword)f.GetSize();
	Buffer<byte>buf(size);
	f.GetAll(buf, size);
	
	int patches = CryptBuf(buf, buf + size, key);
	if(patches)
	{
		Cerr() << "Successfully patched " << patches << " functions\n";
		FileOut f(fName);
		f.Put(buf, size);
	}
	else
		Cerr() << "No patchpoints found\n";

	// sets up exit code
	SetExitCode(0);
	

}

