#ifdef _WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIEx.cpp
// Description: Windows Extended MAPI class 
//
// Copyright (C) 2005-2010, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for the community to benefit.
//
// Usage: see the CodeProject article at http://www.codeproject.com/internet/CMapiEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"
#include "MAPISink.h"

#ifdef _WIN32_WCE
#include <cemapi.h>
#pragma comment(lib, "cemapi.lib")
#pragma comment(lib, "pimstore.lib")
#endif


MAPIFunctions::MAPIFunctions() {
	dll.Load("mapi32.dll");
	CloseIMsgSession = (const void (*)(LPMSGSESS lpMsgSess))dll.GetFunction("CloseIMsgSession");
	FreePadrlist	 = (const void (*)(LPADRLIST lpAdrlist))dll.GetFunction("FreePadrlist");
	MAPIGetDefaultMalloc = (const LPMALLOC (*)(VOID))dll.GetFunction("MAPIGetDefaultMalloc");
	OpenIMsgSession = (const SCODE (*)(LPMALLOC lpMalloc,ULONG ulFlags,LPMSGSESS *lppMsgSess))dll.GetFunction("OpenIMsgSession");
	OpenIMsgOnIStg = (const SCODE (*)(LPMSGSESS lpMsgSess,LPALLOCATEBUFFER lpAllocateBuffer,LPALLOCATEMORE lpAllocateMore,LPFREEBUFFER lpFreeBuffer,LPMALLOC lpMalloc,LPVOID lpMapiSup,LPSTORAGE lpStg,MSGCALLRELEASE *lpfMsgCallRelease,ULONG ulCallerData,ULONG ulFlags,LPMESSAGE *lppMsg))dll.GetFunction("OpenIMsgOnIStg");
	PpropFindProp = (const LPSPropValue (*)(LPSPropValue lpPropArray,ULONG cValues,ULONG ulPropTag))dll.GetFunction("PpropFindProp");
}

MAPIFunctions &MF() {
	static MAPIFunctions data;
	return data;
}


INITBLOCK {
	MF();
	if (!MAPIEx::Init())
		MessageBox(::GetActiveWindow(),	"MAPI Error", "Impossible to initialize MAPI",
	           MB_ICONSTOP|MB_OK|MB_APPLMODAL);
}
	
EXITBLOCK {
	MAPIEx::Term();
}

void WINAPI MAPIEx::FreeProws(LPSRowSet pRows) {
	if(pRows) {
		for(ULONG i = 0; i < pRows->cRows; i++) 
			MAPIFreeBuffer(pRows->aRow[i].lpProps);
		MAPIFreeBuffer(pRows);
	}
}

/////////////////////////////////////////////////////////////
// MAPIEx

int MAPIEx::cm_bComInit = FALSE;
int MAPIEx::cm_nMAPICode = 0;

MAPIEx::MAPIEx() {
	m_pSession = NULL;
	m_pMsgStore = NULL;
	m_sink = 0;
}

MAPIEx::~MAPIEx() {
	Logout();
}

bool MAPIEx::Init(DWORD dwFlags, bool bInitAsService) {
	if(CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_OK) 
		cm_bComInit = true;
#ifdef _WIN32_WCE
	// flags are ignored on Windows CE
	if(MAPIInitialize(NULL) != S_OK) 
		return false;
	CPOOM::Init();
#else
	if(!dwFlags) 
		dwFlags = MAPI_NO_COINIT | MAPI_MULTITHREAD_NOTIFICATIONS;
	if(bInitAsService) 
		dwFlags |= MAPI_NT_SERVICE;

	MAPIINIT_0 MAPIInit = { MAPI_INIT_VERSION, dwFlags };
	if(MAPIInitialize(&MAPIInit) != S_OK) 
		return false;
#endif
	return true;
}

void MAPIEx::Term() {
	MAPIUninitialize();
#ifdef _WIN32_WCE
	CPOOM::Term();
#endif
	if(cm_bComInit) 
		CoUninitialize();
}

// dwFlags set to defaults but added as a param here for special cases (ie MAPI_NO_MAIL)
bool MAPIEx::Login(const String szProfileName, DWORD dwFlags, bool bInitAsService) {
	if(!dwFlags) 
		dwFlags = MAPI_EXTENDED | MAPI_USE_DEFAULT | MAPI_NEW_SESSION;
	if(bInitAsService) 
		dwFlags |= MAPI_EXPLICIT_PROFILE | MAPI_NT_SERVICE;
	return (MAPILogonEx(0, (LPTSTR)(szProfileName.Begin()), NULL, dwFlags, &m_pSession) == S_OK);
}

