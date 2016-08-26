#include "RichText.h"

namespace Upp {

void  RichText::CatPick(RichText&& p)
{
	MergeStyles(p);
	int c = part.GetCount();
	part.AppendPick(pick(p.part));
	for(int i = c; i < part.GetCount(); i++)
		if(IsPara(i))
			part[i].Get<Para>().Invalidate();
	RefreshAll();
}

RichPos::RichPos()
{
	tabtextparti = tabtextpartcount = tabposintabtext = tabtextlen = table = tablen = posintab =
	celllen = posincell = parai = partcount = posinpara = paralen = 0;
	parenttab = Null;

	tabsize = Size(0, 0);
	cell = Point(0, 0);
	chr = '?';
}

RichPos RichText::GetRichPos(int pos, int maxlevel) const
{
	Mutex::Lock __(mutex);
	RichPos rp;
	rp.level = 0;
	RichTxt::GetRichPos(pos, rp, 0, maxlevel, style);
	if(IsNull(rp.parenttab))
		rp.parenttab = 0;
	return rp;
}

int  RichText::AdjustCursor(int anchor, int cursor) const
{
	Mutex::Lock __(mutex);
	int d = anchor;
	const RichTxt& txt = GetConstText(anchor);
	d -= anchor;
	if(!(cursor == txt.GetLength() && !txt.IsTable(txt.GetPartCount() - 1)))
		cursor = minmax(cursor - d, 0, txt.GetLength());
	int c = cursor;
	int pi = txt.FindPart(c);
	if(txt.IsTable(pi)) {
		if(cursor < anchor) {
			while(--pi >= 0)
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi) + txt.GetPartLength(pi);
			while(++pi < txt.GetPartCount())
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi);
			return d + anchor;
		}
		else {
			while(++pi < txt.GetPartCount())
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi);
			while(--pi >= 0)
				if(txt.IsPara(pi))
					return d + txt.GetPartPos(pi) + txt.GetPartLength(pi);
			return d + anchor;
		}
	}
	return d + cursor;
}

void RichText::Remove(int pos, int count)
{
	GetLength();
	int p = pos;
	int l = GetConstText(p).GetLength();
	count = min(l - p, count);
	RichTxt& txt = GetUpdateText(pos);
	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);
	if(pi == pi2) {
		RichPara pa, pa1;
		pa = txt.Get(pi, style);
		pa1 <<= pa;
		pa.Trim(pos);
		pa1.Mid(pos2);
		pa.Append(pa1);
		txt.Put(pi, pa, style);
	}
	else {
		RichPara pa2 = txt.Get(pi2, style);
		RichPara pa = txt.Get(pi, pa2.format.styleid, style);
		txt.part.Remove(pi, pi2 - pi);
		pa.Trim(pos);
		pa2.Mid(pos2);
		pa.Append(pa2);
		pa.format = pa2.format;
		txt.Put(pi, pa, style);
		txt.SetRefreshFrom(pi);
	}
}

void RichText::Insert(int pos, const RichText& p)
{
	MergeStyles(p);
	if(p.GetLength() == 0)
		return;
	ASSERT(pos >= 0 && pos <= GetLength());
	RichTxt& txt = GetUpdateText(pos);
	int pi = txt.FindPart(pos);
	int n = p.part.GetCount() - 1;
	if(n) {
		txt.part.Insert(pi + 1, p.part, 1, n);
		RichPara pa1, pa2;
		pa1 = txt.Get(pi, style);
		pa2 = txt.Get(pi + n, pa1.format.styleid, style);
		pa1.Mid(pos);
		pa2.Append(pa1);
		pa2.format = pa1.format;
		txt.Put(pi + n, pa2, style);
		pa2 = p.RichTxt::Get(0, p.GetStyle(p.GetParaStyle(0)));
		pa1 = txt.Get(pi, pa2.format.styleid, style);
		pa1.Trim(pos);
		pa1.Append(pa2);
		pa1.format = pa2.format;
		txt.Put(pi, pa1, style);
		txt.SetRefreshFrom(pi);
	}
	else {
		RichPara pa0, pa2;
		pa0 = txt.Get(pi, style);
		pa2 <<= pa0;
		pa0.Trim(pos);
		pa2.Mid(pos);
		pa0.Append(p.RichTxt::Get(0, p.GetStyle(pa0.format.styleid)));
		pa0.Append(pa2);
		txt.Put(pi, pa0, style);
	}
}

RichText RichText::Copy(int pos, int count) const
{
	Mutex::Lock __(mutex);

	RichText r;
	r.SetStyles(style);

	const RichTxt& txt = GetConstText(pos);
	bool addp = false;
	if(count > txt.GetLength() - pos) {
		count = txt.GetLength() - pos;
		addp = true;
	}
	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	if(pi == pi2) {
		RichPara pa = txt.Get(pi, style);
		pa.Trim(pos2);
		pa.Mid(pos);
		r.Cat(pa);
	}
	else {
		RichPara pa = txt.Get(pi++, style);
		pa.Mid(pos);
		r.Cat(pa);
		while(pi < pi2) {
			if(txt.IsPara(pi))
				r.Cat(txt.Get(pi, style));
			else {
				RichTable tab(txt.GetTable(pi), 1);
				r.CatPick(pick(tab));
			}
			pi++;
		}
		pa = txt.Get(pi2, style);
		pa.Trim(pos2);
		r.Cat(pa);
	}
	if(addp)
		r.Cat(RichPara());
	return r;
}

