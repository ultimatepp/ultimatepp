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
  	
  	return String(digest, 28);
}

String SHA224Hex(const String& data) {
  	return HexString(SHA224String(data));
}

String SHA256String(const String& data) {
	SHA256_CTX sha;
	byte digest[32];
  	
  	SHA256_Init(&sha);
  	SHA256_Update(&sha, data, data.GetCount());
  	SHA256_Final(digest, &sha);
  	
  	return String(digest, 32);
}

String SHA256Hex(const String& data) {
  	return HexString(SHA256String(data));
}

String SHA384String(const String& data) {
	SHA512_CTX sha;
	byte digest[48];
  	
  	SHA384_Init(&sha);
  	SHA384_Update(&sha, data, data.GetCount());
  	SHA384_Final(digest, &sha);
  	
  	return String(digest, 48);
}

String SHA384Hex(const String& data) {
  	return HexString(SHA384String(data));
}

String SHA512String(const String& data)
{
	SHA512_CTX sha;
	byte digest[64];
  	
  	SHA512_Init(&sha);
  	SHA512_Update(&sha, data, data.GetCount());
  	SHA512_Final(digest, &sha);
  	
  	return String(digest, 64);
}

String SHA512Hex(const String& data) {
  	return HexString(SHA512String(data));
}
