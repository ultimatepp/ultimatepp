#ifdef _WIN32

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

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"

const GUID CLSID_MailMessage = {0x00020D0B, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};

#ifndef _WIN32_WCE
#define INITGUID
#define USES_IID_IMessage
#include <InitGuid.h>
#include <MAPIGuid.h>
#endif

  					
/////////////////////////////////////////////////////////////
// MAPIMessage

MAPIMessage::MAPIMessage() {
	m_pRecipients = NULL;
}

MAPIMessage::~MAPIMessage() {
	Close();
}

bool MAPIMessage::Open(MAPIEx* pMAPI, SBinary entryID) {
	if(!MAPIObject::Open(pMAPI,entryID)) 
		return false;

	m_strSenderName = GetPropertyString(PR_SENDER_NAME);
	FillSenderEmail();
	m_strSubject = GetPropertyString(PR_SUBJECT);
	
	return true;
}

void MAPIMessage::Close() {
	RELEASE(m_pRecipients);
	MAPIObject::Close();
}

String MAPIMessage::GetHeader() {
	return GetPropertyString(PR_TRANSPORT_MESSAGE_HEADERS);
}

void MAPIMessage::FillSenderEmail() {
	String strAddrType = GetPropertyString(PR_SENDER_ADDRTYPE);
	m_strSenderEmail = GetPropertyString(PR_SENDER_EMAIL_ADDRESS);

	// for Microsoft Exchange server internal mails we want to try to resolve the SMTP email address
	if(strAddrType == "EX") {
		LPSPropValue pProp;
		if(GetProperty(PR_SENDER_ENTRYID, pProp)==S_OK) {
			if(m_pMAPI) 
				m_pMAPI->GetExEmail(pProp->Value.bin, m_strSenderEmail);
			MAPIFreeBuffer(pProp);
		}
	}
}

Time MAPIMessage::GetTime(ULONG property) {
	SYSTEMTIME st;
	LPSPropValue pProp;
	if(GetProperty(property, pProp) == S_OK) {
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &st);
		MAPIFreeBuffer(pProp);
		return MAPIEx::GetSystemTime(st);
	}
	return Null;
}

String MAPIMessage::GetTo() {
	return GetPropertyString(PR_DISPLAY_TO);
}

String MAPIMessage::GetCC() {
	return GetPropertyString(PR_DISPLAY_CC);
}

String MAPIMessage::GetBCC() {
	return GetPropertyString(PR_DISPLAY_BCC);
}

int MAPIMessage::GetMessageStatus() {
	return GetPropertyValue(PR_MSG_STATUS, 0);
}

int MAPIMessage::GetPriority() {
	return GetPropertyValue(PR_PRIORITY, PRIO_NONURGENT);
}

DWORD MAPIMessage::GetSize() {
	return GetPropertyValue(PR_MESSAGE_SIZE, 0);
}

bool MAPIMessage::GetRecipients() {
	if(!Message()) 
		return false;
	RELEASE(m_pRecipients);

	if(Message()->GetRecipientTable(MAPIEx::cm_nMAPICode, &m_pRecipients) != S_OK) 
		return false;

	const int nProperties = RECIPIENT_COLS;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_RECIPIENT_TYPE, PR_DISPLAY_NAME, PR_EMAIL_ADDRESS, PR_ADDRTYPE, PR_ENTRYID }};
	return (m_pRecipients->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK);
}

bool MAPIMessage::GetNextRecipient(String& strName, String& strEmail, int &nType) {
	if(!m_pRecipients) 
		return false;

	LPSRowSet pRows = NULL;
	bool bResult = false;
	if(m_pRecipients->QueryRows(1, 0, &pRows) == S_OK) {
		if(pRows->cRows) {
			nType = pRows->aRow[0].lpProps[PROP_RECIPIENT_TYPE].Value.ul;
			strName = MAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_RECIPIENT_NAME]);

			// for Microsoft Exchange server internal mails we want to try to resolve the SMTP email address
			String strAddrType = MAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ADDR_TYPE]);
			if(strAddrType == "EX") {
				if(m_pMAPI) 
					m_pMAPI->GetExEmail(pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin, strEmail);
			} else 
				strEmail = MAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_RECIPIENT_EMAIL]);
			bResult = true;
		}
		MAPIEx::FreeProws(pRows);
	}
	return bResult;
}

