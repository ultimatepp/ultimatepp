////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIFolder.cpp
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

#include "MAPIExPCH.h"
#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// CMAPIFolder

CMAPIFolder::CMAPIFolder()
{
	Init();
}

CMAPIFolder::CMAPIFolder(CMAPIEx* pMAPI, LPMAPIFOLDER pFolder, LPCTSTR szName)
{
	Init();

	m_entryID.cb=0;
	Attach(pMAPI, pFolder, szName);
}

CMAPIFolder::~CMAPIFolder()
{
	Close();
}

void CMAPIFolder::Init()
{
	m_pHierarchy=NULL;
	m_pContents=NULL;
	m_nMaxRowsSize=DEFAULT_FOLDER_BUFFER_SIZE;
	m_pRows=NULL;
	ClearBuffer();
}

void CMAPIFolder::Close()
{
	ClearBuffer();
	RELEASE(m_pHierarchy);
	RELEASE(m_pContents);
	CMAPIObject::Close();
}

void CMAPIFolder::SetBufferSize(int nSize)
{
	m_nMaxRowsSize=max(1, nSize);
}

void CMAPIFolder::ClearBuffer()
{
	if(m_pRows!=NULL)
	{
		FreeProws(m_pRows);
		m_pRows=NULL;
	}
	m_nRowsIndex=0;
}

void CMAPIFolder::Attach(CMAPIEx* pMAPI, LPMAPIFOLDER pFolder, LPCTSTR szName)
{
	Close();
	m_pMAPI=pMAPI;
	m_pItem=pFolder;
	m_strName=szName;
	LPSPropValue pProp;
	if(GetProperty(PR_ENTRYID, pProp)==S_OK) 
	{
		SetEntryID(&pProp->Value.bin);
		MAPIFreeBuffer(pProp);
	} 
	else 
	{
		SetEntryID(NULL);
	}
}

LPMAPIFOLDER CMAPIFolder::Detach()
{
	LPMAPIFOLDER pFolder=Folder();
	m_pItem=NULL;
	return pFolder;
}

LPCTSTR CMAPIFolder::GetName()
{
	return m_strName;
}

LPMAPITABLE CMAPIFolder::GetHierarchy()
{
	RELEASE(m_pHierarchy);
	if(Folder()->GetHierarchyTable(0, &m_pHierarchy)!=S_OK) return NULL;

	const int nProperties=2;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID}};
	if(m_pHierarchy->SetColumns((LPSPropTagArray)&Columns, 0)!=S_OK) 
	{
		RELEASE(m_pHierarchy);
		return NULL;
	}
	return m_pHierarchy;
}

// High Level function to open a sub folder by iterating recursively (DFS) over all folders 
// (use instead of manually calling GetHierarchy and GetNextSubFolder)
CMAPIFolder* CMAPIFolder::OpenSubFolder(LPCTSTR szSubFolder)
{
	if(!GetHierarchy()) return NULL;

	CString strFolder;
	CMAPIFolder folder;
	CMAPIFolder* pSubFolder=NULL;
	while(GetNextSubFolder(folder, strFolder)) 
	{
		if(!strFolder.CompareNoCase(szSubFolder)) 
		{
			pSubFolder=new CMAPIFolder(m_pMAPI,folder.Detach());
		} 
		else 
		{
			pSubFolder=folder.OpenSubFolder(szSubFolder);
		}
		if(pSubFolder) break;
	}
	return pSubFolder;
} 

// Creates a sub-folder under pFolder, opens the folder if it already exists
CMAPIFolder* CMAPIFolder::CreateSubFolder(LPCTSTR szSubFolder)
{
	LPMAPIFOLDER pSubFolder=NULL;
	ULONG ulFolderType=FOLDER_GENERIC;
	ULONG ulFlags=OPEN_IF_EXISTS | CMAPIEx::cm_nMAPICode;

	Folder()->CreateFolder(ulFolderType, (LPTSTR)szSubFolder, NULL, NULL,ulFlags, &pSubFolder);
	return pSubFolder ? new CMAPIFolder(m_pMAPI, pSubFolder) : NULL;
}

// Deletes a sub-folder and ALL sub folders/messages
BOOL CMAPIFolder::DeleteSubFolder(LPCTSTR szSubFolder)
{
	if(GetHierarchy()) 
	{
		CString strFolder;
		CMAPIFolder folder;
		while(GetNextSubFolder(folder, strFolder)) 
		{
			if(!strFolder.CompareNoCase(szSubFolder)) 
			{
				return DeleteSubFolder(&folder);
			}
		}
	}
	return FALSE;
}

// Deletes a sub-folder and ALL sub folders/messages
BOOL CMAPIFolder::DeleteSubFolder(CMAPIFolder* pSubFolder)
{
	if(!pSubFolder) return FALSE;

	LPSPropValue props=NULL;
	ULONG cValues=0;
	ULONG rgTags[]={ 1, PR_ENTRYID };

	if(pSubFolder->Folder()->GetProps((LPSPropTagArray) rgTags, CMAPIEx::cm_nMAPICode, &cValues, &props)==S_OK) 
	{
		HRESULT hr=Folder()->DeleteFolder(props[0].Value.bin.cb, (LPENTRYID)props[0].Value.bin.lpb, NULL, NULL,DEL_FOLDERS|DEL_MESSAGES);
		MAPIFreeBuffer(props);
		return (hr==S_OK);
	}
	return FALSE;
}