void MAPIEx::Logout() {
	if(m_sink) {
		if(m_pMsgStore) 
			m_pMsgStore->Unadvise(m_sink);
		m_sink = 0;
	}
	RELEASE(m_pMsgStore);
	RELEASE(m_pSession);
}

bool MAPIEx::CreateProfile(String szProfileName) {
	LPPROFADMIN	lpProfAdmin = NULL; 
	if(MAPIAdminProfiles(0, &lpProfAdmin) != S_OK) 
		return false;
	HRESULT hr = lpProfAdmin->CreateProfile((LPTSTR)(szProfileName.Begin()), NULL, 0, 0);
	RELEASE(lpProfAdmin);
	return (hr == S_OK);
}

bool MAPIEx::DeleteProfile(String szProfileName) {
	LPPROFADMIN	lpProfAdmin = NULL;
	if(MAPIAdminProfiles(0, &lpProfAdmin) != S_OK) 
		return false;
	HRESULT hr = lpProfAdmin->DeleteProfile((LPTSTR)(szProfileName.Begin()), 0);
	RELEASE(lpProfAdmin);
	return (hr == S_OK);
}

bool MAPIEx::CreateMessageService(LPCTSTR szProfileName, LPCTSTR szService, LPCTSTR szDisplayName) {
	bool bResult=FALSE;
	LPPROFADMIN	lpProfAdmin=NULL;
	if(MAPIAdminProfiles(0, &lpProfAdmin) == S_OK) {
		LPSERVICEADMIN lpMsgServiceAdmin=NULL;
		if(lpProfAdmin->AdminServices((LPTSTR)szProfileName, NULL, 0, MAPI_DIALOG, 
																&lpMsgServiceAdmin) == S_OK) {
			bResult = (lpMsgServiceAdmin->CreateMsgService((LPTSTR)szService, 
				(LPTSTR)szDisplayName, 0, SERVICE_UI_ALWAYS | SERVICE_UI_ALLOWED) == S_OK);
			RELEASE(lpMsgServiceAdmin);
		}
		RELEASE(lpProfAdmin);
	}
	return bResult;
}

String MAPIEx::GetProfileName() {
	String strProfileName;
#ifndef _WIN32_WCE
	if(!m_pSession) 
		return strProfileName;

	LPSRowSet pRows = NULL;
	const int nProperties = 2;
	SizedSPropTagArray(nProperties, Columns) = {nProperties,{PR_DISPLAY_NAME_A, PR_RESOURCE_TYPE}};

	IMAPITable*	pStatusTable;
	if(m_pSession->GetStatusTable(0, &pStatusTable) == S_OK) {
		if(pStatusTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
			while(pStatusTable->QueryRows(1, 0, &pRows) == S_OK) {
				if(pRows->cRows != 1) 
					FreeProws(pRows);
				else if(pRows->aRow[0].lpProps[1].Value.ul == MAPI_SUBSYSTEM) {
					strProfileName = GetValidString(pRows->aRow[0].lpProps[0]);
					FreeProws(pRows);
				} else {
					FreeProws(pRows);
					continue;
				}
				break;
			}
		}
		RELEASE(pStatusTable);
	}
#endif
	return FromSystemCharset(strProfileName);
}

String MAPIEx::GetProfileEmail() {
	String strProfileEmail;
#ifndef _WIN32_WCE
	if(!m_pSession) 
		return strProfileEmail;

	SBinary entryID;
	if(m_pSession->QueryIdentity(&entryID.cb, (LPENTRYID*)&entryID.lpb) == S_OK) {
		GetExEmail(entryID, strProfileEmail);
		return strProfileEmail;
	}
#endif
	return strProfileEmail;
}

int MAPIEx::GetMessageStoreCount() {
	IMAPITable* pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable) == S_OK) {
		ULONG ulCount;
		HRESULT hr = pMsgStoresTable->GetRowCount(0, &ulCount);
		RELEASE(pMsgStoresTable);
		if(hr == S_OK) 
			return ulCount;
	}
	return -1;
}

