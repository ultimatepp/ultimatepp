#ifndef __MAPIEX_H__
#define __MAPIEX_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIEx.h
// Description: Windows Extended MAPI class 
//
// Copyright (C) 2005-2011, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for all to benefit.
//
// Usage: see the CodeProject article at http://www.codeproject.com/internet/CMapiEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32_WCE
#include <cemapi.h>
#include <objidl.h>
#else
#define INITGUID
#define USES_IID_IMessage
#include <InitGuid.h>
#include <MAPIGuid.h>

#include <MAPIX.h>
#include <ObjBase.h>
#endif

#define RELEASE(s) if(s!=NULL) { s->Release();s=NULL; }

#define MAPI_NO_CACHE ((ULONG)0x00000200)
#define MAPIEX_NOTIFICATIONS 0x007F

#include "MAPIObject.h"
#include "MAPIMessage.h"
#include "MAPIContact.h"
#include "MAPIAppointment.h"
#include "MAPIFolder.h"
#include "MAPIAttachment.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMAPIEx

class AFX_EXT_CLASS CMAPIEx 
{
public:
	CMAPIEx();
	~CMAPIEx();

// Attributes
public:
	static int cm_bComInit;
	static int cm_nMAPICode;

protected:
	IMAPISession* m_pSession;
	LPMDB m_pMsgStore;
	ULONG m_ulMDBFlags;
	CMAPIFolder* m_pFolder;
	ULONG m_sink;

// Operations
public:
	static BOOL Init(DWORD dwFlags=0, BOOL bInitAsService=FALSE);
	static void Term();
	BOOL Login(LPCTSTR szProfileName=NULL, DWORD dwFlags=0, BOOL bInitAsService=FALSE);
	void Logout();
	BOOL CreateProfile(LPCTSTR szProfileName);
	BOOL DeleteProfile(LPCTSTR szProfileName);
	BOOL CreateMessageService(LPCTSTR szProfileName, LPCTSTR szService, LPCTSTR szDisplayName);

	IMAPISession* GetSession() { return m_pSession; }
	LPMDB GetMessageStore() { return m_pMsgStore; }
	CMAPIFolder* GetFolder() { return m_pFolder; }
	void SetFolder(CMAPIFolder* pFolder);

	BOOL GetProfileName(CString& strProfileName);
	BOOL GetProfileEmail(CString& strProfileEmail);
	int GetMessageStoreCount();
	BOOL GetMessageStoreProperties(int nIndex, CString& strName, BOOL& bDefaultStore);
	BOOL OpenMessageStore(int nIndex, ULONG ulFlags=MAPI_MODIFY | MAPI_NO_CACHE);
	BOOL OpenMessageStore(LPCTSTR szStore=NULL, ULONG ulFlags=MAPI_MODIFY | MAPI_NO_CACHE);
	ULONG GetMessageStoreSupport();

#ifdef _WIN32_WCE
	CPOOM* GetPOOM();
#endif

	// use bInternal to specify that MAPIEx keeps track of and subsequently deletes the folder 
	// remember to eventually delete returned folders if calling with bInternal=FALSE
	CMAPIFolder* OpenFolder(unsigned long ulFolderID, BOOL bInternal);
	CMAPIFolder* OpenFolder(LPCTSTR szFolderName, BOOL bInternal);
	CMAPIFolder* OpenSpecialFolder(unsigned long ulFolderID, BOOL bInternal);
	CMAPIFolder* OpenRootFolder(BOOL bInternal=TRUE);
	CMAPIFolder* OpenInbox(BOOL bInternal=TRUE);
	CMAPIFolder* OpenOutbox(BOOL bInternal=TRUE);
	CMAPIFolder* OpenSentItems(BOOL bInternal=TRUE);
	CMAPIFolder* OpenDeletedItems(BOOL bInternal=TRUE);
	CMAPIFolder* OpenContacts(BOOL bInternal=TRUE);
	CMAPIFolder* OpenDrafts(BOOL bInternal=TRUE);
	CMAPIFolder* OpenCalendar(BOOL bInternal=TRUE);
	CMAPIFolder* OpenJunkFolder(BOOL bInternal=TRUE);

