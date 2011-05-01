////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIContact.cpp
// Description: MAPI Contact class wrapper
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

#define CATEGORIES_PROPERTY _T("Keywords")

/////////////////////////////////////////////////////////////
// CContactAddress

const ULONG ContactAddressTag[][5]={
	{ PR_HOME_ADDRESS_CITY, PR_HOME_ADDRESS_COUNTRY, PR_HOME_ADDRESS_STATE_OR_PROVINCE,
		PR_HOME_ADDRESS_STREET, PR_HOME_ADDRESS_POSTAL_CODE },
	{ PR_BUSINESS_ADDRESS_CITY, PR_BUSINESS_ADDRESS_COUNTRY, PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE,
		PR_BUSINESS_ADDRESS_STREET, PR_BUSINESS_ADDRESS_POSTAL_CODE },
	{ PR_OTHER_ADDRESS_CITY, PR_OTHER_ADDRESS_COUNTRY, PR_OTHER_ADDRESS_STATE_OR_PROVINCE, 
		PR_OTHER_ADDRESS_STREET, PR_OTHER_ADDRESS_POSTAL_CODE },
};

CContactAddress::CContactAddress()
{
}

BOOL CContactAddress::FillAddress(CMAPIContact& contact,AddressType nType)
{
	if(nType<HOME || nType>OTHER) return FALSE;
	m_nType=nType;

	if(!contact.GetPropertyString(ContactAddressTag[nType][0], m_strCity)) return FALSE;
	if(!contact.GetPropertyString(ContactAddressTag[nType][1], m_strCountry)) return FALSE;
	if(!contact.GetPropertyString(ContactAddressTag[nType][2], m_strStateOrProvince)) return FALSE;
	if(!contact.GetPropertyString(ContactAddressTag[nType][3], m_strStreet)) return FALSE;
	if(!contact.GetPropertyString(ContactAddressTag[nType][4], m_strPostalCode)) return FALSE;
	return TRUE;
}

BOOL CContactAddress::SaveAddress(CMAPIContact& contact,AddressType nType)
{
	if(nType<HOME || nType>OTHER) return FALSE;
	m_nType=nType;

	if(!contact.SetPropertyString(ContactAddressTag[nType][0], m_strCity)) return FALSE;
	if(!contact.SetPropertyString(ContactAddressTag[nType][1], m_strCountry)) return FALSE;
	if(!contact.SetPropertyString(ContactAddressTag[nType][2], m_strStateOrProvince)) return FALSE;
	if(!contact.SetPropertyString(ContactAddressTag[nType][3], m_strStreet)) return FALSE;
	if(!contact.SetPropertyString(ContactAddressTag[nType][4], m_strPostalCode)) return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////
// CMAPIContact

const ULONG PhoneNumberIDs[]={
	PR_PRIMARY_TELEPHONE_NUMBER, PR_BUSINESS_TELEPHONE_NUMBER, PR_HOME_TELEPHONE_NUMBER, 
	PR_CALLBACK_TELEPHONE_NUMBER, PR_BUSINESS2_TELEPHONE_NUMBER, PR_MOBILE_TELEPHONE_NUMBER,
	PR_RADIO_TELEPHONE_NUMBER, PR_CAR_TELEPHONE_NUMBER, PR_OTHER_TELEPHONE_NUMBER,
	PR_PAGER_TELEPHONE_NUMBER, PR_PRIMARY_FAX_NUMBER, PR_BUSINESS_FAX_NUMBER,
	PR_HOME_FAX_NUMBER, PR_TELEX_NUMBER, PR_ISDN_NUMBER, PR_ASSISTANT_TELEPHONE_NUMBER,
	PR_HOME2_TELEPHONE_NUMBER, PR_TTYTDD_PHONE_NUMBER, PR_COMPANY_MAIN_PHONE_NUMBER, 0
};

const ULONG NameIDs[]={ PR_DISPLAY_NAME, PR_GIVEN_NAME, PR_MIDDLE_NAME, PR_SURNAME, 0 };

CMAPIContact::CMAPIContact()
{
#ifdef _WIN32_WCE
	m_pPOOM=NULL;
	m_pContact=NULL;
#endif
}

CMAPIContact::~CMAPIContact()
{
	Close();
}

#ifdef _WIN32_WCE
BOOL CMAPIContact::Open(CMAPIEx* pMAPI, CPOOM* pPOOM, IContact* pContact)
{
	Close();
	m_pMAPI=pMAPI;
	m_pContact=pContact;
	if(!m_pPOOM) m_pPOOM=pPOOM;
	return TRUE;
}

void CMAPIContact::Close()
{
	RELEASE(m_pContact);
	m_pMAPI=NULL;
}

BOOL CMAPIContact::Save(BOOL bClose)
{
	return (m_pContact->Save()==S_OK);
}

BOOL CMAPIContact::GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream)
{
	return m_pPOOM ? m_pPOOM->GetProperty(m_pContact,ulProperty, strProperty) : FALSE;
}