bool MAPIEx::GetMessageStoreProperties(int nIndex, String& strName, bool& bDefaultStore) {
	bool bResult = FALSE;
	IMAPITable *pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable) == S_OK) {
		LPSRowSet pRows = NULL;
		const int nProperties = 3;
		SizedSPropTagArray(nProperties, Columns) = {nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, 
																			PR_DEFAULT_STORE}};

		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
			if(pMsgStoresTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL) == S_OK) {
				if(pMsgStoresTable->QueryRows(1, 0, &pRows) == S_OK) {
					if(pRows->cRows>0) {
						strName = GetValidString(pRows->aRow[0].lpProps[0]);
						bDefaultStore = pRows->aRow[0].lpProps[2].Value.b;
						bResult = true;
					}
					FreeProws(pRows);
				}
			}
		}
		RELEASE(pMsgStoresTable);
	}
	return bResult;
}

bool MAPIEx::OpenMessageStore(int nIndex, ULONG ulFlags) {
	BOOL bResult = false;
	IMAPITable* pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable) == S_OK) {
		LPSRowSet pRows = NULL;
		const int nProperties = 3;
		SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, 
																			PR_DEFAULT_STORE}};
		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK)  {
			if(pMsgStoresTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL) == S_OK) {
				if(pMsgStoresTable->QueryRows(1, 0, &pRows)==S_OK) {
					if(pRows->cRows>0) {
						m_ulMDBFlags=ulFlags;
						RELEASE(m_pMsgStore);
						bResult = (m_pSession->OpenMsgStore(0, 
							pRows->aRow[0].lpProps[1].Value.bin.cb, 
							(ENTRYID*)pRows->aRow[0].lpProps[1].Value.bin.lpb, NULL,
							MDB_NO_DIALOG | MAPI_BEST_ACCESS, &m_pMsgStore) == S_OK);
					}
					FreeProws(pRows);
				}
			}
		}
		RELEASE(pMsgStoresTable);
	}
	return bResult;
}

// do not set MAPI_NO_CACHE flag for Outlook 97 and 2000
bool MAPIEx::OpenMessageStore(String szStore, ULONG ulFlags) {
	bool bResult = false;
	IMAPITable*	pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable) == S_OK) {
		LPSRowSet pRows = NULL;
		const int nProperties = 3;
		SizedSPropTagArray(nProperties, Columns) = {nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, 
																			PR_DEFAULT_STORE}};
		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
			while(pMsgStoresTable->QueryRows(1, 0, &pRows) == S_OK) {
				if(pRows->cRows != 1) 
					FreeProws(pRows);
				else {
					if(szStore.IsEmpty()) { 
						if(pRows->aRow[0].lpProps[2].Value.b) 
							bResult = true;
					} else {
						String strStore = GetValidString(pRows->aRow[0].lpProps[0]);
						if(strStore.Find(szStore) != -1) 
							bResult = true;
					}
					if(!bResult) {
						FreeProws(pRows);
						continue;
					}
				}
				break;
			}
			if(bResult) {
				m_ulMDBFlags = ulFlags;
				RELEASE(m_pMsgStore);
				bResult = (m_pSession->OpenMsgStore(0, pRows->aRow[0].lpProps[1].Value.bin.cb, 
									(ENTRYID*)pRows->aRow[0].lpProps[1].Value.bin.lpb, NULL,
									MDB_NO_DIALOG | MAPI_BEST_ACCESS, &m_pMsgStore) == S_OK);
				MAPIEx::FreeProws(pRows);
			}
		}
		RELEASE(pMsgStoresTable);
	}
	return bResult;
}

ULONG MAPIEx::GetMessageStoreSupport() {
	if(!m_pMsgStore) 
		return false;

	LPSPropValue props = NULL;
	ULONG cValues = 0;
	ULONG rgTags[] = {1, PR_STORE_SUPPORT_MASK};
	ULONG ulSupport = 0;

	if(m_pMsgStore->GetProps((LPSPropTagArray) rgTags, MAPIEx::cm_nMAPICode, &cValues, &props)
																					== S_OK) {
		ulSupport = props->Value.ul;
		MAPIFreeBuffer(props);
	}
	return ulSupport;
}

#ifdef _WIN32_WCE
CPOOM* MAPIEx::GetPOOM() {
	return m_pFolder ? m_pFolder->GetPOOM() : NULL;
}
#endif

