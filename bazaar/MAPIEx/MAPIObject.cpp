#ifdef _WIN32

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

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"
#include <winnls.h>

/////////////////////////////////////////////////////////////
// MAPIObject

MAPIObject::MAPIObject() {
	m_pMAPI = NULL;
	m_pItem = NULL;
	m_entryID.cb = 0;
	SetEntryID(NULL);
}

MAPIObject::~MAPIObject() {
}

String MAPIObject::GetHexString(SBinary bin) {
	String strHex;
	if(bin.cb) {
		for(ULONG i = 0; i < bin.cb; i++) 
			strHex += Format("%02X", bin.lpb[i]);
		return strHex;
	}
	return String();
}

String MAPIObject::GetEntryIDString() {
	return GetHexString(m_entryID);
}

void MAPIObject::SetEntryID(SBinary* pEntryID) {
	if(m_entryID.cb) 
		delete [] m_entryID.lpb;
	m_entryID.lpb=NULL;

	if(pEntryID) {
		m_entryID.cb = pEntryID->cb;
		if(m_entryID.cb) {
			m_entryID.lpb = new BYTE[m_entryID.cb];
			memcpy(m_entryID.lpb, pEntryID->lpb, m_entryID.cb);
		}
	} else 
		m_entryID.cb = 0;
}

bool MAPIObject::Open(MAPIEx* pMAPI, SBinary entryID) {
	Close();
	m_pMAPI = pMAPI;
	ULONG ulObjType;
	if(S_OK != m_pMAPI->GetSession()->OpenEntry(entryID.cb, (LPENTRYID)entryID.lpb, NULL, 
										MAPI_BEST_ACCESS, &ulObjType, (LPUNKNOWN*)&m_pItem)) 
		return false;
	SetEntryID(&entryID);
	return true;
}

void MAPIObject::Close() {
	SetEntryID(NULL);
	RELEASE(m_pItem);
	m_pMAPI = NULL;
}

bool MAPIObject::Save(bool bClose) {
	ULONG ulFlags = bClose ? 0 : KEEP_OPEN_READWRITE;
	if(m_pItem && m_pItem->SaveChanges(ulFlags) == S_OK) {
		if(bClose) 
			Close();
		return true;
	}
	return false;
}

int MAPIObject::GetMessageFlags() {
	return GetPropertyValue(PR_MESSAGE_FLAGS, 0);
}

bool MAPIObject::SetMessageFlags(int nFlags) {
	SPropValue prop;
	prop.ulPropTag = PR_MESSAGE_FLAGS;
	prop.Value.l = nFlags;
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL) == S_OK);
}

int MAPIObject::GetSensitivity() {
	return GetPropertyValue(PR_SENSITIVITY, -1);
}

bool MAPIObject::SetSensitivity(int nSensitivity) {
	SPropValue prop;
	prop.ulPropTag = PR_SENSITIVITY;
	prop.Value.l = nSensitivity;
	return (Message()->SetProps(1, &prop, NULL) == S_OK);
}

int MAPIObject::GetImportance() {
	return GetPropertyValue(PR_IMPORTANCE, IMPORTANCE_NORMAL);
}

bool MAPIObject::SetImportance(int nImportance) {
	SPropValue prop;
	prop.ulPropTag = PR_IMPORTANCE;
	prop.Value.l = nImportance;
	return (Message()->SetProps(1, &prop, NULL) == S_OK);
}

String MAPIObject::GetMessageClass() {
	return GetPropertyString(PR_MESSAGE_CLASS);
}

bool MAPIObject::SetMessageClass(String szMessageClass) {
	SPropValue prop;
	prop.ulPropTag = PR_MESSAGE_CLASS;
	prop.Value.LPSZ = (LPSTR)szMessageClass.Begin();
	return (Message()->SetProps(1, &prop, NULL) == S_OK);
}

int MAPIObject::GetMessageEditorFormat() {
	return GetPropertyValue(PR_MSG_EDITOR_FORMAT, EDITOR_FORMAT_DONTKNOW);
}

bool MAPIObject::SetMessageEditorFormat(int nFormat) {
	SPropValue prop;
	prop.ulPropTag = PR_MSG_EDITOR_FORMAT;
	prop.Value.l = nFormat;
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL) == S_OK);
}

