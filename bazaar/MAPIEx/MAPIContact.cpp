#ifdef _WIN32

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

// Ported to U++ Framework by Koldo. See License.txt file

#include "MAPIEx.h"

#include <Functions4U/Functions4U.h>


#define CATEGORIES_PROPERTY "Keywords"

/////////////////////////////////////////////////////////////
// ContactAddress

const ULONG ContactAddressTag[][5]={
	{ PR_HOME_ADDRESS_CITY, PR_HOME_ADDRESS_COUNTRY, PR_HOME_ADDRESS_STATE_OR_PROVINCE,
		PR_HOME_ADDRESS_STREET, PR_HOME_ADDRESS_POSTAL_CODE },
	{ PR_BUSINESS_ADDRESS_CITY, PR_BUSINESS_ADDRESS_COUNTRY, PR_BUSINESS_ADDRESS_STATE_OR_PROVINCE,
		PR_BUSINESS_ADDRESS_STREET, PR_BUSINESS_ADDRESS_POSTAL_CODE },
	{ PR_OTHER_ADDRESS_CITY, PR_OTHER_ADDRESS_COUNTRY, PR_OTHER_ADDRESS_STATE_OR_PROVINCE, 
		PR_OTHER_ADDRESS_STREET, PR_OTHER_ADDRESS_POSTAL_CODE },
};

ContactAddress::ContactAddress() {
}

bool ContactAddress::FillAddress(MAPIContact& contact,AddressType nType) {
	if(nType < HOME || nType > OTHER) 
		return false;
	m_nType = nType;

	m_strCity = contact.GetPropertyString(ContactAddressTag[nType][0]);
	m_strCountry = contact.GetPropertyString(ContactAddressTag[nType][1]);
	m_strStateOrProvince = contact.GetPropertyString(ContactAddressTag[nType][2]);
	m_strStreet = contact.GetPropertyString(ContactAddressTag[nType][3]);
	m_strPostalCode = contact.GetPropertyString(ContactAddressTag[nType][4]);
	return true;
}

bool ContactAddress::SaveAddress(MAPIContact& contact,AddressType nType) {
	if(nType<HOME || nType>OTHER) 
		return false;
	m_nType=nType;

	if(!contact.SetPropertyString(ContactAddressTag[nType][0], m_strCity)) return false;
	if(!contact.SetPropertyString(ContactAddressTag[nType][1], m_strCountry)) return false;
	if(!contact.SetPropertyString(ContactAddressTag[nType][2], m_strStateOrProvince)) return false;
	if(!contact.SetPropertyString(ContactAddressTag[nType][3], m_strStreet)) return false;
	if(!contact.SetPropertyString(ContactAddressTag[nType][4], m_strPostalCode)) return false;
	return true;
}

/////////////////////////////////////////////////////////////
// MAPIContact

const ULONG PhoneNumberIDs[]={
	PR_PRIMARY_TELEPHONE_NUMBER, PR_BUSINESS_TELEPHONE_NUMBER, PR_HOME_TELEPHONE_NUMBER, 
	PR_CALLBACK_TELEPHONE_NUMBER, PR_BUSINESS2_TELEPHONE_NUMBER, PR_MOBILE_TELEPHONE_NUMBER,
	PR_RADIO_TELEPHONE_NUMBER, PR_CAR_TELEPHONE_NUMBER, PR_OTHER_TELEPHONE_NUMBER,
	PR_PAGER_TELEPHONE_NUMBER, PR_PRIMARY_FAX_NUMBER, PR_BUSINESS_FAX_NUMBER,
	PR_HOME_FAX_NUMBER, PR_TELEX_NUMBER, PR_ISDN_NUMBER, PR_ASSISTANT_TELEPHONE_NUMBER,
	PR_HOME2_TELEPHONE_NUMBER, PR_TTYTDD_PHONE_NUMBER, PR_COMPANY_MAIN_PHONE_NUMBER, 0
};

const ULONG NameIDs[]={ PR_DISPLAY_NAME, PR_GIVEN_NAME, PR_MIDDLE_NAME, PR_SURNAME, 0 };

MAPIContact::MAPIContact() {
#ifdef _WIN32_WCE
	m_pPOOM = NULL;
	m_pContact = NULL;
#endif
}

MAPIContact::~MAPIContact() {
	Close();
}