RichText::FormatInfo RichText::GetFormatInfo(int pos, int count) const
{
	Mutex::Lock __(mutex);

	const RichTxt& txt = GetConstText(pos);

	count = min(txt.GetLength() - pos, count);

	int pos2 = pos + count;
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	FormatInfo fi;
	RichPara::Format fmt;
	RichPara pa = txt.Get(pi, style);
	fmt = pa.format;
	int i = pa.FindPart(pos);
	if(i < pa.GetCount())
		(RichPara::CharFormat&)fmt = pa[i].format;
	fi.Set(fmt);
	i++;
	if(pi == pi2) {
		int i2 = pa.FindPart(pos2);
		while(i < i2)
			fi.Combine(pa[i++].format);
		if(pos2 && i2 < pa.GetCount())
			fi.Combine(pa[i2].format);
	}
	else {
		while(i < pa.GetCount())
			fi.Combine(pa[i++].format);
		fi.Combine(pa.format);
		pi++;
		bool first = false;
		txt.CombineFormat(fi, pi, pi2, first, style);
		pa = txt.Get(pi2, style);
		int i2 = pa.FindPart(pos2);
		for(i = 0; i < i2; i++)
			fi.Combine(pa[i].format);
		if(pos2 && i2 < pa.GetCount())
			fi.Combine(pa[i2].format);
	}
	return fi;
}

void RichText::ApplyFormatInfo(int pos, const FormatInfo& fi, int count)
{
	RichTxt& txt = GetUpdateText(pos);

	int pos2 = min(txt.GetLength(), pos + count);
	int pi = txt.FindPart(pos);
	int pi2 = txt.FindPart(pos2);

	ASSERT(txt.IsPara(pi) && txt.IsPara(pi2));

	if(pi == pi2) {
		RichPara pa, pa1, pa2;
		pa = txt.Get(pi, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa2 <<= pa;
		pa.Trim(pos);
		pa2.Mid(pos2);
		pa1.Trim(pos2);
		pa1.Mid(pos);
		Apply(fi, pa1, style);
		pa.Append(pa1);
		pa.Append(pa2);
		fi.ApplyTo(pa.format);
		txt.Put(pi, pa, style);
	}
	else {
		RichPara pa, pa1;
//		if(fi.paravalid & RichText::STYLE)
//			pa = txt.Get(pi, fi.styleid, style);
//		else
			pa = txt.Get(pi, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa.Trim(pos);
		pa1.Mid(pos);
		Apply(fi, pa1, style);
		fi.ApplyTo(pa.format);
		pa.Append(pa1);
		txt.Put(pi, pa, style);
		pi++;
		txt.ApplyFormat(fi, pi, pi2, style);
//		if(fi.paravalid & RichText::STYLE)
//			pa = txt.Get(pi2, fi.styleid, style);
//		else
			pa = txt.Get(pi2, style);
		ApplyStyle(fi, pa, style);
		pa1 <<= pa;
		pa.Trim(pos2);
		pa1.Mid(pos2);
		Apply(fi, pa, style);
		pa.Append(pa1);
		txt.Put(pi2, pa, style);
	}
}

void RichText::ReplaceStyle(int pos, const Uuid& id)
{
	RichTxt& txt = GetUpdateText(pos);
	int p = txt.FindPart(pos);
	RichPara pa = txt.Get(p, id, style);
	txt.Set(p, pa, style);
}

void RichText::ReStyle(int pos, const Uuid& id)
{
	RichTxt& txt = GetUpdateText(pos);
	int p = txt.FindPart(pos);
	RichPara pa = txt.Get(p, style);
	pa.format.styleid = id;
	txt.Set(p, pa, style);
}

RichText::Formating RichText::SaveFormat(int pos, int count) const
{
	Mutex::Lock __(mutex);
	const RichTxt& txt = GetConstText(pos);
	count += pos;
	Formating r;
	txt.SaveFormat(r, txt.FindPart(pos), txt.FindPart(count), style);
	return r;
}

void RichText::RestoreFormat(int pos, const RichText::Formating& info)
{
	RichTxt& txt = GetUpdateText(pos);
	int ii = 0;
	txt.RestoreFormat(txt.FindPart(pos), info, ii, style);
}

void RichText::Init()
{
	RichTxt::Init();
	RichStyle& s = style.Add(RichStyle::GetDefaultId());
	s.name = "Default";
	nolinks = false;
}

void RichText::Clear()
{
	RichTxt::Clear();
	style.Clear();
	Init();
}

RichText::RichText(const RichText& x, int)
   : RichTxt(x, 1), style(x.style, 1)
{
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
}

RichText::RichText(RichText&& x)
   : RichTxt(pick(x)), style(pick(x.style))
{
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
}

RichText& RichText::operator=(RichText&& x)
{
	(RichTxt&)(*this) = pick(x);
	style = pick(x.style);
	nolinks = x.nolinks;
	footer_hack = x.footer_hack;
	return *this;
}

RichText::RichText(RichTxt&& x, RichStyles&& st)
   : RichTxt(pick(x)), style(pick(st))
{
	nolinks = false;
}

void RichTextLayoutTracer::Paragraph(const Rect& page, PageY y, const RichPara& para) {}
void RichTextLayoutTracer::EndParagraph(PageY y) {}
void RichTextLayoutTracer::Table(const Rect& page, PageY y, const RichTable& table) {}
void RichTextLayoutTracer::EndTable(PageY y) {}
void RichTextLayoutTracer::TableRow(const Rect& page, PageY y, int i, const RichTable& table) {}
void RichTextLayoutTracer::EndTableRow(PageY y) {}
void RichTextLayoutTracer::TableCell(const Rect& page, PageY y, int i, int j, const RichTable& table, PageY npy) {}
void RichTextLayoutTracer::EndTableCell(PageY y) {}

}
