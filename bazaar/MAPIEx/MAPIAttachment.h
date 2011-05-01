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

// Ported to U++ Framework by Koldo. See License.txt file

/////////////////////////////////////////////////////////////
// MAPIAttachment

#define PR_ATTACHMENT_UNKNOWN 0x7FFF000B

class MAPIAttachment : public MAPIObject {
public:
	MAPIAttachment();
	~MAPIAttachment();

// Attributes
protected:
	LPSTREAM m_pStream;
	int m_nBytesWritten;

// Operations
public:
	inline LPATTACH Attachment() { return (LPATTACH)m_pItem; }
	void Attach(LPATTACH pAttachment);
	LPATTACH Detach();
	bool Create(LPMESSAGE pMessage);
	bool Open(LPMESSAGE pMessage, int nIndex);
	void Close();

	bool GetDisplayName(String& strDisplayName);
	bool GetFileName(String& strFileName);
	bool GetLongFileName(String& strLongFileName);
	bool GetCID(String& strCID);

	bool SetDisplayName(LPCTSTR szDisplayName);
	bool SetFileName(LPCTSTR szFileName);
	bool SetLongFileName(LPCTSTR szLongFileName);
	bool SetCID(LPCTSTR szCID);

	bool OpenStream(bool bCreate=FALSE);
	int Read(BYTE* pData, int nCount);
	int Write(BYTE* pData, int nCount);
	void CloseStream();

	bool LoadAttachment(LPCTSTR szPath);
	bool SaveAttachment(LPCTSTR szPath);
};

#endif
