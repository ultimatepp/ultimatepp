/* wave.cpp

	Copyright (c) 1996-2002 by Timothy J. Weber.

	See WAVE.txt for documentation.
*/


#include <stdlib.h>
#include "wave.h"

using namespace std;

/***************************************************************************
	macros and constants
***************************************************************************/

// constants for the canonical WAVE format
const int fmtChunkLength = 16;  // length of fmt contents
const int waveHeaderLength = 4 + 8 + fmtChunkLength + 8;  // from "WAVE" to sample data

/***************************************************************************
	typedefs and class definitions
***************************************************************************/

/***************************************************************************
	prototypes for static functions
***************************************************************************/

/***************************************************************************
	static variables
***************************************************************************/

/***************************************************************************
	public member functions for WaveFile
***************************************************************************/

WaveFile::WaveFile():
	readFile(0),
	writeFile(0),
	formatType(0),
	numChannels(0),
	sampleRate(0),
	bytesPerSecond(0),
	bytesPerSample(0),
	bitsPerChannel(0),
	dataLength(0),
	error(0),
	changed(true)
{
}

WaveFile::~WaveFile()
{
	Close();
}

bool WaveFile::OpenRead(const char* name)
{
	if (readFile || writeFile)
		Close();

	try {
		// open the RIFF file
		readFile = new RiffFile(name);
		if (!readFile->filep())
			throw error = "Couldn't open file";

		// read the header information
		if (strcmp(readFile->chunkName(), "RIFF")
			|| strcmp(readFile->subType(), "WAVE")
			|| !readFile->push("fmt "))
			throw error = "Couldn't find RIFF, WAVE, or fmt";

		size_t dwFmtSize = size_t(readFile->chunkSize());
		char* fmtChunk = new char[dwFmtSize];
		try {
			if (fread(fmtChunk, dwFmtSize, 1, readFile->filep()) != 1)
				throw error = "Error reading format chunk";
			readFile->pop();

			// set the format attribute members
			formatType = *((short*) fmtChunk);
			numChannels = *((short*) (fmtChunk + 2));
			sampleRate = *((long*) (fmtChunk + 4));
			bytesPerSecond = *((long*) (fmtChunk + 8));
			bytesPerSample = *((short*) (fmtChunk + 12));
			bitsPerChannel = *((short*) (fmtChunk + 14));

			// position at the data chunk
			if (!readFile->push("data"))
				throw error = "Couldn't find data chunk";

			// get the size of the data chunk
			dataLength = readFile->chunkSize();

			delete[] fmtChunk;
		} catch (...) {
			delete[] fmtChunk;
			throw error;
		}
	} catch (...) {
		Close();
		return false;
	}
	return true;
}

bool WaveFile::OpenWrite(const char* name)
{
	if (readFile || writeFile)
		Close();

	// open the file
	writeFile = fopen(name, "wb");
	if (!writeFile) {
		error = "Couldn't open output file";
		return false;
	}

	dataLength = 0;

	// write the header
	return WriteHeaderToFile(writeFile);
}

bool WaveFile::ResetToStart()
{
	if (readFile) {
		// pop out of the data chunk
		if (!readFile->rewind()
			|| !readFile->push("data"))
		{
			error = "Couldn't find data chunk on reset";
			return false;
		} else
			return true;
	} else if (writeFile) {
		return fseek(writeFile, waveHeaderLength, SEEK_SET) == 0;
	} else
		return false;
}

bool WaveFile::Close()
{
	bool retval = true;

	if (readFile) {
		delete readFile;  // closes the file before it's destroyed
		readFile = 0;
	} else if (writeFile) {
		// write the header information at the start of the file, if necessary
		if (changed) {
			long currentSpot = ftell(writeFile);  // save the position
			retval = WriteHeaderToFile(writeFile);
			fseek(writeFile, currentSpot, SEEK_SET);  // restore the old position
				// this is necessary so the file gets the right length--otherwise,
				// all the data we wrote would be truncated.
		}

		// close the file
		fclose(writeFile);
		writeFile = 0;
	}

	return retval;
}

bool WaveFile::FormatMatches(const WaveFile& other)
{
	return formatType == other.formatType
		&& numChannels == other.numChannels
		&& sampleRate == other.sampleRate
		&& bytesPerSecond == other.bytesPerSecond
		&& bytesPerSample == other.bytesPerSample
		&& bitsPerChannel == other.bitsPerChannel;
}

void WaveFile::CopyFormatFrom(const WaveFile& other)
{
	formatType = other.formatType;
	numChannels = other.numChannels;
	sampleRate = other.sampleRate;
	bytesPerSecond = other.bytesPerSecond;
	bytesPerSample = other.bytesPerSample;
	bitsPerChannel = other.bitsPerChannel;
}

void WaveFile::SetupFormat(int sampleRate, short bitsPerChannel, short channels)
{
	SetFormatType(1);
	SetNumChannels(channels);
	SetSampleRate(sampleRate);
	SetBytesPerSample((unsigned short)((bitsPerChannel >> 3) * channels));
	SetBytesPerSecond(sampleRate * GetBytesPerSample());
	SetBitsPerChannel(bitsPerChannel);
	SetNumSamples(0);
}

