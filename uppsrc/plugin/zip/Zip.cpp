#include "zip.h"

namespace Upp {

void Zip::WriteFolder(const char *path, Time tm)
{
	String p = UnixPath(path);
	if(p.GetCount() == 0)
		return; // LibreOffice refuses xlsx with "/" folder..
	if(*p.Last() != '/')
		p.Cat('/');
	WriteFile(~p, 0, p, Null, tm, false);
}

int64 zPress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress, bool gzip,
             bool compress, dword *crc, bool hdr);


void Zip::FileHeader(const char *path, Time tm)
{
	File& f = file.Top();
	f.path = UnixPath(path);
	zip->Put32le(0x04034b50);
	zip->Put16le(f.version);
	zip->Put16le(f.gpflag);
	zip->Put16le(f.method);
	zip->Put32le(f.time = (tm.day << 16) | (tm.month << 21) | ((tm.year - 1980) << 25) |
	                      (tm.hour << 11) | (tm.minute << 5) | (tm.second >> 1));
	ASSERT((f.gpflag & 0x8) == 0 || f.crc == 0);
	zip->Put32le(f.crc);
	ASSERT((f.gpflag & 0x8) == 0 || f.csize == 0);
	zip->Put32le((dword)(f.zip64 ? 0xffffffff : f.csize));
	ASSERT((f.gpflag & 0x8) == 0 || f.usize == 0);
	zip->Put32le((dword)(f.zip64 ? 0xffffffff : f.usize));
	zip->Put16le((word)strlen(f.path));
	zip->Put16le(f.zip64 ? 28 : 0); // ZIP64 extra field length
	zip->Put(f.path);
	if(f.zip64){
		zip->Put16le(1); // ZIP64
		zip->Put16le(24); // ZIP64 data to read : usize, csize, offset
		zip->Put64le(0);
		zip->Put64le(0);
		zip->Put64le(done);
	}
	done += 5*2 + 5*4 + f.path.GetCount() + (f.zip64 ? 28 : 0);
}

static bool IsPlainASCII(const String &s){
	for(int i=0;i<s.GetCount();i++) if((s[i]<32) || (s[i]>=127)) return false;
	return true;
}

void Zip::BeginFile(const char *path, Time tm, bool deflate, bool zip64)
{
	ASSERT(!IsFileOpened());
	if(deflate) {
		pipeZLib.Create();
		pipeZLib->WhenOut = THISBACK(PutCompressed);
		pipeZLib->GZip(false).CRC().NoHeader().Compress();
	}
	else {
		crc32.Clear();
		uncompressed = true;
	}
	
	if(done>=0xffffffffULL) zip64 = true; // must switch to Zip64 due to large archive size
	
	File& f = file.Add();
	f.version = zip64 ? 45 : 20;
	f.gpflag = IsPlainASCII(path) ? 0x8 : 0x8 | 1<<11; // Added UTF-8 marker, i.e.: " | 1<<11"; only for files with non-ASCII characters
	f.method = deflate ? 8 : 0;
	f.zip64 = zip64;
	f.crc = 0;
	f.csize = 0;
	f.usize = 0;
	FileHeader(path, tm);
	if (zip->IsError()) WhenError();
}

void Zip::BeginFile(OutFilterStream& oz, const char *path, Time tm, bool deflate, bool zip64)
{
	BeginFile(path, tm, deflate, zip64);
	oz.Filter = THISBACK(Put64);
	oz.End = THISBACK(EndFile);
}

void Zip::Put(const void *ptr, int size)
{
	ASSERT(IsFileOpened());
	File& f = file.Top();
	if(f.method == 0) {
		PutCompressed(ptr, size);
		crc32.Put(ptr, size);
	}
	else
		pipeZLib->Put(ptr, size);
	f.usize += size;
}

void Zip::Put64(const void *ptr, int64 size)
{
	ASSERT(IsFileOpened());
	File& f = file.Top();
	
	int64 done = 0;
	while(done < size){
		int chunk = (int)min(1024*1024LL, size - done);
		if(f.method == 0) {
			PutCompressed((byte *)ptr + done, chunk);
			crc32.Put((byte *)ptr + done, chunk);
		}
		else
			pipeZLib->Put((byte *)ptr + done, chunk);
		
		done += chunk;
	}
	f.usize += size;
}

void Zip::EndFile()
{
	if(!IsFileOpened())
		return;
	File& f = file.Top();
	ASSERT(f.gpflag & 0x8);
	
	if(f.method == 0){
		zip->Put32le(f.crc = crc32);
		done += 4;
	}
	else {
		pipeZLib->End();
		zip->Put32le(f.crc = pipeZLib->GetCRC());
		done += 4;
	}

	if(f.zip64){
		zip->Put64le(f.csize);
		zip->Put64le(f.usize);
		done += 16;
	}
	else{
		zip->Put32le((dword)f.csize);
		zip->Put32le((dword)f.usize);
		done += 8;
	}
	
	pipeZLib.Clear();
	uncompressed = false;
	if(zip->IsError()) WhenError();
}

void Zip::PutCompressed(const void *ptr, int size)
{
	ASSERT(IsFileOpened());
	zip->Put(ptr, size);
	if (zip->IsError()) WhenError();
	done += size;
	file.Top().csize += size;
}

void Zip::WriteFile(const void *ptr, int size, const char *path, Gate<int, int> progress, Time tm, bool deflate)
{
	ASSERT(!IsFileOpened());

	BeginFile(path, tm, deflate);
	int done = 0;
	while(done < size) {
		if(progress(done, size))
			return;
		int chunk = min(size - done, 65536);
		Put((byte *)ptr + done, chunk);
		if(zip->IsError()) {
			WhenError();
			return;
		}
		done += chunk;
	}
	EndFile();
	return;
}

