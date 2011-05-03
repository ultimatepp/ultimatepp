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

// Ported to U++ Framework by Koldo. See License.txt file

class MAPIEx;
class MAPIAppointment;

#ifdef _WIN32_WCE
#include "POOM.h"
#endif 

/////////////////////////////////////////////////////////////
// MAPIAppointment

class MAPIAppointment : public MAPIObject {
public:
	MAPIAppointment();
	~MAPIAppointment();

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
	bool Open(MAPIEx* pMAPI, IAppointment* pAppointment);
	virtual void Close();

	virtual bool GetPropertyString(ULONG ulProperty, String& strProperty, bool bStream = false);
	virtual bool SetPropertyString(ULONG ulProperty, const String &szProperty, bool bStream = false);
#else
	String GetSubject();
	String GetLocation();
	Time GetTime(ULONG property);
	Time GetStartTime()		{return GetTime(OUTLOOK_APPOINTMENT_START);}
	Time GetEndTime()		{return GetTime(OUTLOOK_APPOINTMENT_END);}

	bool SetSubject(const String &szSubject);
	bool SetLocation(const String &szLocation);
	bool SetStartTime(const Time &tm);
	bool SetEndTime(const Time &tm);
	
	String GetMeetingUID();
#endif
};

#define PR_APPOINTMENT_START PROP_TAG( PT_SYSTIME, MAPIAppointment::OUTLOOK_APPOINTMENT_START)
#define PR_APPOINTMENT_END PROP_TAG( PT_SYSTIME, MAPIAppointment::OUTLOOK_APPOINTMENT_END)

#endif
