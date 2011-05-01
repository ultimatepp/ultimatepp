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

class CMAPIEx;
class CMAPIFolder;
class CMAPIAttachment;

/////////////////////////////////////////////////////////////
// CMAPIObject

class AFX_EXT_CLASS CMAPIObject : public CObject
{
public:
	CMAPIObject();
	~CMAPIObject();

// Attributes
protected:
	CMAPIEx* m_pMAPI;
	IMAPIProp* m_pItem;
	SBinary m_entryID;

// Operations
public:
	inline LPMESSAGE Message() { return (LPMESSAGE)m_pItem; }

	SBinary* GetEntryID() { return &m_entryID; }
	BOOL GetHexString(CString& strHex, SBinary bin);
	BOOL GetEntryIDString(CString& strEntryID);
	void SetEntryID(SBinary* pEntryID=NULL);
	int GetMessageFlags();
	BOOL SetMessageFlags(int nFlags);
	int GetSensitivity();
	BOOL SetSensitivity(int nSensitivity);
	int GetImportance();
	BOOL SetImportance(int nImportance);
	BOOL GetMessageClass(CString& strMessageClass);
	BOOL SetMessageClass(LPCTSTR szMessageClass);
	int GetMessageEditorFormat();
	BOOL SetMessageEditorFormat(int nFormat);

	virtual BOOL Open(CMAPIEx* pMAPI, SBinary entry);
	virtual void Close();
	virtual BOOL Save(BOOL bClose=TRUE);

	// Properties
	virtual BOOL GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream=FALSE);
	int GetPropertyValue(ULONG ulProperty, int nDefaultValue);
	BOOL GetNamedProperty(LPCTSTR szFieldName, LPSPropValue& pProp);
	BOOL GetNamedProperty(LPCTSTR szFieldName, CString& strField);
	BOOL GetOutlookProperty(ULONG ulData, ULONG ulProperty, LPSPropValue& pProp);
	BOOL GetOutlookPropertyValue(ULONG ulData, ULONG ulProperty, int& nValue);
	BOOL GetOutlookPropertyString(ULONG ulData, ULONG ulProperty, CString& strProperty);
	virtual BOOL SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream=FALSE);
	BOOL SetPropertyValue(ULONG ulProperty, int nValue);
	BOOL SetNamedProperty(LPCTSTR szFieldName, LPCTSTR szField, BOOL bCreate=TRUE);
	BOOL SetNamedMVProperty(LPCTSTR szFieldName, LPCTSTR* arCategories, int nCount, LPSPropValue &pProp, BOOL bCreate=TRUE);
	BOOL SetOutlookProperty(ULONG ulData, ULONG ulProperty, LPCTSTR szField);
	BOOL SetOutlookProperty(ULONG ulData, ULONG ulProperty, int nField, int nFieldType=PT_LONG);
	BOOL SetOutlookProperty(ULONG ulData, ULONG ulProperty, FILETIME ftField);
	BOOL DeleteNamedProperty(LPCTSTR szFieldName);

	// Attachments
	int GetAttachmentCount();
	BOOL GetAttachment(CMAPIAttachment& attachment, int nIndex);
	BOOL AddAttachment(CMAPIAttachment& attachment);
	BOOL AddAttachment(LPCTSTR szPath, LPCTSTR szName=NULL, LPCTSTR szCID=NULL);
	BOOL DeleteAttachment(int nIndex=-1);
	BOOL SaveAttachment(LPCTSTR szFolder, int nIndex=-1, LPCTSTR szFileName=NULL);

	// Body
	BOOL GetBody(CString& strBody, BOOL bAutoDetect=TRUE);
	BOOL SetBody(LPCTSTR szBody);
	BOOL GetHTML(CString& strHTML);
	BOOL SetHTML(LPCTSTR szHTML);
	BOOL GetRTF(CString& strRTF);
	BOOL SetRTF(LPCTSTR szRTF);

	// Misc
	BOOL GetLastModified(SYSTEMTIME& tmLastModified);
	BOOL GetLastModified(CString& strLastModified, LPCTSTR szFormat=NULL); // NULL defaults to "MM/dd/yyyy"
	BOOL SetLastModified(SYSTEMTIME& tmLastModified);

protected:
	BOOL Create(CMAPIEx* pMAPI, CMAPIFolder* pFolder);
	HRESULT GetProperty(ULONG ulProperty, LPSPropValue &prop);
	BOOL GetPropTagArray(LPCTSTR szFieldName, LPSPropTagArray& lppPropTags, int& nFieldType, BOOL bCreate);
	BOOL GetOutlookPropTagArray(ULONG ulData, ULONG ulProperty, LPSPropTagArray& lppPropTags, int& nFieldType, BOOL bCreate);
};

#endif