BOOL CMAPIContact::SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream)
{
	return m_pPOOM ? m_pPOOM->SetProperty(m_pContact,ulProperty, szProperty) : FALSE;
}
#endif

BOOL CMAPIContact::Create(CMAPIEx* pMAPI, CMAPIFolder* pFolder)
{
	if(!pMAPI) return FALSE;
	if(!pFolder) pFolder=pMAPI->GetFolder();
	if(!pFolder) pFolder=pMAPI->OpenContacts();
#ifdef _WIN32_WCE
	if(pFolder) 
	{
		m_pPOOM=pFolder->GetPOOM();
		return m_pPOOM ? m_pPOOM->Create(pMAPI,*this) : FALSE;
	}
	return FALSE;
#else
	if(!CMAPIObject::Create(pMAPI, pFolder)) return FALSE;

	SetMessageClass(_T("IPM.Contact"));
	SetImportance(IMPORTANCE_NORMAL);
	SetSensitivity(SENSITIVITY_NONE);

	return TRUE;
#endif
}

BOOL CMAPIContact::GetName(CString& strName, ULONG ulNameID)
{
	ULONG i=0;
	while(NameIDs[i]!=ulNameID && NameIDs[i]>0) i++;
	if(!NameIDs[i]) 
	{
		strName=_T("");
		return FALSE;
	} 
	else 
	{
		return GetPropertyString(ulNameID, strName);
	}
}

int CMAPIContact::GetOutlookEmailID(int nIndex)
{
	ULONG ulProperty[]={ OUTLOOK_EMAIL1, OUTLOOK_EMAIL2, OUTLOOK_EMAIL3 };
	if(nIndex<1 || nIndex>3) return 0;
	return ulProperty[nIndex-1];
}