bool WaveFile::GetFirstExtraItem(string& type, string& value)
{
	if (readFile)
		return readFile->rewind() && readFile->getNextExtraItem(type, value);
	else
		return false;
}

bool WaveFile::GetNextExtraItem(string& type, string& value)
{
	if (readFile)
		return readFile->getNextExtraItem(type, value);
	else
		return false;
}

bool WaveFile::CopyFrom(WaveFile& other)
{
	const size_t transferBufSize = 4096;

	if (!writeFile) {
		error = "Copy to an unopened file";
		return false;
	} else if (!other.readFile) {
		error = "Copy from an unopened file";
		return false;
	}

	try {
		// allocate the transfer buffer
		char* transferBuffer = new char[transferBufSize];
		unsigned long bytesRead = 0;

		try {
			if (!other.ResetToStart())
				throw error = "Couldn't reset input file to start";

			while (bytesRead < other.dataLength) {
				// calculate the size of the next buffer
				size_t bytesToRead = (size_t) min(transferBufSize,
					size_t(other.dataLength - bytesRead));

				// read the buffer
				if (fread(transferBuffer, 1, bytesToRead, other.readFile->filep())
					!= bytesToRead)
					throw error = "Error reading samples from input file";
				bytesRead += bytesToRead;

				// write the buffer
				if (fwrite(transferBuffer, 1, bytesToRead, writeFile) != bytesToRead)
					throw error = "Error writing samples to output file";
				dataLength += bytesToRead;
				changed = true;
			}

			// delete the transfer buffer
			delete[] transferBuffer;
		} catch (...) {
			delete[] transferBuffer;
			throw error;
		}
	} catch (...) {
		return false;
	}

	return true;
}

bool WaveFile::WriteHeaderToFile(FILE* fp)
{
	// seek to the start of the file
	if (fseek(fp, 0, SEEK_SET) != 0)
		return false;

	// write the file header
	unsigned long wholeLength = waveHeaderLength + dataLength;
	unsigned long chunkLength = fmtChunkLength;

	if (fputs("RIFF", fp) == EOF
		|| fwrite(&wholeLength, sizeof(wholeLength), 1, fp) != 1
		|| fputs("WAVE", fp) == EOF
		|| fputs("fmt ", fp) == EOF
		|| fwrite(&chunkLength, sizeof(chunkLength), 1, fp) != 1
		|| fwrite(&formatType, sizeof(formatType), 1, fp) != 1
		|| fwrite(&numChannels, sizeof(numChannels), 1, fp) != 1
		|| fwrite(&sampleRate, sizeof(sampleRate), 1, fp) != 1
		|| fwrite(&bytesPerSecond, sizeof(bytesPerSecond), 1, fp) != 1
		|| fwrite(&bytesPerSample, sizeof(bytesPerSample), 1, fp) != 1
		|| fwrite(&bitsPerChannel, sizeof(bitsPerChannel), 1, fp) != 1
		|| fputs("data", fp) == EOF
		|| fwrite(&dataLength, sizeof(dataLength), 1, fp) != 1)
	{
		error = "Error writing header";
		return false;
	}

	// if it's the same file, now we don't have to write it again unless it's
	// been changed.
	if (fp == writeFile)
		changed = false;

	return true;
}

bool WaveFile::ReadSample(float& sample)
{
	double fSample;

	bool retval = ReadSample(fSample);

	sample = fSample;

	return retval;
}

bool WaveFile::WriteSample(float sample)
{
	return WriteSample(double(sample));
}

bool WaveFile::ReadSample(double& sample)
{
	bool retval = false;

	if (GetBitsPerChannel() == 8) {
		unsigned char cSample;
		retval = ReadSample(cSample);
		sample = double(cSample) / ((1 << (8 - 1)) - 1) - 1;
	} else if (GetBitsPerChannel() == 16) {
		short sSample;
		retval = ReadSample(sSample);
		sample = double(sSample) / ((1 << (16 - 1)) - 1);
	} else
		error = "Floats can be written only as 8 or 16-bit samples";

	return retval;
}

bool WaveFile::WriteSample(double sample)
{
	if (GetBitsPerChannel() == 8)
		return WriteSample((unsigned char)((sample + 1) * ((1 << (8 - 1)) - 1)));
	else if (GetBitsPerChannel() == 16)
		return WriteSample(short(sample * ((1 << (16 - 1)) - 1)));
	else {
		error = "Floats can be written only as 8 or 16-bit samples";
		return false;
	}
}

bool WaveFile::ReadSample(unsigned char& sample)
{
	if (GetBitsPerChannel() != 8) {
		error = "Sample size mismatch";
		return false;
	}

	return ReadRaw((char*) &sample);
};

bool WaveFile::WriteSample(unsigned char sample)
{
	if (GetBitsPerChannel() != 8) {
		error = "Sample size mismatch";
		return false;
	}

	return WriteRaw((char*) &sample);
};

