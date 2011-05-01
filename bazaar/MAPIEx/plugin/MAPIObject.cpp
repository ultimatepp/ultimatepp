////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIObject.cpp
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

#include "MAPIExPCH.h"
#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// CMAPIObject

CMAPIObject::CMAPIObject()
{
	m_pMAPI=NULL;
	m_pItem=NULL;
	m_entryID.cb=0;
	SetEntryID(NULL);
}

CMAPIObject::~CMAPIObject()
{
}

BOOL CMAPIObject::GetHexString(CString& strHex, SBinary bin)
{
	strHex.Empty();
	if(bin.cb) 
	{
		TCHAR szBuffer[3];
		for(ULONG i=0;i<bin.cb;i++)
		{
			_stprintf_s(szBuffer, 3, _T("%02X"), bin.lpb[i]);
			strHex+=szBuffer;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::GetEntryIDString(CString& strEntryID)
{
	return GetHexString(strEntryID, m_entryID);
}

void CMAPIObject::SetEntryID(SBinary* pEntryID)
{
	if(m_entryID.cb) delete [] m_entryID.lpb;
	m_entryID.lpb=NULL;

	if(pEntryID) 
	{
		m_entryID.cb=pEntryID->cb;
		if(m_entryID.cb) 
		{
			m_entryID.lpb=new BYTE[m_entryID.cb];
			memcpy(m_entryID.lpb, pEntryID->lpb, m_entryID.cb);
		}
	} 
	else 
	{
		m_entryID.cb=0;
	}
}

BOOL CMAPIObject::Open(CMAPIEx* pMAPI, SBinary entryID)
{
	Close();
	m_pMAPI=pMAPI;
	ULONG ulObjType;
	if(m_pMAPI->GetSession()->OpenEntry(entryID.cb, (LPENTRYID)entryID.lpb, NULL, MAPI_BEST_ACCESS, &ulObjType, (LPUNKNOWN*)&m_pItem)!=S_OK) return FALSE;
	SetEntryID(&entryID);
	return TRUE;
}

void CMAPIObject::Close()
{
	SetEntryID(NULL);
	RELEASE(m_pItem);
	m_pMAPI=NULL;
}

BOOL CMAPIObject::Save(BOOL bClose)
{
	ULONG ulFlags=bClose ? 0 : KEEP_OPEN_READWRITE;
	if(m_pItem && m_pItem->SaveChanges(ulFlags)==S_OK) 
	{
		if(bClose) Close();
		return TRUE;
	}
	return FALSE;
}

int CMAPIObject::GetMessageFlags()
{
	return GetPropertyValue(PR_MESSAGE_FLAGS, 0);
}

BOOL CMAPIObject::SetMessageFlags(int nFlags)
{
	SPropValue prop;
	prop.ulPropTag=PR_MESSAGE_FLAGS;
	prop.Value.l=nFlags;
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL)==S_OK);
}

int CMAPIObject::GetSensitivity()
{
	return GetPropertyValue(PR_SENSITIVITY, -1);
}

BOOL CMAPIObject::SetSensitivity(int nSensitivity)
{
	SPropValue prop;
	prop.ulPropTag=PR_SENSITIVITY;
	prop.Value.l=nSensitivity;
	return (Message()->SetProps(1, &prop, NULL)==S_OK);
}

int CMAPIObject::GetImportance()
{
	return GetPropertyValue(PR_IMPORTANCE, IMPORTANCE_NORMAL);
}

BOOL CMAPIObject::SetImportance(int nImportance)
{
	SPropValue prop;
	prop.ulPropTag=PR_IMPORTANCE;
	prop.Value.l=nImportance;
	return (Message()->SetProps(1, &prop, NULL)==S_OK);
}

BOOL CMAPIObject::GetMessageClass(CString& strMessageClass)
{
	return GetPropertyString(PR_MESSAGE_CLASS, strMessageClass);
}

BOOL CMAPIObject::SetMessageClass(LPCTSTR szMessageClass)
{
	SPropValue prop;
	prop.ulPropTag=PR_MESSAGE_CLASS;
	prop.Value.LPSZ=(LPTSTR)szMessageClass;
	return (Message()->SetProps(1, &prop, NULL)==S_OK);
}

int CMAPIObject::GetMessageEditorFormat()
{
	return GetPropertyValue(PR_MSG_EDITOR_FORMAT, EDITOR_FORMAT_DONTKNOW);
}

BOOL CMAPIObject::SetMessageEditorFormat(int nFormat)
{
	SPropValue prop;
	prop.ulPropTag=PR_MSG_EDITOR_FORMAT;
	prop.Value.l=nFormat;
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL)==S_OK);
}

