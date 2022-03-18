#include "RichText.h"

namespace Upp {

void RichTxt::FormatInfo::Set(const RichPara::Format& fmt)
{
	(RichPara::Format&)*this = fmt;
	charvalid = paravalid = 0xffffffff;
}

void RichTxt::FormatInfo::Set(const RichPara::CharFormat& fmt)
{
	(RichPara::CharFormat&)*this = fmt;
	charvalid = 0xffffffff;
}

void RichTxt::FormatInfo::Combine(const RichPara::CharFormat& fmt)
{
	if(IsBold() != fmt.IsBold()) {
		charvalid &= ~BOLD;
		NoBold();
	}
	if(IsItalic() != fmt.IsItalic()) {
		charvalid &= ~ITALIC;
		NoItalic();
	}
	if(IsUnderline() != fmt.IsUnderline()) {
		charvalid &= ~UNDERLINE;
		NoUnderline();
	}
	if(IsStrikeout() != fmt.IsStrikeout()) {
		charvalid &= ~STRIKEOUT;
		NoStrikeout();
	}
	if(IsNonAntiAliased() != fmt.IsNonAntiAliased()) {
		charvalid &= ~NOAA;
		NoNonAntiAliased();
	}
	if(capitals != fmt.capitals) {
		charvalid &= ~CAPITALS;
		capitals = false;
	}
	if(dashed != fmt.dashed) {
		charvalid &= ~DASHED;
		dashed = false;
	}
	if(sscript != fmt.sscript) {
		charvalid &= ~SSCRIPT;
		sscript = 0;
	}
	if(GetFace() != fmt.GetFace())
		charvalid &= ~FACE;
	if(GetHeight() != fmt.GetHeight())
		charvalid &= ~HEIGHT;
	if(ink != fmt.ink) {
		charvalid &= ~INK;
		ink = Null;
	}
	if(paper != fmt.paper) {
		charvalid &= ~PAPER;
		paper = Null;
	}
	if(language != fmt.language) {
		charvalid &= ~LANG;
		language = 0;
	}
	if(link != fmt.link) {
		charvalid &= ~LINK;
		link = Null;
	}
	if(indexentry != fmt.indexentry) {
		charvalid &= ~INDEXENTRY;
		indexentry = Null;
	}
}

void RichTxt::FormatInfo::Combine(const RichPara::Format& fmt)
{
	if(align != fmt.align)
		paravalid &= ~ALIGN;
	if(before != fmt.before)
		paravalid &= ~BEFORE;
	if(lm != fmt.lm)
		paravalid &= ~LM;
	if(indent != fmt.indent)
		paravalid &= ~INDENT;
	if(rm != fmt.rm)
		paravalid &= ~RM;
	if(after != fmt.after)
		paravalid &= ~AFTER;
	if(tabsize != fmt.tabsize)
		paravalid &= ~TABSIZE;
	if(bullet != fmt.bullet)
		paravalid &= ~BULLET;
	if(newpage != fmt.newpage)
		paravalid &= ~NEWPAGE;
	if(firstonpage != fmt.firstonpage)
		paravalid &= ~FIRSTONPAGE;
	if(keep != fmt.keep)
		paravalid &= ~KEEP;
	if(keepnext != fmt.keepnext)
		paravalid &= ~KEEPNEXT;
	if(orphan != fmt.orphan)
		paravalid &= ~ORPHAN;
	if(label != fmt.label)
		paravalid &= ~LABEL;
	if(NumberingDiffers(*this, fmt))
		paravalid &= ~NUMBERING;
	if(tab.GetCount() != fmt.tab.GetCount()) {
		paravalid &= ~TABS;
		tab.Clear();
	}
	else
		for(int i = 0; i < tab.GetCount(); i++)
			if(tab[i].pos != fmt.tab[i].pos || tab[i].align != fmt.tab[i].align ||
			   tab[i].fillchar != fmt.tab[i].fillchar) {
				paravalid &= ~TABS;
				tab.Clear();
				break;
			}
	if(styleid != fmt.styleid)
		paravalid &= ~STYLE;
	if(linespacing != fmt.linespacing)
		paravalid &= ~SPACING;
	if(ruler != fmt.ruler)
		paravalid &= ~RULER;
	if(rulerink != fmt.rulerink)
		paravalid &= ~RULERINK;
	if(rulerstyle != fmt.rulerstyle)
		paravalid &= ~RULERSTYLE;
	if(newhdrftr != fmt.newhdrftr)
		paravalid &= ~NEWHDRFTR;
}

void RichTxt::FormatInfo::ApplyTo(RichPara::CharFormat& fmt) const
{
	if(charvalid & BOLD)
		fmt.Bold(IsBold());
	if(charvalid & ITALIC)
		fmt.Italic(IsItalic());
	if(charvalid & UNDERLINE)
		fmt.Underline(IsUnderline());
	if(charvalid & STRIKEOUT)
		fmt.Strikeout(IsStrikeout());
	if(charvalid & NOAA)
		fmt.NonAntiAliased(IsNonAntiAliased());
	if(charvalid & CAPITALS)
		fmt.capitals = capitals;
	if(charvalid & DASHED)
		fmt.dashed = dashed;
	if(charvalid & SSCRIPT)
		fmt.sscript = sscript;
	if(charvalid & FACE)
		fmt.Face(GetFace());
	if(charvalid & HEIGHT)
		fmt.Height(GetHeight());
	if(charvalid & LANGUAGE)
		fmt.language = language;
	if(charvalid & INK)
		fmt.ink = ink;
	if(charvalid & PAPER)
		fmt.paper = paper;
	if(charvalid & LANG)
		fmt.language = language;
	if(charvalid & LINK)
		fmt.link = link;
	if(charvalid & INDEXENTRY)
		fmt.indexentry = indexentry;
}

void RichTxt::FormatInfo::ApplyTo(RichPara::Format& fmt) const
{
	ApplyTo((RichPara::CharFormat &)fmt);
	if(paravalid & ALIGN)
		fmt.align = align;
	if(paravalid & RULER)
		fmt.ruler = ruler;
	if(paravalid & BEFORE)
		fmt.before = before;
	if(paravalid & LM)
		fmt.lm = lm;
	if(paravalid & INDENT)
		fmt.indent = indent;
	if(paravalid & RM)
		fmt.rm = rm;
	if(paravalid & AFTER)
		fmt.after = after;
	if(paravalid & TABSIZE)
		fmt.tabsize = tabsize;
	if(paravalid & BULLET)
		fmt.bullet = bullet;
	if(paravalid & NEWPAGE)
		fmt.newpage = newpage;
	if(paravalid & FIRSTONPAGE)
		fmt.firstonpage = firstonpage;
	if(paravalid & KEEP)
		fmt.keep = keep;
	if(paravalid & KEEPNEXT)
		fmt.keepnext = keepnext;
	if(paravalid & ORPHAN)
		fmt.orphan = orphan;
	if(paravalid & LABEL)
		fmt.label = label;
	if(paravalid & NUMBERING) {
		fmt.before_number = before_number;
		fmt.after_number = after_number;
		fmt.reset_number = reset_number;
		memcpy(fmt.number, number, sizeof(number));
	}
	if(paravalid & TABS)
		fmt.tab = tab;
	if(paravalid & STYLE)
		fmt.styleid = styleid;
	if(paravalid & SPACING)
		fmt.linespacing = linespacing;
	if(paravalid & RULERINK)
		fmt.rulerink = rulerink;
	if(paravalid & RULERSTYLE)
		fmt.rulerstyle = rulerstyle;
	if(paravalid & NEWHDRFTR) {
		fmt.newhdrftr = newhdrftr;
		fmt.header_qtf = header_qtf;
		fmt.footer_qtf = footer_qtf;
	}
}

}
