#include "CypherBase.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////////////
//                                      CypherFifo class                                  //
////////////////////////////////////////////////////////////////////////////////////////////

// inserts data
void CypherFifo::Put(byte const *buf, int len)
{
	int l = data.GetCount();
	data.AddN(len);
	byte *b = &data.At(l);
	memcpy(b, buf, len);
}

void CypherFifo::Put(String const &s)
{
	Put((byte const *)~s, s.GetCount());
}

void CypherFifo::Put(byte b)
{
	data.Add(b);
}

// extract data
bool CypherFifo::Get(byte *buf, int len)
{
	if(data.GetCount() < len)
		return false;
	memcpy(buf, &data.At(0), len);
	data.Remove(0, len);
	return true;
}

bool CypherFifo::Get(String &s)
{
	if(!data.GetCount())
		return false;
	StringBuffer sb;
	sb.Cat((const char *)&data.At(0), data.GetCount());
	data.Clear();
	s = sb;
	return true;
}

bool CypherFifo::Get(byte &b)
{
	if(!data.GetCount())
		return false;
	b = data.At(0);
	data.Remove(0);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////
//                                      Cypher class                                  //
////////////////////////////////////////////////////////////////////////////////////////

Cypher::Cypher(size_t _blockSize)
{
	// still no key present
	keyOk = false;
	
	// Cypher in IDLE mode at startup
	mode = IDLE;
	
	// clears FIFO
	FIFO.Clear();
	
	// sets the encryption block size
	blockSize = _blockSize;
	
	// resets total stream size
	// should be set BEFORE streaming (decoding)
	// if we want the resulting stream truncated on right length
	// otherwise we get also the pad bytes
	streamSize = 0;

}

void Cypher::Reset()
{
	keyOk = false;
	mode = IDLE;
	FIFO.Clear();
	streamSize = 0;
}

// set mode (either STREAM or BLOCK
// throws an exception if another mode was already active
void Cypher::SetMode(Mode m)
{
	// checks if another mode already in progress, if so throws an exception
	if(mode != m && mode != IDLE)
		throw "A different encoding mode was already in progress";
	
	// if starting stream mode, allocate block buffer if
	// working with a Block-Cypher
	if(m == STREAM_MODE && mode == IDLE && blockSize > 1)
	{
		// allocates block buffer and set pointer to its start
		blockBuffer.Alloc(blockSize);
		blockBytes = 0;
		
		// resets streamedIn and streamedOut values
		streamedIn = streamedOut = 0;
	}
	mode = m;
}

// checks if key was set, otherwise throws an exception
void Cypher::CheckKey(void)
{
	if(!keyOk)
		throw "Missing key";
}
		
// checks buffer size if it must be multiple of block size
// for Block-Cyphers. Throws an exception if not
void Cypher::CheckBufSize(size_t bufSize)
{
	if(blockSize == 1)
		return;
	if(bufSize % blockSize)
		throw "Buffer size mismatch - not a multiple of block size";
}

// rounds buffer size to nearest bigger multiple of block size
size_t Cypher::RoundBufSize(size_t bufSize)
{
	size_t rem = bufSize % blockSize;
	if(rem)
		bufSize += blockSize - rem;
	return bufSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// key setting
void Cypher::SetKey(byte const *keyBuf, size_t keyLen, byte const *_nonce, size_t nonceLen)
{
	nonce.Clear();
	if(!_nonce || !nonceLen)
	{
		for(size_t i = 0; i < 8; i++)
			nonce.Cat(Random() & 0xff);
	}
	else
	{
		for(size_t i = 0; i < nonceLen; i++)
			nonce.Cat(_nonce[i]);
	}
	if(!CypherKey(keyBuf, keyLen, (const byte *)~nonce, nonce.GetCount()))
		throw "Invalid key";
	keyOk = true;
}

void Cypher::SetKey(String const &key, String const &nonce)
{
	if(IsNull(nonce))
		SetKey((byte const *)~key, key.GetCount(), NULL, 0);
	else
		SetKey((byte const *)~key, key.GetCount(), (byte const *)~nonce, nonce.GetCount());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// encrypt/decrypt functions
String Cypher::operator()(String const &s)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets buffer mode, exception if already in stream mode
	SetMode(BUFFER_MODE);
	
	// checks buffer size, exception if not multiple of block size
	CheckBufSize(s.GetCount());
	
	StringBuffer sb(s.GetCount());
	CypherCypher((const byte *)~s, (byte *)~sb, s.GetCount());
	return sb;
}

void Cypher::operator()(byte const *sourceBuf,  byte *destBuf, size_t bufLen)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets buffer mode, exception if already in stream mode
	SetMode(BUFFER_MODE);
	
	// checks buffer size, exception if not multiple of block size
	CheckBufSize(bufLen);
	
	CypherCypher(sourceBuf, destBuf, bufLen);
}

void Cypher::operator()(byte *buf, size_t bufLen)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets buffer mode, exception if already in stream mode
	SetMode(BUFFER_MODE);
	
	// checks buffer size, exception if not multiple of block size
	CheckBufSize(bufLen);
	
	CypherCypher(buf, buf, bufLen);
}
		