BOOL CMAPIObject::Create(CMAPIEx* pMAPI, CMAPIFolder* pFolder)
{
	if(!pMAPI) return FALSE;
	if(!pFolder) pFolder=pMAPI->GetFolder();
	if(!pFolder) return FALSE;
	Close();
	m_pMAPI=pMAPI;
	if(pFolder->Folder()->CreateMessage(NULL, 0, (LPMESSAGE*)&m_pItem)==S_OK) 
	{
		LPSPropValue pProp;
		if(GetProperty(PR_ENTRYID, pProp)==S_OK) 
		{
			SetEntryID(&pProp->Value.bin);
			MAPIFreeBuffer(pProp);
		} 
		return TRUE;
	}
	return FALSE;
}

HRESULT CMAPIObject::GetProperty(ULONG ulProperty, LPSPropValue& pProp)
{
	if(!m_pItem) return E_INVALIDARG;
	ULONG ulPropCount;
	ULONG p[2]={ 1, ulProperty };
	return m_pItem->GetProps((LPSPropTagArray)p, CMAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
}

BOOL CMAPIObject::GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream)
{
	strProperty=_T("");
	if(bStream)
	{
		IStream* pStream;
		if(Message()->OpenProperty(ulProperty, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&pStream)==S_OK) 
		{
			const int BUF_SIZE=16384;
			TCHAR szBuf[BUF_SIZE+1];
			ULONG ulNumChars;

			do 
			{
				pStream->Read(szBuf, BUF_SIZE*sizeof(TCHAR), &ulNumChars);
				ulNumChars/=sizeof(TCHAR);
				szBuf[min(BUF_SIZE,ulNumChars)]=0;
				strProperty+=szBuf;
			} while(ulNumChars>=BUF_SIZE);

			RELEASE(pStream);
			return TRUE;
		}
	}
	else 
	{
		if (ulProperty==PR_ENTRYID)
		{
			return GetEntryIDString(strProperty);
		}

		LPSPropValue pProp;
		if(GetProperty(ulProperty, pProp)==S_OK) 
		{
			strProperty=CMAPIEx::GetValidString(*pProp);
			MAPIFreeBuffer(pProp);
			return TRUE;
		} 
	}
	return FALSE;
}

int CMAPIObject::GetPropertyValue(ULONG ulProperty, int nDefaultValue)
{
	LPSPropValue pProp;
	if(GetProperty(ulProperty, pProp)==S_OK) 
	{
		nDefaultValue=pProp->Value.l;
		MAPIFreeBuffer(pProp);
	}
	return nDefaultValue;
}

