#ifndef __TCtrlLibHelp__
#define __TCtrlLibHelp__

#include <TCore/TCore.h>
#include <TCtrlLib/TCtrlLib.h>
#include <RichEdit/RichEdit.h>
#include "helputil.h"

NAMESPACE_UPP

//RichObjectType *RichObjectTypeDialogHelp();
RichObjectType *RichObjectTypeDrawing();
String          GetRichObjectTypeDrawingData(const Drawing& drawing);

void            StoreHelpInfo();
void            LoadAppHelpFile();

Drawing         CtrlToDrawing(Ctrl *ctrl, Point arrow = Null, Image arrow_image = Null);
Image           CtrlToStillImage(Ctrl *ctrl, Point arrow = Null, Image arrow_image = Null);
RichObject      CreateDrawingObject(const Drawing& dwg, Size dot_size, Size out_size = Null);
RichObject      CreateImageObject(Image img, Size dot_size, Size out_size = Null);

One<Ctrl>       GetDlgShot(Ctrl *master, const Image& still);
void            RunDlgShot(Ctrl *master, const Image& still);

void            InstallHelpViewerKeyHook();
void            InstallHelpEditorKeyHook();

//void            RunDlgHelpEditor(Ctrl *ctrl = NULL, const Image& still = Null);
//bool            RunDlgHelpLang(VectorMap<int, String>& ext_lang, int& init_lang);
//void            RunDlgHelpEdit();
void            RunDlgShot(Ctrl *ctrl, const Image& still);
void            RunDlgHelpTopic(String topic);
String          RunDlgHelpTopicIndex(String topic = Null, bool index = false);
void            OpenHelpTopicIndex(String topic = Null, bool index = false);

Callback1<Bar&>& HelpTopicBookMenu();
Callback1<RichText&>& HelpTopicOnlineToc();

END_UPP_NAMESPACE

#endif//__TCtrlLibHelp__
