#include "Draw.h"

NAMESPACE_UPP

CH_COLOR(SBlack, Black());
CH_COLOR(SGray, Gray());
CH_COLOR(SLtGray, LtGray());
CH_COLOR(SWhiteGray, WhiteGray());
CH_COLOR(SWhite, White());
CH_COLOR(SRed, Red());
CH_COLOR(SGreen, Green());
CH_COLOR(SBrown, Brown());
CH_COLOR(SBlue, Blue());
CH_COLOR(SMagenta, Magenta());
CH_COLOR(SCyan, Cyan());
CH_COLOR(SYellow, Yellow());
CH_COLOR(SLtRed, LtRed());
CH_COLOR(SLtGreen, LtGreen());
CH_COLOR(SLtYellow, LtYellow());
CH_COLOR(SLtBlue, LtBlue());
CH_COLOR(SLtMagenta, LtMagenta());
CH_COLOR(SLtCyan, LtCyan());

CH_COLOR(SColorPaper, White());
CH_COLOR(SColorFace, LtGray());
CH_COLOR(SColorText, Black());
CH_COLOR(SColorHighlight, Blue());
CH_COLOR(SColorHighlightText, White());
CH_COLOR(SColorMenu, LtGray());
CH_COLOR(SColorMenuText, Black());
CH_COLOR(SColorInfo, LtYellow());
CH_COLOR(SColorInfoText, Black());
CH_COLOR(SColorDisabled, Gray());
CH_COLOR(SColorLight, White());
CH_COLOR(SColorShadow, Gray());
CH_COLOR(SColorMark, IsDark(SColorPaper()) ? LtYellow() : LtBlue());
CH_COLOR(SColorMenuMark, IsDark(SColorMenu()) ? LtYellow() : LtBlue());

CH_COLOR(SColorLtFace, Blend(SColorFace, SColorLight));
CH_COLOR(SColorDkShadow, Blend(SColorShadow, SColorText));
CH_COLOR(SColorLabel, SColorText());

END_UPP_NAMESPACE
