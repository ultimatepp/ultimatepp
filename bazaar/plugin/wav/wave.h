/* wave.h - Copyright (c) 1996-2002 by Timothy J. Weber */

#ifndef __WAVE_H
#define __WAVE_H

/* Headers required to use this module */
#include <stdio.h>
#include "rifffile.h"

/***************************************************************************
	macros, constants, and enums
***************************************************************************/

/***************************************************************************
	typedefs, structs, classes
***************************************************************************/

class WaveFile
{
public:
	WaveFile();
	~WaveFile();

	bool OpenRead(const char* name);
	bool OpenWrite(const char* name);
	bool ResetToStart();
	bool Close();

	unsigned short GetFormatType() const
		{ return formatType; };
	void SetFormatType(unsigned short type)
		{ formatType = type; changed = true; };
	bool IsCompressed() const
		{ return formatType != 1; };

	unsigned short GetNumChannels() const
		{ return numChannels; };
	void SetNumChannels(unsigned short num)
		{ numChannels = num; changed = true; };

	unsigned long GetSampleRate() const
		{ return sampleRate; };
	void SetSampleRate(unsigned long rate)
		{ sampleRate = rate; changed = true; };

	unsigned long GetBytesPerSecond() const
		{ return bytesPerSecond; };
	void SetBytesPerSecond(unsigned long bytes)
		{ bytesPerSecond = bytes; changed = true; };

	unsigned short GetBytesPerSample() const
		{ return bytesPerSample; };
	void SetBytesPerSample(unsigned short bytes)
		{ bytesPerSample = bytes; changed = true; };

	unsigned short GetBitsPerChannel() const
		{ return bitsPerChannel; };
	void SetBitsPerChannel(unsigned short bits)
		{ bitsPerChannel = bits; changed = true; };

	unsigned long GetNumSamples() const
		{ return (GetBytesPerSample())?
			GetDataLength() / GetBytesPerSample(): 0; };
	void SetNumSamples(unsigned long num)
		{ SetDataLength(num * GetBytesPerSample()); };

	float GetNumSeconds() const
		{ return GetBytesPerSecond()?
			float(GetDataLength()) / GetBytesPerSecond(): 0; };

	unsigned long GetDataLength() const
		{ return dataLength; };
	void SetDataLength(unsigned long numBytes)
		{ dataLength = numBytes; changed = true; };

	bool FormatMatches(const WaveFile& other);

	void CopyFormatFrom(const WaveFile& other);

	void SetupFormat(int sampleRate = 44100, short bitsPerChannel = 16, short channels = 1);

	FILE* GetFile()
		{ return readFile? readFile->filep(): writeFile; };

	RiffFile* GetRiffFile()
		{ return readFile? readFile : 0; };

	bool WriteHeaderToFile(FILE* fp);

	bool ReadSample(unsigned char& sample);
	bool WriteSample(unsigned char sample);
	bool ReadSample(short& sample);
	bool WriteSample(short sample);
	bool ReadSample(float& sample);
	bool WriteSample(float sample);
	bool ReadSample(double& sample);
	bool WriteSample(double sample);

	bool ReadSamples(unsigned char* samples, size_t count = 1);
	bool WriteSamples(unsigned char* samples, size_t count = 1);
	bool ReadSamples(short* samples, size_t count = 1);
	bool WriteSamples(short* samples, size_t count = 1);

	bool ReadRaw(char* buffer, size_t numBytes = 1);
	bool WriteRaw(char* buffer, size_t numBytes = 1);

	bool GetFirstExtraItem(std::string& type, std::string& value);
	bool GetNextExtraItem(std::string& type, std::string& value);

	bool CopyFrom(WaveFile& other);

	const char* GetError() const
		{ return error; };
	void ClearError()
		{ error = 0; };

protected:
	RiffFile* readFile;
	FILE* writeFile;

	unsigned short formatType;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short bytesPerSample;
	unsigned short bitsPerChannel;
	unsigned long dataLength;

	const char* error;
	bool changed;  // true if any parameters changed since the header was last written
};

/***************************************************************************
	public variables
***************************************************************************/

#ifndef IN_WAVE
#endif

/***************************************************************************
	function prototypes
***************************************************************************/

#endif
/* __WAVE_H */
