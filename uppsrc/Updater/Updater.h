#if !defined(AFX_UPDATER_H__DA682885_AD16_11D1_AE92_444553540000__INCLUDED_)
#define AFX_UPDATER_H__DA682885_AD16_11D1_AE92_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CUpdaterApp : public CWinApp
{
public:
	CUpdaterApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UPDATER_H__DA682885_AD16_11D1_AE92_444553540000__INCLUDED_)
