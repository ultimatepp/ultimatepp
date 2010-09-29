#ifndef _TestProtec_RC4_h_
#define _TestProtec_RC4_h_

#include <Core/Core.h>

NAMESPACE_UPP

class RC4
{
	private:
	
		byte sbox[256];
		byte si, sj;
		
	public:
	
		RC4();
		RC4(String const &key);
		RC4(byte const *keyBuf, int keyLen);

		bool SetKey(String const &key);
		bool SetKey(byte const *keyBuf, int keyLen);

		void Encode(const byte *src, byte *dst, size_t len);
		void Encode(byte *buf, size_t len);
		String Encode(String const &s);
};

END_UPP_NAMESPACE

#endif