String MAPIMessage::GetReplyTo() {
	String strEmail;
	bool bResult = false;
	LPSPropValue prop;
	if(GetProperty(PR_REPLY_RECIPIENT_ENTRIES, prop) == S_OK) {
		LPFLATENTRYLIST pReplyEntryList = (LPFLATENTRYLIST)prop->Value.bin.lpb;
		if(pReplyEntryList->cEntries > 0) {
			LPFLATENTRY pReplyEntry = (LPFLATENTRY)pReplyEntryList->abEntries;

			SBinary entryID;
			entryID.cb = pReplyEntry->cb;
			entryID.lpb = pReplyEntry->abEntry;
			bResult = m_pMAPI->GetExEmail(entryID, strEmail);
		}
		MAPIFreeBuffer(prop);
	}
	if (bResult)
		return strEmail;
	else
		return String();
}

// nPriority defaults to IMPORTANCE_NORMAL, IMPORTANCE_HIGH or IMPORTANCE_LOW also valid
bool MAPIMessage::Create(MAPIEx &mapi, MAPIFolder &folder, int nPriority, bool bSaveToSentFolder) {
	if(!MAPIObject::Create(mapi, folder)) 
		return false;

	SPropValue prop;
	SetMessageFlags(MSGFLAG_UNSENT | MSGFLAG_FROMME);

	LPSPropValue pProp = NULL;
	ULONG cValues = 0;
	ULONG rgTags[] = { 1, PR_IPM_SENTMAIL_ENTRYID };

	if(m_pMAPI->GetMessageStore()->GetProps((LPSPropTagArray) rgTags, MAPIEx::cm_nMAPICode, &cValues, &pProp)==S_OK) {
		if(bSaveToSentFolder) {
			prop.ulPropTag = PR_SENTMAIL_ENTRYID;
			prop.Value.bin = pProp[0].Value.bin;
		} else {
			prop.ulPropTag = PR_DELETE_AFTER_SUBMIT;
			prop.Value.b = true;
		}
		Message()->SetProps(1, &prop, NULL);
		SetEntryID(&(pProp[0].Value.bin));
		MAPIFreeBuffer(pProp);
	}

	if(nPriority != IMPORTANCE_NORMAL) {
		prop.ulPropTag = PR_IMPORTANCE;
		prop.Value.l = nPriority;
		Message()->SetProps(1, &prop, NULL);
	}
	SetMessageClass("IPM.Note");

#ifdef _WIN32_WCE
	prop.ulPropTag = PR_MSG_STATUS;
	prop.Value.ul = MSGSTATUS_RECTYPE_SMTP;
	Message()->SetProps(1, &prop, NULL);
#endif

	return true;
}

bool MAPIMessage::IsUnread() {
	return (!(GetMessageFlags()&MSGFLAG_READ));
}

bool MAPIMessage::MarkAsRead(bool bRead) {
#ifdef _WIN32_WCE
	int ulMessageFlags = GetMessageFlags();	
	if(bRead) ulMessageFlags |= MSGFLAG_READ;	
	else ulMessageFlags &= ~MSGFLAG_READ;	
	return SetMessageFlags(ulMessageFlags);
#else
	return (Message()->SetReadFlag(bRead ? MSGFLAG_READ : CLEAR_READ_FLAG)==S_OK);
#endif
}

bool MAPIMessage::AddRecipients(LPADRLIST pAddressList) {
	HRESULT hr = E_INVALIDARG;
#ifdef _WIN32_WCE
	hr = Message()->ModifyRecipients(MODRECIP_ADD, pAddressList);
#else
	LPADRBOOK pAddressBook;
	if(m_pMAPI->GetSession()->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook) != S_OK) 
		return false;

	if(pAddressBook->ResolveName(0, MAPIEx::cm_nMAPICode, NULL, pAddressList) == S_OK) 
		hr = Message()->ModifyRecipients(MODRECIP_ADD, pAddressList);
	RELEASE(pAddressBook);
