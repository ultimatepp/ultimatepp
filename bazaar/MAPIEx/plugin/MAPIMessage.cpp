////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIMessage.cpp
// Description: MAPI Message class wrapper
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

#include <IMessage.h>

const GUID CLSID_MailMessage={ 0x00020D0B, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

/////////////////////////////////////////////////////////////
// CMAPIMessage

CMAPIMessage::CMAPIMessage()
{
	m_pRecipients=NULL;
}

CMAPIMessage::~CMAPIMessage()
{
	Close();
}

BOOL CMAPIMessage::Open(CMAPIEx* pMAPI, SBinary entryID)
{
	if(!CMAPIObject::Open(pMAPI,entryID)) return FALSE;

	GetPropertyString(PR_SENDER_NAME, m_strSenderName);
	FillSenderEmail();
	GetPropertyString(PR_SUBJECT, m_strSubject);
	
	return TRUE;
}

void CMAPIMessage::Close()
{
	RELEASE(m_pRecipients);
	CMAPIObject::Close();
}

BOOL CMAPIMessage::GetHeader(CString& strHeader)
{
	return GetPropertyString(PR_TRANSPORT_MESSAGE_HEADERS, strHeader);
}

void CMAPIMessage::FillSenderEmail()
{
	CString strAddrType;
	GetPropertyString(PR_SENDER_ADDRTYPE, strAddrType);
	GetPropertyString(PR_SENDER_EMAIL_ADDRESS, m_strSenderEmail);

	// for Microsoft Exchange server internal mails we want to try to resolve the SMTP email address
	if(strAddrType==_T("EX")) 
	{
		LPSPropValue pProp;
		if(GetProperty(PR_SENDER_ENTRYID, pProp)==S_OK) 
		{
			if(m_pMAPI) m_pMAPI->GetExEmail(pProp->Value.bin, m_strSenderEmail);
			MAPIFreeBuffer(pProp);
		}
	}
}

BOOL CMAPIMessage::GetReceivedTime(SYSTEMTIME& tmReceived)
{
	LPSPropValue pProp;
	if(GetProperty(PR_MESSAGE_DELIVERY_TIME, pProp)==S_OK) 
	{
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &tmReceived);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIMessage::GetReceivedTime(CString& strReceivedTime, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetReceivedTime(tm)) 
	{
		TCHAR szTime[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy hh:mm:ss tt");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szTime, 256);
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szTime, szTime, 256);
		strReceivedTime=szTime;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIMessage::GetSubmitTime(SYSTEMTIME& tmSubmit)
{
	LPSPropValue pProp;
	if(GetProperty(PR_CLIENT_SUBMIT_TIME, pProp)==S_OK) 
	{
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &tmSubmit);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIMessage::GetSubmitTime(CString& strSubmitTime, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetSubmitTime(tm)) 
	{
		TCHAR szTime[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy hh:mm:ss tt");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szTime,256);
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szTime, szTime,256);
		strSubmitTime=szTime;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIMessage::GetTo(CString& strTo)
{
	return GetPropertyString(PR_DISPLAY_TO, strTo);
}

BOOL CMAPIMessage::GetCC(CString& strCC)
{
	return GetPropertyString(PR_DISPLAY_CC, strCC);
}

BOOL CMAPIMessage::GetBCC(CString& strBCC)
{
	return GetPropertyString(PR_DISPLAY_BCC, strBCC);
}

int CMAPIMessage::GetMessageStatus()
{
	return GetPropertyValue(PR_MSG_STATUS, 0);
}

int CMAPIMessage::GetPriority()
{
	return GetPropertyValue(PR_PRIORITY, PRIO_NONURGENT);
}

DWORD CMAPIMessage::GetSize()
{
	return GetPropertyValue(PR_MESSAGE_SIZE, 0);
}

BOOL CMAPIMessage::GetRecipients()
{
	if(!Message()) return FALSE;
	RELEASE(m_pRecipients);

	if(Message()->GetRecipientTable(CMAPIEx::cm_nMAPICode, &m_pRecipients)!=S_OK) return FALSE;

	const int nProperties=RECIPIENT_COLS;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_RECIPIENT_TYPE, PR_DISPLAY_NAME, PR_EMAIL_ADDRESS, PR_ADDRTYPE, PR_ENTRYID }};
	return (m_pRecipients->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK);
}

BOOL CMAPIMessage::GetNextRecipient(CString& strName, CString& strEmail, int& nType)
{
	if(!m_pRecipients) return FALSE;

	LPSRowSet pRows=NULL;
	BOOL bResult=FALSE;
	if(m_pRecipients->QueryRows(1, 0, &pRows)==S_OK) 
	{
		if(pRows->cRows) 
		{
			nType=pRows->aRow[0].lpProps[PROP_RECIPIENT_TYPE].Value.ul;
			strName=CMAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_RECIPIENT_NAME]);

			// for Microsoft Exchange server internal mails we want to try to resolve the SMTP email address
			CString strAddrType=CMAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ADDR_TYPE]);
			if(strAddrType==_T("EX")) 
			{
				if(m_pMAPI) m_pMAPI->GetExEmail(pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin, strEmail);
			} 
			else 
			{
				strEmail=CMAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_RECIPIENT_EMAIL]);
			}
			bResult=TRUE;
		}
		FreeProws(pRows);
	}
	return bResult;
}

