#ifndef __MAPIFOLDER_H__
#define __MAPIFOLDER_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIFolder.h
// Description: MAPI Folder class wrapper
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

#ifdef _WIN32_WCE
#include "POOM.h"
#define DEFAULT_FOLDER_BUFFER_SIZE 128
#else
#define DEFAULT_FOLDER_BUFFER_SIZE 512
#endif

/////////////////////////////////////////////////////////////
// MAPIFolder

class MAPIFolder : public MAPIObject {
public:
	MAPIFolder();
	MAPIFolder(MAPIEx* pMAPI, LPMAPIFOLDER pFolder, String szName = NULL);
	~MAPIFolder();

	enum { PROP_MESSAGE_FLAGS, PROP_ENTRYID, MESSAGE_COLS };

// Attributes
protected:
	LPMAPITABLE m_pHierarchy;
	LPMAPITABLE m_pContents;
	int m_nMaxRowsSize;
	ULONG m_nRowsIndex;
	LPSRowSet m_pRows;
	String m_strName;

#ifdef _WIN32_WCE
	CPOOM m_poom;
#endif

// Operations
//public:
	inline LPMAPIFOLDER Folder() { return (LPMAPIFOLDER)m_pItem; }
	bool Attach(MAPIEx* pMAPI, LPMAPIFOLDER pFolder, String szName = NULL);
	LPMAPIFOLDER Detach();
	LPMAPITABLE GetHierarchy();
	LPMAPITABLE GetContents();
	bool SetRestriction(SRestriction* pRestriction);
	
#ifdef _WIN32_WCE
	CPOOM* GetPOOM() { return &m_poom; }
#endif

	virtual void Close();
	void SetBufferSize(int nSize);
	void ClearBuffer();
	
	bool IsOpened()	{return m_pHierarchy;};
public:
	String GetName();
	bool OpenSubFolder(const String &subFolderName, MAPIFolder &subFolder, 
													bool fullPath = false, String path = Null);
	bool CreateSubFolder(const String &subFolderName, MAPIFolder &subFolder);
	bool DeleteSubFolder(const String &subFolder);
	bool DeleteSubFolder(MAPIFolder &folder);

	int GetCount();
	bool SortContents(ULONG ulSortParam = TABLE_SORT_ASCEND, 
												ULONG ulSortField = PR_MESSAGE_DELIVERY_TIME);
	bool SetUnreadOnly(bool bUnreadOnly = true);
	bool GetNextMessage(MAPIMessage& message);
	bool GetNextContact(MAPIContact& contact);
	bool GetNextAppointment(MAPIAppointment& appointment);
	bool GetNextSubFolder(MAPIFolder& folder, String& strFolder);

	bool DeleteMessage(MAPIMessage& message);
	bool CopyMessage(MAPIMessage& message, MAPIFolder& pFolderDest);
	bool MoveMessage(MAPIMessage& message, MAPIFolder& pFolderDest);

	bool DeleteContact(MAPIContact& contact);
	bool DeleteAppointment(MAPIAppointment& appointment);

protected:
	void Init();
	bool DeleteObject(MAPIObject& object);
	bool QueryRows();
	SRow* GetNextRow();
	
	friend class MAPIEx;
	friend class MAPIMessage;
	friend class MAPIObject;
};

#endif
