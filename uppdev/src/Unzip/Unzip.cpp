#include "Unzip.h"

void WINAPI ReceiveDllMessage(unsigned long ucsize, unsigned long csiz, unsigned cfactor, unsigned mo, unsigned dy, unsigned yr, unsigned hh, unsigned mm, char c, LPSTR filename, LPSTR methbuf, unsigned long crc, char fCrypt) {
	TotalSize += ucsize;
}

int WINAPI password(LPSTR p, int n, LPCSTR m, LPCSTR name) {
	return 1;
}

int WINAPI DisplayBuf(LPSTR buf, unsigned long size) {
	return (int)(unsigned int) size;
}

int WINAPI DisplayProgress(LPCSTR buf, unsigned long size) {

	doneSize += size;
	UnzipPi.Set((int)doneSize, (int)TotalSize);
	return 0;
}

int WINAPI GetReplaceDlgRetVal(LPSTR filename) {
	return 1;
}

int Extract(const String& ZipFile, const String& OutDir, bool WithProgress) {
	int r;
	HANDLE hDCL = GlobalAlloc( GPTR, (DWORD)sizeof(DCL));
	LPDCL lpDCL = (LPDCL)GlobalLock(hDCL);
	HANDLE hUF = GlobalAlloc( GPTR, (DWORD)sizeof(USERFUNCTIONS));
	LPUSERFUNCTIONS lpUserFunctions = (LPUSERFUNCTIONS)GlobalLock(hUF);

	ProgressEnabled = WithProgress;
	lpDCL->ncflag = 0;
	lpDCL->fQuiet = 0;
	lpDCL->ntflag = 0;
	lpDCL->nzflag = 0;
	lpDCL->ndflag = 1;
	lpDCL->naflag = 0;
	lpDCL->nfflag = 0;
	lpDCL->noflag = 1;
	lpDCL->nZIflag = 0;
	lpDCL->C_flag = 0;
	lpDCL->ExtractOnlyNewer = 0;
	lpDCL->PromptToOverwrite = 0;
	lpDCL->SpaceToUnderscore = 0;
	lpDCL->fPrivilege = 1;
	lpDCL->lpszZipFN = (char*)~ZipFile;
	lpDCL->lpszExtractDir = (char*)~OutDir;

	lpUserFunctions->password = password;
	lpUserFunctions->print = DisplayBuf;
	lpUserFunctions->sound = NULL;
	lpUserFunctions->replace = GetReplaceDlgRetVal;
	lpUserFunctions->SendApplicationMessage = ReceiveDllMessage;
	lpUserFunctions->ServCallBk = DisplayProgress;

	ExternLib lib((char*)~String(GetExeDirFile("") + "unzip32.dll"));
	if(lib.IsOpen()) {
		UNZIP unZip = (UNZIP)lib.GetProc("Wiz_SingleEntryUnzip");
		UnzipPi.SetText("Installazione in corso");
		UnzipPi.Set(0, 100);
		doneSize = 0;
		TotalSize = 0;
		lpDCL->nvflag = 1; // listen
		r = unZip(0, NULL, 0, NULL, lpDCL, lpUserFunctions);
		lpDCL->nvflag = 0; // work
		r = unZip(0, NULL, 0, NULL, lpDCL, lpUserFunctions);
		UnzipPi.Close();
	}
	else r = 1000001;

  GlobalUnlock(hDCL);
  GlobalFree(hDCL);
  GlobalUnlock(hUF);
  GlobalFree(hUF);

	return r;
}
