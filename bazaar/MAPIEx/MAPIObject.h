#ifndef __MAPIOBJECT_H__
#define __MAPIOBJECT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIObject.h
// Description: Base class for code common to MAPI Items (messages, contacts etc)
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
class MAPIFolder;
class MAPIAttachment;

/////////////////////////////////////////////////////////////
// MAPIObject

class MAPIObject {
public:
	MAPIObject();
	~MAPIObject();

// Attributes
protected:
	MAPIEx *m_pMAPI;
	IMAPIProp *m_pItem;
	SBinary m_entryID;

// Operations
public:
	inline LPMESSAGE Message() { return (LPMESSAGE)m_pItem; }
	String GetHexString(SBinary bin);
	SBinary* GetEntryID() { return &m_entryID; }
	String GetEntryIDString();
	void SetEntryID(SBinary* pEntryID = NULL);
	int GetMessageFlags();
	bool SetMessageFlags(int nFlags);
	int GetSensitivity();
	bool SetSensitivity(int nSensitivity);
	int GetImportance();
	bool SetImportance(int nImportance);
	String GetMessageClass();
	bool SetMessageClass(String szMessageClass);
	int GetMessageEditorFormat();
	bool SetMessageEditorFormat(int nFormat);

	virtual bool Open(MAPIEx* pMAPI, SBinary entry);
	virtual void Close();
	virtual bool Save(bool bClose=true);

	// Properties
	virtual String GetPropertyString(ULONG ulProperty, bool bStream = false);
	int GetPropertyValue(ULONG ulProperty, int nDefaultValue);
	bool GetNamedProperty(LPCTSTR szFieldName, LPSPropValue& pProp);
	bool GetNamedProperty(LPCTSTR szFieldName, String& strField);
	bool GetOutlookProperty(ULONG ulData, ULONG ulProperty, LPSPropValue& pProp);
	bool GetOutlookPropertyValue(ULONG ulData, ULONG ulProperty, int& nValue);
	bool GetOutlookPropertyString(ULONG ulData, ULONG ulProperty, String& strProperty);
	virtual bool SetPropertyString(ULONG ulProperty, String szProperty, bool bStream=false);
	bool SetPropertyValue(ULONG ulProperty, int nValue);
	bool SetNamedProperty(LPCTSTR szFieldName, LPCTSTR szField, bool bCreate=true);
	bool SetNamedMVProperty(LPCTSTR szFieldName, LPCTSTR* arCategories, int nCount, 
														LPSPropValue &pProp, bool bCreate=true);
	bool SetOutlookProperty(ULONG ulData, ULONG ulProperty, LPCTSTR szField);
	bool SetOutlookProperty(ULONG ulData, ULONG ulProperty, int nField, int nFieldType = PT_LONG);
	bool SetOutlookProperty(ULONG ulData, ULONG ulProperty, FILETIME ftField);
	bool DeleteNamedProperty(LPCTSTR szFieldName);

	// Attachments
	int GetAttachmentCount();
	bool GetAttachment(MAPIAttachment& attachment, int nIndex);
	bool AddAttachment(MAPIAttachment& attachment);
	bool SaveAttachment(LPCTSTR szFolder, int nIndex=-1, LPCTSTR szFileName=NULL);
	bool DeleteAttachment(int nIndex=-1);
	bool AddAttachment(const String &szPath, String szName = Null, String szCID = Null);

	// Body
	String GetBody(bool bAutoDetect = true);
	bool SetBody(const String& strBody);
	String GetHTML();
	bool SetHTML(const String& strHTML);
	String GetRTF();
	bool SetRTF(const String& strRTF);

	// Misc
	Time GetLastModified(SYSTEMTIME& tmLastModified);
	bool SetLastModified(const Time& tmLastModified);
	
protected:
	bool Create(MAPIEx &mapi, MAPIFolder &folder);
	HRESULT GetProperty(ULONG ulProperty, LPSPropValue &prop);
	bool GetPropTagArray(LPCTSTR szFieldName, LPSPropTagArray& lppPropTags, int& nFieldType, 
																					bool bCreate);
	bool GetOutlookPropTagArray(ULONG ulData, ULONG ulProperty, LPSPropTagArray& lppPropTags, 
																	int& nFieldType, bool bCreate);
	//bool SaveAttachment(LPATTACH pAttachment, LPCTSTR szPath);
};

#endif
