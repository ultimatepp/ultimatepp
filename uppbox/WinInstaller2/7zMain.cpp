/*
7zMain.c
LZMA SDK 4.26 Copyright (c) 1999-2005 Igor Pavlov (2005-08-02)
*/

#include "Install.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "7zCrc.h"
#include "7zIn.h"
#include "7zExtract.h"
};

struct CMemInStream {
	ISzInStream     InStream;
	const char     *ptr;
	size_t          pos, len;
};

SZ_RESULT SzMemReadImp(void *object, void *buffer, size_t size, size_t *processedSize)
{
	CMemInStream *s = (CMemInStream *)object;
	LOG("Read: " << size << ", pos: " << s->pos << ", len: " << s->len);
	int ps = min(s->len - s->pos, size);
	memcpy(buffer, s->ptr + s->pos, ps);
	s->pos += ps;
	if(processedSize)
		*processedSize = ps;
	return SZ_OK;
}

SZ_RESULT SzMemSeekImp(void *object, CFileSize p)
{
	LOG("Seek: " << p);
	CMemInStream *s = (CMemInStream *)object;
	if(p >= 0 && p <= s->len) {
		s->pos = p;
		return SZ_OK;
	}
	return SZE_FAIL;
}

void LZMAExtract(const char *ptr, int len, const char *outdir, Gate2<int, int> progress)
{
	CArchiveDatabaseEx db;
	SZ_RESULT res;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	CMemInStream archiveStream;
	archiveStream.InStream.Read = SzMemReadImp;
	archiveStream.InStream.Seek = SzMemSeekImp;
	archiveStream.ptr = ptr;
	archiveStream.pos = 0;
	archiveStream.len = len;

	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	InitCrcTable();
	SzArDbExInit(&db);
	res = SzArchiveOpen(&archiveStream.InStream, &db, &allocImp, &allocTempImp);
	if(res != SZ_OK)
		Error();

	UInt32 blockIndex = 0xFFFFFFFF; // it can have any value before first call (if outBuffer = 0)
	Byte *outBuffer = 0; // it must be 0 before first call for each new archive.
	size_t outBufferSize = 0;  // it can have any value before first call (if outBuffer = 0)
	for(dword i = 0; i < db.Database.NumFiles; i++) {
		if(progress(i, db.Database.NumFiles))
			Error("Installation aborted by user");
		size_t offset;
		size_t outSizeProcessed;
		CFileItem *f = db.Database.Files + i;
		String fn = AppendFileName(outdir, f->Name);
		RealizeDirectory(GetFileFolder(fn));
		if(!f->IsDirectory) {
			res = SzExtract(&archiveStream.InStream, &db, i,
			                &blockIndex, &outBuffer, &outBufferSize,
			                &offset, &outSizeProcessed,
			                &allocImp, &allocTempImp);
			if(res != SZ_OK)
				if (res == SZE_OUTOFMEMORY)
					Error("Not enough memory");
				else
					Error();
			String fn = AppendFileName(outdir, f->Name);
			FileAppend f(fn);
			f.Put(outBuffer + offset, outSizeProcessed);
			if(f.IsError())
				Error("Unable to write output file(s)");
		}
	}
	allocImp.Free(outBuffer);
	SzArDbExFree(&db, allocImp.Free);
}