////////////////////////////////////////////////////////////////////////////////////////////////////
// streaming support
String Cypher::StreamOut(void)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets stream mode, exception if already in buffer mode
	SetMode(STREAM_MODE);
	
	String res;
	FIFO.Get(res);

	// if blockSize == 1 OR user didn't set stream size, simple behaviour
	if(blockSize != 1 && streamSize != 0)
	{
		if(streamedOut >= streamSize)
			return "";
		if(streamedOut + res.GetCount() > streamSize)
			res = res.Left(streamSize - streamedOut);
	}

	// returns encoded buffer from FIFO
	streamedOut += res.GetCount();
	return res;
}

void Cypher::StreamIn(String const &s)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets stream mode, exception if already in buffer mode
	SetMode(STREAM_MODE);
	
	// simpler streaming if blockSize == 1
	if(blockSize == 1)
	{
		// en/decode string and put into FIFO
		StringBuffer sb(s.GetCount());
		CypherCypher((const byte *)~s, (byte *)~sb, s.GetCount());
		FIFO.Put(sb);
	}
	else
	{
		size_t len = s.GetCount();
		size_t idx = 0;
		
		// if block buffer is partially filled, try to finish it before
		if(blockBytes)
		{
			size_t count = min(len, blockSize - blockBytes);
			idx += count;
			len -= count;
			memcpy(blockBuffer + blockBytes, ~s, count);
			blockBytes += count;
			if(blockBytes >= blockSize)
			{
				CypherCypher(blockBuffer, blockBuffer, blockSize);
				FIFO.Put(blockBuffer, blockSize);
				blockBytes = 0;
			}
		}
		
		// stream in full block buffers
		if(len)
		{
			// get number of bytes composing FULL blocks
			size_t count = len - (len % blockSize);
			
			if(count)
			{
				Buffer<byte> buf(count);
				CypherCypher((const byte *)~s + idx, buf, count);
				FIFO.Put(buf, count);
				len -= count;
				idx += count;
			}
		}
		
		// fills block buffer with remaining data, if any
		if(len)
		{
			memcpy(blockBuffer, ~s + idx, len);
			blockBytes = len;
		}
	}
	
	// updates number of streamed-in bytes
	streamedIn += s.GetCount();
}

size_t Cypher::Flush(void)
{
	// checks if KEY is ok, exception if not
	CheckKey();

	// sets stream mode, exception if already in buffer mode
	SetMode(STREAM_MODE);
	
	// nothing to do on Stream-Cyphers, just return stream total size
	if(blockSize == 1)
		return streamedIn;
	
	// on block stream, we should pad partial data (if any)
	// and encrypt them; we pad them with plain random data
	if(blockBytes)
	{
		for(size_t i = blockBytes; i < blockSize; i++)
			blockBuffer[i] = (byte)(Random() & 0xff);

		CypherCypher(blockBuffer, blockBuffer, blockSize);
		FIFO.Put(blockBuffer, blockSize);
		blockBytes = 0;
	}
	
	// return the true streamed-in byte count, not the padded one
	return streamedIn; 
	
};


END_UPP_NAMESPACE
