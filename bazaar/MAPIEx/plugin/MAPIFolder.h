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

#ifdef _WIN32_WCE
#include "POOM.h"
#define DEFAULT_FOLDER_BUFFER_SIZE 128
#else
#define DEFAULT_FOLDER_BUFFER_SIZE 512
#endif

/////////////////////////////////////////////////////////////
// CMAPIFolder

class AFX_EXT_CLASS CMAPIFolder : public CMAPIObject
{
public:
	CMAPIFolder();
	CMAPIFolder(CMAPIEx* pMAPI, LPMAPIFOLDER pFolder, LPCTSTR szName=NULL);
	~CMAPIFolder();

	enum { PROP_MESSAGE_FLAGS, PROP_ENTRYID, MESSAGE_COLS };

// Attributes
protected:
	LPMAPITABLE m_pHierarchy;
	LPMAPITABLE m_pContents;
	int m_nMaxRowsSize;
	ULONG m_nRowsIndex;
	LPSRowSet m_pRows;
	CString m_strName;

#ifdef _WIN32_WCE
	CPOOM m_poom;
#endif

// Operations
public:
	inline LPMAPIFOLDER Folder() { return (LPMAPIFOLDER)m_pItem; }
	void Attach(CMAPIEx* pMAPI, LPMAPIFOLDER pFolder, LPCTSTR szName);
	LPMAPIFOLDER Detach();

#ifdef _WIN32_WCE
	CPOOM* GetPOOM() { return &m_poom; }
#endif

	virtual void Close();
	void SetBufferSize(int nSize);
	void ClearBuffer();

	LPCTSTR GetName();
	LPMAPITABLE GetHierarchy();
	CMAPIFolder* OpenSubFolder(LPCTSTR szSubFolder);
	CMAPIFolder* CreateSubFolder(LPCTSTR szSubFolder);
	BOOL DeleteSubFolder(LPCTSTR szSubFolder);
	BOOL DeleteSubFolder(CMAPIFolder* pFolder);

	LPMAPITABLE GetContents();
	int GetRowCount();
	BOOL SortContents(ULONG ulSortParam=TABLE_SORT_ASCEND, ULONG ulSortField=PR_MESSAGE_DELIVERY_TIME);
	BOOL SetUnreadOnly(BOOL bUnreadOnly=TRUE);
	BOOL SetRestriction(SRestriction* pRestriction);
	BOOL GetNextMessage(CMAPIMessage& message);
	BOOL GetNextContact(CMAPIContact& contact);
	BOOL GetNextAppointment(CMAPIAppointment& appointment);
	BOOL GetNextSubFolder(CMAPIFolder& folder, CString& strFolder);

	BOOL DeleteMessage(CMAPIMessage& message);
	BOOL CopyMessage(CMAPIMessage& message, CMAPIFolder* pFolderDest);
	BOOL MoveMessage(CMAPIMessage& message, CMAPIFolder* pFolderDest);

	BOOL DeleteContact(CMAPIContact& contact);
	BOOL DeleteAppointment(CMAPIAppointment& appointment);

protected:
	void Init();
	BOOL DeleteObject(CMAPIObject& object);
	BOOL QueryRows();
	SRow* GetNextRow();
};

#endif