// uses the built in outlook email fields, OUTLOOK_EMAIL1 etc, minus 1 for ADDR_TYPE and +1 for EmailOriginalDisplayName
BOOL CMAPIContact::GetEmail(CString& strEmail, int nIndex)
{
	ULONG nID=GetOutlookEmailID(nIndex);
	if(!nID) return FALSE;

#ifdef _WIN32_WCE
	return GetPropertyString(nID, strEmail);
#else
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, nID-1, pProp)) 
	{
		CString strAddrType=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		if(GetOutlookProperty(OUTLOOK_DATA1, nID, pProp)) 
		{
			strEmail=CMAPIEx::GetValidString(*pProp);
			MAPIFreeBuffer(pProp);
			if(strAddrType==_T("EX")) 
			{
				// for EX types we use the original display name (seems to contain the appropriate data)
				if(GetOutlookProperty(OUTLOOK_DATA1, nID+1, pProp)) 
				{
					strEmail=CMAPIEx::GetValidString(*pProp);
					MAPIFreeBuffer(pProp);
				}
			}
			return TRUE;
		}
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::GetEmailDisplayAs(CString& strDisplayAs, int nIndex)
{
#ifdef _WIN32_WCE
	return GetEmail(strDisplayAs, nIndex);
#else
	ULONG nID=GetOutlookEmailID(nIndex);
	if(!nID) return FALSE;

	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, nID-3, pProp)) 
	{
		strDisplayAs=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::GetHomePage(CString& strHomePage, BOOL bBusiness)
{
	return GetPropertyString(bBusiness ? PR_BUSINESS_HOME_PAGE  : PR_PERSONAL_HOME_PAGE, strHomePage);
}

BOOL CMAPIContact::GetPhoneNumber(CString& strPhoneNumber, ULONG ulPhoneNumberID)
{
	ULONG i=0;
	while(PhoneNumberIDs[i]!=ulPhoneNumberID && PhoneNumberIDs[i]>0) i++;
	if(!PhoneNumberIDs[i]) 
	{
		strPhoneNumber=_T("");
		return FALSE;
	} 
	else 
	{
		return GetPropertyString(ulPhoneNumberID, strPhoneNumber);
	}
}

BOOL CMAPIContact::GetAddress(CContactAddress& address, CContactAddress::AddressType nType)
{
	return address.FillAddress(*this, nType);
}

BOOL CMAPIContact::GetPostalAddress(CString& strAddress)
{
#ifdef _WIN32_WCE
	return FALSE;
#else
	return GetPropertyString(PR_POSTAL_ADDRESS, strAddress);
#endif
}

BOOL CMAPIContact::GetIMAddress(CString& strIMAddress)
{
#ifndef _WIN32_WCE
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_IM_ADDRESS, pProp)) 
	{
		strIMAddress=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CMAPIContact::GetFileAs(CString& strFileAs)
{
#ifdef _WIN32_WCE
	return GetPropertyString(PR_DISPLAY_NAME, strFileAs);
#else
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, pProp)) 
	{
		strFileAs=CMAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::GetTitle(CString& strTitle)
{
	return GetPropertyString(PR_TITLE, strTitle);
}

BOOL CMAPIContact::GetCompany(CString& strCompany)
{
	return GetPropertyString(PR_COMPANY_NAME, strCompany);
}

BOOL CMAPIContact::GetProfession(CString& strProfession)
{
	return GetPropertyString(PR_PROFESSION, strProfession);
}

BOOL CMAPIContact::GetDisplayNamePrefix(CString& strPrefix)
{
	return GetPropertyString(PR_DISPLAY_NAME_PREFIX, strPrefix);
}

BOOL CMAPIContact::GetGeneration(CString& strGeneration)
{
	return GetPropertyString(PR_GENERATION, strGeneration);
}

BOOL CMAPIContact::GetDepartment(CString& strDepartment)
{
	return GetPropertyString(PR_DEPARTMENT_NAME, strDepartment);
}

BOOL CMAPIContact::GetOffice(CString& strOffice)
{
	return GetPropertyString(PR_OFFICE_LOCATION, strOffice);
}

BOOL CMAPIContact::GetManagerName(CString& strManagerName)
{
	return GetPropertyString(PR_MANAGER_NAME, strManagerName);
}

BOOL CMAPIContact::GetAssistantName(CString& strAssistantName)
{
	return GetPropertyString(PR_ASSISTANT, strAssistantName);
}

BOOL CMAPIContact::GetNickName(CString& strNickName)
{
	return GetPropertyString(PR_NICKNAME, strNickName);
}

BOOL CMAPIContact::GetSpouseName(CString& strSpouseName)
{
	return GetPropertyString(PR_SPOUSE_NAME, strSpouseName);
}

BOOL CMAPIContact::GetBirthday(SYSTEMTIME& tmBirthday)
{
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->GetDate(m_pContact, PR_BIRTHDAY,tmBirthday) : FALSE;
#else
	LPSPropValue pProp;
	if(GetProperty(PR_BIRTHDAY, pProp)==S_OK) 
	{
		SYSTEMTIME tm;
		FileTimeToSystemTime(&pProp->Value.ft, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &tmBirthday);

		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::GetBirthday(CString& strBirthday, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetBirthday(tm)) 
	{
		TCHAR szDate[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szDate, 256);
		strBirthday=szDate;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIContact::GetAnniversary(SYSTEMTIME& tmAnniversary)
{
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->GetDate(m_pContact, PR_WEDDING_ANNIVERSARY,tmAnniversary) : FALSE;
#else
	LPSPropValue pProp;
	if(GetProperty(PR_WEDDING_ANNIVERSARY, pProp)==S_OK) 
	{
		SYSTEMTIME tm;
		FileTimeToSystemTime(&pProp->Value.ft, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &tmAnniversary);

		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::GetAnniversary(CString& strAnniversary, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetAnniversary(tm)) 
	{
		TCHAR szDate[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szDate,256);
		strAnniversary=szDate;
		return TRUE;
	}
	return FALSE;
}

// Categories are stored under the multi-value named property CATEGORIES_PROPERTY
BOOL CMAPIContact::GetCategories(CString& strCategories)
{
#ifdef _WIN32_WCE
	return GetPropertyString(OUTLOOK_CATEGORIES, strCategories);
#else
	LPSPropValue pProp;
	if(GetNamedProperty(CATEGORIES_PROPERTY, pProp)) 
	{
		strCategories="";

		ULONG i;
		CString strCategory;
		for(i=0;i<pProp->Value.MVi.cValues;i++) 
		{
			strCategory=CMAPIEx::GetValidMVString(*pProp, i);
			if(strCategory.GetLength()) 
			{
				if(strCategories.GetLength()) strCategories+=';';
				strCategories+=strCategory;
			}
		}
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::SetName(LPCTSTR szName, ULONG ulNameID)
{
	ULONG i=0;
	while(NameIDs[i]!=ulNameID && NameIDs[i]>0) i++;
	if(!NameIDs[i]) return FALSE;
	return SetPropertyString(ulNameID, szName);
}

// only supports setting SMTP Email Addresses, sets both display properties, use SetEmailDisplayAs to set the 
// outlook DisplayAs property to some other text.  You should check that the text is a valid SMTP email address
// ie name.name@domain.domain because this function will allow any string
BOOL CMAPIContact::SetEmail(LPCTSTR szEmail, int nIndex)
{
	ULONG nID=GetOutlookEmailID(nIndex);
	if(!nID) return FALSE;

#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetProperty(m_pContact, nID, szEmail) : FALSE;
#else
	if(SetOutlookProperty(OUTLOOK_DATA1, nID-1, _T("SMTP"))) 
	{
		// we set the email field and both display properties
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID, szEmail)) return FALSE;
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID+1, szEmail)) return FALSE;
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID-3, szEmail)) return FALSE;
		return TRUE;
	}
	return FALSE;
#endif
}

BOOL CMAPIContact::SetEmailDisplayAs(LPCTSTR szDisplayAs, int nIndex)
{
#ifdef _WIN32_WCE
	return FALSE;
#endif
	ULONG nID=GetOutlookEmailID(nIndex);
	if(!nID) return FALSE;

	return SetOutlookProperty(OUTLOOK_DATA1, nID-3, szDisplayAs);
}

BOOL CMAPIContact::SetHomePage(LPCTSTR szHomePage, BOOL bBuiness)
{
	return SetPropertyString(bBuiness ? PR_BUSINESS_HOME_PAGE : PR_PERSONAL_HOME_PAGE, szHomePage);
}

BOOL CMAPIContact::SetPhoneNumber(LPCTSTR szPhoneNumber, ULONG ulPhoneNumberID)
{
	ULONG i=0;
	while(PhoneNumberIDs[i]!=ulPhoneNumberID && PhoneNumberIDs[i]>0) i++;
	if(!PhoneNumberIDs[i] || !szPhoneNumber) return FALSE;

	// lets do a check for North American phone numbers and change the format if so to "(code) xxx-yyyy"
	CString strPhoneNumber=szPhoneNumber;
	if(strPhoneNumber.GetLength()==12 && szPhoneNumber[3]==(TCHAR)'-' && szPhoneNumber[7]==(TCHAR)'-') 
	{
		strPhoneNumber.Format(_T("(%s) %s"), strPhoneNumber.Mid(0,3), strPhoneNumber.Right(8));
	}
	return SetPropertyString(ulPhoneNumberID, strPhoneNumber);
}

BOOL CMAPIContact::SetAddress(CContactAddress& address, CContactAddress::AddressType nType)
{
	return address.SaveAddress(*this, nType);
}

// Sets the PR_POSTAL_ADDRESS text and checks the appropriate outlook checkbox by setting the index
BOOL CMAPIContact::SetPostalAddress(CContactAddress::AddressType nType)
{
#ifdef _WIN32_WCE
	return FALSE;
#else
	CContactAddress address;
	if(!GetAddress(address, nType)) return FALSE;

	CString strPostalAddress;
	strPostalAddress.Format(_T("%s\r\n%s  %s  %s\r\n%s"),address.m_strStreet,address.m_strCity,address.m_strStateOrProvince,address.m_strPostalCode,address.m_strCountry);
	if(!SetPropertyString(PR_POSTAL_ADDRESS, strPostalAddress)) return FALSE;

	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_POSTAL_ADDRESS, (int)nType+1);
#endif
}