#ifdef _WIN32_WCE
bool MAPIContact::Open(MAPIEx* pMAPI, CPOOM* pPOOM, IContact* pContact) {
	Close();
	m_pMAPI = pMAPI;
	m_pContact = pContact;
	if(!m_pPOOM) 
		m_pPOOM = pPOOM;
	return true;
}

void MAPIContact::Close() {
	RELEASE(m_pContact);
	m_pMAPI = NULL;
}

bool MAPIContact::Save(bool bClose) {
	return (m_pContact->Save() == S_OK);
}

bool MAPIContact::GetPropertyString(ULONG ulProperty, String& strProperty, bool bStream) {
	return m_pPOOM ? m_pPOOM->GetProperty(m_pContact,ulProperty, strProperty) : false;
}

bool MAPIContact::SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, bool bStream) {
	return m_pPOOM ? m_pPOOM->SetProperty(m_pContact,ulProperty, szProperty) : false;
}
#endif

bool MAPIContact::Create(MAPIEx &mapi, MAPIFolder &folder) {
#ifdef _WIN32_WCE
	if(folder.IsOpened()) {
		m_pPOOM = pFolder->GetPOOM();
		return m_pPOOM ? m_pPOOM->Create(pMAPI,*this) : false;
	}
	return false;
#else
	if(!MAPIObject::Create(mapi, folder)) 
		return false;

	SetMessageClass("IPM.Contact");
	SetImportance(IMPORTANCE_NORMAL);
	SetSensitivity(SENSITIVITY_NONE);

	return true;
#endif
}

String MAPIContact::GetName(ULONG ulNameID) {
	ULONG i = 0;
	while(NameIDs[i] != ulNameID && NameIDs[i] > 0) 
		i++;
	if(!NameIDs[i]) 
		return String();
	else 
		return GetPropertyString(ulNameID);
}

int MAPIContact::GetOutlookEmailID(int nIndex) {
	ULONG ulProperty[] = {OUTLOOK_EMAIL1, OUTLOOK_EMAIL2, OUTLOOK_EMAIL3};
	if(nIndex < 1 || nIndex > 3) 
		return 0;
	return ulProperty[nIndex-1];
}

// uses the built in outlook email fields, OUTLOOK_EMAIL1 etc, minus 1 for ADDRt_YPE and +1 for EmailOriginalDisplayName
String MAPIContact::GetEmail(int nIndex) {
	String strEmail;
	ULONG nID = GetOutlookEmailID(nIndex);
	if(!nID) 
		return String();

#ifdef _WIN32_WCE
	return GetPropertyString(nID, strEmail);
#else
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, nID-1, pProp)) {
		String strAddrType = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		if(GetOutlookProperty(OUTLOOK_DATA1, nID, pProp)) {
			strEmail = MAPIEx::GetValidString(*pProp);
			MAPIFreeBuffer(pProp);
			if(strAddrType == "EX") {
				// for EX types we use the original display name (seems to contain the appropriate data)
				if(GetOutlookProperty(OUTLOOK_DATA1, nID+1, pProp)) {
					strEmail = MAPIEx::GetValidString(*pProp);
					MAPIFreeBuffer(pProp);
				}
			}
			return strEmail;
		}
	}
	return String();
#endif
}

String MAPIContact::GetEmailDisplayAs(int nIndex) {
	String strDisplayAs;
#ifdef _WIN32_WCE
	return GetEmail(strDisplayAs, nIndex);
#else
	ULONG nID = GetOutlookEmailID(nIndex);
	if(!nID) 
		return String();

	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, nID-3, pProp)) {
		strDisplayAs = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return strDisplayAs;
	}
	return String();
#endif
}

String MAPIContact::GetHomePage(bool bBusiness) {
	return GetPropertyString(bBusiness ? PR_BUSINESS_HOME_PAGE  : PR_PERSONAL_HOME_PAGE);
}

String MAPIContact::GetPhoneNumber(ULONG ulPhoneNumberID) {
	ULONG i = 0;
	while(PhoneNumberIDs[i] != ulPhoneNumberID && PhoneNumberIDs[i] > 0) 
		i++;
	if(!PhoneNumberIDs[i]) 
		return String();
	else 
		return GetPropertyString(ulPhoneNumberID);
}

bool MAPIContact::GetAddress(ContactAddress& address, ContactAddress::AddressType nType) {
	return address.FillAddress(*this, nType);
}

String MAPIContact::GetPostalAddress() {
#ifdef _WIN32_WCE
	return String();
#else
	return GetPropertyString(PR_POSTAL_ADDRESS);
#endif
}

