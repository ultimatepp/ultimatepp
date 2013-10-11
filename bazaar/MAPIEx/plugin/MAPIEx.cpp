////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIEx.cpp
// Description: Windows Extended MAPI class 
//
// Copyright (C) 2005-2011, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for the community to benefit.
//
// Usage: see the CodeProject article at http://www.codeproject.com/internet/CMapiEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MAPIExPCH.h"
#include "MAPIEx.h"
#include "MAPISink.h"

#ifdef _WIN32_WCE
#include <cemapi.h>
#pragma comment(lib,"cemapi.lib")
#pragma comment(lib,"pimstore.lib")

#else
#pragma comment (lib,"mapi32.lib")
#endif
#pragma comment(lib,"Ole32.lib")

void WINAPI FreeProws(LPSRowSet pRows)
{
	if(pRows) 
	{
		for(ULONG i=0;i<pRows->cRows;i++) 
		{
			MAPIFreeBuffer(pRows->aRow[i].lpProps);
		}
		MAPIFreeBuffer(pRows);
	}
}

/////////////////////////////////////////////////////////////
// CMAPIEx

int CMAPIEx::cm_bComInit=FALSE;

#ifdef UNICODE
int CMAPIEx::cm_nMAPICode=MAPI_UNICODE;
#else
int CMAPIEx::cm_nMAPICode=0;
#endif

CMAPIEx::CMAPIEx()
{
	m_pSession=NULL;
	m_pMsgStore=NULL;
	m_pFolder=NULL;	
	m_sink=0;
}

CMAPIEx::~CMAPIEx()
{
	Logout();
}

// dwFlags set to defaults but added as a param here for special cases.
BOOL CMAPIEx::Init(DWORD dwFlags, BOOL bInitAsService)
{
	if(CoInitializeEx(NULL, COINIT_MULTITHREADED)==S_OK) cm_bComInit=TRUE;
#ifdef _WIN32_WCE
	// flags are ignored on Windows CE
	if(MAPIInitialize(NULL)!=S_OK) return FALSE;
	CPOOM::Init();
#else
	if(!dwFlags) dwFlags=MAPI_NO_COINIT | MAPI_MULTITHREAD_NOTIFICATIONS;
	if(bInitAsService) dwFlags|=MAPI_NT_SERVICE;

	MAPIINIT_0 MAPIInit={ MAPI_INIT_VERSION, dwFlags };
	if(MAPIInitialize(&MAPIInit)!=S_OK) return FALSE;
#endif
	return TRUE;
}

void CMAPIEx::Term()
{
	MAPIUninitialize();
#ifdef _WIN32_WCE
	CPOOM::Term();
#endif
	if(cm_bComInit) CoUninitialize();
}

// dwFlags set to defaults but added as a param here for special cases (ie MAPI_NO_MAIL)
BOOL CMAPIEx::Login(LPCTSTR szProfileName, DWORD dwFlags, BOOL bInitAsService)
{
	if(!dwFlags) dwFlags=MAPI_EXTENDED | MAPI_USE_DEFAULT | MAPI_NEW_SESSION;
	if(bInitAsService) dwFlags|=MAPI_EXPLICIT_PROFILE | MAPI_NT_SERVICE;
	return (MAPILogonEx(NULL, (LPTSTR)szProfileName, NULL, dwFlags, &m_pSession)==S_OK);
}

void CMAPIEx::Logout()
{
	if(m_sink) 
	{
		if(m_pMsgStore) m_pMsgStore->Unadvise(m_sink);
		m_sink=0;
	}

	delete m_pFolder;
	m_pFolder=NULL;
	RELEASE(m_pMsgStore);
	RELEASE(m_pSession);
}

BOOL CMAPIEx::CreateProfile(LPCTSTR szProfileName)
{
	LPPROFADMIN	lpProfAdmin=NULL;
	if(MAPIAdminProfiles(0, &lpProfAdmin)!=S_OK) return FALSE;
	HRESULT hr=lpProfAdmin->CreateProfile((LPTSTR)szProfileName, NULL, 0, 0);
	RELEASE(lpProfAdmin);
	return (hr==S_OK);
}