bool MAPIEx::OpenFolder(unsigned long ulFolderID, MAPIFolder& folder) {
	if(!m_pMsgStore) 
		return NULL;

	LPSPropValue props = NULL;
	ULONG cValues = 0;
	DWORD dwObjType;
	ULONG rgTags[] = { 1, ulFolderID };
	LPMAPIFOLDER pFolder;

	if(S_OK != m_pMsgStore->GetProps((LPSPropTagArray) rgTags, cm_nMAPICode, &cValues, &props)) 
		return false;
	if(S_OK != m_pMsgStore->OpenEntry(props[0].Value.bin.cb, (LPENTRYID)props[0].Value.bin.lpb, 
										NULL, m_ulMDBFlags, &dwObjType, (LPUNKNOWN*)&pFolder)) 
		return false;
	
	MAPIFreeBuffer(props);
	if(pFolder) {
		folder.Attach(this, pFolder);
		folder.GetHierarchy();
		folder.GetContents();
		return true;
	}
	return false;
}

bool MAPIEx::OpenFolder(const String &folderName, MAPIFolder &folder, bool fullPath) {
	MAPIFolder root;
	if(!OpenRootFolder(root))
		return false;
	if (!root.OpenSubFolder(folderName, folder, fullPath, ""))
		return false;
	folder.GetHierarchy();
	folder.GetContents();
	return true;
}

bool MAPIEx::OpenSpecialFolder(unsigned long ulFolderID, MAPIFolder &folder) {
#ifdef _WIN32_WCE
	if(pFolder->GetPOOM()->OpenFolder(ulFolderID, folder) != NULL) {
		folder.GetContents();
		folder.GetHierarchy();
		return true;
	}
	return false;
#else
	MAPIFolder inbox; 
	if(!OpenInbox(inbox) || !m_pMsgStore) 
		return false;

	LPSPropValue props = NULL;
	ULONG cValues = 0;
	DWORD dwObjType;
	ULONG rgTags[] = { 1, ulFolderID };
	LPMAPIFOLDER pFolder;

	if(inbox.Folder()->GetProps((LPSPropTagArray) rgTags, cm_nMAPICode, &cValues, &props)!=S_OK) 
		return false;
	HRESULT hr = m_pMsgStore->OpenEntry(props[0].Value.bin.cb, 
							(LPENTRYID)props[0].Value.bin.lpb, NULL, m_ulMDBFlags, &dwObjType, 
							(LPUNKNOWN*)&pFolder);
	MAPIFreeBuffer(props);
	if(hr != S_OK) 	
		return false;

	if(pFolder) {
		folder.Attach(this, pFolder);
		folder.GetContents();
		folder.GetHierarchy();
		return true;
	}
	return false;
#endif
}

bool MAPIEx::OpenRootFolder(MAPIFolder &folder) {
	return OpenFolder(PR_IPM_SUBTREE_ENTRYID, folder);
}

bool MAPIEx::OpenInbox(MAPIFolder &folder) {
#ifdef _WIN32_WCE
	return OpenFolder(PR_CE_IPM_INBOX_ENTRYID, MAPIFolder folder);
#else
	if(!m_pMsgStore) 
		return false;

	ULONG cbEntryID;
	LPENTRYID pEntryID;
	DWORD dwObjType;
	LPMAPIFOLDER pFolder;

	if(m_pMsgStore->GetReceiveFolder(NULL, 0, &cbEntryID, &pEntryID, NULL) != S_OK) 
		return false;
	HRESULT hr = m_pMsgStore->OpenEntry(cbEntryID, pEntryID, NULL, m_ulMDBFlags, &dwObjType, (LPUNKNOWN*)&pFolder);
	MAPIFreeBuffer(pEntryID);

	if(hr == S_OK && pFolder) {
		folder.Attach(this, pFolder);
		folder.GetHierarchy();
		folder.GetContents();
		return true;
	}
	return false;
#endif
}

bool MAPIEx::OpenOutbox(MAPIFolder &folder) {
	return OpenFolder(PR_IPM_OUTBOX_ENTRYID, folder);
}

bool MAPIEx::OpenSentItems(MAPIFolder &folder) {
	return OpenFolder(PR_IPM_SENTMAIL_ENTRYID, folder);
}

bool MAPIEx::OpenDeletedItems(MAPIFolder &folder) {
	return OpenFolder(PR_IPM_WASTEBASKET_ENTRYID, folder);
}

bool MAPIEx::OpenContacts(MAPIFolder &folder) {
	return OpenSpecialFolder(PR_IPM_CONTACT_ENTRYID, folder);
}

bool MAPIEx::OpenDrafts(MAPIFolder &folder) {
#ifdef _WIN32_WCE
	return OpenFolder(PR_CE_IPM_DRAFTS_ENTRYID, folder);
#else
	return OpenSpecialFolder(PR_IPM_DRAFTS_ENTRYID, folder);
#endif
}

