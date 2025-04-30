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

CH_COLOR(SOrange, AdjustIfDark(Orange()));
CH_COLOR(SPink, AdjustIfDark(Pink()));
CH_COLOR(SDkRed, AdjustIfDark(DkRed()));
CH_COLOR(SDkGreen, AdjustIfDark(DkGreen()));
CH_COLOR(SDkBlue, AdjustIfDark(DkBlue()));
CH_COLOR(SDkYellow, AdjustIfDark(DkYellow()));
CH_COLOR(SDkMagenta, AdjustIfDark(DkMagenta()));
CH_COLOR(SDkCyan, AdjustIfDark(DkCyan()));

bool dark_theme__;
extern bool AColor_dark_mode__;

#define CH_END } // to avoid } highlighting problem

CH_VAR0(ChColor, Color, SColorPaper, White())
	AColor_dark_mode__ = dark_theme__ = IsDark(SColorPaper());
	Iml::SkinAll();
CH_END

CH_COLOR(SColorFace, SLtGray());
CH_COLOR(SColorText, SBlack());
CH_COLOR(SColorHighlight, SBlue());
CH_COLOR(SColorHighlightText, SWhite());
CH_COLOR(SColorMenu, SLtGray());
CH_COLOR(SColorMenuText, SBlack());
CH_COLOR(SColorInfo, SLtYellow());
CH_COLOR(SColorInfoText, SBlack());
CH_COLOR(SColorDisabled, SGray());
CH_COLOR(SColorLight, SWhite());
CH_COLOR(SColorShadow, SGray());
CH_COLOR(SColorMark, IsDark(SColorPaper()) ? DarkTheme(LtBlue()) : LtBlue());
CH_COLOR(SColorMenuMark, IsDark(SColorMenu()) ? DarkTheme(LtBlue()) : LtBlue());

CH_COLOR(SColorLtFace, Blend(SColorFace, SColorLight));
CH_COLOR(SColorDkShadow, Blend(SColorShadow, SBlack));
CH_COLOR(SColorLabel, SColorText());

}
