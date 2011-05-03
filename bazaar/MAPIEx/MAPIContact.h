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

// Ported to U++ Framework by Koldo. See License.txt file

class MAPIEx;
class MAPIContact;

#ifdef _WIN32_WCE
#include "POOM.h"
#endif 

/////////////////////////////////////////////////////////////
// ContactAddress

class ContactAddress {
public:
	ContactAddress();

	enum AddressType { HOME, BUSINESS, OTHER, MAX_ADDRESS_TYPES };

// Attributes
public:
	AddressType m_nType;
	String m_strStreet;
	String m_strCity;
	String m_strStateOrProvince;
	String m_strPostalCode;
	String m_strCountry;

// Operations
protected:
	bool FillAddress(MAPIContact& contact, AddressType nType);
	bool SaveAddress(MAPIContact& contact, AddressType nType);

	friend class MAPIContact;
};

/////////////////////////////////////////////////////////////
// MAPIContact

#define CONTACT_PICTURE "ContactPicture.jpg"

class MAPIContact : public MAPIObject {
public:
	MAPIContact();
	~MAPIContact();

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
protected:
	bool Open(MAPIEx* pMAPI, CPOOM* pPOOM, IContact* pContact);
	virtual void Close();
	virtual bool Save(bool bClose=true);
	
	virtual String GetPropertyString(ULONG ulProperty, bool bStream = false);
	virtual bool SetPropertyString(ULONG ulProperty, const String &szProperty, bool bStream=false);
	
#endif
public:
	bool Create(MAPIEx &mapi, MAPIFolder &folder);

	String GetName(ULONG ulNameID = PR_DISPLAY_NAME);
	String GetEmail(int nIndex = 1); // 1, 2 or 3 for outlook email addresses
	String GetEmailDisplayAs(int nIndex = 1);
	String GetHomePage(bool bBusiness = true);
	String GetPhoneNumber(ULONG ulPhoneNumberID);
	bool GetAddress(ContactAddress& address, ContactAddress::AddressType nType);
	String GetPostalAddress();
	String GetIMAddress();
	String GetFileAs();
	String GetTitle();
	String GetCompany();
	String GetProfession();
	String GetDisplayNamePrefix();
	String GetGeneration();
	String GetDepartment();
	String GetOffice();
	String GetManagerName();
	String GetAssistantName();
	String GetNickName();
	String GetSpouseName();
	Time GetTime(ULONG property);
	Time GetBirthday()		{return GetTime(PR_BIRTHDAY);}
	Time GetAnniversary()	{return GetTime(PR_WEDDING_ANNIVERSARY);}
	String GetCategories();

	bool SetName(const String &szName, ULONG ulNameID = PR_DISPLAY_NAME);
	bool SetEmail(const String &szEmail, int nIndex = 1);
	bool SetEmailDisplayAs(const String &szDisplayAs, int nIndex = 1);
	bool SetHomePage(const String &szHomePage, bool bBusiness = true);
	bool SetPhoneNumber(const String &szPhoneNumber, ULONG ulPhoneNumberID);
	bool SetAddress(ContactAddress& address, ContactAddress::AddressType nType);
	bool SetPostalAddress(ContactAddress::AddressType nType);
	bool UpdateDisplayAddress(ContactAddress::AddressType nType);
	bool SetNotes(const String &szNotes, bool bRTF=false);
	bool SetIMAddress(const String &szIMAddress);
	bool SetFileAs(const String &szFileAs);
	bool SetTitle(const String &szTitle);
	bool SetCompany(const String &szCompany);
	bool SetProfession(const String &szProfession);
	bool SetDisplayNamePrefix(const String &szPrefix);
	bool SetGeneration(const String &szGeneration);
	bool UpdateDisplayName();
	bool SetDepartment(const String &szDepartment);
	bool SetOffice(const String &szOffice);
	bool SetManagerName(const String &szManagerName);
	bool SetAssistantName(const String &szAssistantName);
	bool SetNickName(const String &szNickName);
	bool SetSpouseName(const String &szSpouseName);
	bool SetBirthday(const Time &tm);
	bool SetAnniversary(const Time &tm);
	bool SetCategories(const String &szCategories);
	bool HasPicture();
	bool SetPicture(const String &szPath);

protected:
	int GetOutlookEmailID(int nIndex);
};

#endif