const GUID GUIDPublicStrings={ 0x00020329, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

BOOL CMAPIObject::GetPropTagArray(LPCTSTR szFieldName, LPSPropTagArray& lppPropTags, int& nFieldType, BOOL bCreate)
{
	if(!m_pItem) return FALSE;

	MAPINAMEID nameID;
	nameID.lpguid=(GUID*)&GUIDPublicStrings;
	nameID.ulKind=MNID_STRING;
#ifdef UNICODE
	nFieldType=PT_UNICODE;
	nameID.Kind.lpwstrName=(LPWSTR)szFieldName;
#else
	nFieldType=PT_STRING8;
	WCHAR wszFieldName[256];
	MultiByteToWideChar(CP_ACP, 0, szFieldName, -1, wszFieldName, 255);
	nameID.Kind.lpwstrName=wszFieldName;
#endif

	LPMAPINAMEID lpNameID[1]={ &nameID };

	HRESULT hr=m_pItem->GetIDsFromNames(1, lpNameID, bCreate ? MAPI_CREATE : 0, &lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::GetNamedProperty(LPCTSTR szFieldName, LPSPropValue &pProp)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, FALSE)) return FALSE;

	ULONG ulPropCount;
	HRESULT hr=m_pItem->GetProps(lppPropTags, CMAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::GetNamedProperty(LPCTSTR szFieldName, CString& strField)
{
	LPSPropValue pProp;
	if(GetNamedProperty(szFieldName, pProp)) 
	{
		strField=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::GetOutlookPropTagArray(ULONG ulData, ULONG ulProperty, LPSPropTagArray& lppPropTags, int& nFieldType, BOOL bCreate)
{
	if(!m_pItem) return FALSE;

	const GUID guidOutlookEmail1={ulData, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

	MAPINAMEID nameID;
	nameID.lpguid=(GUID*)&guidOutlookEmail1;
	nameID.ulKind=MNID_ID;
	nameID.Kind.lID=ulProperty;

#ifdef UNICODE
	nFieldType=PT_UNICODE;
#else
	nFieldType=PT_STRING8;
#endif

	LPMAPINAMEID lpNameID[1]={ &nameID };

	HRESULT hr=m_pItem->GetIDsFromNames(1, lpNameID, bCreate ? MAPI_CREATE : 0, &lppPropTags);
	return (hr==S_OK);
}

// gets a custom outlook property (ie EmailAddress1 of a contact)
BOOL CMAPIObject::GetOutlookProperty(ULONG ulData, ULONG ulProperty, LPSPropValue& pProp)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nFieldType, FALSE)) return FALSE;

	ULONG ulPropCount;
	HRESULT hr=m_pItem->GetProps(lppPropTags, CMAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

// gets a custom outlook property value 
BOOL CMAPIObject::GetOutlookPropertyValue(ULONG ulData, ULONG ulProperty, int& nValue)
{
	LPSPropValue pProp;
	if(GetOutlookProperty(ulData, ulProperty, pProp)) 
	{
		nValue=pProp->Value.i;
		MAPIFreeBuffer(pProp);
		return TRUE;
	} 
	return FALSE;
}

// gets a custom outlook property string (ie EmailAddress1 of a contact)
BOOL CMAPIObject::GetOutlookPropertyString(ULONG ulData, ULONG ulProperty, CString& strProperty)
{
	LPSPropValue pProp;
	if(GetOutlookProperty(ulData, ulProperty, pProp)) 
	{
		strProperty=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return TRUE;
	} 
	return FALSE;
}

BOOL CMAPIObject::SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream)
{
	if(m_pItem && szProperty) 
	{
		if(bStream)
		{
			LPSTREAM pStream=NULL;
			if(m_pItem->OpenProperty(ulProperty, &IID_IStream, 0, MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream)==S_OK) 
			{
				pStream->Write(szProperty, (ULONG)(_tcslen(szProperty)+1)*sizeof(TCHAR), NULL);
				RELEASE(pStream);
				return TRUE;
			}
		}
		else
		{
			SPropValue prop;
			prop.ulPropTag=ulProperty;
			prop.Value.LPSZ=(LPTSTR)szProperty;
			return (m_pItem->SetProps(1, &prop, NULL)==S_OK);
		}
	}
	return FALSE;
}

BOOL CMAPIObject::SetPropertyValue(ULONG ulProperty, int nValue)
{
	if(m_pItem)
	{
		SPropValue prop;
		prop.ulPropTag=ulProperty;
		prop.Value.l=nValue;
		return (m_pItem->SetProps(1, &prop, NULL)==S_OK);
	}
	return FALSE;
}

// if bCreate is true, the property will be created if necessary otherwise if not present will return FALSE
BOOL CMAPIObject::SetNamedProperty(LPCTSTR szFieldName, LPCTSTR szField, BOOL bCreate)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, bCreate)) return FALSE;

	SPropValue prop;
	prop.ulPropTag=(lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.LPSZ=(LPTSTR)szField;
	HRESULT hr=m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::SetNamedMVProperty(LPCTSTR szFieldName, LPCTSTR* arCategories, int nCount, LPSPropValue &pProp, BOOL bCreate)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, bCreate)) return FALSE;

	HRESULT hr=MAPIAllocateBuffer(sizeof(SPropValue), (LPVOID*)&pProp);
	if(hr==S_OK) 
	{
		pProp->ulPropTag=(lppPropTags->aulPropTag[0]|nFieldType|MV_FLAG);
		pProp->Value.MVi.cValues=nCount;
		if(PROP_TYPE(pProp->ulPropTag)==PT_MV_STRING8) 
		{
			hr=MAPIAllocateMore(sizeof(LPSTR)*nCount, pProp, (LPVOID*)&pProp->Value.MVszA.lppszA);
		} 
		else 
		{ 
			// PT_MV_UNICODE
			hr=MAPIAllocateMore(sizeof(LPSTR)*nCount, pProp, (LPVOID*)&pProp->Value.MVszW.lppszW);
		}
		if(hr==S_OK) 
		{
			for(int i=0;i<nCount;i++) pProp->Value.MVSZ.LPPSZ[i]=(LPTSTR)arCategories[i];
			hr=m_pItem->SetProps(1, pProp, NULL);
		}
		if(hr!=S_OK) MAPIFreeBuffer(pProp);
	}
	return (hr==S_OK);
}