void Zip::WriteFile(const String& s, const char *path, Gate<int, int> progress, Time tm, bool deflate)
{
	WriteFile(~s, s.GetCount(), path, progress, tm, deflate);
}

void Zip::Create(Stream& out)
{
	Finish();
	done = 0;
	zip = &out;
}

void Zip::Finish()
{
	if(!zip)
		return;
	qword off = done;
	qword rof = 0;
	
	int version = ((file.GetCount() >= 0xffff) || (done >= 0xffffffffULL)) ? 45 : 20;
	
	// Update version info for Zip64 where required:
	for(int i = 0; i < file.GetCount(); i++) if(file[i].zip64 || (file[i].csize>=0xffffffffULL) || (file[i].usize>=0xffffffffULL)) file[i].version = version = 45;
	
	for(int i = 0; i < file.GetCount(); i++) {
		File& f = file[i];
		bool zip64record = f.zip64 || (rof>=0xffffffffULL) || (f.csize>=0xffffffffULL) || (f.usize>=0xffffffffULL);
	
		zip->Put32le(0x02014b50);
		zip->Put16le(version); // version made by
		zip->Put16le(f.version); // version required to extract
		zip->Put16le(f.gpflag);  // general purpose bit flag
		zip->Put16le(f.method);
		zip->Put32le(f.time);
		zip->Put32le(f.crc);
		zip->Put32le((dword)(zip64record ? 0xffffffff : f.csize));
		zip->Put32le((dword)(zip64record ? 0xffffffff : f.usize));
		zip->Put16le(f.path.GetCount());
		zip->Put16le(zip64record ? 28 : 0); // extra field length              2 bytes
		zip->Put16le(0); // file comment length             2 bytes
		zip->Put16le(0); // disk number start               2 bytes
		zip->Put16le(0); // internal file attributes        2 bytes
		zip->Put32le(0); // external file attributes        4 bytes
		zip->Put32le((dword)(zip64record ? 0xffffffff : rof)); // relative offset of local header 4 bytes
		zip->Put(f.path);
		// ZIP64 additions:
		if(zip64record){
			zip->Put16le(1); // ZIP64 extra field : header ID
			zip->Put16le(24); // ZIP64 extra field : bytes to follow
			zip->Put64le(f.usize); // ZIP64 extra field : uncomp size
			zip->Put64le(f.csize); // ZIP64 extra field : comp size
			zip->Put64le(rof); // ZIP64 extra field : relative offset of local header
		}
		done = done + 7 * 4 + 9 * 2 + f.path.GetCount() + (zip64record ? 28 : 0);
		rof = rof + 5 * 2 + 5 * 4 + f.csize + f.path.GetCount() + (f.gpflag & 0x8 ? (f.zip64 ? 20 : 12) : 0) + (f.zip64 ? 28 : 0);
	}
	
	bool zip64 = version==45;
	
	if(zip64){
		zip->Put32le(0x06064b50); // ZIP64 end of central directory record
		zip->Put64le(44); // ZIP64 end of central directory record : the rest of the record after this field
		zip->Put16le(version); // ZIP64 end of central directory record : version made by
		zip->Put16le(version); // ZIP64 end of central directory record : version required to extract
		zip->Put32le(0); // ZIP64 end of central directory record : disk number
		zip->Put32le(0); // ZIP64 end of central directory record : number of disk with the start of central directory
		zip->Put64le(file.GetCount()); // ZIP64 end of central directory record : number of directory entries on this disk
		zip->Put64le(file.GetCount()); // ZIP64 end of central directory record : number of directory entries (total)
		zip->Put64le((qword)(done - off)); // size of the central directory
		zip->Put64le(off); // offset of the central directory
		
		zip->Put32le(0x07064b50); // ZIP64 end of central directory locator
		zip->Put32le(0); // ZIP64 end of central directory locator : number of the disk with the start of the zip64 end of central directory
		zip->Put64le(done); // ZIP64 end of central directory locator : relative offset of the zip64 end of central directory record
		zip->Put32le(1); // ZIP64 end of central directory locator : total number of disks
	}
	
	zip->Put32le(0x06054b50); // end of central directory record
	zip->Put16le(0);  // number of this disk
	zip->Put16le(0);  // number of the disk with the start of the central directory
	zip->Put16le((word)(zip64 ? 0xffff : file.GetCount())); // total number of entries in the central directory on this disk
	zip->Put16le((word)(zip64 ? 0xffff : file.GetCount())); // total number of entries in the central directory
	zip->Put32le((dword)(zip64 ? 0xffffffff : (done - off))); // size of the central directory
	zip->Put32le((dword)(zip64 ? 0xffffffff : off)); //offset of start of central directory with respect to the starting disk number
	zip->Put16le(0);
	if (zip->IsError()) WhenError();
	zip = NULL;
}

Zip::Zip()
{
	done = 0;
	zip = NULL;
	uncompressed = false;
}

Zip::Zip(Stream& out)
{
	done = 0;
	zip = NULL;
	uncompressed = false;
	Create(out);
}

Zip::~Zip()
{
	Finish();
}

bool FileZip::Create(const char *name)
{
	bool b = zip.Open(name);
	Zip::Create(zip); // if there is error, we still need to have to dump data
	return b;
}

bool FileZip::Finish()
{
	if(zip.IsOpen()) {
		Zip::Finish();
		zip.Close();
		return !zip.IsError();
	}
	return false;
}

void StringZip::Create()
{
	Zip::Create(zip);
}

String StringZip::Finish()
{
	Zip::Finish();
	return zip.GetResult();
}

}
