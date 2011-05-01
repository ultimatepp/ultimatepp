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

#include "MapiUtil.h"
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

bool MAPIAppointment::SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, bool bStream) {
	return m_pMAPI->GetPOOM()->SetProperty(m_pAppointment,ulProperty, szProperty);
}

#else

bool MAPIAppointment::GetSubject(String& strSubject) {
	if(GetPropertyString(PR_SUBJECT, strSubject)) 
		return true;
	return false;
}

bool MAPIAppointment::GetLocation(String& strLocation) {
	if(GetOutlookPropertyString(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_LOCATION, strLocation)) 
		return true;
	return false;
}

Time MAPIAppointment::GetTime(ULONG property) {
	SYSTEMTIME st;
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA2, property, pProp)) {
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &st);
		MAPIFreeBuffer(pProp);
		return Time(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	return Null;
}

bool MAPIAppointment::SetSubject(LPCTSTR szSubject) {
	return SetPropertyString(PR_SUBJECT, szSubject);
}

bool MAPIAppointment::SetLocation(LPCTSTR szLocation) {
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, szLocation);
}

bool MAPIAppointment::SetStartTime(Time tm) {
	SYSTEMTIME st;
	st.wYear   = tm.year;
	st.wMonth  = tm.month;
	st.wDay	   = tm.day;
	st.wHour   = tm.hour;
	st.wMinute = tm.minute;
	st.wSecond = tm.second;
	FILETIME ftStart;
	SystemTimeToFileTime(&st, &ftStart);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, ftStart);
}

bool MAPIAppointment::SetEndTime(Time tm) {
	SYSTEMTIME st;
	st.wYear   = tm.year;
	st.wMonth  = tm.month;
	st.wDay	   = tm.day;
	st.wHour   = tm.hour;
	st.wMinute = tm.minute;
	st.wSecond = tm.second;
	FILETIME ftEnd;
	SystemTimeToFileTime(&st, &ftEnd);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, ftEnd);
}

const GUID PSETID_Meeting = {0x6ED8DA90, 0x450B, 0x101B, {0x98, 0xDA, 0x00, 0xAA, 0x00, 0x3F, 0x13, 0x05}};

#define LID_GLOBAL_OBJID 0x23

bool MAPIAppointment::GetMeetingUID(String& strUID) {
	bool bResult = false;

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
			bResult=GetHexString(strUID, pProp->Value.bin);
			MAPIFreeBuffer(pProp);
		}
		MAPIFreeBuffer(lpNamedPropTags);
	}
	return bResult;
}

#endif
