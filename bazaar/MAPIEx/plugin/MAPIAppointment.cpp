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

#include "MAPIExPCH.h"
#include "MAPIEx.h"

/////////////////////////////////////////////////////////////
// CMAPIAppointment

CMAPIAppointment::CMAPIAppointment()
{
#ifdef _WIN32_WCE
	m_pAppointment=NULL;
#endif
}

CMAPIAppointment::~CMAPIAppointment()
{
	Close();
}

#ifdef _WIN32_WCE
BOOL CMAPIAppointment::Open(CMAPIEx* pMAPI, IAppointment* pAppointment)
{
	Close();
	m_pMAPI=pMAPI;
	m_pAppointment=pAppointment;
	return TRUE;
}

void CMAPIAppointment::Close()
{
	RELEASE(m_pAppointment);
	m_pMAPI=NULL;
}

BOOL CMAPIAppointment::GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream)
{
	return m_pMAPI->GetPOOM()->GetProperty(m_pAppointment,ulProperty, strProperty);
}

BOOL CMAPIAppointment::SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream)
{
	return m_pMAPI->GetPOOM()->SetProperty(m_pAppointment,ulProperty, szProperty);
}

#else

BOOL CMAPIAppointment::GetSubject(CString& strSubject)
{
	if(GetPropertyString(PR_SUBJECT, strSubject)) return TRUE;
	return FALSE;
}

BOOL CMAPIAppointment::GetLocation(CString& strLocation)
{
	if(GetOutlookPropertyString(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_LOCATION, strLocation)) return TRUE;
	return FALSE;
}

BOOL CMAPIAppointment::GetStartTime(SYSTEMTIME& tmStart)
{
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, pProp)) 
	{
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &tmStart);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIAppointment::GetStartTime(CString& strStartTime, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetStartTime(tm)) 
	{
		TCHAR szTime[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy hh:mm:ss tt");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szTime, 256);
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szTime, szTime, 256);
		strStartTime=szTime;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIAppointment::GetEndTime(SYSTEMTIME& tmEnd)
{
	LPSPropValue pProp;
	if(GetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_END, pProp)) 
	{
		FILETIME tmLocal;
		FileTimeToLocalFileTime(&pProp->Value.ft, &tmLocal);
		FileTimeToSystemTime(&tmLocal, &tmEnd);
		MAPIFreeBuffer(pProp);
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIAppointment::GetEndTime(CString& strEndTime, LPCTSTR szFormat)
{
	SYSTEMTIME tm;
	if(GetEndTime(tm)) 
	{
		TCHAR szTime[256];
		if(!szFormat) szFormat=_T("MM/dd/yyyy hh:mm:ss tt");
		GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szFormat, szTime, 256);
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &tm, szTime, szTime, 256);
		strEndTime=szTime;
		return TRUE;
	}
	return FALSE;
}

BOOL CMAPIAppointment::SetSubject(LPCTSTR szSubject)
{
	return SetPropertyString(PR_SUBJECT, szSubject);
}

BOOL CMAPIAppointment::SetLocation(LPCTSTR szLocation)
{
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, szLocation);
}

BOOL CMAPIAppointment::SetStartTime(SYSTEMTIME& tmStart)
{
	FILETIME ftStart;
	SystemTimeToFileTime(&tmStart, &ftStart);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, ftStart);
}

BOOL CMAPIAppointment::SetEndTime(SYSTEMTIME& tmEnd)
{
	FILETIME ftEnd;
	SystemTimeToFileTime(&tmEnd, &ftEnd);
	return SetOutlookProperty(OUTLOOK_DATA2, OUTLOOK_APPOINTMENT_START, ftEnd);
}

const GUID PSETID_Meeting = {0x6ED8DA90, 0x450B, 0x101B, {0x98, 0xDA, 0x00, 0xAA, 0x00, 0x3F, 0x13, 0x05}};

#define LID_GLOBAL_OBJID 0x23

BOOL CMAPIAppointment::GetMeetingUID(CString& strUID)
{
	BOOL bResult=FALSE;

	MAPINAMEID NamedID = {0};
	NamedID.lpguid = (LPGUID) &PSETID_Meeting;
	NamedID.ulKind = MNID_ID;
	NamedID.Kind.lID = LID_GLOBAL_OBJID;
	LPMAPINAMEID lpNamedID = &NamedID;

	LPSPropTagArray lpNamedPropTags = NULL;
	if(m_pItem && m_pItem->GetIDsFromNames(1, &lpNamedID, NULL, &lpNamedPropTags)==S_OK)
	{
		// Set our type to binary 
		lpNamedPropTags->aulPropTag[0] = CHANGE_PROP_TYPE(lpNamedPropTags->aulPropTag[0],PT_BINARY); 

		// Get the value of the property. 		
		LPSPropValue pProp;
		ULONG ulVal = 0;
		if(m_pItem->GetProps(lpNamedPropTags, 0, &ulVal, &pProp)==S_OK)
		{
			bResult=GetHexString(strUID, pProp->Value.bin);
			MAPIFreeBuffer(pProp);
		}
		MAPIFreeBuffer(lpNamedPropTags);
	}
	return bResult;
}

#endif
