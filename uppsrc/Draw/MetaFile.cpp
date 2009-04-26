#include "Draw.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

void WinMetaFile::Init() {
	hemf = NULL;
}

void WinMetaFile::Paint(Draw& w, const Rect& r) const {
	ChkP();
	if(hemf)
		PlayEnhMetaFile(w, hemf, r);
}

void WinMetaFile::Paint(Draw& w, int x, int y, int cx, int cy) const {
	Paint(w, RectC(x, y, cx, cy));
}

void WinMetaFile::ReadClipboard() {
	Clear();
	if(::IsClipboardFormatAvailable(CF_ENHMETAFILE) && ::OpenClipboard(NULL)) {
		HENHMETAFILE hemf = (HENHMETAFILE) ::GetClipboardData(CF_ENHMETAFILE);
		if(hemf) Attach(hemf);
		::CloseClipboard();
	}
}

void WinMetaFile::WriteClipboard() const {
	if(hemf && ::OpenClipboard(NULL)) {
		::EmptyClipboard();
		::SetClipboardData(CF_ENHMETAFILE, CopyEnhMetaFile(hemf, NULL));
		::CloseClipboard();
	}
}

void WinMetaFile::Clear() {
	if(hemf && !IsPicked())
		::DeleteEnhMetaFile(hemf);
	hemf = NULL;
}

void WinMetaFile::Pick(pick_ WinMetaFile& src) {
	hemf = src.hemf;
	size = src.size;
	src.hemf = (HENHMETAFILE)(intptr_t) 0xffffffff;
}

void WinMetaFile::Copy(const WinMetaFile& src) {
	hemf = ::CopyEnhMetaFile(src.hemf, NULL);
	size = src.size;
}

void WinMetaFile::Attach(HENHMETAFILE _hemf) {
	Clear();
	ENHMETAHEADER info;
	memset(&info, 0, sizeof(info));
	info.iType = EMR_HEADER;
	info.nSize = sizeof(info);
	info.dSignature = ENHMETA_SIGNATURE;
	if(_hemf && ::GetEnhMetaFileHeader(_hemf, sizeof(info), &info)
	   && info.rclFrame.left < info.rclFrame.right
	   && info.rclFrame.top < info.rclFrame.bottom) {
		size.cx = 600 * (info.rclFrame.right - info.rclFrame.left) / 2540;
		size.cy = 600 * (info.rclFrame.bottom - info.rclFrame.top) / 2540;
		hemf = _hemf;
	}
}

#pragma pack(push, 1)
struct PLACEABLE_METAFILEHEADER
{
    DWORD   key;
    WORD    hmf;
    short   left, top, right, bottom;
    WORD    inch;
    DWORD   reserved;
    WORD    checksum;
};
#pragma pack(pop)

bool WinMetaFile::Load(const char* path) {
	Clear();
	FileIn file(path);
	if(!file.Open(path) || file.GetSize() <= sizeof(ENHMETAHEADER))
		return false;

	int first = file.Get32le();
	file.Seek(0);

	HENHMETAFILE hemf;
	HMETAFILE hMF;
	Size sz(1000, 1000);

	if(first == 0x9AC6CDD7) {
		PLACEABLE_METAFILEHEADER mfh;
		file.Get(&mfh, 22);
		String bits = LoadStream(file);
		if((hMF = ::SetMetaFileBitsEx(bits.GetLength(), bits)) == NULL)
			return false;
		sz = Size(mfh.right - mfh.left, mfh.bottom - mfh.top);
	}
	else
	if((hemf = ::GetEnhMetaFile(path)) != NULL) {
		Attach(hemf);
		return true;
	}
	else
	if((LOWORD(first) == 1 || LOWORD(first) == 2) && HIWORD(first) >= sizeof(METAHEADER) / 2) {
		METAHEADER mh;
		if(!file.GetAll(&mh, sizeof(mh)))
			return false;
		if(mh.mtVersion != 0x100 && mh.mtVersion != 0x300)
			return false;
		if((hMF = ::GetMetaFile(path)) == NULL)
			return false;
	}
	else
		return false;

	dword len = ::GetMetaFileBitsEx(hMF, 0, NULL);
	Buffer<byte> bits(len);
	::GetMetaFileBitsEx(hMF, len, bits);
	Attach(::SetWinMetaFileBits(len, bits, NULL, NULL));
	::DeleteMetaFile(hMF);
	size = sz;
	return true;
}

