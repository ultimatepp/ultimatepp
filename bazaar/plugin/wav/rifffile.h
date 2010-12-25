/* rifffile.h - Copyright (c) 1996, 1998 by Timothy J. Weber */

#ifndef __RIFFFILE_H
#define __RIFFFILE_H

/* Headers required to use this module */
#include <stack>
#include <string>
#include <string.h>
#include <vector>

#include <stdio.h>

/***************************************************************************
	macros, constants, and enums
***************************************************************************/

/***************************************************************************
	typedefs, structs, classes
***************************************************************************/

class RiffFile;
class RiffChunk {
public:
	char name[5];
	unsigned long size;  // the length, read from the second chunk header entry
	char subType[5];  // valid for RIFF and LIST chunks
	long start;  // the file offset in bytes of the chunk contents
	long after;  // the start of what comes after this chunk

	// initialize at the file's current read position, and mark the file as bad
	// if there's an error.
	RiffChunk()
		{};
	RiffChunk(RiffFile& file);

	bool operator < (const RiffChunk& other) const
		{ return start < other.start; };
	bool operator == (const RiffChunk& other) const
	{ return strcmp(name, other.name) == 0
		&& size == other.size
		&& strcmp(subType, other.subType) == 0
		&& start == other.start; };
};

class RiffFile {
	FILE* fp;

	unsigned long formSize;

	std::stack<RiffChunk, std::vector<RiffChunk> > chunks;

public:
	RiffFile(const char *name);
	~RiffFile();

	bool rewind();
	bool push(const char* chunkType = 0);
	bool pop();
	long chunkSize() const;
	const char* chunkName() const;
	const char* subType() const;
	bool getNextExtraItem(std::string& type, std::string& value);
	FILE* filep()
		{ return fp; };

protected:
	bool readExtraItem(std::string& type, std::string& value);
};

/***************************************************************************
	public variables
***************************************************************************/

#ifndef IN_RIFFFILE
#endif

/***************************************************************************
	function prototypes
***************************************************************************/

#endif
/* __RIFFFILE_H */
