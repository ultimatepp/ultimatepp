#include <Core/Core.h>

#include <Protect/Protect.h>
#include <Xed/Xed.h>

// uncomment this line to see encryption details
//#define ENCRYPT_DEBUG

using namespace Upp;

// search a buffer for code sequence
// returns starting position and length of requested sequence in parameters
// return a sequence code if found, SEQ_NONE if not
// the sequence is ALWAYS composed by 6 call instructions directed to 2 dummy functions
// the sequence encodes the start and end of code or data parts to be encrypted
CodeSequence ProtectSearchBuf(byte *buf, byte *bufEnd, uint8_t *&seqStart, uint8_t *&blockStart, uint8_t *&blockEnd, uint8_t *&seqEnd)
{
	int maxPatternSize = PROTECT_PatternSize(SEQ_CODE_START);
	
	while(buf <= bufEnd - maxPatternSize)
	{
		// check for a sequence
		CodeSequence seq = PROTECT_CheckPattern(buf);
		
		// found; it should be a start sequence, otherwise we've got a problem
		CodeSequence requestedEndSeq = SEQ_NONE;
		switch(seq)
		{
			case SEQ_CODE_END:
			case SEQ_DATA_END:
			case SEQ_OBFUSCATE_END:
				Cerr() << "UNEXPECTED END SEQUENCE\n";
			case SEQ_NONE:
				buf++;
				continue;
				
			case SEQ_CODE_START:
				requestedEndSeq = SEQ_CODE_END;
				break;
				
			case SEQ_DATA_START:
				requestedEndSeq = SEQ_DATA_END;
				break;
				
			case SEQ_OBFUSCATE_START:
				requestedEndSeq = SEQ_OBFUSCATE_END;
				break;
		}
		
		// store sequence start
		seqStart = buf;
		
		// skip starting sequence
		buf += PROTECT_PatternSize(seq);
		
		// store block start
		blockStart = buf;
		
		// look for corresponding end sequence; it should be the first sequence found
		// after this one
		while(buf <= bufEnd - maxPatternSize)
		{
			// check for a sequence
			CodeSequence endSeq = PROTECT_CheckPattern(buf);
			
			// if no sequence found, go on
			if(endSeq == SEQ_NONE)
			{
				buf++;
				continue;
			}
			
			// it the requested end sequence was found, all ok
			else if(endSeq == requestedEndSeq)
			{
				// store end sequence position
				blockEnd = buf;
				
				// store pointer after end sequence
				seqEnd = buf + PROTECT_PatternSize(endSeq);
				
				// return found sequence
				return seq;
			}
			
			// unexpected sequence... return null sequence
			Cerr() << "Unexpected END sequence '" << (int)endSeq << " for start sequence " << (int)seq << "\n";
			return SEQ_NONE;
		
		}
	}
	
	// no sequence found
	return SEQ_NONE;
}

// replace the start marker with a jmp and a NONCE random key
static int mkCodeNonce(uint8_t *buf)
{
	int nonceSize = PROTECT_PatternSize(SEQ_CODE_START) - 2;
	
	// modify start code adding a relative jump and embedding
	// a NONCE key after it
	// (also randomize code size area, it will be filled if needed
	// by remaining code
	buf[0] = 0xeb; // JMP REL, with 8 bit displacement
	buf[1] = (uint8_t)nonceSize;
	for(int i = 0; i < nonceSize; i++)
		buf[i + 2] = (uint8_t)Random();
	return nonceSize;
}

static int mkDataNonce(uint8_t *buf)
{
	int nonceSize = PROTECT_PatternSize(SEQ_DATA_START);
	
	for(int i = 0; i < nonceSize; i++)
		buf[i] = (uint8_t)Random();
	return nonceSize;
}

