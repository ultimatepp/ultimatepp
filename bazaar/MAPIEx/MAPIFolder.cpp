#ifdef _WIN32

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

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// MAPIFolder

MAPIFolder::MAPIFolder() {
	Init();
}

MAPIFolder::MAPIFolder(MAPIEx* pMAPI, LPMAPIFOLDER pFolder, String szName) {
	Init();

	m_entryID.cb=0;
	Attach(pMAPI, pFolder, szName);
}

MAPIFolder::~MAPIFolder() {
	Close();
}

void MAPIFolder::Init() {
	m_pHierarchy = NULL;
	m_pContents = NULL;
	m_nMaxRowsSize = DEFAULT_FOLDER_BUFFER_SIZE;
	m_pRows = NULL;
	ClearBuffer();
}

void MAPIFolder::Close() {
	ClearBuffer();
	RELEASE(m_pHierarchy);
	RELEASE(m_pContents);
	MAPIObject::Close();
}

void MAPIFolder::SetBufferSize(int nSize) {
	m_nMaxRowsSize = max(1, nSize);
}

void MAPIFolder::ClearBuffer() {
	if(m_pRows) {
		MAPIEx::FreeProws(m_pRows);
		m_pRows = NULL;
	}
	m_nRowsIndex = 0;
}

bool MAPIFolder::Attach(MAPIEx* pMAPI, LPMAPIFOLDER pFolder, String szName) {
	Close();
	m_pMAPI = pMAPI;
	m_pItem = pFolder;
	m_strName = szName;
	LPSPropValue pProp;
	if(GetProperty(PR_ENTRYID, pProp) == S_OK) {
		SetEntryID(&pProp->Value.bin);
		MAPIFreeBuffer(pProp);
		return true;
	} else {
		SetEntryID(NULL);
		return false;
	}
}

LPMAPIFOLDER MAPIFolder::Detach() {
	LPMAPIFOLDER pFolder = Folder();
	m_pItem = NULL;
	return pFolder;
}

String MAPIFolder::GetName() {
	return m_strName;
}

LPMAPITABLE MAPIFolder::GetHierarchy() {
	RELEASE(m_pHierarchy);
	if(Folder()->GetHierarchyTable(0, &m_pHierarchy) != S_OK) 
		return NULL;

	const int nProperties = 2;
	SizedSPropTagArray(nProperties, Columns)={nProperties,{PR_DISPLAY_NAME, PR_ENTRYID}};
	if(m_pHierarchy->SetColumns((LPSPropTagArray)&Columns, 0) != S_OK) {
		RELEASE(m_pHierarchy);
		return NULL;
	}
	return m_pHierarchy;
}

// High Level function to open a sub folder by iterating recursively (DFS) over all folders 
bool MAPIFolder::OpenSubFolder(const String &subFolderName, MAPIFolder &subFolder, 
																bool fullPath, String path) {
	if(!GetHierarchy()) 
		return NULL;

	String strFolder;
	MAPIFolder folder;
	MAPIFolder* pSubFolder = NULL;
	while(GetNextSubFolder(folder, strFolder)) {
		bool retSubFolder;
		if (fullPath)
			strFolder = path + "/" + strFolder;
		if(!CompareNoCase(strFolder, subFolderName)) 
			retSubFolder = subFolder.Attach(m_pMAPI, folder.Detach());
		else 
			retSubFolder = folder.OpenSubFolder(subFolderName, subFolder, fullPath, strFolder);
		if(retSubFolder) 
			return true;
	}
	return false;
} 

// Creates a sub-folder under pFolder, opens the folder if it already exists
bool MAPIFolder::CreateSubFolder(const String &subFolderName, MAPIFolder &subFolder) {
	LPMAPIFOLDER pSubFolder = NULL;
	ULONG ulFolderType=FOLDER_GENERIC;
	ULONG ulFlags=OPEN_IF_EXISTS | MAPIEx::cm_nMAPICode;

	Folder()->CreateFolder(ulFolderType, (LPTSTR)(subFolderName.Begin()), NULL, NULL, ulFlags, 
																				  &pSubFolder);
	subFolder.Attach(m_pMAPI, pSubFolder);
	return pSubFolder; 
}

// Deletes a sub-folder and ALL sub folders/messages
bool MAPIFolder::DeleteSubFolder(const String &subFolderName) {
	if(GetHierarchy()) {
		String strFolder;
		MAPIFolder folder;
		while(GetNextSubFolder(folder, strFolder)) {
			if(!CompareNoCase(strFolder, subFolderName)) 
				return DeleteSubFolder(folder);
		}
	}
	return false;
}

// Deletes a sub-folder and ALL sub folders/messages
bool MAPIFolder::DeleteSubFolder(MAPIFolder &subFolder) {
	if(!subFolder.IsOpened()) 
		return false;

	LPSPropValue props = NULL;
	ULONG cValues=0;
	ULONG rgTags[]={ 1, PR_ENTRYID };

	if(subFolder.Folder()->GetProps((LPSPropTagArray) rgTags, MAPIEx::cm_nMAPICode, 
																&cValues, &props) == S_OK) {
		HRESULT hr = Folder()->DeleteFolder(props[0].Value.bin.cb, 
						(LPENTRYID)props[0].Value.bin.lpb, 0, NULL,DEL_FOLDERS|DEL_MESSAGES);
		MAPIFreeBuffer(props);
		return (hr == S_OK);
	}
	return false;
}

