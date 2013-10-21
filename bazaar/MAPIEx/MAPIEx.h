#ifndef __MAPIEX_H__
#define __MAPIEX_H__

#include <Core/Core.h>
#include <Functions4U/Functions4U.h>

using namespace Upp;
//#undef CY
//#define CY tagCY


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIEx.h
// Description: Windows Extended MAPI class 
//
// Copyright (C) 2005-2010, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for all to benefit.
//
// Usage: see the CodeProject article at http://www.codeproject.com/internet/CMapiEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ported to U++ Framework by Koldo. See License.txt file

#ifdef _WIN32_WCE
#include <cemapi.h>
#include <objidl.h>
#else
#undef __MINGW_EXTENSION
	#if defined(__GNUC__) || defined(__GNUG__)
		#define __MINGW_EXTENSION       __extension__
	#else
		#define __MINGW_EXTENSION
	#endif
	#define _FILETIME_
	#define _tagCY_DEFINED
	#define CY tagCY
	#ifndef __LP64__        /* 32 bit target, 64 bit Mingw target */ 
		#define __LONG32 long 
	#else                   /* 64 bit Cygwin target */ 
		#define __LONG32 int 
	#endif 
	#ifndef __C89_NAMELESS 
		#define __C89_NAMELESS __MINGW_EXTENSION 
	#endif 
#include <mapix.h>
#include <wabutil.h>
#include <objbase.h>
#include <imessage.h>
#endif

#define RELEASE(s) if(s != NULL) {s->Release(); s=NULL;}

#define MAPI_NO_CACHE ((ULONG)0x00000200)

#include "MAPIObject.h"
#include "MAPIMessage.h"
#include "MAPIContact.h"
#include "MAPIAppointment.h"
#include "MAPIFolder.h"
#include "MAPIAttachment.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAPIEx

class MAPIEx {
public:
	MAPIEx();
	~MAPIEx();

// Attributes
public:
	static int cm_bComInit;
	static int cm_nMAPICode;

protected:
	IMAPISession* m_pSession;
	LPMDB m_pMsgStore;
	ULONG m_ulMDBFlags;
	ULONG m_sink;

// Operations
public:
	static bool Init(DWORD dwFlags = 0, bool bInitAsService = false);
	static void Term();
	
	bool Login(const String szProfileName = NULL, DWORD dwFlags = 0, bool bInitAsService = false);
	void Logout();
	bool CreateProfile(String szProfileName);
	bool DeleteProfile(String szProfileName);
	bool CreateMessageService(LPCTSTR szProfileName, LPCTSTR szService, LPCTSTR szDisplayName);

	String GetProfileName();
	String GetProfileEmail();
	int GetMessageStoreCount();
	bool GetMessageStoreProperties(int nIndex, String& strName, bool& bDefaultStore);
	bool OpenMessageStore(int nIndex, ULONG ulFlags=MAPI_MODIFY | MAPI_NO_CACHE);
	bool OpenMessageStore(String szStore=NULL, ULONG ulFlags=MAPI_MODIFY | MAPI_NO_CACHE);

#ifdef _WIN32_WCE
	CPOOM* GetPOOM();
#endif

	bool OpenFolder(const String &folderName, MAPIFolder &folder, bool fullPath = true);
	bool OpenSpecialFolder(unsigned long ulFolderID, MAPIFolder &folder);
	bool OpenRootFolder(MAPIFolder &folder);
	bool OpenInbox(MAPIFolder &folder);
	bool OpenOutbox(MAPIFolder &folder);
	bool OpenSentItems(MAPIFolder &folder);
	bool OpenDeletedItems(MAPIFolder &folder);
	bool OpenContacts(MAPIFolder &folder);
	bool OpenDrafts(MAPIFolder &folder);
	bool OpenCalendar(MAPIFolder &folder);

	bool Notify(LPNOTIFCALLBACK OnNewEvent, LPVOID lpvContext, ULONG ulEventMask);

	bool SelectContacts(Vector<String> &emails, const String caption);
	
protected:
	static void PumpMessages();
	bool GetExEmail(SBinary entryID, String& strEmail);
	static String GetValidString(SPropValue& prop);
	static String GetValidMVString(SPropValue& prop, int nIndex);
	static void GetNarrowString(SPropValue& prop, String& strNarrow);
	static void SetSystemTime(SYSTEMTIME& tm, int wYear, int wMonth, int wDay, int wHour=0, 
											int wMinute=0, int wSecond=0, int wMilliSeconds=0);
	static void SetSystemTime(SYSTEMTIME& tm, const Time &t);
	static Time GetSystemTime(SYSTEMTIME& tm);
	bool CompareEntryIDs(ULONG cb1, LPENTRYID lpb1, ULONG cb2, LPENTRYID lpb2);
	int ShowAddressBook(LPADRLIST& pAddressList, const String caption);
	bool GetEmail(ADRENTRY& adrEntry, String& strEmail);
	static void ReleaseAddressList(LPADRLIST pAddressList);
	static void WINAPI FreeProws(LPSRowSet pRows);
	
	ULONG GetMessageStoreSupport();
	
	bool OpenFolder(unsigned long ulFolderID, MAPIFolder &folder);
public:	////////
	static String ValidateString(LPCTSTR s);
	IMAPISession* GetSession() 	{return m_pSession;}
	LPMDB GetMessageStore() 	{return m_pMsgStore;}
	
	friend class MAPIMessage;
	friend class MAPIObject;
	friend class MAPIFolder;
	friend class MAPIContact;
	friend class MAPIAttachment;
	friend class MAPIAppointment;
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


class MAPIFunctions {
public:	
	MAPIFunctions();
	const void (*CloseIMsgSession)(LPMSGSESS lpMsgSess);
	const void (*FreePadrlist)(LPADRLIST lpAdrlist);
	const LPMALLOC (*MAPIGetDefaultMalloc)(VOID);
	const SCODE (*OpenIMsgSession)(LPMALLOC lpMalloc,ULONG ulFlags,LPMSGSESS *lppMsgSess);
	const SCODE (*OpenIMsgOnIStg)(LPMSGSESS lpMsgSess,LPALLOCATEBUFFER lpAllocateBuffer,LPALLOCATEMORE lpAllocateMore,LPFREEBUFFER lpFreeBuffer,LPMALLOC lpMalloc,LPVOID lpMapiSup,LPSTORAGE lpStg,MSGCALLRELEASE *lpfMsgCallRelease,ULONG ulCallerData,ULONG ulFlags,LPMESSAGE *lppMsg);
	const LPSPropValue (*PpropFindProp)(LPSPropValue lpPropArray,ULONG cValues,ULONG ulPropTag);
	
private:
	Dl dll;
};

MAPIFunctions &MF();
	
#endif
