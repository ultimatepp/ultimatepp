#include "Core.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

namespace Upp {

int FileMapping::MappingGranularity()
{
	static int mg = 0;
#ifdef PLATFORM_WIN32
	ONCELOCK {
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		mg = info.dwAllocationGranularity;
	}
#else
	ONCELOCK {
		mg = getpagesize();
	}
#endif
	return mg;
}

FileMapping::FileMapping(const char *file_)
{
#ifdef PLATFORM_WIN32
	hfile = INVALID_HANDLE_VALUE;
	hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
	hfile = -1;
	Zero(hfstat);
#endif
	base = rawbase = NULL;
	size = rawsize = 0;
	offset = rawoffset = 0;
	filesize = -1;
	write = false;
	if(file_)
		Open(file_);
}

#ifdef PLATFORM_WIN32
bool FileMapping::Open(const char *filename, dword mode, int64 wsize)
#else
bool FileMapping::Open(const char *filename, dword mode, mode_t acm)
#endif
{
	Close();
	write = (mode & FileStream::MODEMASK) != FileStream::READ;
#ifdef PLATFORM_WIN32
	if(!FileStream::OpenHandle(filename, mode, hfile, filesize))
		return false;
	if(write)
		filesize = wsize;
	else
		wsize = 0;
	hmap = CreateFileMapping(hfile, NULL, write ? PAGE_READWRITE : PAGE_READONLY, HIDWORD(wsize), LODWORD(wsize), NULL);
	if(!hmap) {
		Close();
		return false;
	}
#else
	if(!FileStream::OpenHandle(filename, mode, hfile, filesize, acm))
		return false;
#endif
	return true;
}


bool FileMapping::Create(const char *file, int64 filesize, bool delete_share)
{
	return Open(file, FileStream::CREATE | (delete_share ? FileStream::DELETESHARE : 0), filesize);
}

byte *FileMapping::Map(int64 mapoffset, size_t maplen)
{
	ASSERT(IsOpen());
	Unmap();
	mapoffset = minmax<int64>(mapoffset, 0, filesize);
	int gran = MappingGranularity();
	int64 rawoff = mapoffset & -gran;
	maplen = (size_t)min<int64>(maplen, filesize - mapoffset);
	size_t rawsz = (size_t)min<int64>((maplen + (size_t)(mapoffset - rawoff) + gran - 1) & -gran, filesize - rawoff);
	rawoffset = rawoff;
	rawsize = rawsz;
#ifdef PLATFORM_WIN32
	rawbase = (byte *)MapViewOfFile(hmap, write ? FILE_MAP_WRITE : FILE_MAP_READ,
		(dword)(rawoffset >> 32), (dword)(rawoffset >> 0), rawsize);
#else
	rawbase = (byte *)mmap(0, rawsize,
		PROT_READ | (write ? PROT_WRITE : 0),
#ifdef PLATFORM_FREEBSD
		MAP_NOSYNC,
#else
		MAP_SHARED,
#endif
		hfile, rawoffset);
#endif
#ifdef PLATFORM_POSIX
	if(rawbase == (byte *)~0)
#else
	if(!rawbase)
#endif
		return NULL;
	offset = mapoffset;
	size = maplen;
	base = rawbase + (int)(offset - rawoffset);
	return base;
}

bool FileMapping::Unmap()
{
	bool ok = true;
	if(rawbase) {
#ifdef PLATFORM_WIN32
		ok = !!UnmapViewOfFile(rawbase);
#endif
#ifdef PLATFORM_POSIX
		ok = (munmap((void *)rawbase, rawsize) == 0);
#endif
	}
	base = rawbase = NULL;
	size = 0;
	return ok;
}

bool FileMapping::Expand(int64 new_filesize)
{
	ASSERT(IsOpen());
	if(new_filesize > filesize) {
		if(!Unmap())
			return false;
#ifdef PLATFORM_WIN32
		if(!CloseHandle(hmap)) {
			hmap = NULL;
			return false;
		}
		hmap = NULL;
#endif
#ifdef PLATFORM_POSIX
		if(FTRUNCATE64_(hfile, new_filesize - filesize) != 0) {
			Close();
			return false;
		}
#endif
		filesize = new_filesize;
	}
	return true;
}

bool FileMapping::Close()
{
	bool ok = Unmap();
#ifdef PLATFORM_WIN32
	if(hmap) {
		if(!CloseHandle(hmap)) ok = false;
		hmap = NULL;
	}
	if(IsOpen()) {
		if(!CloseHandle(hfile)) ok = false;
		hfile = INVALID_HANDLE_VALUE;
	}
#endif
#ifdef PLATFORM_POSIX
	if(IsOpen()) {
		if(close(hfile) != 0) ok = false;
		Zero(hfstat);
		hfile = -1;
	}
#endif
	filesize = -1;
	offset = 0;
	size = 0;
	write = false;
	return ok;
}

Time FileMapping::GetTime() const
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	FileTime ft;
	GetFileTime(hfile, NULL, NULL, &ft);
	return ft;
#endif
#ifdef PLATFORM_POSIX
	return Time(hfstat.st_mtime);
#endif
}

String FileMapping::GetData(int64 offset, int len)
{
	if(IsOpen() && Map(offset, len))
		return String(base, len);
	else {
		NEVER();
		return String::GetVoid();
	}
}

}