#endif
	return (hr == S_OK);
}

// AddrType only needed by Windows CE, use SMTP, or SMS etc, default NULL will not set PR_ADDRTYPE
bool MAPIMessage::AddRecipient(const String &email, int nType, const char* szAddrType) {
	if(!Message() || !m_pMAPI) 
		return false;

	int nBufSize=CbNewADRLIST(1);
	LPADRLIST pAddressList = NULL;
	MAPIAllocateBuffer(nBufSize, (LPVOID FAR*)&pAddressList);
	memset(pAddressList, 0, nBufSize);

	int nProperties = 3;
	if(szAddrType==NULL) 
		nProperties--;
	pAddressList->cEntries=1;

	pAddressList->aEntries[0].ulReserved1 = 0;
	pAddressList->aEntries[0].cValues=nProperties;

	MAPIAllocateBuffer(sizeof(SPropValue)*nProperties, (LPVOID FAR*)&pAddressList->aEntries[0].rgPropVals);
	memset(pAddressList->aEntries[0].rgPropVals, 0, sizeof(SPropValue)*nProperties);

	pAddressList->aEntries[0].rgPropVals[0].ulPropTag = PR_RECIPIENT_TYPE;
	pAddressList->aEntries[0].rgPropVals[0].Value.ul = nType;

#ifdef _WIN32_WCE
	pAddressList->aEntries[0].rgPropVals[1].ulPropTag = PR_EMAIL_ADDRESS;
	pAddressList->aEntries[0].rgPropVals[1].Value.LPSZ = (TCHAR*)szEmail;
#else
	pAddressList->aEntries[0].rgPropVals[1].ulPropTag = PR_DISPLAY_NAME;
	pAddressList->aEntries[0].rgPropVals[1].Value.LPSZ = (TCHAR*)(email.Begin());
#endif

	if(szAddrType != NULL) {
		pAddressList->aEntries[0].rgPropVals[2].ulPropTag = PR_ADDRTYPE;
		pAddressList->aEntries[0].rgPropVals[2].Value.LPSZ = (TCHAR*)szAddrType;
	}

	bool bResult = AddRecipients(pAddressList);
	MAPIEx::ReleaseAddressList(pAddressList);
	return bResult;
}

void MAPIMessage::SetSubject(const String &subject) {
	m_strSubject = subject;
	SetPropertyString(PR_SUBJECT, subject);
}

void MAPIMessage::SetSender(const String &senderName, const String &senderEmail) {
	m_strSenderName = senderName;
	m_strSenderEmail = senderEmail;
	LPTSTR szAddrType = (TCHAR*)"SMTP";
	if(m_strSenderName.GetLength() && m_strSenderEmail.GetLength()) {
		SetPropertyString(PR_SENDER_NAME, senderName);
		SetPropertyString(PR_SENDER_EMAIL_ADDRESS, senderEmail);

#ifndef _WIN32_WCE
		LPADRBOOK pAddressBook;
		if(m_pMAPI->GetSession()->OpenAddressBook(0, NULL, AB_NO_DIALOG, &pAddressBook) == S_OK) {
			SPropValue prop;
			if(pAddressBook->CreateOneOff((LPTSTR)(senderName.Begin()), szAddrType, 
										(LPTSTR)(senderEmail.Begin()), 0, &prop.Value.bin.cb, 
										(LPENTRYID*)&prop.Value.bin.lpb) == S_OK) {
				prop.ulPropTag=PR_SENT_REPRESENTING_ENTRYID;
				Message()->SetProps(1, &prop, NULL);

				SetPropertyString(PR_SENT_REPRESENTING_NAME, senderName);
				SetPropertyString(PR_SENT_REPRESENTING_EMAIL_ADDRESS, senderEmail);
				SetPropertyString(PR_SENT_REPRESENTING_ADDRTYPE, szAddrType);
			}
		}
		RELEASE(pAddressBook);
#endif
	}
}

