#include "zip.h"

NAMESPACE_UPP

void Zip::WriteFolder(const char *path, Time tm)
{
	String p = UnixPath(path);
	if(*p.Last() != '/')
		p.Cat('/');
	WriteFile(~p, 0, p, false, tm);
}

int64 zPress(Stream& out, Stream& in, int64 size, Gate2<int64, int64> progress, bool gzip,
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
	zip->Put32le(f.csize);
	ASSERT((f.gpflag & 0x8) == 0 || f.usize == 0);
	zip->Put32le(f.usize);
	zip->Put16le(strlen(f.path));
	zip->Put16le(0);
	zip->Put(f.path);
	done += 5*2 + 5*4 + f.path.GetCount();
}

void Zip::BeginFile(const char *path, Time tm)
{
	ASSERT(!IsFileOpened());
	pipeZLib.Create();
	pipeZLib->WhenOut = THISBACK(PutCompressed);
	pipeZLib->GZip(false).CRC().NoHeader().Compress();
	File& f = file.Add();
	f.version = 21;
	f.gpflag = 0x8;
	f.method = 8;
	f.crc = 0;
	f.csize = 0;
	f.usize = 0;
	FileHeader(path, tm);
	if (zip->IsError()) WhenError(); 
}

void Zip::BeginFile(OutFilterStream& oz, const char *path, Time tm)
{
	BeginFile(path, tm);
	oz.Filter = THISBACK(Put);
	oz.End = THISBACK(EndFile);
}

void Zip::Put(const void *ptr, int size)
{
	ASSERT(IsFileOpened());
	File& f = file.Top();
	pipeZLib->Put(ptr, size);
	f.usize += size;
}

void Zip::EndFile()
{
	if(!IsFileOpened())
		return;
	File& f = file.Top();
	ASSERT(f.gpflag & 0x8);
	pipeZLib->End();
	zip->Put32le(f.crc = pipeZLib->GetCRC());
	zip->Put32le(f.csize);
	zip->Put32le(f.usize);
	done += 3*4;
	pipeZLib.Clear();
	if (zip->IsError()) WhenError(); 
}

void Zip::PutCompressed(const void *ptr, int size)
{
	ASSERT(IsFileOpened());
	zip->Put(ptr, size);
	if (zip->IsError()) WhenError(); 
	done += size;
	file.Top().csize += size;
}

void Zip::WriteFile(const void *ptr, int size, const char *path, Gate2<int, int> progress, Time tm)
{
	ASSERT(!IsFileOpened());
	File& f = file.Add();
	StringStream ss;
	MemReadStream ms(ptr, size);

	f.usize = size;
	zPress(ss, ms, size, AsGate64(progress), false, true, &f.crc, false);

	String data = ss.GetResult();
	const void *r = ~data;
	f.csize = data.GetLength();

	f.version = 20;
	f.gpflag = 0;
	if(data.GetLength() >= size) {
		r = ptr;
		f.csize = size;
		f.method = 0;
	}
	else
		f.method = 8;
	FileHeader(path, tm);
	zip->Put(r, f.csize);
	done += f.csize;
	if (zip->IsError()) WhenError(); 
}

void Zip::WriteFile(const String& s, const char *path, Gate2<int, int> progress, Time tm)
{
	WriteFile(~s, s.GetCount(), path, progress, tm);
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
	dword off = done;
	dword rof = 0;
	for(int i = 0; i < file.GetCount(); i++) {
		File& f = file[i];
		zip->Put32le(0x02014b50);
		zip->Put16le(20);
		zip->Put16le(f.version);
		zip->Put16le(f.gpflag);  // general purpose bit flag
		zip->Put16le(f.method);
		zip->Put32le(f.time);
		zip->Put32le(f.crc);
		zip->Put32le(f.csize);
		zip->Put32le(f.usize);
		zip->Put16le(f.path.GetCount());
		zip->Put16le(0); // extra field length              2 bytes
		zip->Put16le(0); // file comment length             2 bytes
		zip->Put16le(0); // disk number start               2 bytes
		zip->Put16le(0); // internal file attributes        2 bytes
		zip->Put32le(0); // external file attributes        4 bytes
		zip->Put32le(rof); // relative offset of local header 4 bytes
		rof+=5 * 2 + 5 * 4 + f.csize + f.path.GetCount() + (f.gpflag & 0x8 ? 3*4 : 0);
		zip->Put(f.path);
		done += 7 * 4 + 9 * 2 + f.path.GetCount();
	}
	zip->Put32le(0x06054b50);
	zip->Put16le(0);  // number of this disk
	zip->Put16le(0);  // number of the disk with the start of the central directory
	zip->Put16le(file.GetCount()); // total number of entries in the central directory on this disk
	zip->Put16le(file.GetCount()); // total number of entries in the central directory
	zip->Put32le(done - off); // size of the central directory
	zip->Put32le(off); //offset of start of central directory with respect to the starting disk number
	zip->Put16le(0);
	if (zip->IsError()) WhenError(); 
	zip = NULL;
}

Zip::Zip()
{
	done = 0;
	zip = NULL;
}

Zip::Zip(Stream& out)
{
	done = 0;
	zip = NULL;
	Create(out);
}

Zip::~Zip()
{
	Finish();
}

bool FileZip::Create(const char *name)
{
	if(!zip.Open(name))
		return false;
	Zip::Create(zip);
	return true;
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

END_UPP_NAMESPACE