String MAPIContact::GetIMAddress() {
	String strIMAddress;
#ifndef _WIN32_WCE
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_IM_ADDRESS, pProp)) {
		strIMAddress = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return strIMAddress;
	}
#endif
	return String();
}

String MAPIContact::GetFileAs() {
	String strFileAs;
#ifdef _WIN32_WCE
	return GetPropertyString(PR_DISPLAY_NAME, strFileAs);
#else
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, pProp)) {
		strFileAs = MAPIEx::GetValidString(*pProp);
		MAPIFreeBuffer(pProp);
		return strFileAs;
	}
	return String();
#endif
}

String MAPIContact::GetTitle() {
	return GetPropertyString(PR_TITLE);
}

String MAPIContact::GetCompany() {
	return GetPropertyString(PR_COMPANY_NAME);
}

String MAPIContact::GetProfession() {
	return GetPropertyString(PR_PROFESSION);
}

String MAPIContact::GetDisplayNamePrefix() {
	return GetPropertyString(PR_DISPLAY_NAME_PREFIX);
}

String MAPIContact::GetGeneration() {
	return GetPropertyString(PR_GENERATION);
}

String MAPIContact::GetDepartment() {
	return GetPropertyString(PR_DEPARTMENT_NAME);
}

String MAPIContact::GetOffice() {
	return GetPropertyString(PR_OFFICE_LOCATION);
}

String MAPIContact::GetManagerName() {
	return GetPropertyString(PR_MANAGER_NAME);
}

String MAPIContact::GetAssistantName() {
	return GetPropertyString(PR_ASSISTANT);
}

String MAPIContact::GetNickName() {
	return GetPropertyString(PR_NICKNAME);
}

String MAPIContact::GetSpouseName() {
	return GetPropertyString(PR_SPOUSE_NAME);
}

Time MAPIContact::GetTime(ULONG property) {
	SYSTEMTIME st;
#ifdef _WIN32_WCE
	if (m_pPOOM) {
		if (m_pPOOM->GetDate(m_pContact, property, st))
			return Time(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	return Null;
#else
	LPSPropValue pProp;
	if(GetProperty(property, pProp) == S_OK) {
		SYSTEMTIME tm;
		FileTimeToSystemTime(&pProp->Value.ft, &tm);
		SystemTimeToTzSpecificLocalTime(NULL, &tm, &st);

		MAPIFreeBuffer(pProp);
		return MAPIEx::GetSystemTime(st);
	}
	return Null;
#endif
}

// Categories are stored under the multi-value named property CATEGORIES_PROPERTY
String MAPIContact::GetCategories() {
	String strCategories;
#ifdef _WIN32_WCE
	return GetPropertyString(OUTLOOK_CATEGORIES, strCategories);
#else
	LPSPropValue pProp;
	if(GetNamedProperty(CATEGORIES_PROPERTY, pProp)) {
		strCategories = "";

		ULONG i;
		String strCategory;
		for(i = 0; i < pProp->Value.MVi.cValues; i++) {
			strCategory = MAPIEx::GetValidMVString(*pProp, i);
			if(strCategory.GetLength()) {
				if(strCategories.GetLength()) 
					strCategories += ';';
				strCategories += strCategory;
			}
		}
		MAPIFreeBuffer(pProp);
		return strCategories;
	}
	return String();
#endif
}

bool MAPIContact::SetName(const String &szName, ULONG ulNameID) {
	ULONG i = 0;
	while(NameIDs[i] != ulNameID && NameIDs[i] > 0) 
		i++;
	if(!NameIDs[i]) 
		return false;
	return SetPropertyString(ulNameID, szName);
}

// only supports setting SMTP Email Addresses, sets both display properties, use SetEmailDisplayAs to set the 
// outlook DisplayAs property to some other text.  You should check that the text is a valid SMTP email address
// ie name.name@domain.domain because this function will allow any string
bool MAPIContact::SetEmail(const String &szEmail, int nIndex) {
	ULONG nID = GetOutlookEmailID(nIndex);
	if(!nID) 
		return false;

#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetProperty(m_pContact, nID, szEmail) : false;
#else
	if(SetOutlookProperty(OUTLOOK_DATA1, nID-1, "SMTP")) {
		// we set the email field and both display properties
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID, szEmail)) 
			return false;
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID+1, szEmail)) 
			return false;
		if(!SetOutlookProperty(OUTLOOK_DATA1, nID-3, szEmail)) 
			return false;
		return true;
	}
	return false;
