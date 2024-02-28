#include "zip.h"

namespace Upp {

void UnZip::ReadDir()
{
	error = true;
	
	file.Clear();
	current = 0;

	int64 entries = -1;
	int64 offset;

	int64 zipsize = zip->GetSize();
	int64 pos = zipsize - 1; //22;
	zip->Seek(max((int64)0, zip->GetSize() - 4000)); // Precache end of zip
	zip->Get();
	
	int64 zip64eocdl = 0;
	while(pos >= max((int64)0, zip->GetSize() - 65536)) {
		zip->ClearError();
		zip->Seek(pos);
		entries = -1; // ensure error return when header fails
		if(zip->Get32le() == 0x06054b50) {
			zip->Get16le();  // number of this disk
			zip->Get16le();  // number of the disk with the start of the central directory
			int h = (word)zip->Get16le(); // total number of entries in the central directory on this disk
			entries = (word)zip->Get16le(); // total number of entries in the central directory
			if(h != entries) // Multiple disks not supported
				return;
			zip->Get32le(); // size of the central directory
			offset = (dword)zip->Get32le(); //offset of start of central directory with respect to the starting disk number
			zip64eocdl = pos - 20; // offset of zip64 end of central directory locator
			int commentlen = zip->Get16le();
			if(zip->GetPos() + commentlen == zipsize)
				break;
		}
		pos--;
	}
	if(entries < 0)
		return;

	zip->Seek(zip64eocdl);
	if(zip->Get32le() == 0x07064b50) {
		zip->Get32le(); // ZIP64 end of central directory locator : number of the disk with the start of the zip64 end of central directory
		int64 zip64eocdr = zip->Get64le(); // ZIP64 end of central directory locator : relative offset of the zip64 end of central directory record
		zip->Seek(zip64eocdr);
		if(zip->Get32le() == 0x06064b50) {
			zip->Get64le(); // ZIP64 end of central directory record : the rest of the record after this field
			zip->Get16le(); // ZIP64 end of central directory record : version made by
			zip->Get16le(); // ZIP64 end of central directory record : version required to extract
			zip->Get32le(); // ZIP64 end of central directory record : disk number
			zip->Get32le(); // ZIP64 end of central directory record : number of disk with the start of central directory
			int64 de = zip->Get64le(); // ZIP64 end of central directory record : number of directory entries on this disk
			int64 te = zip->Get64le(); // ZIP64 end of central directory record : number of directory entries (total)
			if(de != te) // Multiple disks not supported
				return;
			if(entries == 0xffff) entries = te; // Use ZIP64 entry counter
			zip->Get64le(); // size of the central directory
			if(offset == 0xffffffff) offset = zip->Get64le(); // ZIP64 offset of the central directory
		}
	}

	zip->Seek(offset);
	for(int i = 0; i < entries; i++) {
		if(zip->Get32le() != 0x02014b50 && zip->IsEof())
			return;
		File& f = file.Add();
		zip->Get16le();
		zip->Get16le();
		f.bit = zip->Get16le();  // general purpose bit flag
		f.method = zip->Get16le();
		f.time = zip->Get32le();
		f.crc = zip->Get32le();
		f.csize = (dword)zip->Get32le();
		f.usize = (dword)zip->Get32le();
		int fnlen = zip->Get16le();
		int extralen = zip->Get16le(); // extra field length              2 bytes
		int commentlen = zip->Get16le(); // file comment length             2 bytes
		if(zip->Get16le() != 0) // disk number start               2 bytes
			return; // Multiple disks not supported
		zip->Get16le(); // internal file attributes
		zip->Get32le(); // external file attributes
		f.offset = zip->Get32le();
		f.path = zip->Get(fnlen);
		int64 skipto = zip->GetPos() + extralen + commentlen;
		if(extralen>=4 && zip->Get16le()==1){ // ZIP64 extra field : header ID
			int bytes = zip->Get16le(); // ZIP64 extra field : bytes to follow
			if(bytes>=8) f.usize = zip->Get64le(); // ZIP64 extra field : uncomp size
			if(bytes>=16) f.csize = zip->Get64le(); // ZIP64 extra field : comp size
			if(bytes>=24) f.offset = zip->Get64le(); // ZIP64 extra field : relative offset of local header
		}
		
		zip->Seek(skipto);
		if(zip->IsEof() || zip->IsError())
			return;
	}
	
	error = false;
}

Time UnZip::GetZipTime(dword dt)
{
	Time time;
	time.year = int16(((dt >> 25) & 0x7f) + 1980);
	time.month = byte((dt >> 21) & 0x0f);
	time.day = byte((dt >> 16) & 0x1f);
	time.hour = byte((dt >> 11) & 0x1f);
	time.minute = byte((dt >> 5) & 0x3f);
	time.second = byte((dt << 1) & 0x3e);
	return time;
}

int64 zPress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress, bool gzip,
             bool compress, dword *crc, bool hdr);

bool UnZip::ReadFile(Stream& out, Gate<int, int> progress)
{
	if(error)
		return false;
	if(IsFolder()) {
		current++;
		return true;
	}
	error = true;
	if(current >= file.GetCount())
		return false;
	const File& f = file[current];
	zip->Seek(f.offset);
	if(zip->Get32le() != 0x04034b50)
		return false;
	zip->Get16le();
	zip->Get16le(); // Skip header, use info from centrall dir
	zip->Get16le();
	zip->Get32le();
	zip->Get32le();
	zip->Get32le();
	zip->Get32le();
	dword filelen = zip->Get16le();
	dword extralen = zip->Get16le();
	zip->SeekCur(filelen + extralen);
	dword crc;
	qword l;
	if(f.method == 0) {
		Buffer<byte> temp(65536);
		int loaded;
		int64 count = f.csize;
		Crc32Stream crc32;
		while(count > 0 && (loaded = zip->Get(temp, (int)min<int64>(count, 65536))) > 0) {
			out.Put(temp, loaded);
			crc32.Put(temp, loaded);
			count -= loaded;
		}
		if(count > 0)
			return false;
		l = f.csize;
		crc = crc32;
	}
	else
	if(f.method == 8)
		l = zPress(out, *zip, f.csize, AsGate64(progress), false, false, &crc, false);
	else
		return false;
	if(crc != f.crc || l != f.usize)
		return false;
	current++;
	error = false;
	return true;
}

String UnZip::ReadFile(Gate<int, int> progress)
{
	StringStream ss;
	return ReadFile(ss, progress) ? ss.GetResult() : String::GetVoid();
}

String UnZip::ReadFile(const char *path, Gate<int, int> progress)
{
    for(int i = 0; i < file.GetCount(); i++)
        if(file[i].path == path) {
            Seek(i);
            return ReadFile(progress);
        }

    return String::GetVoid();
}

void UnZip::Create(Stream& _zip)
{
	zip = &_zip;
	ReadDir();
}

UnZip::UnZip(Stream& in)
{
	Create(in);
}

UnZip::UnZip()
{
	error = true;
	zip = NULL;
}

UnZip::~UnZip() {}

}
