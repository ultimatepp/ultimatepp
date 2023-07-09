#include "RichText.h"

namespace Upp {

void RichTxt::GetAllLanguages(Index<int>& all) const
{
	for(int i = 0; i < part.GetCount(); i++) {
		if(IsTable(i)) {
			const RichTable& tab = part[i].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j))
						tab[i][j].text.GetAllLanguages(all);
		}
		else {
			RichPara p = Get(i, RichStyle::GetDefault());
			all.FindAdd(p.format.language);
			for(int i = 0; i < p.GetCount(); i++)
				all.FindAdd(p[i].format.language);
		}
	}
}

Vector<int> RichTxt::GetAllLanguages() const
{
	Index<int> all;
	GetAllLanguages(all);
	return all.PickKeys();
}

bool RichTxt::Update(ParaOp& op)
{
	bool val = false;
	for(int i = 0; i < part.GetCount(); i++)
		if(IsTable(i)) {
			RichTable& tab = part[i].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j))
						if(tab[i][j].text.Update(op)) {
							tab.InvalidateRefresh(i, j);
							val = true;
						}
		}
		else
		if(op(part[i].Get<Para>()))
			val = true;
	RefreshAll();
	return val;
}

RichTxt& RichTxt::GetText0(int& pos, bool update)
{
	if(update)
		Invalidate();
	int p = pos;
	int pi = FindPart(p);
	if(IsTable(pi)) {
		RichTable& tab = part[pi].Get<RichTable>();
		Point cl = tab.FindCell(p);
		if(update) {
			tab.InvalidateRefresh(cl);
			SetRefresh(pi);
		}
		pos = p;
		return tab[cl].text.GetText0(pos, update);
	}
	return *this;
}

RichTxt& RichTxt::GetUpdateText(int& pos)
{
	return GetText0(pos, true);
}

const RichTxt& RichTxt::GetConstText(int& pos) const
{
	return const_cast<RichTxt *>(this)->GetText0(pos, false);
}

RichTable& RichTxt::GetTable0(int table, bool update)
{
	if(update)
		Invalidate();
	for(int i = 0;; i++)
		if(IsTable(i)) {
			table--;
			RichTable& tab = part[i].Get<RichTable>();
			if(table <= tab.GetTableCount()) {
				if(update)
					SetRefresh(i);
				if(table == 0)
					return tab;
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.GetColumns(); j++)
						if(tab(i, j)) {
							RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								if(update)
									tab.InvalidateRefresh(i, j);
								return txt.GetTable0(table, update);
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
}

RichTable& RichTxt::GetUpdateTable(int table)
{
	return GetTable0(table, true);
}

const RichTable& RichTxt::GetConstTable(int table) const {
	return const_cast<RichTxt *>(this)->GetTable0(table, false);
}

void RichTxt::CombineFormat(FormatInfo& fi, int pi, int pi2, bool& first, const RichStyles& style) const
{
	while(pi < pi2) {
		if(IsTable(pi)) {
			const RichTable& tab = part[pi].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j)) {
						const RichTxt& txt = tab[i][j].text;
						txt.CombineFormat(fi, 0, txt.GetPartCount(), first, style);
					}
		}
		else {
			RichPara pa = Get(pi, style);
			if(first) {
				fi.Set(pa.format);
				if(pa.GetCount())
					fi.Set(pa[0].format);
				first = false;
			}
			else
				fi.Combine(pa.format);
			for(int i = first; i < pa.GetCount(); i++)
				fi.Combine(pa[i].format);
		}
		pi++;
	}
}

void RichTxt::ApplyStyle(const RichText::FormatInfo& fi, RichPara& pa, const RichStyles& style)
{
	if(fi.paravalid & STYLE) {
		int q = style.Find(fi.styleid);
		if(q >= 0) {
			pa.ApplyStyle(style[q].format);
			pa.format.styleid = fi.styleid;
		}
	}
}

void RichTxt::Apply(const RichText::FormatInfo& fi, RichPara& pa, const RichStyles& style)
{
	ApplyStyle(fi, pa, style);
	for(int i = 0; i < pa.GetCount(); i++)
		fi.ApplyTo(pa[i].format);
	fi.ApplyTo(pa.format);
}

void RichTxt::ApplyFormat(const FormatInfo& fi, int pi, int pi2, const RichStyles& style)
{
	while(pi < pi2) {
		if(IsTable(pi)) {
			RichTable& tab = part[pi].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j)) {
						RichTxt& txt = tab[i][j].text;
						tab.InvalidateRefresh(i, j);
						txt.ApplyFormat(fi, 0, txt.GetPartCount(), style);
					}
		}
		else {
			RichPara pa;
			if(fi.paravalid & RichText::STYLE)
				pa = RichTxt::Get(pi, fi.styleid, style);
			else
				pa = Get(pi, style);
			Apply(fi, pa, style);
			Put(pi, pa, style);
		}
		pi++;
	}
}