// updates the outlook display address; call this after changing address fields
BOOL CMAPIContact::UpdateDisplayAddress(CContactAddress::AddressType nType)
{
#ifdef _WIN32_WCE
	return FALSE;
#else
	CContactAddress address;
	if(!GetAddress(address, nType)) return FALSE;

	CString strDisplayAddress;
	strDisplayAddress.Format(_T("%s\r\n%s  %s  %s\r\n%s"),address.m_strStreet,address.m_strCity,address.m_strStateOrProvince,address.m_strPostalCode,address.m_strCountry);

	ULONG ulProperty=OUTLOOK_DISPLAY_ADDRESS_HOME+(int)nType;
	return SetOutlookProperty(OUTLOOK_DATA1,ulProperty, strDisplayAddress);
#endif
}

BOOL CMAPIContact::SetNotes(LPCTSTR szNotes, BOOL bRTF)
{
#ifdef _WIN32_WCE
	return SetPropertyString(PR_BODY, szNotes);
#else
	if(!Contact() || !szNotes) return FALSE;
	ULONG nLen=(ULONG)_tcslen(szNotes);

	HRESULT hr=E_FAIL;
	LPSTREAM pStream=NULL;
	if(bRTF) 
	{
		if(Contact()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_CREATE | STGM_WRITE, MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream)==S_OK) 
		{
			IStream *pUncompressed;
			if(WrapCompressedRTFStream(pStream,MAPI_MODIFY, &pUncompressed)==S_OK) 
			{
				hr=pUncompressed->Write(szNotes, nLen*sizeof(TCHAR), NULL);
				if(pUncompressed->Commit(STGC_DEFAULT)==S_OK) pStream->Commit(STGC_DEFAULT);
				RELEASE(pUncompressed);
			}
		}
	} 
	else 
	{
		if(Contact()->OpenProperty(PR_BODY, &IID_IStream, 0, MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream)==S_OK) 
		{
			hr=pStream->Write(szNotes, (nLen+1)*sizeof(TCHAR), NULL);
		}
	}
	RELEASE(pStream);
	return (hr==S_OK);
