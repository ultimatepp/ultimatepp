#ifndef STDAPP_DEF_HPP
#define STDAPP_DEF_HPP

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define         APP_COPYRIGHT                   t_("February 19, 2007");
#define         APP_SUBTITLE                    t_("Based on Werner Wenzel's Standard Application UPT")
#define         APP_TITLE                       t_("My Application")
#define         APP_VERSION                     t_("V1.0.0.0")

#define         SPLASH_FONT_1                   Courier
#define         SPLASH_FONT_2                   Courier
#define         SPLASH_FONT_3                   Courier
#define         SPLASH_FONT_4                   Courier
#define         SPLASH_FONT_SIZE_1              16
#define         SPLASH_FONT_SIZE_2              12
#define         SPLASH_FONT_SIZE_3              12
#define         SPLASH_FONT_SIZE_4              12
#define         SPLASH_INK_COLOR_1              Blue
#define         SPLASH_INK_COLOR_2              Blue
#define         SPLASH_INK_COLOR_3              Blue
#define         SPLASH_INK_COLOR_4              Blue
#define         SPLASH_PAPER_COLOR              Yellow
#define         SPLASH_PERIOD                   5000
#define         SPLASH_RECT_CX                  600
#define         SPLASH_RECT_CY                  70

const int       default_language            =   LNG_ENGLISH;
const int       default_main_window_height  =   600;
const int       default_main_window_left    =   0;
const int       default_main_window_top     =   0;
const int       default_main_window_width   =   800;
const bool      default_maximized           =   false;
const bool      default_minimized           =   false;
const bool      default_show_splash         =   true;
const bool      default_restore_cfg         =   false;
const int       help_window_height          =   400;
const int       help_window_left            =   0;
const int       help_window_top             =   0;
const int       help_window_width           =   600;
const String    IniFile                     =   "stdapp.ini";

#endif
