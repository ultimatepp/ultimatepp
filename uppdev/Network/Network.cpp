#include <Core/Core.h>
#include <winnetwk.h>

#pragma comment (lib, "mpr")

using namespace Upp;

void EnumLocalNetwork(NETRESOURCE *lpnr)
{
	HANDLE hEnum;
	DWORD cEntries = -1, cbBuffer = 0x4000;
	if(::WNetOpenEnum(lpnr ? RESOURCE_GLOBALNET : RESOURCE_CONTEXT,
	                  RESOURCETYPE_DISK, 0, lpnr, &hEnum) == 0) {
		if ((lpnr = (LPNETRESOURCE)::GlobalAlloc(GPTR, cbBuffer)) != NULL) {
			while(::WNetEnumResource(hEnum, &cEntries, lpnr, &cbBuffer) == 0) {
				for(int ctr = 0; ctr < (int)cEntries; ctr++) {
//					if (lpnr[ctr].dwDisplayType == RESOURCEDISPLAYTYPE_NETWORK ||
//					    lpnr[ctr].dwDisplayType == RESOURCEDISPLAYTYPE_DOMAIN)
					LOG("----------");
					if(lpnr[ctr].lpLocalName) LOG(lpnr[ctr].lpLocalName);
					if(lpnr[ctr].lpComment) LOG("  comment: " << lpnr[ctr].lpComment);
					if(lpnr[ctr].lpRemoteName) LOG("  remote name: " << lpnr[ctr].lpRemoteName);
					if(lpnr[ctr].lpProvider) LOG("  provider: " << lpnr[ctr].lpProvider);
					DUMP(lpnr[ctr].dwScope);
					DUMP(lpnr[ctr].dwUsage);
					DUMP(lpnr[ctr].dwType);
					DUMP(lpnr[ctr].dwDisplayType);
					
//					LOG(lpnr[ctr].lpProvider);
					if(lpnr[ctr].dwUsage & RESOURCEUSAGE_CONTAINER) {
						LOGBEGIN();
						EnumLocalNetwork(&lpnr[ctr]);
						LOGEND();
					}
					else {
					/*
						LOG("Is a file share");
						LOG(lpnr[ctr].lpRemoteName);
						FindFile ff(String().Cat() << lpnr[ctr].lpRemoteName << "\\*.*");
						while(ff) {
							LOG("  " << ff.GetName());
							ff.Next();
						}
					*/
					}
				}
				cEntries = -1;
				cbBuffer = 0x4000;
			}
			::GlobalFree(lpnr);
		}
		::WNetCloseEnum(hEnum);
	}
}

CONSOLE_APP_MAIN
{
	EnumLocalNetwork(NULL);
}