BOOL CMAPIMessage::GetReplyTo(CString& strEmail)
{
	BOOL bResult=FALSE;
	LPSPropValue prop;
	if(GetProperty(PR_REPLY_RECIPIENT_ENTRIES, prop) == S_OK)
	{
		LPFLATENTRYLIST pReplyEntryList=(LPFLATENTRYLIST)prop->Value.bin.lpb;
		if(pReplyEntryList->cEntries>0) 
		{
			LPFLATENTRY pReplyEntry=(LPFLATENTRY)pReplyEntryList->abEntries;

			SBinary entryID;
			entryID.cb=pReplyEntry->cb;
			entryID.lpb=pReplyEntry->abEntry;
			bResult=m_pMAPI->GetExEmail(entryID, strEmail);
		}
		MAPIFreeBuffer(prop);
	}
	return bResult;
}

// nPriority defaults to IMPORTANCE_NORMAL, IMPORTANCE_HIGH or IMPORTANCE_LOW also valid
BOOL CMAPIMessage::Create(CMAPIEx* pMAPI, int nPriority, BOOL bSaveToSentFolder, CMAPIFolder* pFolder)
{
	if(!pMAPI) return FALSE;
	if(!pFolder) pFolder=pMAPI->GetFolder();
	if(!pFolder) pFolder=pMAPI->OpenOutbox();
	if(!CMAPIObject::Create(pMAPI, pFolder)) return FALSE;

	SPropValue prop;
	SetMessageFlags(MSGFLAG_UNSENT | MSGFLAG_FROMME);

	LPSPropValue pProp=NULL;
	ULONG cValues=0;
	ULONG rgTags[]={ 1, PR_IPM_SENTMAIL_ENTRYID };

	if(m_pMAPI->GetMessageStore()->GetProps((LPSPropTagArray) rgTags, CMAPIEx::cm_nMAPICode, &cValues, &pProp)==S_OK) 
	{
		if(bSaveToSentFolder)
		{
			prop.ulPropTag=PR_SENTMAIL_ENTRYID;
			prop.Value.bin=pProp[0].Value.bin;
		}
		else
		{
			prop.ulPropTag=PR_DELETE_AFTER_SUBMIT;
			prop.Value.b=TRUE;
		}
		Message()->SetProps(1, &prop, NULL);
		SetEntryID(&(pProp[0].Value.bin));
		MAPIFreeBuffer(pProp);
	}

	if(nPriority!=IMPORTANCE_NORMAL) 
	{
		prop.ulPropTag=PR_IMPORTANCE;
		prop.Value.l=nPriority;
		Message()->SetProps(1, &prop, NULL);
	}

	SetMessageClass(_T("IPM.Note"));

#ifdef _WIN32_WCE
	prop.ulPropTag=PR_MSG_STATUS;
	prop.Value.ul=MSGSTATUS_RECTYPE_SMTP;
	Message()->SetProps(1, &prop, NULL);
#endif

	return TRUE;
}

BOOL CMAPIMessage::IsUnread()
{
	return (!(GetMessageFlags()&MSGFLAG_READ));
}

BOOL CMAPIMessage::MarkAsRead(BOOL bRead)
{
#ifdef _WIN32_WCE
	int ulMessageFlags=GetMessageFlags();	
	if(bRead) ulMessageFlags|=MSGFLAG_READ;	
	else ulMessageFlags&=~MSGFLAG_READ;	
	return SetMessageFlags(ulMessageFlags);
#else
	return (Message()->SetReadFlag(bRead ? MSGFLAG_READ : CLEAR_READ_FLAG)==S_OK);
#endif
}

