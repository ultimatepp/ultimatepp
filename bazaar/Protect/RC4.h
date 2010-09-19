#ifndef _TestProtec_RC4_h_
#define _TestProtec_RC4_h_

#include <Core/Core.h>

using namespace Upp;

class RC4
{
	private:
	
		unsigned char sbox[256];
		unsigned char si, sj;
		
	public:
	
		RC4();
		RC4(String const &key);
		void SetKey(String const &key);
		void Crypt(byte *src, byte *dst, size_t len);
};

#endif
