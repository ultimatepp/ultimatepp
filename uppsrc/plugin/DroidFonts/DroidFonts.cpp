#include <plugin/FT_fontsys/FT_fontsys.h>

#include "DroidFonts.brc"

namespace Upp {

INITIALIZER(DroidFonts) {
	SetMemoryFont(Font::SANSSERIF, Font_DroidSans, Font_DroidSans_length);
	SetMemoryFont(Font::SANSSERIF, Font_DroidSans_Bold, Font_DroidSans_Bold_length, FtBOLD);
	SetMemoryFont(Font::SERIF, Font_DroidSerif, Font_DroidSerif_length);
	SetMemoryFont(Font::SERIF, Font_DroidSerif_Bold, Font_DroidSerif_Bold_length, FtBOLD);
	SetMemoryFont(Font::SERIF, Font_DroidSerif_Italic, Font_DroidSerif_Italic_length, FtITALIC);
	SetMemoryFont(Font::SERIF, Font_DroidSerif_BoldItalic, Font_DroidSerif_BoldItalic_length, FtBOLD|FtITALIC);
	SetMemoryFont(Font::MONOSPACE, Font_DroidSansMono, Font_DroidSansMono_length);
}

}