#endif
}

BOOL CMAPIContact::SetIMAddress(LPCTSTR szIMAddress)
{
#ifndef _WIN32_WCE
	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_IM_ADDRESS, szIMAddress);
#else
	return FALSE;
#endif
}

BOOL CMAPIContact::SetFileAs(LPCTSTR szFileAs)
{
#ifdef _WIN32_WCE
	return SetPropertyString(PR_DISPLAY_NAME, szFileAs);
#else
	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, szFileAs);
#endif
}

BOOL CMAPIContact::SetTitle(LPCTSTR szTitle)
{
	return SetPropertyString(PR_TITLE, szTitle);
}

BOOL CMAPIContact::SetCompany(LPCTSTR szCompany)
{
	return SetPropertyString(PR_COMPANY_NAME, szCompany);
}

BOOL CMAPIContact::SetProfession(LPCTSTR szProfession)
{
	return SetPropertyString(PR_PROFESSION, szProfession);
}

BOOL CMAPIContact::SetDisplayNamePrefix(LPCTSTR szPrefix)
{
	return SetPropertyString(PR_DISPLAY_NAME_PREFIX, szPrefix);
}

BOOL CMAPIContact::SetGeneration(LPCTSTR szGeneration)
{
	return SetPropertyString(PR_GENERATION, szGeneration);
}

