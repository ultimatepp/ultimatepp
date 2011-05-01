#ifndef __MAPICONTACT_H__
#define __MAPICONTACT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIContact.h
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

class CMAPIEx;
class CMAPIContact;

#ifdef _WIN32_WCE
#include "POOM.h"
#endif 

/////////////////////////////////////////////////////////////
// CContactAddress

class AFX_EXT_CLASS CContactAddress : public CObject
{
public:
	CContactAddress();

	enum AddressType { HOME, BUSINESS, OTHER, MAX_ADDRESS_TYPES };

// Attributes
public:
	AddressType m_nType;
	CString m_strStreet;
	CString m_strCity;
	CString m_strStateOrProvince;
	CString m_strPostalCode;
	CString m_strCountry;

// Operations
protected:
	BOOL FillAddress(CMAPIContact& contact,AddressType nType);
	BOOL SaveAddress(CMAPIContact& contact,AddressType nType);

	friend CMAPIContact;
};

/////////////////////////////////////////////////////////////
// CMAPIContact

#define CONTACT_PICTURE _T("ContactPicture.jpg")

class AFX_EXT_CLASS CMAPIContact : public CMAPIObject
{
public:
	CMAPIContact();
	~CMAPIContact();

	enum { OUTLOOK_DATA1=0x00062004, OUTLOOK_EMAIL1=0x8083, OUTLOOK_EMAIL2=0x8093, OUTLOOK_EMAIL3=0x80A3,
		OUTLOOK_IM_ADDRESS=0x8062, OUTLOOK_FILE_AS=0x8005, OUTLOOK_POSTAL_ADDRESS=0x8022, OUTLOOK_DISPLAY_ADDRESS_HOME=0x801A,
		OUTLOOK_PICTURE_FLAG=0x8015, OUTLOOK_CATEGORIES=0xF101E
	};

// Attributes
protected:
#ifdef _WIN32_WCE
	CPOOM* m_pPOOM;
	IContact* m_pContact;
#endif

// Operations
public:
	inline LPMAILUSER Contact() { return (LPMAILUSER)m_pItem; }

#ifdef _WIN32_WCE
	BOOL Open(CMAPIEx* pMAPI, CPOOM* pPOOM, IContact* pContact);
	virtual void Close();
	virtual BOOL Save(BOOL bClose=TRUE);

	virtual BOOL GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream=FALSE);
	virtual BOOL SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream=FALSE);
#endif

	BOOL Create(CMAPIEx* pMAPI, CMAPIFolder* pFolder=NULL);

	BOOL GetName(CString& strName, ULONG ulNameID=PR_DISPLAY_NAME);
	BOOL GetEmail(CString& strEmail, int nIndex=1); // 1, 2 or 3 for outlook email addresses
	BOOL GetEmailDisplayAs(CString& strDisplayAs, int nIndex=1);
	BOOL GetHomePage(CString& strHomePage, BOOL bBuiness=TRUE);
	BOOL GetPhoneNumber(CString& strPhoneNumber, ULONG ulPhoneNumberID);
	BOOL GetAddress(CContactAddress& address, CContactAddress::AddressType nType);
	BOOL GetPostalAddress(CString& strAddress);
	BOOL GetIMAddress(CString& strIMAddress);
	BOOL GetFileAs(CString& strFileAs);
	BOOL GetTitle(CString& strTitle);
	BOOL GetCompany(CString& strCompany);
	BOOL GetProfession(CString& strProfession);
	BOOL GetDisplayNamePrefix(CString& strPrefix);
	BOOL GetGeneration(CString& strGeneration);
	BOOL GetDepartment(CString& strDepartment);
	BOOL GetOffice(CString& strOffice);
	BOOL GetManagerName(CString& strManagerName);
	BOOL GetAssistantName(CString& strAssistantName);
	BOOL GetNickName(CString& strNickName);
	BOOL GetSpouseName(CString& strSpouseName);
	BOOL GetBirthday(SYSTEMTIME& tmBirthday);
	BOOL GetBirthday(CString& strBirthday, LPCTSTR szFormat=NULL); // NULL defaults to "MM/dd/yyyy"
	BOOL GetAnniversary(SYSTEMTIME& tmAnniversary);
	BOOL GetAnniversary(CString& strAnniversary, LPCTSTR szFormat=NULL);
	BOOL GetCategories(CString& strCategories);

	BOOL SetName(LPCTSTR szName, ULONG ulNameID=PR_DISPLAY_NAME);
	BOOL SetEmail(LPCTSTR szEmail, int nIndex=1);
	BOOL SetEmailDisplayAs(LPCTSTR szDisplayAs, int nIndex=1);
	BOOL SetHomePage(LPCTSTR szHomePage, BOOL bBuiness=TRUE);
	BOOL SetPhoneNumber(LPCTSTR szPhoneNumber, ULONG ulPhoneNumberID);
	BOOL SetAddress(CContactAddress& address, CContactAddress::AddressType nType);
	BOOL SetPostalAddress(CContactAddress::AddressType nType);
	BOOL UpdateDisplayAddress(CContactAddress::AddressType nType);
	BOOL SetNotes(LPCTSTR szNotes, BOOL bRTF=FALSE);
	BOOL SetIMAddress(LPCTSTR szIMAddress);
	BOOL SetFileAs(LPCTSTR szFileAs);
	BOOL SetTitle(LPCTSTR szTitle);
	BOOL SetCompany(LPCTSTR szCompany);
	BOOL SetProfession(LPCTSTR szProfession);
	BOOL SetDisplayNamePrefix(LPCTSTR szPrefix);
	BOOL SetGeneration(LPCTSTR szGeneration);
	BOOL UpdateDisplayName();
	BOOL SetDepartment(LPCTSTR szDepartment);
	BOOL SetOffice(LPCTSTR szOffice);
	BOOL SetManagerName(LPCTSTR szManagerName);
	BOOL SetAssistantName(LPCTSTR szAssistantName);
	BOOL SetNickName(LPCTSTR szNickName);
	BOOL SetSpouseName(LPCTSTR szSpouseName);
	BOOL SetBirthday(SYSTEMTIME& tmBirthday);
	BOOL SetAnniversary(SYSTEMTIME& tmAnniversary);
	BOOL SetCategories(LPCTSTR szCategories);
	BOOL HasPicture();
	BOOL SetPicture(LPCTSTR szPath);

protected:
	int GetOutlookEmailID(int nIndex);
};

#endif
