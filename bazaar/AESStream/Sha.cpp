#include <Core/Core.h>

#include <openssl/sha.h>
#include "AESStream.h"

//SHA2 functions from OpenSSL

String SHA224String(const String& data) {
	SHA256_CTX sha;
	byte digest[28];
  	
  	SHA224_Init(&sha);
  	SHA224_Update(&sha, data, data.GetCount());
  	SHA224_Final(digest, &sha);
  	
  	return HexString(digest, 28);
}

String SHA256String(const String& data) {
	SHA256_CTX sha;
	byte digest[32];
  	
  	SHA256_Init(&sha);
  	SHA256_Update(&sha, data, data.GetCount());
  	SHA256_Final(digest, &sha);
  	
  	return HexString(digest, 32);
}

String SHA384String(const String& data) {
	SHA512_CTX sha;
	byte digest[48];
  	
  	SHA384_Init(&sha);
  	SHA384_Update(&sha, data, data.GetCount());
  	SHA384_Final(digest, &sha);
  	
  	return HexString(digest, 48);
}

String SHA512String(const String& data)
{
	SHA512_CTX sha;
	byte digest[64];
  	
  	SHA512_Init(&sha);
  	SHA512_Update(&sha, data, data.GetCount());
  	SHA512_Final(digest, &sha);
  	
  	return HexString(digest, 64);
}