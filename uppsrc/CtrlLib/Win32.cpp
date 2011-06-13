#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>

#endif
#endif


NAMESPACE_UPP

#ifdef GUI_WIN

#ifndef PLATFORM_WINCE

#define LLOG(x)

FileSelector::FileSelector() {
	activetype = 0;
	readonly = rdonly = multi = false;
	asking = true;
}

FileSelector& FileSelector::Type(const char *name, const char *ext) {
	FileType& t = type.Add();
	t.name = ToSystemCharset(name);
	t.ext = ToSystemCharset(ext);
	return *this;
}

FileSelector& FileSelector::AllFilesType() {
	return Type(t_("All files"), "*.*");
}

void FileSelector::Serialize(Stream& s) {
	int version = 2;
	s / version;
	s / activetype % activedir;
	if(version < 2) {
		String dummy;
		s % dummy;
	}
}

String FileSelector::Get() const {
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

bool FileSelector::Execute(bool open, const char *dlgtitle) {
	String filter;
	for(int i = 0; i < type.GetCount(); i++) {
		filter.Cat(type[i].name);
		filter.Cat('\0');
		filter.Cat(type[i].ext);
		filter.Cat('\0');
	}
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	Ctrl *q = Ctrl::GetActiveWindow();
	if(q) ofn.hwndOwner = q->GetHWND();
	ofn.Flags = OFN_ENABLESIZING|OFN_ENABLEHOOK|OFN_EXPLORER;
	if(asking) ofn.Flags |= (open ? OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST : OFN_OVERWRITEPROMPT);
	if(!rdonly) ofn.Flags |= OFN_HIDEREADONLY;
	if(multi) ofn.Flags |= OFN_ALLOWMULTISELECT;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = activetype;
	ofn.lpstrInitialDir = activedir;
	ofn.lpfnHook = sCenterHook;
	int bufsize = ofn.nMaxFile = (multi ? 32000 : _MAX_PATH);
	Buffer<char> buffer(bufsize);
	*(ofn.lpstrFile = buffer) = 0;
	if(!filename.IsEmpty())
	{
		String out;
		for(int i = 0; i < filename.GetCount(); i++)
		{
			if(*ofn.lpstrInitialDir == 0 && FindFile().Search(AppendFileName(GetFileDirectory(filename[i]), "*")))
				ofn.lpstrInitialDir = GetFileDirectory(filename[i]);
			if(!open || FileExists(filename[i]))
			{
				String fn = GetFileName(filename[i]);
				if(!IsNull(fn))
				{
					if(multi && fn.Find(' ') >= 0)
						out << '\"' << ToSystemCharset(fn) << '\"';
					else
						out << ToSystemCharset(fn);
					out.Cat(0);
				}
			}
		}
		int l = min(out.GetLength(), bufsize - 1);
		memcpy(buffer, out, l + 1);
	}

	String title;
	if(dlgtitle)
		title = ToSystemCharset(dlgtitle);
	else if(open)
		title = ToSystemCharset(t_("Open.."));
	else
		title = ToSystemCharset(t_("Save as"));
	ofn.lpstrTitle = ~title;
	if(!defext.IsEmpty())
		ofn.lpstrDefExt = defext;
	bool res = !!(open ? GetOpenFileName : GetSaveFileName)(&ofn);
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		*buffer = 0;
		res = !!(open ? GetOpenFileName : GetSaveFileName)(&ofn);
	}
	if(!res && CommDlgExtendedError() == FNERR_INVALIDFILENAME)
	{
		ofn.lpstrInitialDir = "";
		res = !!(open ? GetOpenFileName : GetSaveFileName)(&ofn);
	}
	if(!res)
		return false;
	filename.Clear();
	activetype = ofn.nFilterIndex;
	if(multi) {
		const char *s = ofn.lpstrFile;
		activedir = s;
		s += strlen(s);
		if(s[1] == 0)
			filename.Add() = FromSystemCharset(activedir);
		else
			do
				filename.Add() = FromSystemCharset(AppendFileName(activedir, ++s));
			while((s += strlen(s))[1]);
	}
	else {
		filename.Add(FromSystemCharset(ofn.lpstrFile));
		activedir = GetFileDirectory(filename[0]);
	}
	readonly = ofn.Flags & OFN_READONLY ? TRUE : FALSE;
	return true;
}

#endif
#endif

END_UPP_NAMESPACE