LPMAPITABLE MAPIFolder::GetContents() {
	RELEASE(m_pContents);
	if(Folder()->GetContentsTable(MAPIEx::cm_nMAPICode, &m_pContents)!=S_OK) 
		return NULL;

	const int nProperties = MESSAGE_COLS;
	SizedSPropTagArray(nProperties, Columns) = {nProperties,{PR_MESSAGE_FLAGS, PR_ENTRYID }};
	if(m_pContents->SetColumns((LPSPropTagArray)&Columns, 0)!=S_OK) {
		RELEASE(m_pContents);
		return NULL;
	}
	return m_pContents;
}

int MAPIFolder::GetCount() {
	ULONG ulCount;
	if(!m_pContents || m_pContents->GetRowCount(0, &ulCount) != S_OK) 
		return -1;
	return ulCount;
}

bool MAPIFolder::SortContents(ULONG ulSortParam, ULONG ulSortField) {
	if(!m_pContents) 
		return false;

	SizedSSortOrderSet(1, SortColums) = {1, 0, 0, {{ulSortField, ulSortParam}}};
	return (m_pContents->SortTable((LPSSortOrderSet)&SortColums, 0) == S_OK);
}

bool MAPIFolder::SetUnreadOnly(bool bUnreadOnly) {
	if(bUnreadOnly) {
		SRestriction res;
		res.rt = RES_BITMASK;
		res.res.resBitMask.relBMR = BMR_EQZ;
		res.res.resBitMask.ulPropTag = PR_MESSAGE_FLAGS;
		res.res.resBitMask.ulMask = MSGFLAG_READ;
		return SetRestriction(&res);
	}
	return SetRestriction(NULL);
}

bool MAPIFolder::SetRestriction(SRestriction* pRestriction) {
	return (m_pContents && m_pContents->Restrict(pRestriction, 0)==S_OK);
}

bool MAPIFolder::QueryRows() {
	ClearBuffer();
	if(m_pContents) {
		if(m_pContents->QueryRows(m_nMaxRowsSize, 0, &m_pRows) == S_OK) {
			return true;
		}
		m_pRows = NULL;
	}
	return false;
}

SRow* MAPIFolder::GetNextRow() {
	if(m_pRows == NULL || m_nRowsIndex >= m_pRows->cRows) {
		if(!QueryRows()) 
			return NULL;
	}
	if(m_pRows->cRows==0) {
		ClearBuffer();
		return NULL;
	}
	return &m_pRows->aRow[m_nRowsIndex++];
}

bool MAPIFolder::GetNextMessage(MAPIMessage& message) {
	SRow* pRow = GetNextRow();
	return pRow ? message.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : false;
}

bool MAPIFolder::GetNextContact(MAPIContact& contact) {
#ifdef _WIN32_WCE
	return m_poom.GetNextContact(m_pMAPI, contact);
#else
	SRow* pRow = GetNextRow();
	return pRow ? contact.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : false;
#endif
}

bool MAPIFolder::GetNextAppointment(MAPIAppointment& appointment) {
#ifdef _WIN32_WCE
	return m_poom.GetNextAppointment(m_pMAPI,appointment);
#else
	SRow* pRow = GetNextRow();
	return pRow ? appointment.Open(m_pMAPI, pRow->lpProps[PROP_ENTRYID].Value.bin) : false;
#endif
}

bool MAPIFolder::GetNextSubFolder(MAPIFolder& folder, String& strFolder) {
	if(!m_pHierarchy) 
		return NULL;
	folder.Close();

	DWORD dwObjType;
	LPSRowSet pRows = NULL;

	LPMAPIFOLDER pSubFolder = NULL;
	if(m_pHierarchy->QueryRows(1, 0, &pRows) == S_OK) {
		if(pRows->cRows) {
			if(S_OK == Folder()->OpenEntry(pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin.cb, 
								(LPENTRYID)pRows->aRow[0].lpProps[PROP_ENTRYID].Value.bin.lpb, 	
									NULL, MAPI_MODIFY, &dwObjType, (LPUNKNOWN*)&pSubFolder)) {
				strFolder = MAPIEx::GetValidString(pRows->aRow[0].lpProps[0]);
				folder.Attach(m_pMAPI, pSubFolder, strFolder);
			}
		}
		MAPIEx::FreeProws(pRows);
	}
	if (!pSubFolder)
		return false;
	folder.GetHierarchy();
	folder.GetContents();
	return true;
}

bool MAPIFolder::DeleteMessage(MAPIMessage& message) {
	return DeleteObject(message);
}

bool MAPIFolder::CopyMessage(MAPIMessage& message, MAPIFolder &folderDest) {
	ENTRYLIST entries = { 1, message.GetEntryID() };
	HRESULT hr = Folder()->CopyMessages(&entries, NULL, folderDest.Folder(), 0, NULL, 0);
	return (hr == S_OK);
}

bool MAPIFolder::MoveMessage(MAPIMessage& message, MAPIFolder &folderDest) {
	if(!folderDest.IsOpened()) 
		return false;

	ENTRYLIST entries = { 1, message.GetEntryID() };
	HRESULT hr = Folder()->CopyMessages(&entries, NULL, folderDest.Folder(), 0, NULL,MESSAGE_MOVE);
	return (hr == S_OK);
}

bool MAPIFolder::DeleteContact(MAPIContact& contact) {
	return DeleteObject(contact);
}

bool MAPIFolder::DeleteAppointment(MAPIAppointment& appointment) {
	return DeleteObject(appointment);
}

bool MAPIFolder::DeleteObject(MAPIObject& object) {
	ENTRYLIST entries={ 1, object.GetEntryID() };
	HRESULT hr = Folder()->DeleteMessages(&entries, 0, NULL, 0);
	return (hr==S_OK);
}

#endif