bool MAPIObject::Create(MAPIEx &mapi, MAPIFolder &folder) {
	if(!folder.IsOpened()) 
		return false;
	Close();
	m_pMAPI = &mapi;
	if(folder.Folder()->CreateMessage(NULL, 0, (LPMESSAGE*)&m_pItem) == S_OK) {
		LPSPropValue pProp;
		if(GetProperty(PR_ENTRYID, pProp) == S_OK) {
			SetEntryID(&pProp->Value.bin);
			MAPIFreeBuffer(pProp);
		} 
		return true;
	}
	return false;
}

HRESULT MAPIObject::GetProperty(ULONG ulProperty, LPSPropValue& pProp) {
	if(!m_pItem) 
		return E_INVALIDARG;
	ULONG ulPropCount;
	ULONG p[2] = {1, ulProperty};
	return m_pItem->GetProps((LPSPropTagArray)p, MAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
}

String MAPIObject::GetPropertyString(ULONG ulProperty, bool bStream) {
	String strProperty; 
	if(bStream) {
		IStream* pStream;
		if(Message()->OpenProperty(ulProperty, &IID_IStream,STGM_READ, 0, (LPUNKNOWN*)&pStream) 
																					== S_OK) {
			const int BUF_SIZE = 16384;
			TCHAR szBuf[BUF_SIZE+1];
			ULONG ulNumChars;

			do {
				pStream->Read(szBuf, BUF_SIZE*sizeof(TCHAR), &ulNumChars);
				ulNumChars /= sizeof(TCHAR);
				szBuf[min<ULONG>(BUF_SIZE,ulNumChars)] = 0;
				strProperty += szBuf;
			} while(ulNumChars >= BUF_SIZE);

			RELEASE(pStream);
			return strProperty;
		}
	} else {
		if (ulProperty == PR_ENTRYID)
			return GetEntryIDString();
		LPSPropValue pProp;
		if(GetProperty(ulProperty, pProp) == S_OK) {
			strProperty = MAPIEx::GetValidString(*pProp);
			MAPIFreeBuffer(pProp);
			return strProperty;
		} 
	}
	return String();
}

int MAPIObject::GetPropertyValue(ULONG ulProperty, int nDefaultValue) {
	LPSPropValue pProp;
	if(GetProperty(ulProperty, pProp) == S_OK) {
		nDefaultValue = pProp->Value.l;
		MAPIFreeBuffer(pProp);
	}
	return nDefaultValue;
}

const GUID GUIDPublicStrings={0x00020329, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

bool MAPIObject::GetPropTagArray(LPCTSTR szFieldName, LPSPropTagArray& lppPropTags, int& nFieldType, bool bCreate) {
	if(!m_pItem) 
		return false;

	MAPINAMEID nameID;
	nameID.lpguid = (GUID*)&GUIDPublicStrings;
	nameID.ulKind = MNID_STRING;

	nFieldType = PT_STRING8;
	WCHAR wszFieldName[256];
	MultiByteToWideChar(CP_ACP, 0, szFieldName,-1,wszFieldName,255);
	nameID.Kind.lpwstrName = wszFieldName;

	LPMAPINAMEID lpNameID[1] = {&nameID};

	HRESULT hr = m_pItem->GetIDsFromNames(1, lpNameID, bCreate ? MAPI_CREATE : 0, &lppPropTags);
	return (hr == S_OK);
}

bool MAPIObject::GetNamedProperty(LPCTSTR szFieldName, LPSPropValue &pProp) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, false)) 
		return false;

	ULONG ulPropCount;
	HRESULT hr = m_pItem->GetProps(lppPropTags, MAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

bool MAPIObject::GetNamedProperty(LPCTSTR szFieldName, String& strField) {
	LPSPropValue pProp;
	if(GetNamedProperty(szFieldName, pProp)) {
		strField = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return true;
	}
	return false;
}

bool MAPIObject::GetOutlookPropTagArray(ULONG ulData, ULONG ulProperty, 
								LPSPropTagArray& lppPropTags, int& nFieldType, bool bCreate) {
	if(!m_pItem) 
		return false;

	const GUID guidOutlookEmail1={ulData, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

	MAPINAMEID nameID;
	nameID.lpguid = (GUID*)&guidOutlookEmail1;
	nameID.ulKind = MNID_ID;
	nameID.Kind.lID = ulProperty;

	nFieldType = PT_STRING8;

	LPMAPINAMEID lpNameID[1] = {&nameID};

	HRESULT hr=m_pItem->GetIDsFromNames(1, lpNameID, bCreate ? MAPI_CREATE : 0, &lppPropTags);
	return (hr==S_OK);
}

// gets a custom outlook property (ie EmailAddress1 of a contact)
bool MAPIObject::GetOutlookProperty(ULONG ulData, ULONG ulProperty, LPSPropValue& pProp) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nFieldType, false)) 
		return false;

	ULONG ulPropCount;
	HRESULT hr = m_pItem->GetProps(lppPropTags, MAPIEx::cm_nMAPICode, &ulPropCount, &pProp);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

