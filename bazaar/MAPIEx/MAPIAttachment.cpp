#ifdef _WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIAttachment.cpp
// Description: MAPI Attachment class wrapper
//
// Copyright (C) 2005-2011, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for the community to benefit.
//
// Usage: see the CodeProject article at http://www.codeproject.com/internet/MAPIEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// MAPIAttachment

MAPIAttachment::MAPIAttachment() {
	m_pStream = NULL;
	m_nBytesWritten = 0;
}

MAPIAttachment::~MAPIAttachment() {
	Close();
}

void MAPIAttachment::Attach(LPATTACH pAttachment) {
	Close();
	m_pItem = pAttachment;
}

LPATTACH MAPIAttachment::Detach() {
	LPATTACH pAttachment = Attachment();
	m_pItem = NULL;
	return pAttachment;
}

bool MAPIAttachment::Create(LPMESSAGE pMessage) {
	Close();

	bool bResult = false;
	if(pMessage) {
		LPATTACH pAttachment;
		ULONG ulAttachmentNum=0;

		if(pMessage->CreateAttach(NULL, 0, &ulAttachmentNum, &pAttachment) == S_OK) {
			m_pItem=pAttachment;

			const int nProperties = 2;
			SPropValue prop[nProperties];
			memset(prop, 0,sizeof(SPropValue)*nProperties);
			prop[0].ulPropTag = PR_ATTACH_METHOD;
			prop[0].Value.ul = ATTACH_BY_VALUE;
			prop[1].ulPropTag = PR_RENDERING_POSITION;
			prop[1].Value.l = 1;
			if(pAttachment->SetProps(nProperties, prop, NULL) == S_OK) 
				bResult=true;
		}
	}
	return bResult;
}

bool MAPIAttachment::Open(LPMESSAGE pMessage, int nIndex) {
	Close();

	bool bResult = false;
	LPMAPITABLE pAttachTable;
	if(!(nIndex >= 0 && pMessage && pMessage->GetAttachmentTable(0, &pAttachTable) == S_OK))
		return false;
	
	enum { PROP_ATTACH_NUM, ATTACH_COLS };
	static SizedSPropTagArray(ATTACH_COLS, Columns) = {ATTACH_COLS, PR_ATTACH_NUM };
	if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
		if(pAttachTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL) == S_OK) {
			LPSRowSet pRows = NULL;
			if(pAttachTable->QueryRows(1, 0, &pRows) == S_OK) {
				if(pRows->cRows > 0) {
					LPATTACH pAttachment;
					if(pMessage->OpenAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.ul, 
															NULL, 0, &pAttachment) == S_OK) {
						m_pItem=pAttachment;
						bResult=true;
					}
				}
				MAPIEx::FreeProws(pRows);
			}
		}
	}
	return bResult;
}

void MAPIAttachment::Close() {
	CloseStream();
	MAPIObject::Close();
}

String MAPIAttachment::GetDisplayName() {
	return GetPropertyString(PR_DISPLAY_NAME);
}

String MAPIAttachment::GetFileName() {
	return GetPropertyString(PR_ATTACH_FILENAME);
}

String MAPIAttachment::GetLongFileName() {
	return GetPropertyString(PR_ATTACH_LONG_FILENAME);
}

String MAPIAttachment::GetCID() {
	return GetPropertyString(PR_ATTACH_CONTENT_ID);
}

bool MAPIAttachment::SetDisplayName(const String &szDisplayName) {
	return SetPropertyString(PR_DISPLAY_NAME, szDisplayName);
}

bool MAPIAttachment::SetFileName(const String &szFileName) {
	return SetPropertyString(PR_ATTACH_FILENAME, szFileName);
}

bool MAPIAttachment::SetLongFileName(const String &szLongFileName) {
	return SetPropertyString(PR_ATTACH_LONG_FILENAME, szLongFileName);
}

bool MAPIAttachment::SetCID(const String &szCID) {
	return SetPropertyString(PR_ATTACH_CONTENT_ID, szCID);
}

bool MAPIAttachment::OpenStream(bool bCreate) {
	if(m_pItem) {
		ULONG ulInterfaceOptions = STGM_READ;
		ULONG ulFlags = 0;
		if(bCreate) {
			ulInterfaceOptions = 0;
			ulFlags = MAPI_MODIFY | MAPI_CREATE;
		}
		return (m_pItem->OpenProperty(PR_ATTACH_DATA_BIN, &IID_IStream, ulInterfaceOptions, ulFlags, (LPUNKNOWN*)&m_pStream)==S_OK);
	}
	return false;
}

int MAPIAttachment::Read(BYTE* pData, int nCount) {
	if(m_pStream) {
		ULONG ulRead;
		m_pStream->Read(pData, nCount, &ulRead);
		return ulRead;
	}
	return -1;
}

int MAPIAttachment::Write(BYTE* pData, int nCount) {
	if(m_pStream) {
		ULONG ulWritten;
		m_pStream->Write(pData, nCount, &ulWritten);
		m_nBytesWritten += ulWritten;
		return ulWritten;
	}
	return -1;
}

void MAPIAttachment::CloseStream() {
	if(m_pStream) {
		m_pStream->Commit(STGC_DEFAULT);
		RELEASE(m_pStream);
		SetPropertyValue(PR_ATTACH_SIZE, m_nBytesWritten);
		m_nBytesWritten = 0;
	}
}

bool MAPIAttachment::LoadAttachment(const String &szPath) {
	String file = LoadFile(szPath);
	
	if(file.IsEmpty() || !OpenStream(true))
		return false;
	 
	Write((BYTE *)file.Begin(), file.GetLength());
	return true;
}

bool MAPIAttachment::SaveAttachment(const String &szPath) {
	if (!OpenStream(false))
		return false;
	String file;
	const int BUF_SIZE = 4096;
	BYTE pData[BUF_SIZE];
	ULONG ulRead;

	do {
		ulRead = Read(pData, BUF_SIZE);
		if(ulRead >= 0) 
			file.Cat(pData, ulRead);
	} while(ulRead >= BUF_SIZE);
	return SaveFile(szPath, file);
}

#endif