// encrypts code and data inside buffer
void CryptBuf(byte *buf, byte *bufEnd, String const &key)
{
	int codePatches = 0;
	int dataPatches = 0;
	int obfuscatePatches = 0;
	
	// scan all code and patch found sequences
	CodeSequence seq;
	uint8_t *seqStart, *blockStart, *blockEnd, *seqEnd;
	while( (seq = ProtectSearchBuf(buf, bufEnd, seqStart, blockStart, blockEnd, seqEnd)) != SEQ_NONE)
	{
		switch(seq)
		{
			case SEQ_CODE_START:
			{
#ifdef ENCRYPT_DEBUG
				Cerr() << "Encrypt code block\n";
#endif
				// replace the start sequence with a relative jump
				// and the nonce random encryption key
				int nonceSize = mkCodeNonce(seqStart);
				
				// replace first nonce part with block size
				*(uint32_t *)(seqStart + 2) = (uint32_t)(blockEnd - blockStart);
				
				// replace end sequence with a relative jump and some random data
				// so hackers can't easily find it
				mkCodeNonce(blockEnd);
				
				// create the encrypter
				Snow2 snow2((uint8_t const *)~key, key.GetCount(), seqStart + 2, nonceSize);
				
				// encrypt first byte of each instruction
				while(blockStart < blockEnd)
				{
					int len = XED.InstructionLength(blockStart);
					Cerr() << "LEN:" << len << "   " << XED.DisassembleInstruction(blockStart, true) << "\n";
					snow2(blockStart, 1);
					blockStart += len;
				}
				codePatches++;
				
#ifdef ENCRYPT_DEBUG
				Cerr() << "END encrypt code block\n";
#endif
				break;
			}

			case SEQ_OBFUSCATE_START:
			{
#ifdef ENCRYPT_DEBUG
				Cerr() << "Obfuscate code block\n";
#endif
				// replace the start sequence with a relative jump
				// and the nonce random encryption key + encryption key itself
				// (obfuscation don't need an external key, it will embed it into code start)
				int nonceSize = mkCodeNonce(seqStart);

				// replace first nonce part with block size
				*(uint32_t *)(seqStart + 2) = (uint32_t)(blockEnd - blockStart);
				
				// replace end sequence with a relative jump and some random data
				// so hackers can't easily find it
				int keySize = mkCodeNonce(blockEnd);
				
				// snow keys must be 16 or 32 bytes long
				// we've an area of 5*6 - 2 = 28 bytes, so we shall use 16 bytes key
				uint8_t const *key = blockEnd + 2 + (keySize - 16);
				keySize = 16;
				
				// store relative position of seqStart from blockEnd to end sequence block
				// this will also be part of obfuscation key
				*(uint32_t *)(blockEnd + 2) = (uint32_t)(blockEnd - seqStart);
				
				// use last random data as an encryption key -- obfuscation doesn't
				// need an external key
								
				// create the encrypter
				Snow2 snow2(key, keySize, seqStart + 2, nonceSize);
				
				// encrypt first byte of each instruction
				while(blockStart < blockEnd)
				{
					int len = XED.InstructionLength(blockStart);
					Cerr() << "LEN:" << len << "   " << XED.DisassembleInstruction(blockStart, true) << "\n";
					snow2(blockStart, 1);
					blockStart += len;
				}
				obfuscatePatches++;
#ifdef ENCRYPT_DEBUG
				Cerr() << "END obfuscate code block\n";
#endif
				break;
			}

			case SEQ_DATA_START:
			{
#ifdef ENCRYPT_DEBUG
				Cerr() << "Encrypt data block\n";
#endif
				// randomize data start and use it as a NONCE
				mkDataNonce(seqStart);
				
				// replace first 4 bytes of start sequence with 32 bit data size block
				*(uint32_t *)seqStart = blockEnd - blockStart;
				
				// randomize data end and use it as a NONCE
				int nonceSize = mkDataNonce(blockEnd);
				
				// create the encrypter
				Snow2 snow2((uint8_t const *)~key, key.GetCount(), blockEnd, nonceSize);
				
				snow2(blockStart, blockEnd - blockStart);
				dataPatches++;
#ifdef ENCRYPT_DEBUG
				Cerr() << "END encrypt data block\n";
#endif
				break;
			}
			
			default:
				Cerr() << "OPS! UNEXPECTED SEQUENCE!\n";
				return;
		}
		buf = seqEnd;
	}
	
	Cout() << "ENCRYPT RESULTS:\n";
	Cout() << "Code sequences      : " << codePatches << "\n";
	Cout() << "Data sequences      : " << dataPatches << "\n";
	Cout() << "Obfuscate sequences : " << obfuscatePatches << "\n";
}

CONSOLE_APP_MAIN
{
	// setup exit code for errors
	SetExitCode(1);

	// command line parameters are :
	// 1) file to encrypt (full path)
	// 2) key, in hex ascii format (AABB001122CCDD....)
	if(CommandLine().GetCount() != 2)
	{
		Cerr() << "USAGE : ProtectEncrypt filename key\n";
		return;
	}
	
	// gets key, check it and convert to HEX String

	// string must contain an even number of digits
	// and they must be hax ones (0-9, A-F)
	String key0 = ToUpper(CommandLine()[1]);
	if(key0.GetCount() != 32 && key0.GetCount() != 64)
	{
		Cerr() << "Error: key MUST be 32 or 64 chars (16 or 32 bytes) long, not " << key0.GetCount() << "\n";
		return;
	}
	String key;
	for(int i = 0; i < key0.GetCount()-1;)
	{
		byte b;
		char c = key0[i++];
		if(c >= '0' && c <= '9')
			b = c - '0';
		else if(c >= 'A' && c <= 'F')
			b = c - 'A' + 10;
		else
		{
			Cerr() << "Invalid hex digit '" << c << "' in key\n";
			return;
		}
		b <<= 4;
		c = key0[i++];
		if(c >= '0' && c <= '9')
			b += c - '0';
		else if(c >= 'A' && c <= 'F')
			b += c - 'A' + 10;
		else
		{
			Cerr() << "Invalid hex digit '" << c << "' in key\n";
			return;
		}
		key.Cat(b);
	}
	Cerr() << "ENCRYPTION KEY : " << HexString(key) << "\n";
	
	// loads file into buffer
	String fName = CommandLine()[0];
	if(!FileExists(fName))
	{
		Cerr() << "File '" << fName << "' not found\n";
		return;
	}
	FileIn f(fName);
	dword size = (dword)f.GetSize();
	Buffer<byte>buf(size);
	f.GetAll(buf, size);
	f.Close();
	
	// encrypt the application
	CryptBuf(buf, buf + size, key);

	// save the encrypted file
	FileOut fOut(fName);
	fOut.Put(buf, size);

	// sets up exit code
	SetExitCode(0);
}