void RichTxt::SaveFormat(Formating& r, int p1, int p2, const RichStyles& style) const
{
	Array<RichObject> dummy;
	for(int i = p1; i <= p2; i++)
		if(IsTable(i)) {
			const RichTable& tab = part[i].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j)) {
						const RichTxt& txt = tab[i][j].text;
						txt.SaveFormat(r, 0, txt.GetPartCount() - 1, style);
					}
		}
		else {
			RichPara pa = Get(i, style);
			for(int i = 0; i < pa.GetCount(); i++) {
				RichPara::Part& p = pa[i];
				int q = p.GetLength();
				p.field = Id();
				p.object = RichObject();
				WString h;
				while(q) {
					int c = min(q, 50000);
					h.Cat(c + 32);
					q -= c;
				}
				p.text = h;
			}
			r.styleid.Add(pa.format.styleid);
			r.format.Add(pa.Pack(GetStyle(style, pa.format.styleid).format, dummy));
		}
}

void RichTxt::RestoreFormat(int pi, const Formating& info, int& ii, const RichStyles& style)
{
	Array<RichObject> dummy;
	while(ii < info.format.GetCount() && pi < GetPartCount()) {
		if(IsTable(pi)) {
			RichTable& tab = part[pi].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++) {
					if(tab(i, j)) {
						if(ii >= info.format.GetCount())
							return;
						tab.InvalidateRefresh(i, j);
						tab[i][j].text.RestoreFormat(0, info, ii, style);
					}
				}
			pi++;
		}
		else {
			RichPara pa = Get(pi, style);
			RichPara pf;
			pf.Unpack(info.format[ii], dummy, GetStyle(style, info.styleid[ii]).format);
			RichPara t;
			t.format = pf.format;
			int si = 0;
			int sp = 0;
			for(int j = 0; j < pf.GetCount(); j++) {
				const RichPara::Part& q = pf[j];
				for(int k = 0; k < q.text.GetLength(); k++) {
					int len = q.text[k] - 32;
					t.part.Add().format = q.format;
					while(len) {
						const RichPara::Part& p = pa[si];
						if(p.IsText()) {
							int l = min(len, p.GetLength() - sp);
							t.part.Top().text.Cat(p.text.Mid(sp, l));
							sp += l;
							len -= l;
							ASSERT(sp <= p.GetLength());
							if(sp >= p.GetLength()) {
								sp = 0;
								si++;
							}
						}
						else {
							ASSERT(sp == 0);
							(t.part.Add() = pa[si++]).format = q.format;
							len--;
							sp = 0;
						}
					}
				}
			}
			ASSERT(si == pa.GetCount() && sp == 0);
			Put(pi, t, style);
			ii++;
			pi++;
		}
	}
}

WString RichTxt::GetPlainText(bool withcr, bool allow_tabs) const
{
	WString clip;
	for(int pass = 0; pass < 2; pass++) { // test if we can use tabs
		clip.Clear();
		auto Eol = [&] {
			if(withcr)
				clip.Cat('\r');
			clip.Cat('\n');
		};
		for(int pi = 0; pi < GetPartCount(); pi++) {
			if(pi)
				Eol();
			if(IsTable(pi)) {
				const RichTable& tab = part[pi].Get<RichTable>();
				for(int i = 0; i < tab.GetRows(); i++) {
					bool next = false;
					for(int j = 0; j < tab.GetColumns(); j++)
						if(tab(i, j)) {
							if(next) {
								if(pass)
									clip.Cat('\t');
								else
									Eol();
							}
							WString h = tab[i][j].text.GetPlainText(withcr);
							if(allow_tabs && h.Find('\n') >= 0)
								allow_tabs = false;
							clip << h;
							next = true;
						}
					Eol();
				}
			}
			else
				clip.Cat(Get(pi, RichStyle::GetDefault()).GetText());
		}
		if(!allow_tabs)
			break;
	}
	return clip;
}

RichTxt& RichTxt::GetTableUpdateText(int table, const RichStyles& style, int& pi)
{
	Invalidate();
	for(int i = 0;; i++)
		if(IsTable(i)) {
			table--;
			RichTable& tab = part[i].Get<RichTable>();
			if(table <= tab.GetTableCount()) {
				SetRefresh(i);
				if(table == 0) {
					pi = i;
					return *this;
				}
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.GetColumns(); j++)
						if(tab(i, j)) {
							RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								tab.InvalidateRefresh(i, j);
								return txt.GetTableUpdateText(table, style, pi);
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
	NEVER();
}

void RichTxt::Normalize()
{
	RichPara pa;
	if(GetPartCount() && IsTable(0)) {
		part.Insert(0);
		Put(0, pa, RichStyle::GetDefault());
		Invalidate();
	}
	if(GetPartCount() == 0 || IsTable(GetPartCount() - 1)) {
		Put(GetPartCount(), pa, RichStyle::GetDefault());
		Invalidate();
	}
}

}
