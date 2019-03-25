#include "Draw.h"

namespace Upp {

CH_COLOR(SBlack, AdjustIfDark(Black()));
CH_COLOR(SGray, AdjustIfDark(Gray()));
CH_COLOR(SLtGray, AdjustIfDark(LtGray()));
CH_COLOR(SWhiteGray, AdjustIfDark(WhiteGray()));
CH_COLOR(SWhite, AdjustIfDark(White()));
CH_COLOR(SRed, AdjustIfDark(Red()));
CH_COLOR(SGreen, AdjustIfDark(Green()));
CH_COLOR(SBrown, AdjustIfDark(Brown()));
CH_COLOR(SBlue, AdjustIfDark(Blue()));
CH_COLOR(SMagenta, AdjustIfDark(Magenta()));
CH_COLOR(SCyan, AdjustIfDark(Cyan()));
CH_COLOR(SYellow, AdjustIfDark(Yellow()));
CH_COLOR(SLtRed, AdjustIfDark(LtRed()));
CH_COLOR(SLtGreen, AdjustIfDark(LtGreen()));
CH_COLOR(SLtYellow, AdjustIfDark(LtYellow()));
CH_COLOR(SLtBlue, AdjustIfDark(LtBlue()));
CH_COLOR(SLtMagenta, AdjustIfDark(LtMagenta()));
CH_COLOR(SLtCyan, AdjustIfDark(LtCyan()));

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
CH_COLOR(SColorMark, IsDark(SColorPaper()) ? DarkTheme(LtBlue()) : LtBlue());
CH_COLOR(SColorMenuMark, IsDark(SColorMenu()) ? DarkTheme(LtBlue()) : LtBlue());

CH_COLOR(SColorLtFace, Blend(SColorFace, SColorLight));
CH_COLOR(SColorDkShadow, Blend(SColorShadow, SColorText));
CH_COLOR(SColorLabel, SColorText());

}