BOOL CMAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, LPCTSTR szField)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData, ulProperty, lppPropTags, nFieldType, FALSE)) return FALSE;

	SPropValue prop;
	prop.ulPropTag=(lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.LPSZ=(LPTSTR)szField;
	HRESULT hr=m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, int nField, int nFieldType)
{
	LPSPropTagArray lppPropTags;
	int nIgnored;
	if(!GetOutlookPropTagArray(ulData, ulProperty, lppPropTags, nIgnored, FALSE)) return FALSE;

	SPropValue prop;
	prop.ulPropTag=(lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.l=nField;
	HRESULT hr=m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, FILETIME ftField)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nFieldType, FALSE)) return FALSE;
	nFieldType=PT_SYSTIME;

	SPropValue prop;
	prop.ulPropTag=(lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.ft=ftField;
	HRESULT hr=m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

BOOL CMAPIObject::DeleteNamedProperty(LPCTSTR szFieldName)
{
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, FALSE)) return FALSE;

	HRESULT hr=m_pItem->DeleteProps(lppPropTags, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

int CMAPIObject::GetAttachmentCount()
{
	ULONG ulCount=0;
	LPMAPITABLE pAttachTable;
	if(Message()->GetAttachmentTable(0, &pAttachTable)==S_OK) 
	{
#ifndef _WIN32_WCE
		if(pAttachTable->GetRowCount(0, &ulCount)!=S_OK) ulCount=0;
#else
		enum { PROP_ATTACH_CONTENT_ID, ATTACH_COLS };
		static SizedSPropTagArray(ATTACH_COLS, Columns)={ATTACH_COLS, PR_ATTACH_CONTENT_ID };
		if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK)
		{
			ULONG cRows;
			do {
				LPSRowSet pRows=NULL;
				if(pAttachTable->QueryRows(1, 0, &pRows)!=S_OK) 
				{
					MAPIFreeBuffer(pRows);
					break;
				}
				cRows=pRows->cRows;
				ulCount+=cRows;
				FreeProws(pRows);
			} while(cRows);
		}
#endif
		RELEASE(pAttachTable);
	}
	return ulCount;
}

BOOL CMAPIObject::GetAttachment(CMAPIAttachment& attachment, int nIndex)
{
	return attachment.Open(Message(), nIndex);
}

BOOL CMAPIObject::AddAttachment(CMAPIAttachment& attachment)
{
	return attachment.Create(Message());
}

BOOL CMAPIObject::AddAttachment(LPCTSTR szPath, LPCTSTR szName, LPCTSTR szCID)
{
	CFile file;
	if(!file.Open(szPath, CFile::modeRead)) return FALSE;
	file.Close();

	LPTSTR szFileName=(LPTSTR)szName;
	if(!szFileName) 
	{
		szFileName=(LPTSTR)szPath;
		for(int i=(int)_tcsclen(szPath)-1;i>=0;i--) if(szPath[i]=='\\' || szPath[i]=='/') 
		{
			szFileName=(LPTSTR)&szPath[i+1];
			break;
		}
	}

	CMAPIAttachment attachment;
	if(attachment.Create(Message()))
	{
		if(!szCID || _tcscmp(szCID, CONTACT_PICTURE)) 
		{
			attachment.SetFileName(szFileName);
			attachment.SetLongFileName(szFileName);
			if(szCID) attachment.SetCID(szCID);
		} 
		else 
		{
			attachment.SetFileName(szCID);
			attachment.SetLongFileName(szCID);
			attachment.SetPropertyValue(PR_ATTACHMENT_UNKNOWN, TRUE);
		}
		if(attachment.LoadAttachment(szPath))
		{
			attachment.Save();
			return TRUE;
		}
	}
	return FALSE;
}