bool WaveFile::ReadSample(short& sample)
{
	if (GetBitsPerChannel() != 16) {
		error = "Sample size mismatch";
		return false;
	}

	return ReadRaw((char*) &sample, 2);
};

bool WaveFile::WriteSample(short sample)
{
	if (GetBitsPerChannel() != 16) {
		error = "Sample size mismatch";
		return false;
	}

	return WriteRaw((char*) &sample, 2);
};

bool WaveFile::ReadSamples(unsigned char* samples, size_t count)
{
	if (GetBitsPerChannel() != 8) {
		error = "Sample size mismatch";
		return false;
	}

	return ReadRaw((char*) samples, GetNumChannels() * count);
}

bool WaveFile::WriteSamples(unsigned char* samples, size_t count)
{
	if (GetBitsPerChannel() != 8) {
		error = "Sample size mismatch";
		return false;
	}

	return WriteRaw((char*) samples, GetNumChannels() * count);
}

bool WaveFile::ReadSamples(short* samples, size_t count)
{
	if (GetBitsPerChannel() != 16) {
		error = "Sample size mismatch";
		return false;
	}

	return ReadRaw((char*) samples, 2 * GetNumChannels() * count);
}

bool WaveFile::WriteSamples(short* samples, size_t count)
{
	if (GetBitsPerChannel() != 16) {
		error = "Sample size mismatch";
		return false;
	}

	return WriteRaw((char*) samples, 2 * GetNumChannels() * count);
}

bool WaveFile::ReadRaw(char* buffer, size_t numBytes)
{
	if (fread(buffer, 1, numBytes, GetFile()) != numBytes) {
		error = "Couldn't read samples";
		return false;
	}

	return true;
}

bool WaveFile::WriteRaw(char* buffer, size_t numBytes)
{
	if (fwrite(buffer, 1, numBytes, writeFile) != numBytes) {
		error = "Couldn't write samples";
		return false;
	}

	SetDataLength(GetDataLength() + numBytes);

	return true;
}

/***************************************************************************
	private member functions for WaveFile
***************************************************************************/

/***************************************************************************
	main()
***************************************************************************/

#ifdef TEST_WAVE

#include <iostream>

static void reportProblem()
{
	cout << "  *** ERROR: Result incorrect." << endl;
}

static void checkResult(bool got, bool expected)
{
	if (got)
		cout << "success." << endl;
	else 
		cout << "fail." << endl;

	if (got != expected)
		reportProblem();
}

static void pause()
{
	cout << "Press Enter to continue." << endl;
	cin.get();
}

static void ShowErrors(WaveFile& from, WaveFile& to)
{
	bool any = from.GetError() || to.GetError();

	if (from.GetError())
		cout << "Error on input: " << from.GetError() << "." << endl;

	if (to.GetError())
		cout << "Error on output: " << to.GetError() << "." << endl;

	if (!any)
		cout << "Success." << endl;
}

static void ShowFormat(WaveFile& wave, bool details = true)
{
	cout
		<< "Format:           " << wave.GetFormatType()
		<< (wave.IsCompressed()? " (compressed)" : " (PCM)") << endl
		<< "Channels:         " << wave.GetNumChannels() << endl
		<< "Sample rate:      " << wave.GetSampleRate() << endl
		<< "Bytes per second: " << wave.GetBytesPerSecond() << endl
		<< "Bytes per sample: " << wave.GetBytesPerSample() << endl
		<< "Bits per channel: " << wave.GetBitsPerChannel() << endl
		<< "Bytes:            " << wave.GetDataLength() << endl
		<< "Samples:          " << wave.GetNumSamples() << endl
		<< "Seconds:          " << wave.GetNumSeconds() << endl;

	if(wave.GetFile())
		cout << "File pointer:     " << ftell(wave.GetFile()) << endl;
	else
		cout << "File pointer:     null" << endl;

	if (details) {
		string type, value;
		if (wave.GetFirstExtraItem(type, value)) {
			cout << "Extra data:" << endl;
			do {
				cout << "  " << type << ": " << value << endl;
			} while (wave.GetNextExtraItem(type, value));
		}
		wave.ResetToStart();
	}
	pause();
}

int main(int argc, const char* argv[])
{
	if (argc < 3)
		cout << "Copies one WAVE file to another, in canonical form." << endl;
	else {
		WaveFile From, To;

		cout << "Opening input..." << endl;
		From.OpenRead(argv[1]);
		ShowErrors(From, To);
		ShowFormat(From);

		cout << "Setting formats..." << endl;
		To.CopyFormatFrom(From);
		ShowFormat(To);

		cout << "Opening output..." << endl;
		To.OpenWrite(argv[2]);
		ShowErrors(From, To);

		cout << "Copying..." << endl;
		To.CopyFrom(From);
		ShowErrors(From, To);
		cout << "Resulting format: " << endl;
		ShowFormat(To);
		cout << "Source format: " << endl;
		ShowFormat(From);

		cout << "Closing..." << endl;
		To.Close();
		From.Close();
		ShowErrors(From, To);
	}

	return 0;
}

#endif