// gets a custom outlook property value 
bool MAPIObject::GetOutlookPropertyValue(ULONG ulData, ULONG ulProperty, int& nValue) {
	LPSPropValue pProp;
	if(GetOutlookProperty(ulData, ulProperty, pProp)) {
		nValue = pProp->Value.i;
		MAPIFreeBuffer(pProp);
		return true;
	} 
	return false;
}

// gets a custom outlook property string (ie EmailAddress1 of a contact)
bool MAPIObject::GetOutlookPropertyString(ULONG ulData, ULONG ulProperty, String& strProperty) {
	LPSPropValue pProp;
	if(GetOutlookProperty(ulData, ulProperty, pProp)) {
		strProperty = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return true;
	} 
	return false;
}

bool MAPIObject::SetPropertyString(ULONG ulProperty, String szProperty, bool bStream) {
	if(m_pItem && !IsNull(szProperty)) {
		if(bStream) {
			LPSTREAM pStream = NULL;
			if(m_pItem->OpenProperty(ulProperty, &IID_IStream, 0, MAPI_MODIFY | MAPI_CREATE, 
																(LPUNKNOWN*)&pStream)==S_OK) {
				pStream->Write(szProperty, (szProperty.ToWString().GetCount()+1)*sizeof(wchar), 
																						NULL);
				RELEASE(pStream);
				return true;
			}
		} else {
			SPropValue prop;
			prop.ulPropTag = ulProperty;
			prop.Value.LPSZ = (LPTSTR)szProperty.Begin();
			return (m_pItem->SetProps(1, &prop, NULL) == S_OK);
		}
	}
	return false;
}

bool MAPIObject::SetPropertyValue(ULONG ulProperty, int nValue) {
	if(!m_pItem) 
		return false;
	SPropValue prop;
	prop.ulPropTag = ulProperty;
	prop.Value.l = nValue;
	return (m_pItem->SetProps(1, &prop, NULL) == S_OK);
}

// if bCreate is true, the property will be created if necessary otherwise if not present will return false
bool MAPIObject::SetNamedProperty(LPCTSTR szFieldName, LPCTSTR szField, bool bCreate) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, bCreate)) 
		return false;

	SPropValue prop;
	prop.ulPropTag = (lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.LPSZ = (LPTSTR)szField;
	HRESULT hr = m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

bool MAPIObject::SetNamedMVProperty(LPCTSTR szFieldName, LPCTSTR* arCategories, int nCount, 
														LPSPropValue &pProp, bool bCreate) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, bCreate)) 
		return false;

	HRESULT hr = MAPIAllocateBuffer(sizeof(SPropValue), (LPVOID*)&pProp);
	if(hr == S_OK) {
		pProp->ulPropTag = (lppPropTags->aulPropTag[0]|nFieldType|MV_FLAG);
		pProp->Value.MVi.cValues=nCount;
		if(PROP_TYPE(pProp->ulPropTag)==PT_MV_STRING8) 
			hr = MAPIAllocateMore(sizeof(LPSTR)*nCount, pProp, (LPVOID*)&pProp->Value.MVszA.lppszA);
		else 
			// PT_MV_UNICODE
			hr = MAPIAllocateMore(sizeof(LPSTR)*nCount, pProp, (LPVOID*)&pProp->Value.MVszW.lppszW);
		if(hr == S_OK) {
			for(int i = 0; i < nCount; i++) 
				pProp->Value.MVSZ.LPPSZ[i] = (LPTSTR)arCategories[i];
			hr=m_pItem->SetProps(1, pProp, NULL);
		}
		if(hr != S_OK) 
			MAPIFreeBuffer(pProp);
	}
	return (hr == S_OK);
}

