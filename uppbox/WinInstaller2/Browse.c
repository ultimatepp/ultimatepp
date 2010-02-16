#include <shlobj.h>  //link to shell32.lib
#include <ole2.h>    //link to ole32.lib
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma once

BOOL BrowseFolder( HWND hwndOwner, LPTSTR lpszDir, LPCTSTR lpszTitle )
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	IMalloc* pMalloc;
	
	OleInitialize(NULL);
	
	if(FAILED(SHGetMalloc(&pMalloc)))
		return FALSE;
	
	bi.hwndOwner = hwndOwner;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = lpszDir;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_NEWDIALOGSTYLE; 
	bi.lpfn = NULL;
	bi.lParam = 0;
	
	pidl = SHBrowseForFolder(&bi);
	
	if(!pidl)
		return FALSE;
	
	// SHBrowseForFolder filled lpszDir with the name of the chosen item,
	// but it did not give us the full path to it. So, we need to get
	// that using SHGetPathFromIDList.
	
	if(!SHGetPathFromIDList(pidl, lpszDir))
		return FALSE;
	
	// SHBrowseForFolder itself allocated the memory that holds the directory name
	// (the pidl pointer). We must now de-allocate it ourselves using the shell task
	// allocator (IMalloc).
	//
	// The calling convention is slightly different between C and C++.
	// (Read chapter 20 in Petzold if you're not familiar with this)
	pMalloc->lpVtbl->Free(pMalloc,pidl);
	pMalloc->lpVtbl->Release(pMalloc);
	return TRUE;
}
