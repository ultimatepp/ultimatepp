#include "Designers.h"

bool  FileIsBinary(const char *path)
{
	FileIn in(path);
	if(in) {
		String data = in.Get(150000);
		if(*data.Last() == '\r')
			data.Cat(in.Get());
		const char *end = data.End();
		const char *s = data;
		while(s < end) {
			int c = (byte)*s;
			if(c < 32) {
				if(c == '\r') {
					if(s[1] != '\n')
						return true;
					s += 2;
				}
				else
				if(c == '\t' || c == '\n')
					s++;
				else
					return true;
			}
			else
				s++;
		}
	}
	return false;
}

int64 FileHexView::Key() const
{
	return blk;
}

int FileHexView::Make(String& object) const
{
	FileIn& in = const_cast<FileIn&>(file);
	in.Seek(blk * FileHexView::BLKSIZE);
	object = in.Get(FileHexView::BLKSIZE);
	return 1;
}

int FileHexView::Byte(int64 addr)
{
	blk = addr >> BLKSHIFT;
	String h = cache.Get(*this);
	cache.Shrink(10); // cache only 10 blocks (~150 KB)
	int ii = addr & (BLKSIZE - 1);
	return ii < h.GetCount() ? (byte)h[ii] : 0;
}

void FileHexView::Open(const char *path)
{
	filename = path;
	file.Open(path);
	SetTotal(file.GetSize());
}

FileHexView::FileHexView()
{
	InfoMode(2);
	LoadFromGlobal(*this, "FileHexView");
}

FileHexView::~FileHexView()
{
	StoreToGlobal(*this, "FileHexView");
}

INITIALIZER(HexView)
{
	RegisterGlobalConfig("FileHexView");
}