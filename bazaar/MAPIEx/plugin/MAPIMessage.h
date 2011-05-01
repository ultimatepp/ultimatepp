#ifndef __MAPIMESSAGE_H__
#define __MAPIMESSAGE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIMessage.h
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

class CMAPIEx;

/////////////////////////////////////////////////////////////
// CMAPIMessage

class AFX_EXT_CLASS CMAPIMessage : public CMAPIObject
{
public:
	CMAPIMessage();
	~CMAPIMessage();

	enum { PROP_RECIPIENT_TYPE, PROP_RECIPIENT_NAME, PROP_RECIPIENT_EMAIL, PROP_ADDR_TYPE, PROP_ENTRYID, RECIPIENT_COLS };

// Attributes
protected:
	CString m_strSenderName;
	CString m_strSenderEmail;
	CString m_strSubject;
	CString m_strBody;
	CString m_strRTF;

	LPMAPITABLE m_pRecipients;

// Operations
public:
	virtual BOOL Open(CMAPIEx* pMAPI,SBinary entry);
	virtual void Close();

	BOOL IsUnread();
	BOOL MarkAsRead(BOOL bRead=TRUE);
	BOOL Create(CMAPIEx* pMAPI, int nPriority=IMPORTANCE_NORMAL, BOOL bSaveToSentFolder=TRUE, CMAPIFolder* pFolder=NULL); 
	int ShowForm(CMAPIEx* pMAPI);
	BOOL Send();

	BOOL GetHeader(CString& strHeader);
	LPCTSTR GetSenderName() { return m_strSenderName; }
	LPCTSTR GetSenderEmail() { return m_strSenderEmail; }
	LPCTSTR GetSubject() { return m_strSubject; }
	BOOL GetReceivedTime(SYSTEMTIME& tmReceived);
	BOOL GetReceivedTime(CString& strReceivedTime, LPCTSTR szFormat=NULL); // NULL defaults to "MM/dd/yyyy hh:mm:ss tt"
	BOOL GetSubmitTime(SYSTEMTIME& tmSubmit);
	BOOL GetSubmitTime(CString& strSubmitTime, LPCTSTR szFormat=NULL);
	BOOL GetTo(CString& strTo);
	BOOL GetCC(CString& strCC);
	BOOL GetBCC(CString& strBCC);
	int GetSensitivity();
	int GetMessageStatus();
	int GetPriority();
	DWORD GetSize();

	BOOL GetRecipients();
	BOOL GetNextRecipient(CString& strName, CString& strEmail, int& nType);
	BOOL GetReplyTo(CString& strEmail);

	BOOL AddRecipients(LPADRLIST pAddressList);
	BOOL AddRecipient(LPCTSTR szEmail, int nType=MAPI_TO, LPCTSTR szAddrType=NULL);  // MAPI_CC and MAPI_BCC also valid
	void SetSubject(LPCTSTR szSubject);
	void SetSender(LPCTSTR szSenderName, LPCTSTR szSenderEmail);
	BOOL SetReceivedTime(SYSTEMTIME tmReceived, BOOL bLocalTime=FALSE);
	BOOL SetSubmitTime(SYSTEMTIME tmSubmit, BOOL bLocalTime=FALSE);
	BOOL SetReadReceipt(BOOL bSet=TRUE, LPCTSTR szReceiverEmail=NULL);
	BOOL SetDeliveryReceipt(BOOL bSet=TRUE);
	BOOL MarkAsPrivate();
	BOOL SetMessageStatus(int nMessageStatus);

	// compares entryID and subject only
	BOOL operator==(CMAPIMessage& message);

	BOOL SaveAsMsg(LPCTSTR szPath);

protected:
	void FillSenderEmail();
};

#endif