// After changing any name field you should call this to update PR_INITIALS and PR_DISPLAY_NAME
BOOL CMAPIContact::UpdateDisplayName()
{
	CString strPrefix, strFirst, strMiddle, strLast, strGeneration, strFullName, strFileAs, strInitials;
	if(GetDisplayNamePrefix(strPrefix) && strPrefix.GetLength()) 
	{
		strFullName=strPrefix;
		strFullName+=(TCHAR)' ';
	}
	if(GetName(strFirst, PR_GIVEN_NAME) && strFirst.GetLength()) 
	{
		strFileAs+=strFirst;
		strInitials+=strFirst.GetAt(0);
		strInitials+=(TCHAR)'.';
	}
	if(GetName(strMiddle, PR_MIDDLE_NAME) && strMiddle.GetLength()) 
	{
		if(strFileAs.GetLength()) strFileAs+=(TCHAR)' ';
		strFileAs+=strMiddle;
		strInitials+=(TCHAR)strMiddle.GetAt(0);
		strInitials+=(TCHAR)'.';

		int i=1, nLen=strMiddle.GetLength();
		while(i<nLen) 
		{
			if(strMiddle.GetAt(i-1)==(TCHAR)' ') 
			{
				strInitials+=(TCHAR)strMiddle.GetAt(i);
				strInitials+=(TCHAR)'.';
			}
			i++;
		}
	}
	if(GetName(strLast, PR_SURNAME) && strLast.GetLength()) 
	{
		if(strFileAs.GetLength()) strFileAs+=(TCHAR)' ';
		strFileAs+=strLast;
		strInitials+=(TCHAR)strLast.GetAt(0);
		strInitials+=(TCHAR)'.';
	}
	if(strFileAs.GetLength()) strFullName+=strFileAs;
	if(GetGeneration(strGeneration) && strGeneration.GetLength()) 
	{
		if(strFullName.GetLength()) strFullName+=(TCHAR)' ';
		strFullName+=strGeneration;
	}

#ifdef _WIN32_WCE
	return SetName(strFileAs, PR_DISPLAY_NAME);
#else
	if(!SetPropertyString(PR_INITIALS, strInitials)) return FALSE;
	if(!SetName(strFullName, PR_DISPLAY_NAME)) return FALSE;
	SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, strFileAs);
	if(!SetPropertyString(PR_SUBJECT, strFileAs)) return FALSE;
	return SetPropertyString(PR_NORMALIZED_SUBJECT, strFileAs);
#endif
}

BOOL CMAPIContact::SetDepartment(LPCTSTR szDepartment)
{
	return SetPropertyString(PR_DEPARTMENT_NAME, szDepartment);
}

BOOL CMAPIContact::SetOffice(LPCTSTR szOffice)
{
	return SetPropertyString(PR_OFFICE_LOCATION, szOffice);
}

BOOL CMAPIContact::SetManagerName(LPCTSTR szManagerName)
{
	return SetPropertyString(PR_MANAGER_NAME, szManagerName);
}

BOOL CMAPIContact::SetAssistantName(LPCTSTR szAssistantName)
{
	return SetPropertyString(PR_ASSISTANT, szAssistantName);
}

BOOL CMAPIContact::SetNickName(LPCTSTR szNickName)
{
	return SetPropertyString(PR_NICKNAME, szNickName);
}

BOOL CMAPIContact::SetSpouseName(LPCTSTR szSpouseName)
{
	return SetPropertyString(PR_SPOUSE_NAME, szSpouseName);
}