BOOL CMAPIMessage::AddRecipients(LPADRLIST pAddressList)
{
	HRESULT hr=E_INVALIDARG;
#ifdef _WIN32_WCE
	hr=Message()->ModifyRecipients(MODRECIP_ADD, pAddressList);
#else
	LPADRBOOK pAddressBook;
	if(m_pMAPI->GetSession()->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook)!=S_OK) return FALSE;

	if(pAddressBook->ResolveName(0, CMAPIEx::cm_nMAPICode, NULL, pAddressList)==S_OK) hr=Message()->ModifyRecipients(MODRECIP_ADD, pAddressList);
	RELEASE(pAddressBook);
#endif
	return (hr==S_OK);
}

// AddrType only needed by Windows CE, use SMTP, or SMS etc, default NULL will not set PR_ADDRTYPE
BOOL CMAPIMessage::AddRecipient(LPCTSTR szEmail, int nType, LPCTSTR szAddrType)
{
	if(!Message() || !m_pMAPI) return FALSE;

	int nBufSize=CbNewADRLIST(1);
	LPADRLIST pAddressList=NULL;
	MAPIAllocateBuffer(nBufSize, (LPVOID FAR*)&pAddressList);
	memset(pAddressList, 0, nBufSize);

	int nProperties=3;
	if(szAddrType==NULL) nProperties--;
	pAddressList->cEntries=1;

	pAddressList->aEntries[0].ulReserved1=0;
	pAddressList->aEntries[0].cValues=nProperties;

	MAPIAllocateBuffer(sizeof(SPropValue)*nProperties, (LPVOID FAR*)&pAddressList->aEntries[0].rgPropVals);
	memset(pAddressList->aEntries[0].rgPropVals, 0, sizeof(SPropValue)*nProperties);

	pAddressList->aEntries[0].rgPropVals[0].ulPropTag=PR_RECIPIENT_TYPE;
	pAddressList->aEntries[0].rgPropVals[0].Value.ul=nType;

#ifdef _WIN32_WCE
	pAddressList->aEntries[0].rgPropVals[1].ulPropTag=PR_EMAIL_ADDRESS;
	pAddressList->aEntries[0].rgPropVals[1].Value.LPSZ=(TCHAR*)szEmail;
#else
	pAddressList->aEntries[0].rgPropVals[1].ulPropTag=PR_DISPLAY_NAME;
	pAddressList->aEntries[0].rgPropVals[1].Value.LPSZ=(TCHAR*)szEmail;
#endif

	if(szAddrType!=NULL) 
	{
		pAddressList->aEntries[0].rgPropVals[2].ulPropTag=PR_ADDRTYPE;
		pAddressList->aEntries[0].rgPropVals[2].Value.LPSZ=(TCHAR*)szAddrType;
	}

	BOOL bResult=AddRecipients(pAddressList);
	CMAPIEx::ReleaseAddressList(pAddressList);
	return bResult;
}

void CMAPIMessage::SetSubject(LPCTSTR szSubject)
{
	m_strSubject=szSubject;
	SetPropertyString(PR_SUBJECT, szSubject);
}

void CMAPIMessage::SetSender(LPCTSTR szSenderName, LPCTSTR szSenderEmail)
{
	m_strSenderName=szSenderName;
	m_strSenderEmail=szSenderEmail;
	LPTSTR szAddrType=_T("SMTP");
	if(m_strSenderName.GetLength() && m_strSenderEmail.GetLength()) 
	{
		SetPropertyString(PR_SENDER_NAME, szSenderName);
		SetPropertyString(PR_SENDER_EMAIL_ADDRESS, szSenderEmail);

#ifndef _WIN32_WCE
		LPADRBOOK pAddressBook;
		if(m_pMAPI->GetSession()->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook)==S_OK) 
		{
			SPropValue prop;
			if(pAddressBook->CreateOneOff((LPTSTR)szSenderName, szAddrType, (LPTSTR)szSenderEmail, 0, &prop.Value.bin.cb, (LPENTRYID*)&prop.Value.bin.lpb)==S_OK)
			{
				prop.ulPropTag=PR_SENT_REPRESENTING_ENTRYID;
				Message()->SetProps(1, &prop, NULL);

				SetPropertyString(PR_SENT_REPRESENTING_NAME, szSenderName);
				SetPropertyString(PR_SENT_REPRESENTING_EMAIL_ADDRESS, szSenderEmail);
				SetPropertyString(PR_SENT_REPRESENTING_ADDRTYPE, szAddrType);
			}
		}
		RELEASE(pAddressBook);
#endif
	}
}

