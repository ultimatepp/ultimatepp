#ifndef _CtrlLib_ChMac_h_
#define _CtrlLib_ChMac_h_

#include "CtrlLib.h"
#include "ChCocoMM.h"

namespace Upp {

Image Coco_ThemeImage(int cx, int cy, int margin, int type,
                      int value = 0, int state = CTRL_NORMAL, bool focus = false);

Color CocoBrush(int k);

};

#endif
