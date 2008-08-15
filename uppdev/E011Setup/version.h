#ifndef _version_h_
#define _version_h_

#define APP_MAJORVER    			1
#define APP_MINORVER    			5
#define APP_MAJORPATCHLEVEL  	2
#define APP_MINORPATCHLEVEL  	0

#define EPD_MAJORVER    			1
#define EPD_MINORVER    			0
#define EPD_MAJORPATCHLEVEL  	0
#define EPD_MINORPATCHLEVEL  	0

#define APP_VER_STRING  "1.5.2.0"
#define EPD_VER_STRING  "1.0.0.0"

#ifndef EPD_COMPANY_NAME
#  define COMMENTS 					 ""
#  define EPD_COMPANY_NAME   "EPD"
#  define FILE_DESCRIPTION   "E011 Setup for Win32"
#  define INTERNAL_NAME      "Setup"
#  define LEGAL_COPYRIGHT    "Copyright 2005-2007 EPD"
#  define TRADEMARKS		     ""
#  define ORIGINAL_FILE_NAME "Setup.exe"
#  define PRODUCT_NAME       "E011 Setup program"
#endif

/* these are obsolete but remain for backward compatibility: */
#if (defined(OS2) || defined(__OS2__))
#  define D2_MAJORVER    APP_MAJORVER
#  define D2_MINORVER    APP_MINORVER
#  define D2_PATCHLEVEL  APP_PATCHLEVEL
#endif

#define DW_MAJORVER    APP_MAJORVER
#define DW_MINORVER    APP_MINORVER
#define DW_PATCHLEVEL  APP_PATCHLEVEL

#define WIN_VERSION_DATE  APP_VERSION_DATE

#define APP_DLL_VERSION   APP_VER_STRING

#endif

