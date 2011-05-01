#ifndef __MAPIAPPOINTMENT_H__
#define __MAPIAPPOINTMENT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIAppointment.h
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

class CMAPIEx;
class CMAPIAppointment;

#ifdef _WIN32_WCE
#include "POOM.h"
#endif 

/////////////////////////////////////////////////////////////
// CMAPIAppointment

class AFX_EXT_CLASS CMAPIAppointment : public CMAPIObject
{
public:
	CMAPIAppointment();
	~CMAPIAppointment();

	enum { OUTLOOK_DATA2=0x00062002, OUTLOOK_APPOINTMENT_START=0x820D, OUTLOOK_APPOINTMENT_END=0x820E,
			OUTLOOK_APPOINTMENT_LOCATION=0x8208
	};

// Attributes
protected:
#ifdef _WIN32_WCE
	IAppointment* m_pAppointment;
#endif

// Operations
public:
#ifdef _WIN32_WCE
	BOOL Open(CMAPIEx* pMAPI, IAppointment* pAppointment);
	virtual void Close();

	virtual BOOL GetPropertyString(ULONG ulProperty, CString& strProperty, BOOL bStream=FALSE);
	virtual BOOL SetPropertyString(ULONG ulProperty, LPCTSTR szProperty, BOOL bStream=FALSE);
#else
	BOOL GetSubject(CString& strSubject);
	BOOL GetLocation(CString& strLocation);
	BOOL GetStartTime(SYSTEMTIME& tmStart);
	BOOL GetStartTime(CString& strStartTime, LPCTSTR szFormat=NULL); // NULL defaults to "MM/dd/yyyy hh:mm:ss tt"
	BOOL GetEndTime(SYSTEMTIME& tmEnd);
	BOOL GetEndTime(CString& strEndTime, LPCTSTR szFormat=NULL); // NULL defaults to "MM/dd/yyyy hh:mm:ss tt"

	BOOL SetSubject(LPCTSTR szSubject);
	BOOL SetLocation(LPCTSTR szLocation);
	BOOL SetStartTime(SYSTEMTIME& tmStart);
	BOOL SetEndTime(SYSTEMTIME& tmEnd);

	BOOL GetMeetingUID(CString& strUID);
#endif
};

#define PR_APPOINTMENT_START PROP_TAG( PT_SYSTIME, CMAPIAppointment::OUTLOOK_APPOINTMENT_START)
#define PR_APPOINTMENT_END PROP_TAG( PT_SYSTIME, CMAPIAppointment::OUTLOOK_APPOINTMENT_END)

#endif
