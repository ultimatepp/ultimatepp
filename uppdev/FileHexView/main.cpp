#include <CtrlLib/CtrlLib.h>
#include <HexView/HexView.h>

using namespace Upp;

class FileHexView : public HexView, private LRUCache<String, int64>::Maker {
public:
	virtual int Byte(int64 addr);

	virtual int64 Key() const;
	virtual int Make(String& object) const;

private:
	FileIn file;
	int64  blk;
	
	LRUCache<String, int64> cache;

	enum { BLKSHIFT = 14, BLKSIZE = 1 << BLKSHIFT };

public:
	void Open(const char *path);
};

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
	file.Open(path);
	SetTotal(file.GetSize());
}


bool  FileIsBinary(const char *path)
{
	FileIn in(path);
	if(in) {
		String data = in.Get(150000);
		data.Cat('\n');
		const char *end = data.End();
		int le = -1;
		const char *s = data;
		while(s < end) {
			int c = (byte)*s;
			if(c < 32) {
				if(c == '\r') {
					if(s[1] != '\n' || le == 0)
						return true;
					le = 1;
					s += 2;
				}
				else
				if(c == '\n') {
					if(le == 1)
						return true;
					s++;
				}
				else
				if(c == '\t')
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

GUI_APP_MAIN
{
/*	FindFile ff("c:/xxx/*.*");
	while(ff) {
		if(ff.IsFile())
			DLOG(ff.GetName() << " " << FileIsBinary(ff.GetPath()));
		ff.Next();
	}
	return;
*/
	FileHexView view;
//	view.Open("C:/Users/cxl/WinBase.pp");
	view.Open("N:/Exodus.Gods.and.Kings.2014.1080p.WEB-DL.DD5.1.H264-RARBG/Exodus.Gods.and.Kings.2014.1080p.WEB-DL.DD5.1.H264-RARBG.mkv");
	
	TopWindow win;
	win.Add(view.SizePos());
	
	win.Run();
}
