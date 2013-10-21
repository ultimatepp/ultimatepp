#ifdef _WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIAppointment.cpp
// Description: MAPI Appointment class wrapper
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
// MAPIAppointment

MAPIAppointment::MAPIAppointment() {
#ifdef _WIN32_WCE
	m_pAppointment=NULL;
#endif
}

MAPIAppointment::~MAPIAppointment() {
	Close();
}

#ifdef _WIN32_WCE
bool MAPIAppointment::Open(MAPIEx* pMAPI, IAppointment* pAppointment) {
	Close();
	m_pMAPI = pMAPI;
	m_pAppointment=pAppointment;
	return true;
}

void MAPIAppointment::Close() {
	RELEASE(m_pAppointment);
	m_pMAPI = NULL;
}

bool MAPIAppointment::GetPropertyString(ULONG ulProperty, String& strProperty, bool bStream) {
	return m_pMAPI->GetPOOM()->GetProperty(m_pAppointment,ulProperty, strProperty);
}

bool MAPIAppointment::SetPropertyString(ULONG ulProperty, const String &szProperty, bool bStream) {
	return m_pMAPI->GetPOOM()->SetProperty(m_pAppointment,ulProperty, szProperty);
}

#else

String MAPIAppointment::GetSubject() {
	return GetPropertyString(PR_SUBJECT); 
}

String MAPIAppointment::GetLocation() {
	String strLocation;
	if(GetOutlookPropertyString(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_LOCATION, strLocation)) 
		return strLocation;
	return String();
}

Time MAPIAppointment::GetTime(ULONG property) {
	SYSTEMTIME st;
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA2, property, pProp)) {
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &st);
		MAPIFreeBuffer(pProp);
		return MAPIEx::GetSystemTime(st);
	}
	return Null;
}

bool MAPIAppointment::SetSubject(const String &szSubject) {
	return SetPropertyString(PR_SUBJECT, szSubject);
}

bool MAPIAppointment::SetLocation(const String &szLocation) {
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_LOCATION, szLocation);
}

bool MAPIAppointment::SetStartTime(const Time &tm) {
	SYSTEMTIME st;
	MAPIEx::SetSystemTime(st, tm);
	FILETIME ftStart;
	SystemTimeToFileTime(&st, &ftStart);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, ftStart);
}

bool MAPIAppointment::SetEndTime(const Time &tm) {
	SYSTEMTIME st;
	MAPIEx::SetSystemTime(st, tm);
	FILETIME ftEnd;
	SystemTimeToFileTime(&st, &ftEnd);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_END, ftEnd);
}

const GUID PSETID_Meeting = {0x6ED8DA90, 0x450B, 0x101B, {0x98, 0xDA, 0x00, 0xAA, 0x00, 0x3F, 0x13, 0x05}};

#define LID_GLOBAL_OBJID 0x23

String MAPIAppointment::GetMeetingUID() {
	String strUID;

	MAPINAMEID NamedID = {0};
	NamedID.lpguid = (LPGUID) &PSETID_Meeting;
	NamedID.ulKind = MNID_ID;
	NamedID.Kind.lID = LID_GLOBAL_OBJID;
	LPMAPINAMEID lpNamedID = &NamedID;

	LPSPropTagArray lpNamedPropTags = NULL;
	if(m_pItem && m_pItem->GetIDsFromNames(1, &lpNamedID, 0, &lpNamedPropTags) == S_OK) {
		// Set our type to binary 
		lpNamedPropTags->aulPropTag[0] = CHANGE_PROP_TYPE(lpNamedPropTags->aulPropTag[0],PT_BINARY); 

		// Get the value of the property. 		
		LPSPropValue pProp;
		ULONG ulVal = 0;
		if(m_pItem->GetProps(lpNamedPropTags, 0, &ulVal, &pProp) == S_OK) {
			strUID = GetHexString(pProp->Value.bin);
			MAPIFreeBuffer(pProp);
		}
		MAPIFreeBuffer(lpNamedPropTags);
	}
	return strUID;
}

#endif
#endif