	LPMAPITABLE GetHierarchy();
	LPMAPITABLE GetContents();
	int GetRowCount();
	BOOL SortContents(ULONG ulSortParam=TABLE_SORT_ASCEND, ULONG ulSortField=PR_MESSAGE_DELIVERY_TIME);
	BOOL SetUnreadOnly(BOOL bUnreadOnly=TRUE);
	BOOL SetRestriction(SRestriction* pRestriction);
	BOOL GetNextMessage(CMAPIMessage& message);
	BOOL GetNextContact(CMAPIContact& contact);
	BOOL GetNextAppointment(CMAPIAppointment& appointment);
	BOOL GetNextSubFolder(CMAPIFolder& folder, CString& strFolder);

	BOOL Notify(LPNOTIFCALLBACK lpfnCallback, LPVOID lpvContext, ULONG ulEventMask=MAPIEX_NOTIFICATIONS);
	static void PumpMessages();

	int ShowAddressBook(LPADRLIST& pAddressList, LPCTSTR szCaption=NULL);
	BOOL GetEmail(ADRENTRY& adrEntry, CString& strEmail);
	BOOL GetExEmail(SBinary entryID, CString& strEmail);
	static LPCTSTR GetValidString(SPropValue& prop);
	static LPCTSTR GetValidMVString(SPropValue& prop, int nIndex);
	static void GetNarrowString(SPropValue& prop, CString& strNarrow);
	static void GetSystemTime(SYSTEMTIME& tm, int wYear, int wMonth, int wDay, int wHour=0, int wMinute=0, int wSecond=0, int wMilliSeconds=0);
	static void ReleaseAddressList(LPADRLIST pAddressList);
	BOOL CompareEntryIDs(ULONG cb1, LPENTRYID lpb1, ULONG cb2, LPENTRYID lpb2);

protected:
	static LPCTSTR ValidateString(LPCTSTR s);
};

#ifndef MSGSTATUS_HAS_PR_BODY_HTML
#define MSGSTATUS_HAS_PR_BODY_HTML 0x80000000
#endif

#ifndef PR_BODY_HTML
#define PR_BODY_HTML PROP_TAG( PT_TSTRING, 0x1013)
#define PR_BODY_HTML_A PROP_TAG( PT_STRING8, 0x1013)
#endif

#ifndef STORE_HTML_OK
#define	STORE_HTML_OK ((ULONG)0x00010000)
#endif

#ifndef PR_ATTACH_CONTENT_ID
#define PR_ATTACH_CONTENT_ID PROP_TAG(PT_TSTRING, 0x3712)
#endif

#ifndef PR_SMTP_ADDRESS
#define PR_SMTP_ADDRESS PROP_TAG(PT_TSTRING, 0x39FE)
#endif

#ifndef PR_MSG_EDITOR_FORMAT 
#define PR_MSG_EDITOR_FORMAT PROP_TAG(PT_LONG, 0x5909)
#define EDITOR_FORMAT_DONTKNOW ((ULONG)0) 
#define EDITOR_FORMAT_PLAINTEXT ((ULONG)1) 
#define EDITOR_FORMAT_HTML ((ULONG)2) 
#define EDITOR_FORMAT_RTF ((ULONG)3) 
#endif

#define PR_IPM_APPOINTMENT_ENTRYID (PROP_TAG(PT_BINARY, 0x36D0))
#define PR_IPM_CONTACT_ENTRYID (PROP_TAG(PT_BINARY, 0x36D1))
#define PR_IPM_JOURNAL_ENTRYID (PROP_TAG(PT_BINARY, 0x36D2))
#define PR_IPM_NOTE_ENTRYID (PROP_TAG(PT_BINARY, 0x36D3))
#define PR_IPM_TASK_ENTRYID (PROP_TAG(PT_BINARY, 0x36D4))
#define PR_IPM_DRAFTS_ENTRYID (PROP_TAG(PT_BINARY, 0x36D7))

#ifndef MAPI_NO_COINIT
#define MAPI_NO_COINIT 0x8
#endif

#endif
