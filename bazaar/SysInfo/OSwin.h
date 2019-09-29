#ifndef _SysInfo_OSwin_h_
#define _SysInfo_OSwin_h_

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

#ifdef __cplusplus
extern "C" {
#endif

PGNSI Get_GetNativeSystemInfo();
PGPI Get_GetProductInfo();

#ifdef __cplusplus
}
#endif	

#endif