void WinMetaFile::Serialize(Stream& s) {
	dword size = 0;
	if(s.IsStoring()) {
		if(hemf) {
			size = ::GetEnhMetaFileBits(hemf, 0, 0);
			s % size;
			Buffer<byte> buffer(size);
			::GetEnhMetaFileBits(hemf, size, buffer);
			s.SerializeRaw(buffer, size);
		}
		else
			s % size;
	}
	else {
		Clear();
		s % size;
		if(size) {
			Buffer<byte> buffer(size);
			s.SerializeRaw(buffer, size);
			HENHMETAFILE hemf = ::SetEnhMetaFileBits(size, buffer);
			Attach(hemf);
		}
	}
}

WinMetaFile::WinMetaFile(HENHMETAFILE hemf) {
	Init();
	Attach(hemf);
}

WinMetaFile::WinMetaFile(HENHMETAFILE hemf, Size sz) {
	Init();
	Attach(hemf);
	size = sz;
}

WinMetaFile::WinMetaFile(const char *file) {
	Init();
	Load(file);
}

bool WinMetaFileDraw::Create(HDC hdc, int cx, int cy, const char *app, const char *name, const char *file) {
	if(handle) Close();

	String s;
	if(app) s.Cat(app);
	s.Cat('\0');
	if(name) s.Cat(name);
	s.Cat('\0');

	bool del = false;
	if(!hdc) {
		hdc = ::GetWindowDC((HWND) NULL);
		del = true;
	}
	size = Size(iscale(cx, 2540, 600), iscale(cy, 2540, 600));

	Rect r = size;

	HDC mfdc = ::CreateEnhMetaFile(hdc, file, r, name || app ? (const char *)s : NULL);
	if(!mfdc)
		return false;
	Size px(max(1, ::GetDeviceCaps(mfdc, HORZRES)),  max(1, ::GetDeviceCaps(mfdc, VERTRES)));
	Size mm(max(1, ::GetDeviceCaps(mfdc, HORZSIZE)), max(1, ::GetDeviceCaps(mfdc, VERTSIZE)));
	Attach(mfdc);

	Init();

	pixels = false;

	::SetMapMode(handle, MM_ANISOTROPIC);
	::SetWindowOrgEx(handle, 0, 0, 0);
	::SetWindowExtEx(handle, 600, 600, 0);
	::SetViewportOrgEx(handle, 0, 0, 0);
	::SetViewportExtEx(handle, px.cx * 254 / (10 * mm.cx), px.cy * 254 / (10 * mm.cy), 0);
	::SelectClipRgn(mfdc, NULL);
	::IntersectClipRect(mfdc, 0, 0, cx, cy);

	if(del) {
		::ReleaseDC((HWND) NULL, hdc);
		hdc = NULL;
	}

	actual_offset = Point(0, 0);

	printer = false;
	pixels = false;
	actual_offset = Point(0, 0);
	device = -1;
	aborted = false;
	palette = false;
	backdraw = true;

	return true;
}

bool WinMetaFileDraw::Create(int cx, int cy, const char *app, const char *name, const char *file) {
	return Create(NULL, cx, cy, app, name, file);
}

WinMetaFile WinMetaFileDraw::Close() {
	HDC hdc = Detach();
	ASSERT(hdc);
	return WinMetaFile(CloseEnhMetaFile(hdc), size);
}

WinMetaFileDraw::~WinMetaFileDraw() {
	if(handle) Close();
}

WinMetaFileDraw::WinMetaFileDraw(HDC hdc, int cx, int cy, const char *app, const char *name, const char *file) {
	Create(hdc, cx, cy, app, name, file);
}

WinMetaFileDraw::WinMetaFileDraw(int cx, int cy, const char *app, const char *name, const char *file) {
	Create(cx, cy, app, name, file);
}

#endif
#endif

END_UPP_NAMESPACE