#endif
}

bool MAPIContact::SetEmailDisplayAs(const String &szDisplayAs, int nIndex) {
#ifdef _WIN32_WCE
	return false;
#endif
	ULONG nID = GetOutlookEmailID(nIndex);
	if(!nID) 
		return false;

	return SetOutlookProperty(OUTLOOK_DATA1, nID-3, szDisplayAs);
}

bool MAPIContact::SetHomePage(const String &szHomePage, bool bBuiness) {
	return SetPropertyString(bBuiness ? PR_BUSINESS_HOME_PAGE : PR_PERSONAL_HOME_PAGE, szHomePage);
}

bool MAPIContact::SetPhoneNumber(const String &szPhoneNumber, ULONG ulPhoneNumberID) {
	ULONG i = 0;
	while(PhoneNumberIDs[i] != ulPhoneNumberID && PhoneNumberIDs[i] > 0) 
		i++;
	if(!PhoneNumberIDs[i] || szPhoneNumber.IsEmpty()) 
		return false;

	// lets do a check for North American phone numbers and change the format if so to "(code) xxx-yyyy"
	String strPhoneNumber = szPhoneNumber;
	if(strPhoneNumber.GetLength()==12 && szPhoneNumber[3]==(TCHAR)'-' && szPhoneNumber[7]==(TCHAR)'-') 
		strPhoneNumber = Format(t_("(%s) %s"), strPhoneNumber.Mid(0,3), strPhoneNumber.Right(8));
	return SetPropertyString(ulPhoneNumberID, strPhoneNumber);
}

bool MAPIContact::SetAddress(ContactAddress& address, ContactAddress::AddressType nType) {
	return address.SaveAddress(*this, nType);
}

// Sets the PR_POSTAL_ADDRESS text and checks the appropriate outlook checkbox by setting the index
bool MAPIContact::SetPostalAddress(ContactAddress::AddressType nType) {
#ifdef _WIN32_WCE
	return false;
#else
	ContactAddress address;
	if(!GetAddress(address, nType)) 
		return false;

	String strPostalAddress;
	strPostalAddress = Format(t_("%s\r\n%s  %s  %s\r\n%s"),address.m_strStreet, address.m_strCity,
				address.m_strStateOrProvince,address.m_strPostalCode,address.m_strCountry);
	if(!SetPropertyString(PR_POSTAL_ADDRESS, strPostalAddress)) 
		return false;

	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_POSTAL_ADDRESS, (int)nType+1);
#endif
}

// updates the outlook display address; call this after changing address fields
bool MAPIContact::UpdateDisplayAddress(ContactAddress::AddressType nType) {
#ifdef _WIN32_WCE
	return false;
#else
	ContactAddress address;
	if(!GetAddress(address, nType)) 
		return false;

	String strDisplayAddress;
	strDisplayAddress = Format(t_("%s\r\n%s  %s  %s\r\n%s"), address.m_strStreet,
											address.m_strCity,address.m_strStateOrProvince,
											address.m_strPostalCode,address.m_strCountry);

	ULONG ulProperty = OUTLOOK_DISPLAY_ADDRESS_HOME+(int)nType;
	return SetOutlookProperty(OUTLOOK_DATA1,ulProperty, strDisplayAddress);
#endif
}

bool MAPIContact::SetNotes(const String &szNotes, bool bRTF) {
#ifdef _WIN32_WCE
	return SetPropertyString(PR_BODY, szNotes);
#else
	if(!Contact() || IsNull(szNotes)) 
		return false;
	ULONG nLen = (ULONG)szNotes.GetLength();

	HRESULT hr = E_FAIL;
	LPSTREAM pStream = NULL;
	if(bRTF) {
		if(Contact()->OpenProperty(PR_RTF_COMPRESSED, &IID_IStream, STGM_CREATE | STGM_WRITE, 
								MAPI_MODIFY | MAPI_CREATE, (LPUNKNOWN*)&pStream) == S_OK) {
			IStream *pUncompressed;
			if(WrapCompressedRTFStream(pStream,MAPI_MODIFY, &pUncompressed) == S_OK) {
				hr = pUncompressed->Write(szNotes, nLen*sizeof(TCHAR), NULL);
				if(pUncompressed->Commit(STGC_DEFAULT)==S_OK) 
					pStream->Commit(STGC_DEFAULT);
				RELEASE(pUncompressed);
			}
		}
	} else {
		if(Contact()->OpenProperty(PR_BODY, &IID_IStream, 0, MAPI_MODIFY | MAPI_CREATE, 
																(LPUNKNOWN*)&pStream) == S_OK) 
			hr = pStream->Write(szNotes, (nLen+1)*sizeof(TCHAR), NULL);
	}
	RELEASE(pStream);
	return (hr == S_OK);
#endif
}

