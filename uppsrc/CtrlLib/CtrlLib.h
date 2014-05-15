#ifndef CTRLLIB_H
#define CTRLLIB_H

#include <CtrlCore/CtrlCore.h>

#ifdef Status //X11 defines this, likely name clash...
#undef Status 
#endif

NAMESPACE_UPP

#define IMAGECLASS CtrlImg
#define IMAGEFILE <CtrlLib/Ctrl.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS CtrlsImg
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS ClassicCtrlsImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml_header.h>

class Bar;

#include <CtrlLib/LabelBase.h>
#include <CtrlLib/StaticCtrl.h>
#include <CtrlLib/PushCtrl.h>
#include <CtrlLib/MultiButton.h>
#include <CtrlLib/ScrollBar.h>
#include <CtrlLib/HeaderCtrl.h>
#include <CtrlLib/EditCtrl.h>
#include <CtrlLib/AKeys.h>
#include <CtrlLib/Bar.h>
#include <CtrlLib/StatusBar.h>
#include <CtrlLib/TabCtrl.h>
#include <CtrlLib/DlgColor.h>
#include <CtrlLib/ArrayCtrl.h>
#include <CtrlLib/DropChoice.h>
#include <CtrlLib/TreeCtrl.h>
#include <CtrlLib/Splitter.h>
#include <CtrlLib/RichText.h>
#include <CtrlLib/TextEdit.h>
#include <CtrlLib/SliderCtrl.h>
#include <CtrlLib/ColumnList.h>
#include <CtrlLib/DateTimeCtrl.h>
#include <CtrlLib/SuggestCtrl.h>

#define  LAYOUTFILE <CtrlLib/Ctrl.lay>
#include <CtrlCore/lay.h>

#include <CtrlLib/Progress.h>
#include <CtrlLib/FileSel.h>
#include <CtrlLib/CtrlUtil.h>
#include <CtrlLib/Lang.h>

#include <CtrlLib/Ch.h>

END_UPP_NAMESPACE

#endif
