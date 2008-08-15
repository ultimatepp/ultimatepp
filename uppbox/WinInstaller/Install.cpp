#include "Install.h"

void FileError()
{
	Panic("Input file is corrupted !");
}

String GetString(StringStream& ss)
{
	int len = ss.GetL();
	StringBuffer b(len);
	if(ss.Get(b, len) != len)
		FileError();
	return b;
}

void SaveFiles(StringStream& ss, const char *dir, String path, Gate2<int, int> progress)
{
	String p1 = AppendFileName(dir, path);
	RealizeDirectory(p1);
	progress(ss.GetPos(), ss.GetSize());
	for(;;)
		switch(ss.Get()) {
		case 0:
			SaveFiles(ss, dir, AppendFileName(path, GetString(ss)), progress);
			break;
		case 1: {
			String name = GetString(ss);
			String file = GetString(ss);
			SaveFile(AppendFileName(p1, name), file);
			break;
		}
		case 2:
			return;
		default:
			FileError();
		}
}

void SaveFiles(const String& archive, const char *dir)
{
	ProgressMeter pi;
	pi.SetText("Decompressing files");
	String data = BZ2Decompress(archive, pi);
	pi.SetText("Storing files");
	StringStream ss(data);
	SaveFiles(ss, dir, "", pi);
}

String GetRCDATA(int n)
{
	HRSRC hrsrc = FindResource(NULL, LPCTSTR(n), RT_RCDATA);
	if(!hrsrc) return Null;
	return String((const char *)LockResource(LoadResource(NULL, hrsrc)),
	              SizeofResource(NULL, hrsrc));
}

BOOL CALLBACK Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
	case WM_INITDIALOG: {
		Rect wr;
		SystemParametersInfo(SPI_GETWORKAREA, 0, wr, 0);
		Rect dr;
		GetWindowRect(hwnd, dr);
		Point p = wr.CenterPos(dr.Size());
		SetWindowPos(hwnd, 0, p.x, p.y, dr.Width(), dr.Height(),
		             SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOZORDER);
		SetWindowText(GetDlgItem(hwnd, 101), "C:\\upp");
		SendDlgItemMessage(hwnd, 101, EM_SETSEL, (WPARAM) 0, (LPARAM) -1);
		SetWindowText(hwnd, GetExeTitle());
 		break;
	}
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK: {
			char h[2048];
			GetWindowText(GetDlgItem(hwnd, 101), h, 2048);
			String exe = AppendFileName(h, "theide.exe");
			if(FileExists(exe)) {
				MessageBox(::GetActiveWindow(),
				           "Please uninstall previous version before installing the new one.",
			               "Ultimate++", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
				WinExec(exe + " -uninstall", 0);
				break;
			}
			SaveFiles(GetRCDATA(1112), h);
			EndDialog(hwnd, 0);
			WinExec(exe, 0);
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
	DialogBox(hinst, MAKEINTRESOURCE(99), NULL, (DLGPROC)Proc);
	return 0;
}