bool MAPIContact::SetIMAddress(const String &szIMAddress) {
#ifndef _WIN32_WCE
	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_IM_ADDRESS, szIMAddress);
#else
	return false;
#endif
}

bool MAPIContact::SetFileAs(const String &szFileAs) {
#ifdef _WIN32_WCE
	return SetPropertyString(PR_DISPLAY_NAME, szFileAs);
#else
	return SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, szFileAs);
#endif
}

bool MAPIContact::SetTitle(const String &szTitle) {
	return SetPropertyString(PR_TITLE, szTitle);
}

bool MAPIContact::SetCompany(const String &szCompany) {
	return SetPropertyString(PR_COMPANY_NAME, szCompany);
}

bool MAPIContact::SetProfession(const String &szProfession) {
	return SetPropertyString(PR_PROFESSION, szProfession);
}

bool MAPIContact::SetDisplayNamePrefix(const String &szPrefix) {
	return SetPropertyString(PR_DISPLAY_NAME_PREFIX, szPrefix);
}

bool MAPIContact::SetGeneration(const String &szGeneration) {
	return SetPropertyString(PR_GENERATION, szGeneration);
}

// After changing any name field you should call this to update PR_INITIALS and PR_DISPLAY_NAME
bool MAPIContact::UpdateDisplayName() {
	String strPrefix, strFirst, strMiddle, strLast, strGeneration, strFullName, strFileAs, strInitials;
	strPrefix = GetDisplayNamePrefix();
	if(strPrefix.GetLength()) {
		strFullName = strPrefix;
		strFullName += ' ';
	}
	strFirst = GetName(PR_GIVEN_NAME);
	if(strFirst.GetLength()) {
		strFileAs += strFirst;
		strInitials += strFirst[0];
		strInitials += '.';
	}
	strMiddle = GetName(PR_MIDDLE_NAME);
	if(strMiddle.GetLength()) {
		if(strFileAs.GetLength()) 
			strFileAs += ' ';
		strFileAs += strMiddle;
		strInitials += strMiddle[0];
		strInitials += '.';

		int i = 1, nLen = strMiddle.GetLength();
		while(i < nLen) {
			if(strMiddle[i-1] == ' ') {
				strInitials += strMiddle[i];
				strInitials += '.';
			}
			i++;
		}
	}
	strLast = GetName(PR_SURNAME);
	if(strLast.GetLength())  {
		if(strFileAs.GetLength()) 
			strFileAs += ' ';
		strFileAs += strLast;
		strInitials += strLast[0];
		strInitials += '.';
	}
	if(strFileAs.GetLength()) 
		strFullName += strFileAs;
	strGeneration = GetGeneration();
	if(strGeneration.GetLength()) {
		if(strFullName.GetLength()) 
			strFullName += ' ';
		strFullName += strGeneration;
	}

#ifdef _WIN32_WCE
	return SetName(strFileAs, PR_DISPLAY_NAME);
#else
	if(!SetPropertyString(PR_INITIALS, strInitials)) 
		return false;
	if(!SetName(strFullName, PR_DISPLAY_NAME)) 
		return false;
	SetOutlookProperty(OUTLOOK_DATA1, OUTLOOK_FILE_AS, strFileAs);
	if(!SetPropertyString(PR_SUBJECT, strFileAs)) 
		return false;
	return SetPropertyString(PR_NORMALIZED_SUBJECT, strFileAs);
#endif
}

bool MAPIContact::SetDepartment(const String &szDepartment) {
	return SetPropertyString(PR_DEPARTMENT_NAME, szDepartment);
}

bool MAPIContact::SetOffice(const String &szOffice) {
	return SetPropertyString(PR_OFFICE_LOCATION, szOffice);
}

bool MAPIContact::SetManagerName(const String &szManagerName) {
	return SetPropertyString(PR_MANAGER_NAME, szManagerName);
}

bool MAPIContact::SetAssistantName(const String &szAssistantName) {
	return SetPropertyString(PR_ASSISTANT, szAssistantName);
}