BOOL CMAPIMessage::SetReceivedTime(SYSTEMTIME tmReceived, BOOL bLocalTime)
{
	SPropValue prop;
	prop.ulPropTag=PR_MESSAGE_DELIVERY_TIME;
#ifndef _WIN32_WCE
	if(bLocalTime) TzSpecificLocalTimeToSystemTime(NULL, &tmReceived, &tmReceived);
#endif
	SystemTimeToFileTime(&tmReceived, &prop.Value.ft);
	return (Message() && Message()->SetProps(1, &prop, NULL)==S_OK);
}

BOOL CMAPIMessage::SetSubmitTime(SYSTEMTIME tmSubmit, BOOL bLocalTime)
{
	SPropValue prop;
	prop.ulPropTag=PR_CLIENT_SUBMIT_TIME;
#ifndef _WIN32_WCE
	if(bLocalTime) TzSpecificLocalTimeToSystemTime(NULL, &tmSubmit, &tmSubmit);
#endif
	SystemTimeToFileTime(&tmSubmit, &prop.Value.ft);
	return (Message() && Message()->SetProps(1, &prop, NULL)==S_OK);
}

// request a Read Receipt sent to szReceiverEmail 
BOOL CMAPIMessage::SetReadReceipt(BOOL bSet, LPCTSTR szReceiverEmail)
{
	if(!Message()) return FALSE;

	SPropValue prop;
	prop.ulPropTag=PR_READ_RECEIPT_REQUESTED;
	prop.Value.b=(unsigned short)bSet;
	if(Message()->SetProps(1, &prop, NULL)!=S_OK) return FALSE;

	if(bSet && szReceiverEmail && _tcslen(szReceiverEmail)>0) SetPropertyString(PR_READ_RECEIPT_REQUESTED, szReceiverEmail);
	return TRUE;
}

BOOL CMAPIMessage::SetDeliveryReceipt(BOOL bSet)
{
	if(!Message()) return FALSE;

	SPropValue prop;
	prop.ulPropTag=PR_ORIGINATOR_DELIVERY_REPORT_REQUESTED;
	prop.Value.b=(unsigned short)bSet;
	return (Message()->SetProps(1, &prop, NULL)!=S_OK);
}

// limited compare, compares entry IDs and subject to determine if two emails are equal
BOOL CMAPIMessage::operator==(CMAPIMessage& message)
{
	if(!m_entryID.cb || !message.m_entryID.cb || m_entryID.cb!=message.m_entryID.cb) return FALSE;
	if(memcmp(m_entryID.lpb,message.m_entryID.lpb, m_entryID.cb)) return FALSE;
	return (!m_strSubject.Compare(message.m_strSubject));
}

// Novell GroupWise customization by jcadmin
#ifndef GROUPWISE
BOOL CMAPIMessage::MarkAsPrivate() { return FALSE; }
#else
#include GWMAPI.h
//(from Novell Developer Kit)
#define SEND_OPTIONS_MARK_PRIVATE 0x00080000L

void CMAPIMessage::MarkAsPrivate() 
{
	SPropValue prop;
	prop.ulPropTag=PR_NGW_SEND_OPTIONS;
	prop.Value.l=NGW_SEND_OPTIONS_MARK_PRIVATE;
	return (Message()->SetProps(1, &prop, NULL)==S_OK);
}
#endif

// In WinCE, use MSGSTATUS_RECTYPE_SMS for an SMS, MSGSTATUS_RECTYPE_SMTP for an email
BOOL CMAPIMessage::SetMessageStatus(int nMessageStatus)
{
	SPropValue prop;
	prop.ulPropTag=PR_MSG_STATUS;
	prop.Value.ul=nMessageStatus;
	return (Message()->SetProps(1, &prop, NULL)==S_OK);
}