bool MAPIMessage::SetReceivedTime(const Time &tm) {
	SYSTEMTIME tmReceived;
	MAPIEx::SetSystemTime(tmReceived, tm);
	
	SPropValue prop;
	prop.ulPropTag = PR_MESSAGE_DELIVERY_TIME;
#ifndef _WIN32_WCE
//	if(bLocalTime) TzSpecificLocalTimeToSystemTime(NULL, &tmReceived, &tmReceived);
#endif
	SystemTimeToFileTime(&tmReceived, &prop.Value.ft);
	return (Message() && Message()->SetProps(1, &prop, NULL) == S_OK);
}

bool MAPIMessage::SetSubmitTime(const Time &tm) {
	SYSTEMTIME tmSubmit;
	MAPIEx::SetSystemTime(tmSubmit, tm);
	
	SPropValue prop;
	prop.ulPropTag = PR_CLIENT_SUBMIT_TIME;
#ifndef _WIN32_WCE
//	if(bLocalTime) TzSpecificLocalTimeToSystemTime(NULL, &tmSubmit, &tmSubmit);
#endif
	SystemTimeToFileTime(&tmSubmit, &prop.Value.ft);
	return (Message() && Message()->SetProps(1, &prop, NULL) == S_OK);
}

// request a Read Receipt sent to szReceiverEmail 
bool MAPIMessage::SetReadReceipt(bool bSet, String szReceiverEmail) {
	if(!Message()) 
		return false;

	SPropValue prop;
	prop.ulPropTag = PR_READ_RECEIPT_REQUESTED;
	prop.Value.b = (unsigned short)bSet;
	if(Message()->SetProps(1, &prop, NULL) != S_OK) 
		return false;

	if(bSet && !IsNull(szReceiverEmail) && szReceiverEmail.GetLength() > 0) 
		SetPropertyString(PR_READ_RECEIPT_REQUESTED, szReceiverEmail);
	return true;
}

bool MAPIMessage::SetDeliveryReceipt(bool bSet) {
	if(!Message()) 
		return false;

	SPropValue prop;
	prop.ulPropTag = PR_ORIGINATOR_DELIVERY_REPORT_REQUESTED;
	prop.Value.b = (unsigned short)bSet;
	return (Message()->SetProps(1, &prop, NULL) != S_OK);
}

// limited compare, compares entry IDs and subject to determine if two emails are equal
bool MAPIMessage::operator==(MAPIMessage& message) {
	if(!m_entryID.cb || !message.m_entryID.cb || m_entryID.cb!=message.m_entryID.cb) 
		return false;
	if(memcmp(m_entryID.lpb,message.m_entryID.lpb, m_entryID.cb)) 
		return false;
	return (!m_strSubject.Compare(message.m_strSubject));
}

// Novell GroupWise customization by jcadmin
#ifndef GROUPWISE
bool MAPIMessage::MarkAsPrivate() { return false; }
#else
#include GWMAPI.h
//(from Novell Developer Kit)
#define SEND_OPTIONS_MARK_PRIVATE 0x00080000L

void MAPIMessage::MarkAsPrivate()  {
	SPropValue prop;
	prop.ulPropTag = PR_NGW_SEND_OPTIONS;
	prop.Value.l = NGW_SEND_OPTIONS_MARK_PRIVATE;
	return (Message()->SetProps(1, &prop, NULL) == S_OK);
}
#endif

// In WinCE, use MSGSTATUS_RECTYPE_SMS for an SMS, MSGSTATUS_RECTYPE_SMTP for an email
bool MAPIMessage::SetMessageStatus(int nMessageStatus) {
	SPropValue prop;
	prop.ulPropTag = PR_MSG_STATUS;
	prop.Value.ul = nMessageStatus;
	return (Message()->SetProps(1, &prop, NULL) == S_OK);
}