BOOL CMAPIContact::SetBirthday(SYSTEMTIME& tmBirthday)
{
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetDate(m_pContact, PR_BIRTHDAY,tmBirthday) : FALSE;
#else
	SPropValue prop;
	prop.ulPropTag=PR_BIRTHDAY;
	SystemTimeToFileTime(&tmBirthday, &prop.Value.ft);
	return (Contact()->SetProps(1, &prop, NULL)==S_OK);
#endif
}

BOOL CMAPIContact::SetAnniversary(SYSTEMTIME& tmAnniversary)
{
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetDate(m_pContact, PR_WEDDING_ANNIVERSARY,tmAnniversary) : FALSE;
#else
	SPropValue prop;
	prop.ulPropTag=PR_WEDDING_ANNIVERSARY;
	SystemTimeToFileTime(&tmAnniversary, &prop.Value.ft);
	return (Contact()->SetProps(1, &prop, NULL)==S_OK);
#endif
}

// Categories are stored under the multi-value named property CATEGORIES_PROPERTY
// Pass in a string of semicolon separated category names
BOOL CMAPIContact::SetCategories(LPCTSTR szCategories)
{
#ifdef _WIN32_WCE
	return SetPropertyString(OUTLOOK_CATEGORIES, szCategories);
#else
	CString strCategories=szCategories;
	int nCount=0, nIndex=0;
	CString strCategory=strCategories.Tokenize(_T(";"), nIndex);
	while(strCategory.GetLength()) 
	{
		nCount++;
		strCategory=strCategories.Tokenize(_T(";"), nIndex);
	}

	HRESULT hr=E_FAIL;
	if(!nCount) 
	{
		if(DeleteNamedProperty(CATEGORIES_PROPERTY)) return TRUE;
	} 
	else 
	{
		LPTSTR* arCategories=new LPTSTR[nCount];

		nCount=0;
		nIndex=0;
		int nLen=0;
		strCategory=strCategories.Tokenize(_T(";"), nIndex);
		do {
			nLen=strCategory.GetLength();
			if(nLen>0) 
			{
				arCategories[nCount]=new TCHAR[nLen+1];
				memcpy(arCategories[nCount], (LPCTSTR)strCategory, nLen*sizeof(TCHAR));
				arCategories[nCount++][nLen]=(TCHAR)0;
				strCategory=strCategories.Tokenize(_T(";"), nIndex);
			}
		} while(nLen);

		LPSPropValue pProp;
		if(SetNamedMVProperty(CATEGORIES_PROPERTY, (LPCTSTR*)arCategories, nCount, pProp)) 
		{
			hr=Contact()->SetProps(1, pProp, NULL);
			MAPIFreeBuffer(pProp);
		}

		for(nIndex=0;nIndex<nCount;nIndex++) delete [] arCategories[nIndex];
		delete [] arCategories;
	}
	return (hr==S_OK);
#endif
}

BOOL CMAPIContact::HasPicture()
{
	BOOL bPicture=FALSE;
	GetOutlookPropertyValue(CMAPIContact::OUTLOOK_DATA1, CMAPIContact::OUTLOOK_PICTURE_FLAG, bPicture);
	return bPicture;
}

// special case attachment that is used by outlook for contact pictures
// szPath should point to a valid JPG or PNG (others may work but only tried these)
// For optimal results the image should be max 72x96 pixels but anything works AFAIK
BOOL CMAPIContact::SetPicture(LPCTSTR szPath)
{
#ifdef _WIN32_WCE
	return FALSE;
#else
	if(GetAttachmentCount()>0 && !DeleteAttachment()) return FALSE;

	BOOL bPicture=FALSE;
	if(szPath!=NULL) bPicture=AddAttachment(szPath, CONTACT_PICTURE, CONTACT_PICTURE);
	SetOutlookProperty(CMAPIContact::OUTLOOK_DATA1, CMAPIContact::OUTLOOK_PICTURE_FLAG, bPicture, PT_BOOLEAN);
	if(szPath==NULL) return TRUE;
	return bPicture;
#endif
}