BOOL CMAPIEx::DeleteProfile(LPCTSTR szProfileName)
{
	LPPROFADMIN	lpProfAdmin=NULL;
	if(MAPIAdminProfiles(0, &lpProfAdmin)!=S_OK) return FALSE;
	HRESULT hr=lpProfAdmin->DeleteProfile((LPTSTR)szProfileName, 0);
	RELEASE(lpProfAdmin);
	return (hr==S_OK);
}

BOOL CMAPIEx::CreateMessageService(LPCTSTR szProfileName, LPCTSTR szService, LPCTSTR szDisplayName)
{
	BOOL bResult=FALSE;
	LPPROFADMIN	lpProfAdmin=NULL;
	if(MAPIAdminProfiles(0, &lpProfAdmin)==S_OK) 
	{
		LPSERVICEADMIN lpMsgServiceAdmin=NULL;
		if(lpProfAdmin->AdminServices((LPTSTR)szProfileName, NULL, NULL, MAPI_DIALOG, &lpMsgServiceAdmin) == S_OK)
		{
			bResult=(lpMsgServiceAdmin->CreateMsgService((LPTSTR)szService, (LPTSTR)szDisplayName, NULL, SERVICE_UI_ALWAYS | SERVICE_UI_ALLOWED)==S_OK);
			RELEASE(lpMsgServiceAdmin);
		}
		RELEASE(lpProfAdmin);
	}
	return bResult;
}

// Sets (replaces) the internal folder, will be deleted when this goes out of scope
void CMAPIEx::SetFolder(CMAPIFolder* pFolder)
{
	delete m_pFolder;
	m_pFolder=pFolder;
}

// if I try to use MAPI_UNICODE when UNICODE is defined I get the MAPI_E_BAD_CHARWIDTH 
// error so I force narrow strings here
BOOL CMAPIEx::GetProfileName(CString& strProfileName)
{
	BOOL bResult=FALSE;
#ifndef _WIN32_WCE
	if(!m_pSession) return FALSE;

	LPSRowSet pRows=NULL;
	const int nProperties=2;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME_A, PR_RESOURCE_TYPE}};

	IMAPITable*	pStatusTable;
	if(m_pSession->GetStatusTable(0, &pStatusTable)==S_OK) 
	{
		if(pStatusTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK) 
		{
			while(pStatusTable->QueryRows(1, 0, &pRows)==S_OK) 
			{
				if(pRows->cRows!=1) FreeProws(pRows);
				else if(pRows->aRow[0].lpProps[1].Value.ul==MAPI_SUBSYSTEM) 
				{
					strProfileName=(LPSTR)GetValidString(pRows->aRow[0].lpProps[0]);
					FreeProws(pRows);
					bResult=TRUE;
				} 
				else 
				{
					FreeProws(pRows);
					continue;
				}
				break;
			}
		}
		RELEASE(pStatusTable);
	}
#endif
	return bResult;
}

BOOL CMAPIEx::GetProfileEmail(CString& strProfileEmail)
{
#ifndef _WIN32_WCE
	if(!m_pSession) return NULL;

	SBinary entryID;
	if(m_pSession->QueryIdentity(&entryID.cb, (LPENTRYID*)&entryID.lpb)==S_OK) 
	{
		return GetExEmail(entryID, strProfileEmail);
	}
#endif
	return FALSE;
}

int CMAPIEx::GetMessageStoreCount()
{
	IMAPITable* pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable)==S_OK) 
	{
		ULONG ulCount;
		HRESULT hr=pMsgStoresTable->GetRowCount(0, &ulCount);
		RELEASE(pMsgStoresTable);
		if(hr==S_OK) return ulCount;
	}
	return -1;
}

