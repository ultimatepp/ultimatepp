#ifndef _TestProtec_RC4_h_
#define _TestProtec_RC4_h_

#include "CypherBase.h"

NAMESPACE_UPP

class RC4 : public Cypher
{
	private:
	
		byte sbox[256];
		byte si, sj;
		
	protected:
	
		// encode/decode buffer, dest on different buffer
		virtual void CypherCypher(const byte *src, byte *dst, size_t len);

		// key settings
		virtual bool CypherKey(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);

	public:
	
		// constructors
		RC4();
		RC4(String const &key, String const &nonce = Null);
		RC4(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);

};

END_UPP_NAMESPACE

#endif
