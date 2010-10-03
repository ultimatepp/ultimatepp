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
	
		// constructors
		RC4();
		RC4(String const &key, String const &nonce = Null);
		RC4(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);

		// key settings
		bool SetKey(String const &key, String const &nonce = Null);
		bool SetKey(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);

		// encode/decode string
		String operator()(String const &s);

		// encode/decode buffer, dest on different buffer
		void operator()(const byte *src, byte *dst, size_t len);

		// encode/decode buffer in place
		void operator()(byte *buf, size_t len);
};

END_UPP_NAMESPACE

#endif