bool MAPIEx::OpenCalendar(MAPIFolder &folder) {
	return OpenSpecialFolder(PR_IPM_APPOINTMENT_ENTRYID, folder);
}

// call with ulEventMask set to ALL notifications ORed together, only one Advise Sink is used.
bool MAPIEx::Notify(LPNOTIFCALLBACK lpfnCallback, LPVOID lpvContext, ULONG ulEventMask) {
	if(GetMessageStoreSupport()&STORE_NOTIFY_OK) {
		if(m_sink) 
			m_pMsgStore->Unadvise(m_sink);
		MAPISink* pAdviseSink = new MAPISink(lpfnCallback, lpvContext);
		if(m_pMsgStore->Advise(0, NULL, ulEventMask, pAdviseSink, &m_sink) == S_OK) 
			return true;
		delete pAdviseSink;
		m_sink = 0;
	}
	return false;
}

// pumps the message queue, use this when waiting for notifications
void MAPIEx::PumpMessages() {
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// sometimes the string in prop is invalid, causing unexpected crashes
#ifndef _WIN32_WCE
String MAPIEx::ValidateString(LPCTSTR s) {
	if(s && !::IsBadStringPtr(s, (UINT_PTR)-1)) 
		return FromSystemCharset(s);
	return Null;
}
#else
// Windows Mobile does not have IsBadStringPtr so I use SEH
// I shut off optimization because the optimizer still had trouble with the try/catch
#pragma optimize("",off)
String MAPIEx::ValidateString(String s) {
	try {
		if(s) {
			TCHAR ch = *s;
			return s;
		}
	} 
	catch(...) {
	}
	return Null;
}
#pragma optimize("",on)
#endif

String MAPIEx::GetValidString(SPropValue& prop) {
	return ValidateString(prop.Value.LPSZ);
}

// sometimes the string in prop is invalid, causing unexpected crashes
String MAPIEx::GetValidMVString(SPropValue& prop, int nIndex) {
	return ValidateString(prop.Value.MVSZ.LPPSZ[nIndex]);
}

// special case of GetValidString to take the narrow string in UNICODE
void MAPIEx::GetNarrowString(SPropValue& prop, String& strNarrow) {
	String s = GetValidString(prop);
	if(s.IsEmpty()) 
		strNarrow = "";
	else 
		strNarrow = s;
}

bool MAPIEx::SelectContacts(Vector<String> &emails, const String caption) {
	emails.Clear();
	LPADRLIST pAddressList;
	if(ShowAddressBook(pAddressList, caption) == IDOK) {
		String strEmail;
		for(ULONG i = 0; i < pAddressList->cEntries; i++) {
			if(GetEmail(pAddressList->aEntries[i], strEmail)) 
				emails.Add(strEmail);
		}
		MAPIEx::ReleaseAddressList(pAddressList);
	}
	return emails.GetCount() > 0;
}

// Shows the default Address Book dialog, return false on failure, IDOK or IDCANCEL on success
// I force narrow strings here because it for some reason doesn't work in UNICODE
int MAPIEx::ShowAddressBook(LPADRLIST& pAddressList, const String caption) {
#ifndef _WIN32_WCE
	if(!m_pSession) 
		return false;

	LPADRBOOK pAddressBook;
	if(m_pSession->OpenAddressBook(0, NULL, 0, &pAddressBook) == S_OK) {
		pAddressList = NULL;

		const char* lppszDestTitles[]={ "To" };
		ULONG lpulDestComps[] = { MAPI_TO };

		ADRPARM adrparm;
		memset(&adrparm, 0, sizeof(ADRPARM));
		adrparm.ulFlags = DIALOG_MODAL | AB_RESOLVE;

		adrparm.lpszCaption = (LPTSTR)(caption.Begin());

		adrparm.cDestFields = 1;
		adrparm.lppszDestTitles=(LPTSTR*)lppszDestTitles;
		adrparm.lpulDestComps=lpulDestComps;

		HWND hDesktop = ::GetDesktopWindow();
		HRESULT hr = pAddressBook->Address((ULONG_PTR*)&hDesktop, &adrparm, &pAddressList);
		RELEASE(pAddressBook);
		if(hr == S_OK) 
			return IDOK;
		if(hr == MAPI_E_USER_CANCEL) 
			return IDCANCEL;
	}
#endif
	return false;
}

// utility function to release ADRLIST entries
void MAPIEx::ReleaseAddressList(LPADRLIST pAddressList) {
#ifndef _WIN32_WCE
	MF().FreePadrlist(pAddressList);
#else
	if(pAddressList) {
		for(ULONG i = 0; i < pAddressList->cEntries; i++) 
			MAPIFreeBuffer(pAddressList->aEntries[i].rgPropVals);
		MAPIFreeBuffer(pAddressList);
	}
#endif
}

bool MAPIEx::CompareEntryIDs(ULONG cb1, LPENTRYID lpb1, ULONG cb2, LPENTRYID lpb2) {
	ULONG ulResult;
	if(m_pSession && m_pSession->CompareEntryIDs(cb1, lpb1, cb2, lpb2, 0, &ulResult) == S_OK) 
		return ulResult;
	return false;
}

// ADDRENTRY objects from Address don't come in unicode so I check for _A and force narrow strings
bool MAPIEx::GetEmail(ADRENTRY& adrEntry, String& strEmail) {
#ifndef _WIN32_WCE
	LPSPropValue pProp = MF().PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ADDRTYPE);
	if(!pProp) 
		pProp = MF().PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ADDRTYPE_A);
	if(pProp) {
		String strAddrType;
		GetNarrowString(*pProp, strAddrType);
		if(strAddrType == "EX") {
			pProp = MF().PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ENTRYID);

			SBinary entryID;
			entryID.cb = pProp->Value.bin.cb;
			entryID.lpb = pProp->Value.bin.lpb;

			return GetExEmail(entryID, strEmail);
		}
	}
	pProp = MF().PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_EMAIL_ADDRESS);
	if(!pProp) 
		pProp = MF().PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_EMAIL_ADDRESS_A);
	if(pProp) {
		GetNarrowString(*pProp, strEmail);
		return true;
	}
