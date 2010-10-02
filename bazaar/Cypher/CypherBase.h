#ifndef _CypherBase_h
#define _CypherBase_h

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
class CypherBase
{
	protected:
		// key ok flag
		bool keyOk;
		
		// mode of operation
		// used to avoid mixing of stream and block modes
		typedef enum { IDLE = 0, STREAM_MODE, BUFFER_MODE } Mode;
		Mode mode;
		
		// streaming buffer and handling functions
		CypherFifo FIFO;
		
		// stream support
		virtual String StreamOut(void) = 0;
		virtual void StreamIn(String const &s) = 0;
		virtual void Flush(void) = 0;
		
		// resets the Cypher, clearing mode, key flag and FIFO
		// SetKey() should call this one before re-keying
		void Reset(void);
		
	public:
	
		// Cypher exceptions
		typedef enum { CypherOk = 0, NoKey, StreamModeInProgress, BufferModeInProgress} CypherException;
		
		// empty constructor -- sets state to NOKEY, and resets stream buffer
		CypherBase();
		
		// key setting - rekeying
		// return true on success, false otherwise
		// rekeyng clears all stream states and buffers and sets state to idle
		virtual bool SetKey(String const &key, qword nonce = 0) = 0;
		virtual bool SetKey(byte const *keyBuf, size_t keyLen, qword nonce = 0) = 0;
		
		// encrypt/decrypt functions
		virtual String operator()(String const &s) = 0;
		virtual void operator()(byte const *sourceBuf,  byte *destBuf, size_t bufLen) = 0;
		virtual void operator()(byte *buf, size_t bufLen) = 0;
		
		// streaming support
		CypherBase & operator <<(String const &s) { StreamIn(s); return *this; }
		CypherBase & operator >>(String &s) { s = StreamOut(); return *this; }
		bool IsEof(void) { return !FIFO.GetCount(); }
		bool operator!(void) { return IsEof(); }
		operator bool() { return !IsEof(); }
	
}; // END Class CypherBase

END_UPP_NAMESPACE

#endif
