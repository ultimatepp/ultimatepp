#ifndef __MAPIATTACHMENT_H__
#define __MAPIATTACHMENT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIAttachment.h
// Description: MAPI Attachment class wrapper
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
class CMAPIAttachment;

/////////////////////////////////////////////////////////////
// CMAPIAttachment

#define PR_ATTACHMENT_UNKNOWN 0x7FFF000B

class AFX_EXT_CLASS CMAPIAttachment : public CMAPIObject
{
public:
	CMAPIAttachment();
	~CMAPIAttachment();

// Attributes
protected:
	LPSTREAM m_pStream;
	int m_nBytesWritten;

// Operations
public:
	inline LPATTACH Attachment() { return (LPATTACH)m_pItem; }
	void Attach(LPATTACH pAttachment);
	LPATTACH Detach();
	BOOL Create(LPMESSAGE pMessage);
	BOOL Open(LPMESSAGE pMessage, int nIndex);
	void Close();

	BOOL GetDisplayName(CString& strDisplayName);
	BOOL GetFileName(CString& strFileName);
	BOOL GetLongFileName(CString& strLongFileName);
	BOOL GetCID(CString& strCID);

	BOOL SetDisplayName(LPCTSTR szDisplayName);
	BOOL SetFileName(LPCTSTR szFileName);
	BOOL SetLongFileName(LPCTSTR szLongFileName);
	BOOL SetCID(LPCTSTR szCID);

	BOOL OpenStream(BOOL bCreate=FALSE);
	int Read(BYTE* pData, int nCount);
	int Write(BYTE* pData, int nCount);
	void CloseStream();

	BOOL LoadAttachment(LPCTSTR szPath);
	BOOL SaveAttachment(LPCTSTR szPath);
};

#endif
