#ifndef _Snow2_h_
#define _Snow2_h_

#include "CypherBase.h"

NAMESPACE_UPP

class Snow2 : public Cypher
{
	private:
	
		// snow2 box and data
		dword s15,s14,s13,s12,s11,s10,s9,s8,s7,s6,s5,s4,s3,s2,s1,s0;
		dword outfrom_fsm,fsmtmp,r1,r2;
		
		// index of byte being encoded -- needed to sync with 16 x dword generated
		// keystream smaller chunks
		dword byteIndex;
		
		// keystream buffer
		dword keyStream[16];

		/*
		 * Function:  loadkey
		 *
		 * Synopsis:
		 *   Loads the key material and performs the initial mixing.
		 *
		 * Returns: void
		 *
		 * Assumptions:
		 *   keysize is either 128 or 256.
		 *   key is of proper length, for keysize=128, key is of lenght 16 bytes
		 *      and for keysize=256, key is of length 32 bytes.
		 *   key is given in big endian format, 
		 *   thus key[0]->msb of s15
		 *        key[1]->second msb of s15
		 *         ...
		 *        key[3]-> lsb of s15
		 *         ...
		 */
		void loadkey(byte const *key, dword keysize, dword IV3, dword IV2, dword IV1, dword IV0);

		/*
		 * Function: keystream
		 * 
		 * Synopsis:
		 *   Clocks the cipher 16 times and returns 16 words of keystream symbols
		 *   in keystream_block.
		 * 
		 * Returns: void
		 * 
		 */
		void keystream(dword *keystream_block);

	protected:
	
		// encode/decode buffer, dest on different buffer
		virtual void CypherCypher(byte const *sBuf, byte *dBuf, size_t bufLen);
		
		// key settings
		virtual bool CypherKey(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);
		
	public:
	
		// constructors
		Snow2();
		Snow2(const String &key, String const &nonce = Null);
		Snow2(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);
		
}; // END Class Snow2Cypher

END_UPP_NAMESPACE

#endif
