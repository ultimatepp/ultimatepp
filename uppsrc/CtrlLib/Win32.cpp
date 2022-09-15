#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>

#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY

#endif
#endif


namespace Upp {

#ifdef GUI_WIN

#ifndef PLATFORM_WINCE

#define LLOG(x)

FileSelNative::FileSelNative() {
	activetype = 0;
	readonly = rdonly = multi = false;
	asking = true;
}

FileSelNative& FileSelNative::Type(const char *name, const char *ext) {
	FileType& t = type.Add();
	t.name = name;
	t.ext = ext;
	return *this;
}

FileSelNative& FileSelNative::AllFilesType() {
	return Type(t_("All files"), "*.*");
}

void FileSelNative::Serialize(Stream& s) {
	int version = 2;
	s / version;
	s / activetype % activedir;
	if(version < 2) {
		String dummy;
		s % dummy;
	}
}

String FileSelNative::Get() const {
	return filename.GetCount() ? filename[0] : String::GetVoid();
}

#ifdef _DEBUG
static String Dump(Ctrl *ctrl)
{
	if(!ctrl)
		return "NULL";
	StringStream out;
	ctrl -> Dump(out);
	return out;
}
#endif

Rect MonitorRectForHWND(HWND hwnd);

static UINT_PTR CALLBACK sCenterHook(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LLOG("msg = " << (int)msg << ", wParam = " << Format("%08x", (int)wParam)
	    << ", lParam = " << Format("%08x", (int)lParam)
		<< ", focus = " << Dump(Ctrl::GetFocusCtrl()));

	if(msg == WM_NOTIFY && ((LPNMHDR)lParam) -> code == CDN_INITDONE) {
		//&& !(GetWindowLong(hdlg, GWL_STYLE) & WS_CHILD)) {
		while(GetWindowLong(hdlg, GWL_STYLE) & WS_CHILD) {
			HWND p = GetParent(hdlg);
			if(!p) break;
			hdlg = p;
		}
		Rect dr, pr;
		::GetWindowRect(hdlg, dr);
		Rect wa = Ctrl::GetPrimaryWorkArea();
		if(HWND owner = ::GetWindow(hdlg, GW_OWNER)) {
			::GetWindowRect(owner, pr);
			wa = MonitorRectForHWND(owner);
		}
		else
			pr = wa;
		pr.Intersect(wa);
		Point p = pr.CenterPos(dr.Size());
		::SetWindowPos(hdlg, NULL, p.x, p.y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//		::MoveWindow(hdlg, p.x, p.y, dr.Width(), dr.Height(), false);
	}
	return 0;
}

#ifndef OFN_ENABLESIZING
#define OFN_ENABLESIZING             0x00800000
#endif

INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg==BFFM_INITIALIZED) {
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		SendMessage(hwnd, BFFM_SETEXPANDED, TRUE, pData);
	}
	return 0;
}

bool FileSelNative::ExecuteSelectDir(const char *title)
{
	Vector<Vector<char16>> s16;
	auto W32 = [&](const String& s) -> char16* {
		auto& h = s16.Add();
		h = ToSystemCharsetW(s);
		return h;
	};

	String ret;

	BROWSEINFOW br;
	memset(&br, 0, sizeof(BROWSEINFO));
	Ctrl *q = Ctrl::GetActiveWindow();
	if(q) br.hwndOwner = q->GetHWND();
	br.lpfn = BrowseCallbackProc;
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	br.lpszTitle = W32(title);
	br.lParam = (LPARAM)W32(activedir);

	LPITEMIDLIST pidl = NULL;
	if((pidl = SHBrowseForFolderW(&br)) != NULL) {
		char16 buffer[MAX_PATH];
		if(SHGetPathFromIDListW(pidl, buffer))
			filename << FromSystemCharsetW(buffer);
		else
			filename << Null;
        IMalloc *pMalloc;
        if(SHGetMalloc(&pMalloc) == NOERROR) {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
		return true;
	}

	return false;
}

bool FileSelNative::Execute(bool open, const char *dlgtitle) {
	Vector<Vector<char16>> s16;
	auto W32 = [&](const String& s) -> char16* {
		auto& h = s16.Add();
		h = ToSystemCharsetW(s);
		return h;
	};
	String filter;
	for(int i = 0; i < type.GetCount(); i++) {
		filter.Cat(type[i].name);
		filter.Cat('\0');
		filter.Cat(type[i].ext);
		filter.Cat('\0');
	}
	OPENFILENAMEW ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	Ctrl *q = Ctrl::GetActiveWindow();
	if(q) ofn.hwndOwner = q->GetHWND();
	ofn.Flags = OFN_ENABLESIZING|OFN_ENABLEHOOK|OFN_EXPLORER;
	if(asking) ofn.Flags |= (open ? OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST : OFN_OVERWRITEPROMPT);
	if(!rdonly) ofn.Flags |= OFN_HIDEREADONLY;
	if(multi) ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.lpstrFilter = W32(filter);
	ofn.nFilterIndex = activetype;
	ofn.lpstrInitialDir = W32(activedir);
	ofn.lpfnHook = sCenterHook;
	int bufsize = ofn.nMaxFile = (multi ? 32000 : _MAX_PATH);
	Buffer<char16> buffer(bufsize, 0);
	if(filename.GetCount()) {
		if(*ofn.lpstrInitialDir == 0 && FindFile().Search(AppendFileName(GetFileDirectory(filename[0]), "*")))
			ofn.lpstrInitialDir = W32(GetFileDirectory(filename[0]));
		if(!open || FileExists(filename[0])) {
			String out;
			String fn = GetFileName(filename[0]);
			if(multi && fn.Find(' ') >= 0)
				out << String() << '\"' << fn << '\"';
			else
				out << fn;
			Vector<char16> w32 = ToSystemCharsetW(out);
			memcpy(buffer, w32, min(bufsize - 1, int(w32.GetCount() * sizeof(wchar))));
		}
	}
	ofn.lpstrFile = buffer;

	if(dlgtitle)
		ofn.lpstrTitle = W32(dlgtitle);
	else if(open)
		ofn.lpstrTitle = W32(t_("Open.."));
	else
		ofn.lpstrTitle = W32(t_("Save as"));
	if(!defext.IsEmpty())
		ofn.lpstrDefExt = W32(defext);
	bool res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		*buffer = 0;
		res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	}
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		ofn.lpstrInitialDir = W32("");
		res = !!(open ? GetOpenFileNameW : GetSaveFileNameW)(&ofn);
	}
	if(!res)
		return false;
	filename.Clear();
	activetype = ofn.nFilterIndex;
	if(multi) {
		const char16 *s = ofn.lpstrFile;
		activedir = FromSystemCharsetW(s);
		s += strlen16(s);
		if(s[1] == 0)
			filename.Add() = activedir;
		else
			do
				filename.Add() = AppendFileName(activedir, FromSystemCharsetW(++s));
			while((s += strlen16(s))[1]);
	}
	else {
		filename.Add(FromSystemCharsetW(ofn.lpstrFile));
		activedir = GetFileDirectory(filename[0]);
	}
	readonly = ofn.Flags & OFN_READONLY ? TRUE : FALSE;
	return true;
}

#endif
#endif

}
