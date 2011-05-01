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
// Usage: see the CodeProject article at http://www.codeproject.com/internet/CMapiEx.asp
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MAPIExPCH.h"
#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// MAPIAttachment

CMAPIAttachment::CMAPIAttachment()
{
	m_pStream=NULL;
	m_nBytesWritten=0;
}

CMAPIAttachment::~CMAPIAttachment()
{
	Close();
}

void CMAPIAttachment::Attach(LPATTACH pAttachment)
{
	Close();
	m_pItem=pAttachment;
}

LPATTACH CMAPIAttachment::Detach()
{
	LPATTACH pAttachment=Attachment();
	m_pItem=NULL;
	return pAttachment;
}

BOOL CMAPIAttachment::Create(LPMESSAGE pMessage)
{
	Close();

	BOOL bResult=FALSE;
	if(pMessage)
	{
		LPATTACH pAttachment;
		ULONG ulAttachmentNum=0;

		if(pMessage->CreateAttach(NULL, 0, &ulAttachmentNum, &pAttachment)==S_OK) 
		{
			m_pItem=pAttachment;

			const int nProperties=2;
			SPropValue prop[nProperties];
			memset(prop, 0,sizeof(SPropValue)*nProperties);
			prop[0].ulPropTag=PR_ATTACH_METHOD;
			prop[0].Value.ul=ATTACH_BY_VALUE;
			prop[1].ulPropTag=PR_RENDERING_POSITION;
			prop[1].Value.l=1;
			if(pAttachment->SetProps(nProperties, prop, NULL)==S_OK) 
			{
				bResult=TRUE;
			}
		}
	}
	return bResult;
}

BOOL CMAPIAttachment::Open(LPMESSAGE pMessage, int nIndex)
{
	Close();

	BOOL bResult=FALSE;
	LPMAPITABLE pAttachTable;
	if(nIndex>=0 && pMessage && pMessage->GetAttachmentTable(0, &pAttachTable)==S_OK) 
	{
		enum { PROP_ATTACH_NUM, ATTACH_COLS };
		static SizedSPropTagArray(ATTACH_COLS, Columns)={ATTACH_COLS, PR_ATTACH_NUM };
		if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK)
		{
			if(pAttachTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL)==S_OK)
			{
				LPSRowSet pRows=NULL;
				if(pAttachTable->QueryRows(1, 0, &pRows)==S_OK) 
				{
					if(pRows->cRows > 0)
					{
						LPATTACH pAttachment;
						if(pMessage->OpenAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.ul, NULL, 0, &pAttachment)==S_OK)
						{
							m_pItem=pAttachment;
							bResult=TRUE;
						}
					}
					FreeProws(pRows);
				}
			}
		}
	}
	return bResult;
}

void CMAPIAttachment::Close()
{
	CloseStream();
	CMAPIObject::Close();
}

BOOL CMAPIAttachment::GetDisplayName(CString& strDisplayName)
{
	return GetPropertyString(PR_DISPLAY_NAME, strDisplayName);
}

BOOL CMAPIAttachment::GetFileName(CString& strFileName)
{
	return GetPropertyString(PR_ATTACH_FILENAME, strFileName);
}

BOOL CMAPIAttachment::GetLongFileName(CString& strLongFileName)
{
	return GetPropertyString(PR_ATTACH_LONG_FILENAME, strLongFileName);
}

BOOL CMAPIAttachment::GetCID(CString& strCID)
{
	return GetPropertyString(PR_ATTACH_CONTENT_ID, strCID);
}

BOOL CMAPIAttachment::SetDisplayName(LPCTSTR szDisplayName)
{
	return SetPropertyString(PR_DISPLAY_NAME, szDisplayName);
}

BOOL CMAPIAttachment::SetFileName(LPCTSTR szFileName)
{
	return SetPropertyString(PR_ATTACH_FILENAME, szFileName);
}

BOOL CMAPIAttachment::SetLongFileName(LPCTSTR szLongFileName)
{
	return SetPropertyString(PR_ATTACH_LONG_FILENAME, szLongFileName);
}

BOOL CMAPIAttachment::SetCID(LPCTSTR szCID)
{
	return SetPropertyString(PR_ATTACH_CONTENT_ID, szCID);
}

BOOL CMAPIAttachment::OpenStream(BOOL bCreate)
{
	if(m_pItem) 
	{
		ULONG ulInterfaceOptions=STGM_READ;
		ULONG ulFlags=0;
		if(bCreate)
		{
			ulInterfaceOptions=0;
			ulFlags=MAPI_MODIFY | MAPI_CREATE;
		}
		return (m_pItem->OpenProperty(PR_ATTACH_DATA_BIN, &IID_IStream, ulInterfaceOptions, ulFlags, (LPUNKNOWN*)&m_pStream)==S_OK);
	}
	return FALSE;
}

int CMAPIAttachment::Read(BYTE* pData, int nCount)
{
	if(m_pStream)
	{
		ULONG ulRead;
		m_pStream->Read(pData, nCount, &ulRead);
		return ulRead;
	}
	return -1;
}

int CMAPIAttachment::Write(BYTE* pData, int nCount)
{
	if(m_pStream)
	{
		ULONG ulWritten;
		m_pStream->Write(pData, nCount, &ulWritten);
		m_nBytesWritten+=ulWritten;
		return ulWritten;
	}
	return -1;
}

void CMAPIAttachment::CloseStream()
{
	if(m_pStream)
	{
		m_pStream->Commit(STGC_DEFAULT);
		RELEASE(m_pStream);
		SetPropertyValue(PR_ATTACH_SIZE, m_nBytesWritten);
		m_nBytesWritten=0;
	}
}

BOOL CMAPIAttachment::LoadAttachment(LPCTSTR szPath)
{
	CFile file;
	if(file.Open(szPath, CFile::modeRead) && OpenStream(TRUE))
	{
		const int BUF_SIZE=4096;
		BYTE pData[BUF_SIZE];
		ULONG ulRead;

		ulRead=file.Read(pData, BUF_SIZE);
		while(ulRead) 
		{
			Write(pData, ulRead);
			ulRead=file.Read(pData, BUF_SIZE);
		}

		file.Close();
		CloseStream();
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIAttachment::SaveAttachment(LPCTSTR szPath)
{
	CFile file;
	if(file.Open(szPath, CFile::modeCreate | CFile::modeWrite) && OpenStream(FALSE))
	{
		const int BUF_SIZE=4096;
		BYTE pData[BUF_SIZE];
		ULONG ulRead;

		do {
			ulRead=Read(pData, BUF_SIZE);
			if(ulRead) file.Write(pData, ulRead);
		} while(ulRead>=BUF_SIZE);


		file.Close();
		CloseStream();
		return TRUE;
	}
	return FALSE;
}