bool MAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, LPCTSTR szField) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nFieldType, false)) 
		return false;

	SPropValue prop;
	prop.ulPropTag = (lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.LPSZ = (LPTSTR)szField;
	HRESULT hr=m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

bool MAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, int nField, int nFieldType) {
	LPSPropTagArray lppPropTags;
	int nIgnored;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nIgnored, false)) 
		return false;

	SPropValue prop;
	prop.ulPropTag = (lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.l = nField;
	HRESULT hr = m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

bool MAPIObject::SetOutlookProperty(ULONG ulData, ULONG ulProperty, FILETIME ftField) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetOutlookPropTagArray(ulData,ulProperty, lppPropTags, nFieldType, false)) 
		return false;
	nFieldType = PT_SYSTIME;

	SPropValue prop;
	prop.ulPropTag = (lppPropTags->aulPropTag[0]|nFieldType);
	prop.Value.ft = ftField;
	HRESULT hr = m_pItem->SetProps(1, &prop, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr==S_OK);
}

bool MAPIObject::DeleteNamedProperty(LPCTSTR szFieldName) {
	LPSPropTagArray lppPropTags;
	int nFieldType;
	if(!GetPropTagArray(szFieldName, lppPropTags, nFieldType, false)) 
		return false;

	HRESULT hr = m_pItem->DeleteProps(lppPropTags, NULL);
	MAPIFreeBuffer(lppPropTags);
	return (hr == S_OK);
}

int MAPIObject::GetAttachmentCount() {
	ULONG ulCount = 0;
	LPSPropValue pProp;
	if(GetProperty(PR_HASATTACH, pProp) == S_OK) {
		if(pProp->Value.b) {
			LPMAPITABLE pAttachTable=NULL;
			if(Message()->GetAttachmentTable(0, &pAttachTable) == S_OK) {
#ifndef _WIN32_WCE
				if(pAttachTable->GetRowCount(0, &ulCount) != S_OK) 
					ulCount = 0;
#else
				enum {PROP_ATTACH_CONTENT_ID, ATTACH_COLS};
				static SizedSPropTagArray(ATTACH_COLS, Columns) = {ATTACH_COLS, PR_ATTACH_CONTENT_ID};
				if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
					ULONG cRows;
					do {
						LPSRowSet pRows = NULL;
						if(pAttachTable->QueryRows(1, 0, &pRows)!=S_OK) {
							MAPIFreeBuffer(pRows);
							break;
						}
						cRows = pRows->cRows;
						ulCount += cRows;
						MAPIEx::FreeProws(pRows);
					} while(cRows);
				}
#endif
				RELEASE(pAttachTable);
			}
		}
		MAPIFreeBuffer(pProp);
	}
	return ulCount;
}

bool MAPIObject::GetAttachment(MAPIAttachment& attachment, int nIndex) {
	return attachment.Open(Message(), nIndex);
}

bool MAPIObject::AddAttachment(MAPIAttachment& attachment) {
	return attachment.Create(Message());
}

// use nIndex of -1 to save all attachments to szFolder
bool MAPIObject::SaveAttachment(LPCTSTR szFolder, int nIndex, LPCTSTR szFileName) {
	LPMAPITABLE pAttachTable = NULL;
	if(Message()->GetAttachmentTable(0, &pAttachTable) != S_OK) 
		return false;

	String strPath;
	bool bResult = false;
	enum {PROP_ATTACH_NUM, PROP_ATTACH_LONG_FILENAME, PROP_ATTACH_FILENAME, ATTACH_COLS};
	static SizedSPropTagArray(ATTACH_COLS, Columns) =
					{ATTACH_COLS, PR_ATTACH_NUM, PR_ATTACH_LONG_FILENAME, PR_ATTACH_FILENAME};
	if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
		LPSRowSet pRows = NULL;
		MAPIAttachment attachment;
		while(true) {
			strPath = "";
			if(nIndex>=0 && pAttachTable->SeekRow(BOOKMARK_BEGINNING, nIndex, NULL) != S_OK) 
				break; 
			if(pAttachTable->QueryRows(1, 0, &pRows) == S_OK) {
				if(!pRows->cRows) 
					break;
				LPATTACH pAttachment;
				if(Message()->OpenAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.bin.cb, 
															NULL, 0, &pAttachment) == S_OK) {
					if (szFileName != NULL)
						strPath += Format("%s\\%s", szFolder, szFileName);
					else {
						if(!IsNull(MAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ATTACH_LONG_FILENAME]))) 
							strPath += Format("%s\\%s", szFolder, pRows->aRow[0].lpProps[PROP_ATTACH_LONG_FILENAME].Value.LPSZ);
						else if(!IsNull(MAPIEx::GetValidString(pRows->aRow[0].lpProps[PROP_ATTACH_FILENAME]))) 
							strPath += Format("%s\\%s", szFolder, pRows->aRow[0].lpProps[PROP_ATTACH_FILENAME].Value.LPSZ);
						else 
							strPath += Format("%s\\Attachment.dat", szFolder);
					}
					strPath = FromSystemCharset(strPath);
					attachment.Attach(pAttachment);
					//attachment.SaveAttachment(strPath);
					bResult = attachment.SaveAttachment(strPath);
				}
			}
			MAPIEx::FreeProws(pRows);
			if(nIndex >= 0) 
				break;
		}
	}
	RELEASE(pAttachTable);
	return bResult;
}