// Shows the default MAPI form for IMessage, returns FALSE on failure, IDOK on success or close existing messages 
// and IDCANCEL on close new messages
int CMAPIMessage::ShowForm(CMAPIEx* pMAPI)
{
	CMAPIFolder* pFolder=pMAPI->GetFolder();
	IMAPISession* pSession=pMAPI->GetSession();
	ULONG ulMessageToken;

	if(pFolder && pSession && pSession->PrepareForm(NULL,Message(), &ulMessageToken)==S_OK) 
	{
		ULONG ulMessageStatus=GetPropertyValue(PR_MSG_STATUS, 0);
		ULONG ulMessageFlags=GetMessageFlags();
		ULONG ulAccess=GetPropertyValue(PR_ACCESS, 0);

		LPSPropValue pProp;
		if(GetProperty(PR_MESSAGE_CLASS, pProp)==S_OK) 
		{
#ifdef UNICODE
			char szMessageClass[256];
			WideCharToMultiByte(CP_ACP, 0, pProp->Value.LPSZ,-1, szMessageClass,255, NULL, NULL);
#else
			char* szMessageClass=pProp->Value.LPSZ;
#endif
			HRESULT hr=pSession->ShowForm(NULL, pMAPI->GetMessageStore(), pFolder->Folder(), NULL,ulMessageToken, NULL, 0,ulMessageStatus,ulMessageFlags,ulAccess, szMessageClass);
			MAPIFreeBuffer(pProp);
			if(hr==S_OK) return IDOK;
			if(hr==MAPI_E_USER_CANCEL) return IDCANCEL;
		}
	}
	return FALSE;
}

BOOL CMAPIMessage::Send()
{
	if(Message() && Message()->SubmitMessage(0)==S_OK) 
	{
		Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIMessage::SaveAsMsg(LPCTSTR szPath)
{
	CString strPath=szPath;
	if(strPath.GetLength()<4 || strPath.Right(4).CompareNoCase(".msg")) strPath+=".msg";
#ifdef _WIN32_WCE
		return FALSE;
#else
	LPSTORAGE pStorage;
	DWORD dwFlags=STGM_READWRITE | STGM_TRANSACTED | STGM_CREATE;
#ifdef UNICODE
	if(::StgCreateDocfile(strPath, dwFlags, 0, &pStorage) == S_OK)
#else
	WCHAR wszPath[_MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strPath, -1, wszPath, _MAX_PATH);
	if(StgCreateDocfile(wszPath, dwFlags, 0, &pStorage) == S_OK)
#endif
	{
		LPMSGSESS pMsgSession;
		LPMALLOC pMalloc = MAPIGetDefaultMalloc();
		if(OpenIMsgSession(pMalloc, 0, &pMsgSession) == S_OK) {
			LPMESSAGE pIMsg;
			if(OpenIMsgOnIStg(pMsgSession, MAPIAllocateBuffer, MAPIAllocateMore, MAPIFreeBuffer, pMalloc, NULL, pStorage, NULL, 0, 0, &pIMsg) == S_OK) {
				// client must support CLSID_MailMessage as the compound document
				if(WriteClassStg(pStorage, CLSID_MailMessage) == S_OK) {
					// exclude these properties from the copy operation to speed things up
					SizedSPropTagArray ( 7, excludeTags );
					excludeTags.cValues=7;
					excludeTags.aulPropTag[0]=PR_ACCESS;
					excludeTags.aulPropTag[1]=PR_BODY;
					excludeTags.aulPropTag[2]=PR_RTF_SYNC_BODY_COUNT;
					excludeTags.aulPropTag[3]=PR_RTF_SYNC_BODY_CRC;
					excludeTags.aulPropTag[4]=PR_RTF_SYNC_BODY_TAG;
					excludeTags.aulPropTag[5]=PR_RTF_SYNC_PREFIX_COUNT;
					excludeTags.aulPropTag[6]=PR_RTF_SYNC_TRAILING_COUNT;

					if(Message()->CopyTo(0, NULL, (LPSPropTagArray)&excludeTags, NULL, NULL, (LPIID)&IID_IMessage, pIMsg, 0, NULL ) == S_OK) {
						pIMsg->SaveChanges(KEEP_OPEN_READWRITE);
						pStorage->Commit(STGC_DEFAULT);
					}
				}
				RELEASE(pIMsg);
			}
			CloseIMsgSession(pMsgSession);
		}
		RELEASE(pStorage);
		return TRUE;
	}
	return FALSE;
#endif
}
