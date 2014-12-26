#include "Install.h"

void FileError()
{
	Panic("Input file is corrupted !");
}

char *outdir;

bool IsWow64()
{
	BOOL bIsWow64 = FALSE;
	
	typedef BOOL (APIENTRY *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	
	HMODULE module = GetModuleHandle("kernel32");
	const char funcName[] = "IsWow64Process";
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(module, funcName);
	
	if(fnIsWow64Process && !fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		return false;

	return bIsWow64;
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
				WinExec(exe + " -uninstall", SW_SHOWNORMAL);
				break;
			}

			ProgressMeter pi;
			pi.SetText("Installing files");
			HRSRC hrsrc = FindResource(NULL, LPCTSTR(1112), RT_RCDATA);
			if(!hrsrc) Error();
			outdir = h;
			LZMAExtract((const char *)LockResource(LoadResource(NULL, hrsrc)), SizeofResource(NULL, hrsrc),
			            h, pi);
			if (BST_CHECKED == SendDlgItemMessage(hwnd, 10 /*IDC_ASSOC*/, BM_GETCHECK, 0, 0)) {
				SetWinRegString(exe, "", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\theide.exe\\");
				SetWinRegString(h, "Path", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\theide.exe\\");
				SetWinRegString("TheIDE.upp", "", ".upp\\", HKEY_CLASSES_ROOT);
				SetWinRegString("open", "", "TheIDE.upp\\shell\\", HKEY_CLASSES_ROOT);
				SetWinRegString("\"" + exe + "\" \"%1\"", "", "TheIDE.upp\\shell\\open\\command\\", HKEY_CLASSES_ROOT);
			}
			pi.Destroy();
			FileMove(AppendFileName(h, IsWow64() ? "theide64.exe" : "theide32.exe"), exe);
			EndDialog(hwnd, 0);
			WinExec(exe, SW_SHOWNORMAL);
			break;
		}
		case 999: {
		    BOOL retVal;
			char Folder[256] = "C:\\";
			char FolderName[17] = "Select Directory";
			if(BrowseFolder( 0, Folder, FolderName)) {
				strcat(Folder, "\\upp");
				SetWindowText(GetDlgItem(hwnd, 101), Folder);
			}
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
	return 0;
}

void Error(char *sz)
{
	MessageBox(GetActiveWindow(), sz, "Ultimate++ installer", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
	if(outdir)
		DeleteFolderDeep(outdir);
	exit(1);
}


int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
	DialogBox(hinst, MAKEINTRESOURCE(99), NULL, (DLGPROC)Proc);
	return 0;
}
