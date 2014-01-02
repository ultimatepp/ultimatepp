#include "zip.h"

NAMESPACE_UPP

void UnZip::ReadHeader()
{
	if(zip->Get32le() != 0x04034b50) {
		eof = true;
		return;
	}
	if(zip->Get16le() > 20) {
		SetError();
		return;
	}
	bit = zip->Get16le();
	method = zip->Get16le();

	dword dt = zip->Get32le();

	time.year = int16(((dt >> 25) & 0x7f) + 1980);
	time.month = byte((dt >> 21) & 0x0f);
	time.day = byte((dt >> 16) & 0x1f);
	time.hour = byte((dt >> 11) & 0x1f);
	time.minute = byte((dt >> 5) & 0x3f);
	time.second = byte((dt << 1) & 0x3e);

	crc32 = zip->Get32le();
	csize = zip->Get32le();
	usize = zip->Get32le();

	dword filelen = zip->Get16le();
	dword extralen = zip->Get16le();
	path = zip->Get(filelen);
	zip->SeekCur(extralen);

	done += 5 * 2 + 5 * 4;
}

void UnZip::SkipFile()
{
	zip->SeekCur(csize);
	done += csize;
	ReadHeader();
}

int64 zPress(Stream& out, Stream& in, int64 size, Gate2<int64, int64> progress, bool gzip, bool compress, dword *crc);

bool UnZip::ReadFile(Stream& out, Gate2<int, int> progress)
{
	dword crc;
	int l;
	if(method == 0) {
		Buffer<byte> temp(65536);
		int loaded;
		int count = csize;
		Crc32Stream crc32;
		while(count > 0 && (loaded = zip->Get(temp, (int)min<int64>(count, 65536))) > 0) {
			out.Put(temp, loaded);
			crc32.Put(temp, loaded);
			count -= loaded;
		}
		if(count > 0) {
			SetError();
			return false;
		}
		l = csize;
		crc = crc32;
	}
	else
	if(method == 8)
		l = (int)zPress(out, *zip, csize, AsGate64(progress), true, false, &crc);
	else {
		SetError();
		return false;
	}
	if(crc != crc32 || l != usize) {
		SetError();
		return false;
	}
	done += csize;
	ReadHeader();
	return true;
}

String UnZip::ReadFile(Gate2<int, int> progress)
{
	StringStream ss;
	return ReadFile(ss, progress) ? ss.GetResult() : String::GetVoid();
}

void UnZip::Create(Stream& _zip)
{
	eof = error = false;
	zip = &_zip;
	done = 0;
	ReadHeader();
}

bool UnZip::IsEof() const
{
	return eof || !zip || error || zip->IsEof();
}

bool UnZip::IsFolder() const
{
	return *path.Last() == '/';
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

bool FileUnZip::Create(const char *name)
{
	if(!zip.Open(name))
		return false;
	UnZip::Create(zip);
	return true;
}

void MemUnZip::Create(const void *ptr, int count)
{
	zip.Create(ptr, count);
	UnZip::Create(zip);
}

void StringUnZip::Create(const String& s)
{
	zip.Open(s);
	UnZip::Create(zip);
}

END_UPP_NAMESPACE