// use nIndex of -1 to delete all attachments
BOOL CMAPIObject::DeleteAttachment(int nIndex)
{
	LPMAPITABLE pAttachTable=NULL;
	if(Message()->GetAttachmentTable(0, &pAttachTable)!=S_OK) return FALSE;

	BOOL bResult=FALSE;
	enum { PROP_ATTACH_NUM, ATTACH_COLS };
	static SizedSPropTagArray(ATTACH_COLS, Columns)={ATTACH_COLS, PR_ATTACH_NUM };
	if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK) 
	{
		int i=0;
		LPSRowSet pRows=NULL;
		while(TRUE) 
		{
			if(pAttachTable->QueryRows(1, 0, &pRows)==S_OK) 
			{
				if(!pRows->cRows) break;
				else if(i<nIndex)
				{
					i++;
					continue;
				}
				else
				{
					bResult=(Message()->DeleteAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.ul, 0, NULL, 0)==S_OK);
				}
				FreeProws(pRows);
				if(!bResult || nIndex==i) break;
			}
			break;
		}
	}
	RELEASE(pAttachTable);
	return bResult;
}

// use nIndex of -1 to save all attachments to szFolder
BOOL CMAPIObject::SaveAttachment(LPCTSTR szFolder, int nIndex, LPCTSTR szFileName)
{
	LPMAPITABLE pAttachTable=NULL;
	if(Message()->GetAttachmentTable(0, &pAttachTable)!=S_OK) return FALSE;

	CString strPath;
	BOOL bResult=FALSE;
	enum { PROP_ATTACH_NUM, PROP_ATTACH_LONG_FILENAME, PROP_ATTACH_FILENAME, ATTACH_COLS };
	static SizedSPropTagArray(ATTACH_COLS, Columns)={ATTACH_COLS, PR_ATTACH_NUM, PR_ATTACH_LONG_FILENAME, PR_ATTACH_FILENAME };
	if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0)==S_OK)
	{
		LPSRowSet pRows=NULL;
		CMAPIAttachment attachment;
		while(TRUE)
		{
			if(nIndex>=0 && pAttachTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL)!=S_OK) break; 
			if(pAttachTable->QueryRows(1, 0, &pRows)==S_OK)
			{
				if(!pRows->cRows) break;
				LPATTACH pAttachment;
				if(Message()->OpenAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.bin.cb, NULL, 0, &pAttachment)==S_OK)
				{
					if (szFileName != NULL)
					{
						strPath.Format(_T("%s\\%s"), szFolder, szFileName);
					}
					else
					{
						if(CMAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ATTACH_LONG_FILENAME])) strPath.Format(_T("%s\\%s"), szFolder, pRows->aRow[0].lpProps[PROP_ATTACH_LONG_FILENAME].Value.LPSZ);
						else if(CMAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ATTACH_FILENAME])) strPath.Format(_T("%s\\%s"), szFolder, pRows->aRow[0].lpProps[PROP_ATTACH_FILENAME].Value.LPSZ);
						else strPath.Format(_T("%s\\Attachment.dat"), szFolder);
					}
					attachment.Attach(pAttachment);
					attachment.SaveAttachment(strPath);
					bResult=attachment.SaveAttachment(strPath);
				}
			}
			FreeProws(pRows);
			if(nIndex>=0) break;
		}
	}
	RELEASE(pAttachTable);
	return bResult;
}

// Gets the body of the item, if bAutoDetect is set, uses the MessageEditorFormat to try to determine which property to return 
BOOL CMAPIObject::GetBody(CString& strBody, BOOL bAutoDetect)
{
	if(bAutoDetect)
	{
		int nFormat=GetMessageEditorFormat();
		if(nFormat==EDITOR_FORMAT_DONTKNOW || nFormat==EDITOR_FORMAT_RTF) return GetRTF(strBody);
		else if(nFormat==EDITOR_FORMAT_HTML) return GetHTML(strBody);
	}
	return GetPropertyString(PR_BODY, strBody, TRUE);
}