LPMAPITABLE CMAPIFolder::GetContents()
{
	RELEASE(m_pContents);
	if(Folder()->GetContentsTable(CMAPIEx::cm_nMAPICode, &m_pContents)!=S_OK) return NULL;

	const int nProperties=MESSAGE_COLS;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_MESSAGE_FLAGS, PR_ENTRYID }};
	if(m_pContents->SetColumns((LPSPropTagArray)&Columns, 0)!=S_OK) 
	{
		RELEASE(m_pContents);
		return NULL;
	}
	return m_pContents;
}

int CMAPIFolder::GetRowCount()
{
	ULONG ulCount;
	if(!m_pContents || m_pContents->GetRowCount(0, &ulCount)!=S_OK) return -1;
	return ulCount;
}

BOOL CMAPIFolder::SortContents(ULONG ulSortParam, ULONG ulSortField)
{
	if(!m_pContents) return FALSE;

	SizedSSortOrderSet(1, SortColums) = {1, 0, 0, {{ulSortField, ulSortParam}}};
	return (m_pContents->SortTable((LPSSortOrderSet)&SortColums, 0)==S_OK);
}

BOOL CMAPIFolder::SetUnreadOnly(BOOL bUnreadOnly)
{
	if(bUnreadOnly)
	{
		SRestriction res;
		res.rt=RES_BITMASK;
		res.res.resBitMask.relBMR=BMR_EQZ;
		res.res.resBitMask.ulPropTag=PR_MESSAGE_FLAGS;
		res.res.resBitMask.ulMask=MSGFLAG_READ;
		return SetRestriction(&res);
	}
	return SetRestriction(NULL);
}

BOOL CMAPIFolder::SetRestriction(SRestriction* pRestriction)
{
	return (m_pContents && m_pContents->Restrict(pRestriction, 0)==S_OK);
}

BOOL CMAPIFolder::QueryRows()
{
	ClearBuffer();
	if(m_pContents)
	{
		if(m_pContents->QueryRows(m_nMaxRowsSize, 0, &m_pRows)==S_OK)
		{
			return TRUE;
		}
		m_pRows=NULL;
	}
	return FALSE;
}

SRow* CMAPIFolder::GetNextRow()
{
	if(m_pRows==NULL || m_nRowsIndex>=m_pRows->cRows)
	{
		if(!QueryRows()) return NULL;
	}
	if(m_pRows->cRows==0)
	{
		ClearBuffer();
		return NULL;
	}
	return &m_pRows->aRow[m_nRowsIndex++];
}

BOOL CMAPIFolder::GetNextMessage(CMAPIMessage& message)
{
	SRow* pRow=GetNextRow();
	return pRow ? message.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : FALSE;
}

BOOL CMAPIFolder::GetNextContact(CMAPIContact& contact)
{
#ifdef _WIN32_WCE
	return m_poom.GetNextContact(m_pMAPI, contact);
#else
	SRow* pRow=GetNextRow();
	return pRow ? contact.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : FALSE;
#endif
}

BOOL CMAPIFolder::GetNextAppointment(CMAPIAppointment& appointment)
{
#ifdef _WIN32_WCE
	return m_poom.GetNextAppointment(m_pMAPI,appointment);
#else
	SRow* pRow=GetNextRow();
	return pRow ? appointment.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : FALSE;
#endif
}

BOOL CMAPIFolder::GetNextSubFolder(CMAPIFolder& folder, CString& strFolder)
{
	if(!m_pHierarchy) return NULL;
	folder.Close();

	DWORD dwObjType;
	LPSRowSet pRows=NULL;

	LPMAPIFOLDER pSubFolder=NULL;
	if(m_pHierarchy->QueryRows(1, 0, &pRows)==S_OK)
	{
		if(pRows->cRows)
		{
			if(Folder()->OpenEntry(pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin.cb, (LPENTRYID)pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin.lpb, NULL, MAPI_MODIFY, &dwObjType, (LPUNKNOWN*)&pSubFolder)==S_OK)
			{
				strFolder=CMAPIEx::GetValidString(pRows->aRow[0].lpProps[0]);
				folder.Attach(m_pMAPI, pSubFolder, strFolder);
			}
		}
		FreeProws(pRows);
	}
	return (pSubFolder!=NULL);
}

BOOL CMAPIFolder::DeleteMessage(CMAPIMessage& message)
{
	return DeleteObject(message);
}

BOOL CMAPIFolder::CopyMessage(CMAPIMessage& message, CMAPIFolder* pFolderDest)
{
	if(!pFolderDest) return FALSE;

	ENTRYLIST entries={ 1, message.GetEntryID() };
	HRESULT hr=Folder()->CopyMessages(&entries, NULL, pFolderDest->Folder(), NULL, NULL, 0);
	return (hr==S_OK);
}

BOOL CMAPIFolder::MoveMessage(CMAPIMessage& message, CMAPIFolder* pFolderDest)
{
	if(!pFolderDest) return FALSE;

	ENTRYLIST entries={ 1, message.GetEntryID() };
	HRESULT hr=Folder()->CopyMessages(&entries, NULL, pFolderDest->Folder(), NULL, NULL,MESSAGE_MOVE);
	return (hr==S_OK);
}

BOOL CMAPIFolder::DeleteContact(CMAPIContact& contact)
{
	return DeleteObject(contact);
}

BOOL CMAPIFolder::DeleteAppointment(CMAPIAppointment& appointment)
{
	return DeleteObject(appointment);
}

BOOL CMAPIFolder::DeleteObject(CMAPIObject& object)
{
	ENTRYLIST entries={ 1, object.GetEntryID() };
	HRESULT hr=Folder()->DeleteMessages(&entries, NULL, NULL, 0);
	return (hr==S_OK);
}
