#ifndef _TestProtec_RC4_h_
#define _TestProtec_RC4_h_

#include "CypherBase.h"

NAMESPACE_UPP

class RC4 : public CypherBase
{
	private:
	
		byte sbox[256];
		byte si, sj;
		
	protected:
	
		// stream support
		virtual String StreamOut(void);
		virtual void StreamIn(String const &s);
		virtual void Flush(void);
		
	public:
	
		RC4();
		RC4(String const &key, qword nonce = 0);
		RC4(byte const *keyBuf, size_t keyLen, qword nonce = 0);

		bool SetKey(String const &key, qword nonce = 0);
		bool SetKey(byte const *keyBuf, size_t keyLen, qword nonce = 0);

		void operator()(const byte *src, byte *dst, size_t len);
		void operator()(byte *buf, size_t len);
		String operator()(String const &s);
};

END_UPP_NAMESPACE

#endif