BOOL CMAPIObject::SetBody(LPCTSTR szBody)
{
	if(SetPropertyString(PR_BODY, szBody, TRUE))
	{
		SetMessageEditorFormat(EDITOR_FORMAT_PLAINTEXT);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::GetHTML(CString& strHTML)
{
	return GetPropertyString(PR_BODY_HTML, strHTML, TRUE);
}

BOOL CMAPIObject::SetHTML(LPCTSTR szHTML)
{
	if(szHTML)
	{
		// does this Message Store support HTML directly?
		ULONG ulSupport=m_pMAPI ? m_pMAPI->GetMessageStoreSupport() : 0;
		if(ulSupport&STORE_HTML_OK) 
		{
			if(SetPropertyString(PR_BODY_HTML, szHTML, TRUE))
			{
				SetMessageEditorFormat(EDITOR_FORMAT_HTML);
				return TRUE;
			}
		} 
		else 
		{
			// otherwise lets encode it into RTF 
			TCHAR szCodePage[6]=_T("1252"); // default codepage is ANSI - Latin I
			GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szCodePage, sizeof(szCodePage));

			CString strRTF;
			strRTF.Format(_T("{\\rtf1\\ansi\\ansicpg%s\\fromhtml1 {\\*\\htmltag1 "), szCodePage);
			strRTF+=szHTML;
			strRTF+=_T(" }}");

			return SetRTF(strRTF);
		}
	}
	return FALSE;
}

BOOL CMAPIObject::GetRTF(CString& strRTF)
{
	strRTF=_T("");
	IStream* pStream;

	const int BUF_SIZE=16384;
	char szBuf[BUF_SIZE+1];
	ULONG ulNumChars;

#ifdef _WIN32_WCE
	int nMessageStatus=GetMessageStatus();
	if(nMessageStatus & MSGSTATUS_PARTIAL) return;

	if(nMessageStatus & MSGSTATUS_HAS_PR_BODY_HTML) 
	{
		if(Message()->OpenProperty(PR_BODY_HTML_A, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&pStream)!=S_OK) return FALSE;

		do 
		{
			pStream->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min(BUF_SIZE,ulNumChars)]=0;
			strRTF+=szBuf;
		} while(ulNumChars>=BUF_SIZE);

		RELEASE(pStream);
	} 
	else if(nMessageStatus & MSGSTATUS_HAS_PR_BODY) 
	{
		if(Message()->OpenProperty(PR_BODY_A, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&pStream)!=S_OK) return FALSE;

		do 
		{
			pStream->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min(BUF_SIZE,ulNumChars)]=0;
			strRTF+=szBuf;
		} while(ulNumChars>=BUF_SIZE);

		RELEASE(pStream);
	}
#else
	if(Message()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_READ, 0, (LPUNKNOWN*)&pStream)!=S_OK) return FALSE;

	IStream *pUncompressed;
	if(WrapCompressedRTFStream(pStream, 0, &pUncompressed)==S_OK) 
	{
		do 
		{
			pUncompressed->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min(BUF_SIZE,ulNumChars)]=0;
			strRTF+=szBuf;
		} while(ulNumChars>=BUF_SIZE);
		RELEASE(pUncompressed);
	}

	RELEASE(pStream);
#endif

	// does this RTF contain encoded HTML, or text? 
	if(strRTF.Find(_T("\\fromhtml"))!=-1) 
	{
		return GetHTML(strRTF);
	}
	else if(strRTF.Find(_T("\\fromtext"))!=-1) 
	{
		return GetBody(strRTF, FALSE);
	}

	return TRUE;
}

BOOL CMAPIObject::SetRTF(LPCTSTR szRTF)
{
	LPSTREAM pStream=NULL;
	if(Message()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_CREATE | STGM_WRITE, MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream)==S_OK) 
	{
#ifndef _WIN32_WCE
		IStream *pUncompressed;
		if(WrapCompressedRTFStream(pStream,MAPI_MODIFY, &pUncompressed)==S_OK) 
		{
			pUncompressed->Write(szRTF, (ULONG)_tcslen(szRTF)*sizeof(TCHAR), NULL);
			if(pUncompressed->Commit(STGC_DEFAULT)==S_OK) pStream->Commit(STGC_DEFAULT);
			RELEASE(pUncompressed);
		}
#endif
		RELEASE(pStream);
		SetMessageEditorFormat(EDITOR_FORMAT_RTF);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::GetLastModified(SYSTEMTIME& tmLastModified)
{
	LPSPropValue pProp;
	if(GetProperty(PR_LAST_MODIFICATION_TIME, pProp)==S_OK) 
	{
		SYSTEMTIME tm;
		FileTimeToSystemTime(&pProp->Value.ft, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &tmLastModified);

		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::GetLastModified(CString& strLastModified, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetLastModified(tm)) 
	{
		TCHAR szDate[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szDate, 256);
		strLastModified=szDate;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIObject::SetLastModified(SYSTEMTIME& tmLastModified)
{
	SPropValue prop;
	prop.ulPropTag=PR_LAST_MODIFICATION_TIME;
	SystemTimeToFileTime(&tmLastModified, &prop.Value.ft);
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL)==S_OK);
}