// use nIndex of -1 to delete all attachments
bool MAPIObject::DeleteAttachment(int nIndex)
{
	LPMAPITABLE pAttachTable = NULL;
	if(Message()->GetAttachmentTable(0, &pAttachTable) != S_OK) 
		return false;

	bool bResult=false;
	enum {PROP_ATTACH_NUM, ATTACH_COLS};
	static SizedSPropTagArray(ATTACH_COLS, Columns) = {ATTACH_COLS, PR_ATTACH_NUM };
	if(pAttachTable->SetColumns((LPSPropTagArray)&Columns, 0) == S_OK) {
		int i = 0;
		LPSRowSet pRows = NULL;
		while(true) {
			if(pAttachTable->QueryRows(1, 0, &pRows) == S_OK) {
				if(!pRows->cRows) 
					break;
				else if(i<nIndex) {
					i++;
					continue;
				} else
					bResult = (Message()->DeleteAttach(pRows->aRow[0].lpProps[PROP_ATTACH_NUM].Value.ul, 
																		0, NULL, 0) == S_OK);
				MAPIEx::FreeProws(pRows);
				if(!bResult || nIndex == i) 
					break;
			}
			break;
		}
	}
	RELEASE(pAttachTable);
	return bResult;
}

bool MAPIObject::AddAttachment(const String &szPath, String szName, String szCID) {
	if (!FileExists(szPath))
		return false;

	String szFileName = szName;
	if(IsNull(szFileName)) {
		szFileName = szPath;
		for(int i = szPath.GetCount() - 1; i >= 0; i--) 
			if(szPath[i] == '\\' || szPath[i] == '/') {
				szFileName = szPath.Mid(i+1);
				break;
			}
	}

	MAPIAttachment attachment;
	if(attachment.Create(Message())) {
		if(IsNull(szCID) || szCID != CONTACT_PICTURE) {
			attachment.SetFileName(szFileName);
			attachment.SetLongFileName(szFileName);
			if(!IsNull(szCID)) 
				attachment.SetCID(szCID);
		} else {
			attachment.SetFileName(szCID);
			attachment.SetLongFileName(szCID);
			attachment.SetPropertyValue(PR_ATTACHMENT_UNKNOWN, true);
		}
		if(attachment.LoadAttachment(szPath)) {
			attachment.Save();
			return true;
		}
	}
	return false;
}

// Gets the body of the item, if bAutoDetect is set, uses the MessageEditorFormat to try to determine which property to return 
String MAPIObject::GetBody(bool bAutoDetect) {
	String strBody;
	if(bAutoDetect) {
		int nFormat = GetMessageEditorFormat();
		if(nFormat == EDITOR_FORMAT_DONTKNOW || nFormat == EDITOR_FORMAT_RTF) 
			return GetRTF();
		else if(nFormat == EDITOR_FORMAT_HTML) 
			return GetHTML();
	}
	return GetPropertyString(PR_BODY, true);
}

bool MAPIObject::SetBody(const String& strBody) {
	if(SetPropertyString(PR_BODY, strBody, true)) {
		SetMessageEditorFormat(EDITOR_FORMAT_PLAINTEXT);
		return true;
	}
	return false;
}

String MAPIObject::GetHTML() {
	return GetPropertyString(PR_BODY_HTML, true);
}

