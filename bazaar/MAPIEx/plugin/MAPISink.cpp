////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPISink.cpp
// Description: MAPI Advise Sink Wrapper
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
#include "MAPISink.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMAPISink

CMAPISink::CMAPISink(LPNOTIFCALLBACK lpfnCallback, LPVOID lpvContext)
{
	m_lpfnCallback=lpfnCallback;
	m_lpvContext=lpvContext;
	m_nRef=0;
}

HRESULT CMAPISink::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	if(riid==IID_IUnknown) 
	{
		*ppvObj=this;
		AddRef();
		return S_OK;
	}  
	return E_NOINTERFACE;
}

ULONG CMAPISink::AddRef()
{
	return InterlockedIncrement(&m_nRef);
}

ULONG CMAPISink::Release()
{
	ULONG ul=InterlockedDecrement(&m_nRef);
	if(!ul) delete this;
	return ul;
}

ULONG CMAPISink::OnNotify(ULONG cNotification, LPNOTIFICATION lpNotifications)
{
	if(m_lpfnCallback) m_lpfnCallback(m_lpvContext, cNotification, lpNotifications);
	return 0;
}