// Shows the default MAPI form for IMessage, returns false on failure, IDOK on success or close existing messages 
// and IDCANCEL on close new messages
int MAPIMessage::ShowForm(MAPIEx* pMAPI, MAPIFolder &folder) {
	//MAPIFolder* pFolder=pMAPI->GetFolder();
	IMAPISession* pSession = pMAPI->GetSession();
	ULONG ulMessageToken;

	if(folder.IsOpened() && pSession && pSession->PrepareForm(NULL,Message(), &ulMessageToken) == S_OK) {
		ULONG ulMessageStatus=GetPropertyValue(PR_MSG_STATUS, 0);
		ULONG ulMessageFlags=GetMessageFlags();
		ULONG ulAccess=GetPropertyValue(PR_ACCESS, 0);

		LPSPropValue pProp;
		if(GetProperty(PR_MESSAGE_CLASS, pProp) == S_OK) {
			char* szMessageClass = pProp->Value.LPSZ;
			HRESULT hr = pSession->ShowForm(0, pMAPI->GetMessageStore(), folder.Folder(), 
							NULL, ulMessageToken, NULL, 0, ulMessageStatus,ulMessageFlags,
							ulAccess, szMessageClass);
			MAPIFreeBuffer(pProp);
			if(hr == S_OK) 
				return IDOK;
			if(hr == MAPI_E_USER_CANCEL) 
				return IDCANCEL;
		}
	}
	return false;
}

bool MAPIMessage::Send() {
	if(Message()) {
		HRESULT ret = Message()->SubmitMessage(0);
		if (ret == S_OK) {
			Close();
			return true;
		} else
			return false;
	}
	return false;
}

bool MAPIMessage::SaveToFile(const String &fileName) {
	if (!RealizePath(fileName))
		return false;
	
#ifdef _WIN32_WCE
	return false;
#else
	LPSTORAGE pStorage;
	DWORD dwFlags = STGM_READWRITE | STGM_TRANSACTED | STGM_CREATE;
	if(StgCreateDocfile(fileName.ToWString(), dwFlags, 0, &pStorage) != S_OK) 
		return false;
	LPMSGSESS pMsgSession;
	LPMALLOC pMalloc = MF().MAPIGetDefaultMalloc();
	if(MF().OpenIMsgSession(pMalloc, 0, &pMsgSession) == S_OK) {
		LPMESSAGE pIMsg;
		if(MF().OpenIMsgOnIStg(pMsgSession, MAPIAllocateBuffer, MAPIAllocateMore, MAPIFreeBuffer, 
										pMalloc, NULL, pStorage, NULL, 0, 0, &pIMsg) == S_OK) {
			// client must support CLSID_MailMessage as the compound document
			if(WriteClassStg(pStorage, CLSID_MailMessage) == S_OK) {
				// exclude these properties from the copy operation to speed things up
				SizedSPropTagArray (7, excludeTags);
				excludeTags.cValues = 7;
				excludeTags.aulPropTag[0] = PR_ACCESS;
				excludeTags.aulPropTag[1] = PR_BODY;
				excludeTags.aulPropTag[2] = PR_RTF_SYNC_BODY_COUNT;
				excludeTags.aulPropTag[3] = PR_RTF_SYNC_BODY_CRC;
				excludeTags.aulPropTag[4] = PR_RTF_SYNC_BODY_TAG;
				excludeTags.aulPropTag[5] = PR_RTF_SYNC_PREFIX_COUNT;
				excludeTags.aulPropTag[6] = PR_RTF_SYNC_TRAILING_COUNT;

				if(Message()->CopyTo(0, NULL, (LPSPropTagArray)&excludeTags, 0, NULL, 
											(LPIID)&IID_IMessage, pIMsg, 0, NULL ) == S_OK) {
					pIMsg->SaveChanges(KEEP_OPEN_READWRITE);
					pStorage->Commit(STGC_DEFAULT);
				}
			}
			RELEASE(pIMsg);
		}
		MF().CloseIMsgSession(pMsgSession);
	}
	RELEASE(pStorage);
	return true;
#endif
}

#endif