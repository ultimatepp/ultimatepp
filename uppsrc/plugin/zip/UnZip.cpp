#include "zip.h"

NAMESPACE_UPP

void UnZip::ReadDir()
{
	error = true;
	
	current = 0;

	int entries = -1;
	int offset;

	int64 zipsize = zip->GetSize();
	int64 pos = zipsize - 1; //22;
	zip->Seek(max((int64)0, zip->GetSize() - 4000)); // Precache end of zip
	zip->Get();
	while(pos >= max((int64)0, zip->GetSize() - 65536)) {
		zip->ClearError();
		zip->Seek(pos);
		if(zip->Get32le() == 0x06054b50) {
			zip->Get16le();  // number of this disk
			zip->Get16le();  // number of the disk with the start of the central directory
			int h = zip->Get16le(); // total number of entries in the central directory on this disk
			entries = zip->Get16le(); // total number of entries in the central directory
			if(h != entries) // Multiple disks not supported
				return;
			zip->Get32le(); // size of the central directory
			offset = zip->Get32le(); //offset of start of central directory with respect to the starting disk number
			int commentlen = zip->Get16le();
			if(zip->GetPos() + commentlen == zipsize)
				break;			
		}
		pos--;
	}
	if(entries < 0)
		return;

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
		f.csize = zip->Get32le();
		f.usize = zip->Get32le();
		int fnlen = zip->Get16le();
		int extralen = zip->Get16le(); // extra field length              2 bytes
		int commentlen = zip->Get16le(); // file comment length             2 bytes
		if(zip->Get16le() != 0) // disk number start               2 bytes
			return; // Multiple disks not supported
		zip->Get16le(); // internal file attributes
		zip->Get32le(); // external file attributes
		f.offset = zip->Get32le();
		f.path = zip->Get(fnlen);
		zip->SeekCur(extralen + commentlen);
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

int64 zPress(Stream& out, Stream& in, int64 size, Gate2<int64, int64> progress, bool gzip,
             bool compress, dword *crc, bool hdr);

bool UnZip::ReadFile(Stream& out, Gate2<int, int> progress)
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
	dword l;
	if(f.method == 0) {
		Buffer<byte> temp(65536);
		int loaded;
		int count = f.csize;
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
		l = (int)zPress(out, *zip, f.csize, AsGate64(progress), false, false, &crc, false);
	else
		return false;
	if(crc != f.crc || l != f.usize)
		return false;
	current++;
	error = false;
	return true;
}

String UnZip::ReadFile(Gate2<int, int> progress)
{
	StringStream ss;
	return ReadFile(ss, progress) ? ss.GetResult() : String::GetVoid();
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

END_UPP_NAMESPACE