bool MAPIObject::SetHTML(const String &strHTML) {
	// does this Message Store support HTML directly?
	ULONG ulSupport = m_pMAPI ? m_pMAPI->GetMessageStoreSupport() : 0;
	if(ulSupport&STORE_HTML_OK) {
		if(SetPropertyString(PR_BODY_HTML, strHTML, true)) {
			SetMessageEditorFormat(EDITOR_FORMAT_HTML);
			return true;
		}
	} else {
		// otherwise lets encode it into RTF 
		const char *szCodePage = "1252"; // default codepage is ANSI - Latin I
		GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, (CHAR*)szCodePage, (int)strlen(szCodePage));

		String strRTF;
		strRTF = Format("{\\rtf1\\ansi\\ansicpg%s\\fromhtml1 {\\*\\htmltag1 ", szCodePage);
		strRTF += strHTML;
		strRTF += " }}";

		return SetRTF(strRTF);
	}
	return false;
}

String MAPIObject::GetRTF() {
	String strRTF;
	IStream* pStream;

	const int BUF_SIZE = 16384;
	char szBuf[BUF_SIZE+1];
	ULONG ulNumChars;

#ifdef _WIN32_WCE
	int nMessageStatus = GetMessageStatus();
	if(nMessageStatus & MSGSTATUS_PARTIAL) 
		return Null;

	if(nMessageStatus & MSGSTATUS_HAS_PR_BODY_HTML) {
		if(Message()->OpenProperty(PR_BODY_HTML_A, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&pStream)!=S_OK) 
			return Null;

		do {
			pStream->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min(BUF_SIZE,ulNumChars)] = 0;
			strRTF += szBuf;
		} while(ulNumChars >= BUF_SIZE);

		RELEASE(pStream);
	} else if(nMessageStatus & MSGSTATUS_HAS_PR_BODY) {
		if(Message()->OpenProperty(PR_BODY_A, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&pStream)!=S_OK) 
			return Null;
		do {
			pStream->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min(BUF_SIZE,ulNumChars)] = 0;
			strRTF += szBuf;
		} while(ulNumChars >= BUF_SIZE);

		RELEASE(pStream);
	}
#else
	if(Message()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream,STGM_READ, 0, (LPUNKNOWN*)&pStream)!=S_OK) 
		return Null;

	IStream *pUncompressed;
	if(WrapCompressedRTFStream(pStream, 0, &pUncompressed) == S_OK) {
		do {
			pUncompressed->Read(szBuf, BUF_SIZE, &ulNumChars);
			szBuf[min<ULONG>(BUF_SIZE,ulNumChars)]=0;
			strRTF += szBuf;
		} while(ulNumChars >= BUF_SIZE);
		RELEASE(pUncompressed);
	}
	RELEASE(pStream);
#endif

	// does this RTF contain encoded HTML? If so decode it
	// code taken from Lucian Wischik's example at http://www.wischik.com/lu/programmer/mapi_utils.html
	if(strRTF.Find("\\fromhtml") != -1) 
		return GetHTML();
	else if(strRTF.Find("\\fromtext") != -1) 
		return GetBody(false);

	return strRTF;
}

bool MAPIObject::SetRTF(const String &strRTF) {
	LPSTREAM pStream = NULL;
	if(Message()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_CREATE | STGM_WRITE, 
									MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream) == S_OK) {
#ifndef _WIN32_WCE
		IStream *pUncompressed;
		if(WrapCompressedRTFStream(pStream,MAPI_MODIFY, &pUncompressed) == S_OK) {
			pUncompressed->Write(strRTF.ToWString(), strRTF.ToWString().GetCount()*sizeof(wchar), NULL);
			if(pUncompressed->Commit(STGC_DEFAULT) == S_OK) 
				pStream->Commit(STGC_DEFAULT);
			RELEASE(pUncompressed);
		}
#endif
		RELEASE(pStream);
		SetMessageEditorFormat(EDITOR_FORMAT_RTF);
		return true;
	}
	return false;
}

Time MAPIObject::GetLastModified(SYSTEMTIME &tmLastModified) {
	LPSPropValue pProp;
	if(GetProperty(PR_LAST_MODIFICATION_TIME, pProp) == S_OK) {
		SYSTEMTIME tm;
		FileTimeToSystemTime(&pProp->Value.ft, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &tmLastModified);

		MAPIFreeBuffer(pProp);
		return MAPIEx::GetSystemTime(tm);
	}
	return Null;
}

bool MAPIObject::SetLastModified(const Time &tm) {
	SPropValue prop;
	prop.ulPropTag = PR_LAST_MODIFICATION_TIME;
	SYSTEMTIME st;
	MAPIEx::SetSystemTime(st, tm);
	SystemTimeToFileTime(&st, &prop.Value.ft);
	return (m_pItem && m_pItem->SetProps(1, &prop, NULL) == S_OK);
}

#endif