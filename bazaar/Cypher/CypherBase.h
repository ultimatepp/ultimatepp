#ifndef _CypherBase_h_
#define _CypherBase_h_

#include <Core/Core.h>

NAMESPACE_UPP

// FIFO class for streaming
class CypherFifo
{
	private:
		Vector<byte> data;
	
	protected:
	
	public:
	
		// clears FIFO contents
		void Clear(void) { data.Clear(); }
		
		// gets available data count
		int GetCount() { return data.GetCount(); }
		
		// inserts data
		void Put(byte const *buf, int len);
		void Put(String const &s);
		void Put(byte b);
		
		// extract data
		bool Get(byte *buf, int len);
		bool Get(String &s);
		bool Get(byte &b);
};

// Base class for all Cypher modules
// provides the common interface
class Cypher
{
	private:
	
		// key ok flag
		bool keyOk;
		
		// mode of operation
		// used to avoid mixing of stream and block modes
		typedef enum { IDLE = 0, STREAM_MODE, BUFFER_MODE } Mode;
		Mode mode;
		
		// block size, in bytes, for Block-Cyphers (like AES)
		// for Stream-Cyphers it's 1 (exactly 1 byte)
		size_t blockSize;
		
		// block buffer, needed for streaming on Block-Cyphers
		Buffer<byte>blockBuffer;
		size_t blockBytes;
		
		// streaming buffer and handling functions
		CypherFifo FIFO;
		
		// number of streamed-in and streamed-out bytes
		size_t streamedIn;
		size_t streamedOut;
		
		// total stream size, used on stream-decoding to truncate last block
		// on Block-Cyphers; useless on Stream-Cyphers
		size_t streamSize;
		
		// resets the Cypher, clearing mode, key flag and FIFO
		// SetKey() should call this one before re-keying
		void Reset(void);
		
		// set mode (either STREAM or BLOCK
		// throws an exception if another mode was already active
		void SetMode(Mode m);
		
		// checks if key was set, otherwise throws an exception
		void CheckKey(void);
		
		// checks buffer size if it must be multiple of block size
		// for Block-Cyphers. Throws an exception if not
		void CheckBufSize(size_t bufSize);
		
		// rounds buffer size to nearest bigger multiple of block size
		size_t RoundBufSize(size_t bufSize);
		
		// Initialization vector (IV or Nonce)
		String nonce;
		
		// stream support
		String StreamOut(void);
		void StreamIn(String const &s);
		
	protected:
		// main encoding/decoding function
		// must be redefined on each derived class
		virtual void CypherCypher(byte const *sourceBuf,  byte *destBuf, size_t bufLen) = 0;
		
		// main key setting function
		// must be redefined on each derived class
		virtual bool CypherKey(byte const *keyBuf, size_t keyLen, byte const *nonce, size_t nonceLen) = 0;

	public:
	
		// empty constructor -- sets state to NOKEY, and resets stream buffer
		// blockSize is the size of encryption block for Block-Cyphers
		// for Stream-Cyphers it must be 1
		Cypher(size_t blockSize);
		
		// key setting - rekeying
		// throws an exception if invalid key
		// rekeyng clears all stream states and buffers and sets state to idle
		void SetKey(String const &key, String const &nonce = Null);
		void SetKey(byte const *keyBuf, size_t keyLen, byte const *nonce = NULL, size_t nonceLen = 0);
		
		// gets the IV (Nonce) vector
		String const &GetNonce(void) { return nonce; }
		
		// encrypt/decrypt functions
		String operator()(String const &s);
		void operator()(byte const *sourceBuf,  byte *destBuf, size_t bufLen);
		void operator()(byte *buf, size_t bufLen);
		
		// streaming support
		void SetStreamSize(size_t size) { streamSize = size; }
		Cypher & operator <<(String const &s) { StreamIn(s); return *this; }
		Cypher & operator >>(String &s) { s = StreamOut(); return *this; }
		size_t Flush(void);
		bool IsEof(void) { return !FIFO.GetCount(); }
		bool operator!(void) { return IsEof(); }
		operator bool() { return !IsEof(); }
		
		// virtual destructor -- for correct polymorphic behaviour
		virtual ~Cypher() {}
	
}; // END Class Cypher

END_UPP_NAMESPACE

#endif