BOOL CMAPIEx::GetMessageStoreProperties(int nIndex, CString& strName, BOOL& bDefaultStore)
{
	BOOL bResult=FALSE;
	IMAPITable* pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable)==S_OK) 
	{
		LPSRowSet pRows=NULL;
		const int nProperties=3;
		SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, PR_DEFAULT_STORE}};

		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK) 
		{
			if(pMsgStoresTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL)==S_OK)
			{
				if(pMsgStoresTable->QueryRows(1, 0, &pRows)==S_OK) 
				{
					if(pRows->cRows>0) 
					{
						strName=GetValidString(pRows->aRow[0].lpProps[0]);
						bDefaultStore=pRows->aRow[0].lpProps[2].Value.b;
						bResult=TRUE;
					}
					FreeProws(pRows);
				}
			}
		}
		RELEASE(pMsgStoresTable);
	}
	return bResult;
}

BOOL CMAPIEx::OpenMessageStore(int nIndex, ULONG ulFlags)
{
	BOOL bResult=FALSE;
	IMAPITable* pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable)==S_OK) 
	{
		LPSRowSet pRows=NULL;
		const int nProperties=3;
		SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, PR_DEFAULT_STORE}};

		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK) 
		{
			if(pMsgStoresTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL)==S_OK)
			{
				if(pMsgStoresTable->QueryRows(1, 0, &pRows)==S_OK) 
				{
					if(pRows->cRows>0) 
					{
						m_ulMDBFlags=ulFlags;
						RELEASE(m_pMsgStore);
						bResult=(m_pSession->OpenMsgStore(NULL, pRows->aRow[0].lpProps[1].Value.bin.cb, (ENTRYID*)pRows->aRow[0].lpProps[1].Value.bin.lpb, NULL,MDB_NO_DIALOG | MAPI_BEST_ACCESS, &m_pMsgStore)==S_OK);
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
BOOL CMAPIEx::OpenMessageStore(LPCTSTR szStore, ULONG ulFlags)
{
	BOOL bResult=FALSE;
	IMAPITable*	pMsgStoresTable;
	if(m_pSession && m_pSession->GetMsgStoresTable(0, &pMsgStoresTable)==S_OK) 
	{
		LPSRowSet pRows=NULL;
		const int nProperties=3;
		SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID, PR_DEFAULT_STORE}};

		if(pMsgStoresTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK) 
		{
			while(pMsgStoresTable->QueryRows(1, 0, &pRows)==S_OK) 
			{
				if(pRows->cRows!=1) FreeProws(pRows);
				else {
					if(!szStore) 
					{ 
						if(pRows->aRow[0].lpProps[2].Value.b) bResult=TRUE;
					} 
					else 
					{
						CString strStore=GetValidString(pRows->aRow[0].lpProps[0]);
						if(strStore.Find(szStore)!=-1) bResult=TRUE;
					}
					if(!bResult) 
					{
						FreeProws(pRows);
						continue;
					}
				}
				break;
			}
			if(bResult) 
			{
				m_ulMDBFlags=ulFlags;
				RELEASE(m_pMsgStore);
				bResult=(m_pSession->OpenMsgStore(NULL, pRows->aRow[0].lpProps[1].Value.bin.cb, (ENTRYID*)pRows->aRow[0].lpProps[1].Value.bin.lpb, NULL,MDB_NO_DIALOG | MAPI_BEST_ACCESS, &m_pMsgStore)==S_OK);
				FreeProws(pRows);
			}
		}
		RELEASE(pMsgStoresTable);
	}
	return bResult;
}

ULONG CMAPIEx::GetMessageStoreSupport()
{
	if(!m_pMsgStore) return FALSE;

	LPSPropValue props=NULL;
	ULONG cValues=0;
	ULONG rgTags[]={ 1, PR_STORE_SUPPORT_MASK };
	ULONG ulSupport=0;

	if(m_pMsgStore->GetProps((LPSPropTagArray) rgTags, CMAPIEx::cm_nMAPICode, &cValues, &props)==S_OK) 
	{
		ulSupport=props->Value.ul;
		MAPIFreeBuffer(props);
	}
	return ulSupport;
}

#ifdef _WIN32_WCE
CPOOM* CMAPIEx::GetPOOM()
{
	return m_pFolder ? m_pFolder->GetPOOM() : NULL;
}
#endif

CMAPIFolder* CMAPIEx::OpenFolder(unsigned long ulFolderID, BOOL bInternal)
{
	if(!m_pMsgStore) return NULL;

	LPSPropValue props=NULL;
	ULONG cValues=0;
	DWORD dwObjType;
	ULONG rgTags[]={ 1, ulFolderID };
	LPMAPIFOLDER pFolder;

	if(m_pMsgStore->GetProps((LPSPropTagArray) rgTags, cm_nMAPICode, &cValues, &props)!=S_OK) return NULL;
	if(m_pMsgStore->OpenEntry(props[0].Value.bin.cb, (LPENTRYID)props[0].Value.bin.lpb, NULL, m_ulMDBFlags, &dwObjType, (LPUNKNOWN*)&pFolder)!=S_OK) return NULL;
	MAPIFreeBuffer(props);

	if(pFolder) 
	{
		CMAPIFolder* pMAPIFolder=new CMAPIFolder(this, pFolder);
		if(bInternal) 
		{
			delete m_pFolder;
			m_pFolder=pMAPIFolder;
		}
		return pMAPIFolder;
	}
	return NULL;
}

CMAPIFolder* CMAPIEx::OpenFolder(LPCTSTR szFolderName, BOOL bInternal)
{
	CMAPIFolder* pRoot=OpenRootFolder(FALSE);
	CMAPIFolder* pFolder=NULL;
	if(pRoot)
	{
		pFolder=pRoot->OpenSubFolder(szFolderName);
		if(bInternal) 
		{
			delete m_pFolder;
			m_pFolder=pFolder;
		}
		delete pRoot;
	}
	return pFolder;
}

CMAPIFolder* CMAPIEx::OpenSpecialFolder(unsigned long ulFolderID, BOOL bInternal)
{
#ifdef _WIN32_WCE
	CMAPIFolder* pFolder=new CMAPIFolder(this, NULL);
	if(pFolder->GetPOOM()->OpenFolder(ulFolderID, bInternal)!=NULL) 
	{
		if(bInternal) 
		{
			delete m_pFolder;
			m_pFolder=pFolder;
		}
		return pFolder;
	}
	return NULL;
#else
	CMAPIFolder* pInbox=OpenInbox(FALSE);
	if(!pInbox || !m_pMsgStore) return FALSE;

	LPSPropValue props=NULL;
	ULONG cValues=0;
	DWORD dwObjType;
	ULONG rgTags[]={ 1, ulFolderID };
	LPMAPIFOLDER pFolder;

	if(pInbox->Folder()->GetProps((LPSPropTagArray) rgTags, cm_nMAPICode, &cValues, &props)!=S_OK) return NULL;
	HRESULT hr=m_pMsgStore->OpenEntry(props[0].Value.bin.cb, (LPENTRYID)props[0].Value.bin.lpb, NULL, m_ulMDBFlags, &dwObjType, (LPUNKNOWN*)&pFolder);
	MAPIFreeBuffer(props);
	delete pInbox;
	if(hr!=S_OK) return NULL;

	if(pFolder) 
	{
		CMAPIFolder* pMAPIFolder=new CMAPIFolder(this, pFolder);
		if(bInternal) 
		{
			delete m_pFolder;
			m_pFolder=pMAPIFolder;
		}
		return pMAPIFolder;
	}
	return NULL;
#endif
}

CMAPIFolder* CMAPIEx::OpenRootFolder(BOOL bInternal)
{
	return OpenFolder(PR_IPM_SUBTREE_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenInbox(BOOL bInternal)
{
#ifdef _WIN32_WCE
	return OpenFolder(PR_CE_IPM_INBOX_ENTRYID, bInternal);
#else
	if(!m_pMsgStore) return NULL;

	ULONG cbEntryID;
	LPENTRYID pEntryID;
	DWORD dwObjType;
	LPMAPIFOLDER pFolder;

	if(m_pMsgStore->GetReceiveFolder(NULL, 0, &cbEntryID, &pEntryID, NULL)!=S_OK) return NULL;
	HRESULT hr=m_pMsgStore->OpenEntry(cbEntryID, pEntryID, NULL, m_ulMDBFlags, &dwObjType, (LPUNKNOWN*)&pFolder);
	MAPIFreeBuffer(pEntryID);

	if(hr==S_OK && pFolder) 
	{
		CMAPIFolder* pMAPIFolder=new CMAPIFolder(this, pFolder);
		if(bInternal) 
		{
			delete m_pFolder;
			m_pFolder=pMAPIFolder;
		}
		return pMAPIFolder;
	}
	return NULL;
#endif
}

CMAPIFolder* CMAPIEx::OpenOutbox(BOOL bInternal)
{
	return OpenFolder(PR_IPM_OUTBOX_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenSentItems(BOOL bInternal)
{
	return OpenFolder(PR_IPM_SENTMAIL_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenDeletedItems(BOOL bInternal)
{
	return OpenFolder(PR_IPM_WASTEBASKET_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenContacts(BOOL bInternal)
{
	return OpenSpecialFolder(PR_IPM_CONTACT_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenDrafts(BOOL bInternal)
{
#ifdef _WIN32_WCE
	return OpenFolder(PR_CE_IPM_DRAFTS_ENTRYID, bInternal);
#else
	return OpenSpecialFolder(PR_IPM_DRAFTS_ENTRYID, bInternal);
#endif
}

CMAPIFolder* CMAPIEx::OpenCalendar(BOOL bInternal)
{
	return OpenSpecialFolder(PR_IPM_APPOINTMENT_ENTRYID, bInternal);
}

CMAPIFolder* CMAPIEx::OpenJunkFolder(BOOL bInternal)
{
	return OpenFolder(_T("Junk E-mail"), bInternal);
}

LPMAPITABLE CMAPIEx::GetHierarchy()
{
	return m_pFolder ? m_pFolder->GetHierarchy() : NULL;
}

LPMAPITABLE CMAPIEx::GetContents()
{
	return m_pFolder ? m_pFolder->GetContents() : NULL;
}

int CMAPIEx::GetRowCount()
{
	return m_pFolder ? m_pFolder->GetRowCount() : 0;
}

BOOL CMAPIEx::SortContents(ULONG ulSortParam, ULONG ulSortField)
{
	return m_pFolder ? m_pFolder->SortContents(ulSortParam,ulSortField) : FALSE;
}

BOOL CMAPIEx::SetUnreadOnly(BOOL bUnreadOnly)
{
	return m_pFolder ? m_pFolder->SetUnreadOnly(bUnreadOnly) : FALSE;
}

BOOL CMAPIEx::SetRestriction(SRestriction* pRestriction)
{
	return m_pFolder ? m_pFolder->SetRestriction(pRestriction) : FALSE;
}

BOOL CMAPIEx::GetNextMessage(CMAPIMessage& message)
{
	return m_pFolder ? m_pFolder->GetNextMessage(message) : FALSE;
}

BOOL CMAPIEx::GetNextContact(CMAPIContact& contact)
{
	return m_pFolder ? m_pFolder->GetNextContact(contact) : FALSE;
}

BOOL CMAPIEx::GetNextAppointment(CMAPIAppointment& appointment)
{
	return m_pFolder ? m_pFolder->GetNextAppointment(appointment) : FALSE;
}

BOOL CMAPIEx::GetNextSubFolder(CMAPIFolder& folder, CString& strFolder)
{
	return m_pFolder ? m_pFolder->GetNextSubFolder(folder, strFolder) : FALSE;
}

// call with ulEventMask set to ALL notifications ORed together, only one Advise Sink is used.
BOOL CMAPIEx::Notify(LPNOTIFCALLBACK lpfnCallback, LPVOID lpvContext, ULONG ulEventMask)
{
	if(GetMessageStoreSupport()&STORE_NOTIFY_OK) 
	{
		if(m_sink) m_pMsgStore->Unadvise(m_sink);
		CMAPISink* pAdviseSink=new CMAPISink(lpfnCallback, lpvContext);
		if(m_pMsgStore->Advise(0, NULL,ulEventMask, pAdviseSink, &m_sink)==S_OK) return TRUE;
		delete pAdviseSink;
		m_sink=0;
	}
	return FALSE;
}

// pumps the message queue, use this when waiting for notifications
void CMAPIEx::PumpMessages()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// sometimes the string in prop is invalid, causing unexpected crashes
#ifndef _WIN32_WCE
LPCTSTR CMAPIEx::ValidateString(LPCTSTR s)
{
	if(s && !::IsBadStringPtr(s, (UINT_PTR)-1)) return s;
	return NULL;
}
#else
// Windows Mobile does not have IsBadStringPtr so I use SEH
// I shut off optimization because the optimizer still had trouble with the try/catch
#pragma optimize("",off)
LPCTSTR CMAPIEx::ValidateString(LPCTSTR s)
{
	try 
	{
		if(s) 
		{
			TCHAR ch=*s;
			return s;
		}
	} 
	catch(...) 
	{
	}
	return NULL;
}
#pragma optimize("",on)
#endif

LPCTSTR CMAPIEx::GetValidString(SPropValue& prop)
{
	return ValidateString(prop.Value.LPSZ);
}

// sometimes the string in prop is invalid, causing unexpected crashes
LPCTSTR CMAPIEx::GetValidMVString(SPropValue& prop, int nIndex)
{
	return ValidateString(prop.Value.MVSZ.LPPSZ[nIndex]);
}

// special case of GetValidString to take the narrow string in UNICODE
void CMAPIEx::GetNarrowString(SPropValue& prop, CString& strNarrow)
{
	LPCTSTR s=GetValidString(prop);
	if(!s) strNarrow=_T("");
	else {
#ifdef UNICODE
		// VS2005 can copy directly
		if(_MSC_VER>=1400) 
		{
			strNarrow=(LPCSTR)s;
		}
		else
		{
			WCHAR wszWide[256];
			MultiByteToWideChar(CP_ACP, 0, (LPCSTR)s, -1, wszWide, 255);
			strNarrow=wszWide;
		}
#else
		strNarrow=s;
#endif
	}
}

// Shows the default Address Book dialog, return FALSE on failure, IDOK or IDCANCEL on success
// I force narrow strings here because it for some reason doesn't work in UNICODE
int CMAPIEx::ShowAddressBook(LPADRLIST& pAddressList, LPCTSTR szCaption)
{
#ifndef _WIN32_WCE
	if(!m_pSession) return FALSE;

	LPADRBOOK pAddressBook;
	if(m_pSession->OpenAddressBook(0, NULL, 0, &pAddressBook)==S_OK) 
	{
		pAddressList=NULL;

		char* lppszDestTitles[]={ "To" };
		ULONG lpulDestComps[]={ MAPI_TO };

		ADRPARM adrparm;
		memset(&adrparm, 0,sizeof(ADRPARM));
		adrparm.ulFlags=DIALOG_MODAL | AB_RESOLVE;

		adrparm.lpszCaption=(LPTSTR)szCaption;
#ifdef UNICODE
		char szNarrowCaption[256];
		if(szCaption) 
		{
			WideCharToMultiByte(CP_ACP, 0, szCaption, -1, szNarrowCaption, 255, NULL, NULL);
			adrparm.lpszCaption=(LPTSTR)szNarrowCaption;
		}
#endif
		adrparm.cDestFields = 1;
		adrparm.lppszDestTitles = (LPTSTR*)lppszDestTitles;
		adrparm.lpulDestComps = lpulDestComps;

		HWND hDesktop = ::GetDesktopWindow();
		HRESULT hr = pAddressBook->Address((ULONG_PTR*)&hDesktop, &adrparm, &pAddressList);
		RELEASE(pAddressBook);
		if(hr == S_OK) 
			return IDOK;
		if(hr == MAPI_E_USER_CANCEL) 
			return IDCANCEL;
	}
#endif
	return FALSE;
}

// utility function to release ADRLIST entries
void CMAPIEx::ReleaseAddressList(LPADRLIST pAddressList)
{
#ifndef _WIN32_WCE
	FreePadrlist(pAddressList);
#else
	if(pAddressList) 
	{
		for(ULONG i=0;i<pAddressList->cEntries;i++) MAPIFreeBuffer(pAddressList->aEntries[i].rgPropVals);
		MAPIFreeBuffer(pAddressList);
	}
#endif
}

BOOL CMAPIEx::CompareEntryIDs(ULONG cb1, LPENTRYID lpb1, ULONG cb2, LPENTRYID lpb2)
{
	ULONG ulResult;
	if(m_pSession && m_pSession->CompareEntryIDs(cb1, lpb1, cb2, lpb2, 0, &ulResult)==S_OK) 
	{
		return ulResult;
	}
	return FALSE;
}

// ADDRENTRY objects from Address don't come in unicode so I check for _A and force narrow strings
BOOL CMAPIEx::GetEmail(ADRENTRY& adrEntry, CString& strEmail)
{
#ifndef _WIN32_WCE
	LPSPropValue pProp=PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ADDRTYPE);
	if(!pProp) pProp=PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ADDRTYPE_A);
	if(pProp) 
	{
		CString strAddrType;
		GetNarrowString(*pProp, strAddrType);
		if(strAddrType==_T("EX")) 
		{
			pProp=PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_ENTRYID);

			SBinary entryID;
			entryID.cb=pProp->Value.bin.cb;
			entryID.lpb=pProp->Value.bin.lpb;

			return GetExEmail(entryID, strEmail);
		}
	}
	pProp=PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_EMAIL_ADDRESS);
	if(!pProp) pProp=PpropFindProp(adrEntry.rgPropVals,adrEntry.cValues, PR_EMAIL_ADDRESS_A);
	if(pProp) 
	{
		GetNarrowString(*pProp, strEmail);
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CMAPIEx::GetExEmail(SBinary entryID, CString& strEmail)
{
	BOOL bResult=FALSE;
#ifndef _WIN32_WCE
	if(!m_pSession) return FALSE;

	LPADRBOOK pAddressBook;
	if(m_pSession->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook)==S_OK) 
	{
		ULONG ulObjType;
		IMAPIProp* pItem=NULL;
		if(pAddressBook->OpenEntry(entryID.cb, (ENTRYID*)entryID.lpb, NULL,MAPI_BEST_ACCESS, &ulObjType, (LPUNKNOWN*)&pItem)==S_OK) 
		{
			if(ulObjType==MAPI_MAILUSER) 
			{
				LPSPropValue pProp;
				ULONG ulPropCount;
				ULONG pSMTP[]={ 1, PR_SMTP_ADDRESS };
				ULONG pEmail[]={ 1, PR_EMAIL_ADDRESS };

				if(pItem->GetProps((LPSPropTagArray)pSMTP, CMAPIEx::cm_nMAPICode, &ulPropCount, &pProp)==S_OK) 
				{
					strEmail=CMAPIEx::GetValidString(*pProp);
					MAPIFreeBuffer(pProp);
					bResult=TRUE;
				}
				if(!bResult && pItem->GetProps((LPSPropTagArray)pEmail, CMAPIEx::cm_nMAPICode, &ulPropCount, &pProp)==S_OK) 
				{
					strEmail=CMAPIEx::GetValidString(*pProp);
					MAPIFreeBuffer(pProp);
					bResult=TRUE;
				}
			}
			RELEASE(pItem);
		}
		RELEASE(pAddressBook);
	}
#endif
	return bResult;
}

void CMAPIEx::GetSystemTime(SYSTEMTIME& tm, int wYear, int wMonth, int wDay, int wHour, int wMinute, int wSecond, int wMilliSeconds)
{
	tm.wYear=(WORD)wYear;
	tm.wMonth=(WORD)wMonth;
	tm.wDay=(WORD)wDay;
	tm.wHour=(WORD)wHour;
	tm.wMinute=(WORD)wMinute;
	tm.wSecond=(WORD)wSecond;
	tm.wMilliseconds=(WORD)wMilliSeconds;
	tm.wDayOfWeek=0;
}