bool MAPIContact::SetNickName(const String &szNickName) {
	return SetPropertyString(PR_NICKNAME, szNickName);
}

bool MAPIContact::SetSpouseName(const String &szSpouseName) {
	return SetPropertyString(PR_SPOUSE_NAME, szSpouseName);
}

bool MAPIContact::SetBirthday(const Time &tm) {
	SYSTEMTIME st;
	MAPIEx::SetSystemTime(st, tm);
	
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetDate(m_pContact, PR_BIRTHDAY, st) : false;
#else
	SPropValue prop;
	prop.ulPropTag = PR_BIRTHDAY;
	SystemTimeToFileTime(&st, &prop.Value.ft);
	return (Contact()->SetProps(1, &prop, NULL) == S_OK);
#endif
}

bool MAPIContact::SetAnniversary(const Time &tm) {
	SYSTEMTIME st;
	MAPIEx::SetSystemTime(st, tm);
	
#ifdef _WIN32_WCE
	return m_pPOOM ? m_pPOOM->SetDate(m_pContact, PR_WEDDING_ANNIVERSARY,tmAnniversary) : false;
#else
	SPropValue prop;
	prop.ulPropTag = PR_WEDDING_ANNIVERSARY;
	SystemTimeToFileTime(&st, &prop.Value.ft);
	return (Contact()->SetProps(1, &prop, NULL) == S_OK);
#endif
}

// Categories are stored under the multi-value named property CATEGORIES_PROPERTY
// Pass in a string of semicolon separated category names
bool MAPIContact::SetCategories(const String &szCategories) {
#ifdef _WIN32_WCE
	return SetPropertyString(OUTLOOK_CATEGORIES, szCategories);
#else
	String strCategories = szCategories;
	int nCount = 0, nIndex = 0;
	if (!strCategories.IsEmpty())
		nCount++;
	String strCategory = Tokenize(strCategories, ";", nIndex);
	while(strCategory.IsEmpty()) {
		nCount++;
		strCategory = Tokenize(strCategories, ";", nIndex);
	}

	HRESULT hr = E_FAIL;
	if(!nCount) {
		if(DeleteNamedProperty(CATEGORIES_PROPERTY)) 
			return true;
	} else {
		LPTSTR* arCategories = new LPTSTR[nCount];

		nCount = 0;
		nIndex = 0;
		int nLen = 0;
		strCategory = Tokenize(strCategories, ";", nIndex);		
		do {
			nLen = strCategory.GetLength();
			if(nLen > 0) {
				arCategories[nCount] = new TCHAR[nLen+1];
				memcpy(arCategories[nCount], (LPCTSTR)strCategory, nLen*sizeof(TCHAR));
				arCategories[nCount++][nLen] = (TCHAR)0;
				strCategory = Tokenize(strCategories, ";", nIndex);		
			}
		} while(nLen);

		LPSPropValue pProp;
		if(SetNamedMVProperty(CATEGORIES_PROPERTY, (LPCTSTR*)arCategories, nCount, pProp)) {
			hr=Contact()->SetProps(1, pProp, NULL);
			MAPIFreeBuffer(pProp);
		}

		for(nIndex=0;nIndex<nCount;nIndex++) 
			delete [] arCategories[nIndex];
		delete [] arCategories;
	}
	return (hr == S_OK);
#endif
}

bool MAPIContact::HasPicture() {
	int bPicture = false;
	GetOutlookPropertyValue(MAPIContact::OUTLOOK_DATA1, MAPIContact::OUTLOOK_PICTURE_FLAG, 
																					bPicture);
	return bPicture;
}

// special case attachment that is used by outlook for contact pictures
// szPath should point to a valid JPG or PNG (others may work but only tried these)
// For optimal results the image should be max 72x96 pixels but anything works AFAIK
bool MAPIContact::SetPicture(const String &szPath) {
#ifdef _WIN32_WCE
	return false;
#else
	if(GetAttachmentCount() > 0 && !DeleteAttachment()) 
		return false;

	bool bPicture = false;
	if(!szPath.IsEmpty()) 
		bPicture = AddAttachment(szPath, CONTACT_PICTURE, CONTACT_PICTURE);
	SetOutlookProperty(MAPIContact::OUTLOOK_DATA1, MAPIContact::OUTLOOK_PICTURE_FLAG, bPicture, 
																					PT_BOOLEAN);
	if(szPath.IsEmpty()) 
		return true;
	return bPicture;
#endif
}

#endif