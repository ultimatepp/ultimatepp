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

// Ported to U++ Framework by Koldo. See License.txt file

class MAPIEx;

/////////////////////////////////////////////////////////////
// MAPIMessage

class MAPIMessage : public MAPIObject {
public:
	MAPIMessage();
	~MAPIMessage();

	enum { PROP_RECIPIENT_TYPE, PROP_RECIPIENT_NAME, PROP_RECIPIENT_EMAIL, PROP_ADDR_TYPE, PROP_ENTRYID, RECIPIENT_COLS };

// Attributes
protected:
	String m_strSenderName;
	String m_strSenderEmail;
	String m_strSubject;
	String m_strBody;
	String m_strRTF;

	LPMAPITABLE m_pRecipients;

// Operations
public:
	virtual bool Open(MAPIEx* pMAPI,SBinary entry);
	virtual void Close();

	bool IsUnread();
	bool MarkAsRead(bool bRead=true);
	bool Create(MAPIEx &mapi, MAPIFolder &folder, int nPriority = IMPORTANCE_NORMAL, 
												  bool bSaveToSentFolder = true); 
	int ShowForm(MAPIEx* pMAPI, MAPIFolder &folder);
	bool Send();

	bool GetHeader(String& strHeader);
	String GetSenderName() 	{return m_strSenderName;}
	String GetSenderEmail() {return m_strSenderEmail;}
	String GetSubject() 	{return m_strSubject;}
	Time GetTime(ULONG property);
	Time GetSubmitTime() 			{return GetTime(PR_CLIENT_SUBMIT_TIME);}
	Time GetReceivedTime()			{return GetTime(PR_MESSAGE_DELIVERY_TIME);}
	Time GetLastModificationTime()	{return GetTime(PR_LAST_MODIFICATION_TIME);}
	Time GetCreationTime()			{return GetTime(PR_CREATION_TIME);};
	bool GetTo(String& strTo);
	bool GetCC(String& strCC);
	bool GetBCC(String& strBCC);
	int GetSensitivity();
	int GetMessageStatus();
	int GetPriority();
	DWORD GetSize();

	bool GetRecipients();
	bool GetNextRecipient(String& strName, String& strEmail, int& nType);
	bool GetReplyTo(String& strEmail);

	bool AddRecipients(LPADRLIST pAddressList);
	bool AddRecipient(const String email, int nType=MAPI_TO, const char* szAddrType=NULL);  // MAPI_CC and MAPI_BCC also valid
	void SetSubject(const String subject);
	void SetSender(const String senderName, const String senderEmail);
	bool SetReceivedTime(SYSTEMTIME tmReceived, bool bLocalTime=false);
	bool SetSubmitTime(SYSTEMTIME tmSubmit, bool bLocalTime=false);
	bool SetReadReceipt(bool bSet=true, String szReceiverEmail = Null);
	bool SetDeliveryReceipt(bool bSet = true);
	bool MarkAsPrivate();
	bool SetMessageStatus(int nMessageStatus);

	bool SaveToFile(const String fileName);

	// compares entryID and subject only
	bool operator==(MAPIMessage& message);

protected:
	void FillSenderEmail();
};

#endif
