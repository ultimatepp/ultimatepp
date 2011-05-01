#ifndef __MAPIEXPCH_H__
#define __MAPIEXPCH_H__

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components

#ifdef MAPIEX_EXPORTS
#undef AFX_EXT_CLASS
#define AFX_EXT_CLASS __declspec(dllexport)
#else
#undef AFX_EXT_CLASS
#define AFX_EXT_CLASS
#endif

#include <MapiUtil.h>

#endif