#endif
	return false;
}

bool MAPIEx::GetExEmail(SBinary entryID, String& strEmail) {
	bool bResult = false;
#ifndef _WIN32_WCE
	if(!m_pSession) 
		return false;

	LPADRBOOK pAddressBook;
	if(m_pSession->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook) != S_OK) 
		return false;
	ULONG ulObjType;
	IMAPIProp* pItem = NULL;
	if(pAddressBook->OpenEntry(entryID.cb, (ENTRYID*)entryID.lpb, NULL,MAPI_BEST_ACCESS, 
													&ulObjType, (LPUNKNOWN*)&pItem) == S_OK) {
		if(ulObjType == MAPI_MAILUSER) {
			LPSPropValue pProp;
			ULONG ulPropCount;
			ULONG pSMTP[]  = { 1, PR_SMTP_ADDRESS };
			ULONG pEmail[] = { 1, PR_EMAIL_ADDRESS };

			if(pItem->GetProps((LPSPropTagArray)pSMTP, MAPIEx::cm_nMAPICode, &ulPropCount, 
																				&pProp)==S_OK) {
				strEmail=MAPIEx::GetValidString(*pProp);
				MAPIFreeBuffer(pProp);
				bResult = true;
			}
			if(!bResult && pItem->GetProps((LPSPropTagArray)pEmail, MAPIEx::cm_nMAPICode, 
													&ulPropCount, &pProp) == S_OK) {
				strEmail=MAPIEx::GetValidString(*pProp);
				MAPIFreeBuffer(pProp);
				bResult = true;
			}
		}
		RELEASE(pItem);
	}
	RELEASE(pAddressBook);
#endif
	return bResult;
}

void MAPIEx::SetSystemTime(SYSTEMTIME& tm, int wYear, int wMonth, int wDay, int wHour, 
												int wMinute, int wSecond, int wMilliSeconds) {
	tm.wYear 	= (WORD)wYear;
	tm.wMonth	= (WORD)wMonth;
	tm.wDay		= (WORD)wDay;
	tm.wHour	= (WORD)wHour;
	tm.wMinute	= (WORD)wMinute;
	tm.wSecond	= (WORD)wSecond;
	tm.wMilliseconds= (WORD)wMilliSeconds;
	tm.wDayOfWeek	= 0;
}

void MAPIEx::SetSystemTime(SYSTEMTIME& tm, const Time &t) {
	tm.wYear 	= t.year;
	tm.wMonth	= t.month;
	tm.wDay		= t.day;
	tm.wHour	= t.hour;
	tm.wMinute	= t.minute;
	tm.wSecond	= t.second;
	tm.wMilliseconds = 0;
	tm.wDayOfWeek	 = 0;
}

Time MAPIEx::GetSystemTime(SYSTEMTIME& tm) {
	return Time(tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
}
	
#endif