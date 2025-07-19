#include "RichText.h"

namespace Upp {

void RichTxt::Para::Invalidate() const
{
	INTERLOCKED {
		static int64 ss;
		updateserial = ++ss;
		ccx = -1;
	}
}

RichTxt::Para::Para(const Para& src, int)
: object(src.object, 1)
{
	length = src.length;
	styleid = src.styleid;
	content = src.content;
	haspos = src.haspos;
	if(src.number_fmt)
		number_fmt.Create<RichPara::NumberFormat>(*src.number_fmt);
	Invalidate();
	checked = false;
}

RichTxt::Part::Part(const Part& src, int) {
	if(src.Is<Para>())
		Create<Para>() <<= src.Get<Para>();
	else
		Create<RichTable>() <<= src.Get<RichTable>();
}

RichTxt::Part::Part() {}

void RichTxt::Invalidate()
{
	length = -1;
	tabcount = -1;
}

int RichTxt::GetPartLength(int pi) const
{
	return part[pi].Is<RichTable>() ? part[pi].Get<RichTable>().GetLength() : part[pi].Get<Para>().length;
}

bool             RichTxt::IsTable(int i) const  { return part[i].Is<RichTable>(); }
const RichTable& RichTxt::GetTable(int i) const { return part[i].Get<RichTable>(); }


int RichTxt::FindPart(int& pos) const
{
	int pi = 0;
	while(pi < part.GetCount()) {
		int l = GetPartLength(pi) + 1;
		if(pos < l)
			break;
		pos -= l;
		pi++;
	}
	if(pi >= part.GetCount())
		pos = 0;
	return pi;
}

int  RichTxt::GetPartPos(int pi) const
{
	int pos = 0;
	for(int i = 0; i < pi; i++)
		pos += GetPartLength(i) + 1;
	return pos;
}

void RichTxt::SetRefresh(int parti)
{
	switch(r_type) {
	case NONE:
		r_parti = parti;
		r_type = PARA;
		if(IsPara(parti)) {
			Para& pp = part[parti].Get<Para>();
			if(pp.ccx >= 0) {
				r_paraocx = pp.ccx;
				r_paraocy = Sum(pp.linecy, 0) + pp.before + pp.after;
				r_keep = pp.keep;
				r_keepnext = pp.keepnext;
				r_firstonpage = pp.firstonpage;
				r_newpage = pp.newpage || pp.header_qtf.GetCount() || pp.footer_qtf.GetCount();
				break;
			}
			else
				r_type = FROM;
		}
		break;
	case PARA:
		if(parti == r_parti) break;
	case FROM:
		r_parti = min(parti, r_parti);
		r_type = FROM;
		break;
	}
}

void RichTxt::SetRefreshFrom(int parti)
{
	if(r_type == NONE)
		r_parti = parti;
	else
		r_parti = min(parti, r_parti);
	r_type = FROM;
}

void RichTxt::Put(int i, const RichPara& p, const RichStyle& s)
{
	if(i >= part.GetCount() || !IsPara(i))
		part.At(i).Create<Para>();
	Para& pp = part[i].Get<Para>();
	int numbering = p.format.GetNumberLevel(); // level 0 just adds after and before...
	if(pp.numbering != numbering)
		SetRefreshFrom(i);
	else
		SetRefresh(i);
	pp.number_fmt.Clear();
	pp.content = p.Pack(s.format, pp.object);
	pp.Invalidate();
	pp.checked = false;
	pp.styleid = p.format.styleid;
	pp.length = p.GetLength();
	pp.numbering = numbering;
	pp.spellerrors.Clear();
	pp.haspos = p.HasPos();
	if(numbering >= 0 || p.format.reset_number)
		pp.number_fmt.Create() = p.format;
}

void RichTxt::Put(int i, const RichPara& p, const RichStyles& s)
{
	Put(i, p, GetStyle(s, p.format.styleid));
}

void RichTxt::Set(int i, const RichPara& p, const RichStyles& s)
{
	Put(i, p, s);
	Invalidate();
}

void RichTxt::RemovePart(int parti)
{
	part.Remove(parti);
	Invalidate();
}

void RichTxt::SetPick(int i, RichTable&& p)
{
	const_cast<RichTable&>(p).Normalize();
	part.At(i).Create<RichTable>() = pick(p);
	Invalidate();
	SetRefresh(i);
}

RichPara RichTxt::Get(int parai, const RichStyle& style, bool usecache) const
{
	ASSERT(part[parai].Is<Para>());
	const Para& pp = part[parai].Get<Para>();
	RichPara p;
	if(usecache)
		p.CacheId(pp.updateserial);
	p.Unpack(pp.content, pp.object, style.format);
	return p;
}

RichPara RichTxt::Get(int parai, const Uuid& styleid, const RichStyles& s, bool usecache) const
{
	RichPara p = Get(parai, GetStyle(s, styleid), usecache);
	p.format.styleid = styleid;
	return p;
}

RichPara RichTxt::Get(int parti, const RichStyles& s, bool usecache) const
{
	return Get(parti, part[parti].Get<Para>().styleid, s, usecache);
}

RichPara RichTxt::Get(int parai, const RichStyle& style) const
{
	return Get(parai, style, false);
}

RichPara RichTxt::Get(int parai, const Uuid& styleid, const RichStyles& s) const
{
	return Get(parai, styleid, s, false);
}

RichPara RichTxt::Get(int i, const RichStyles& s) const
{
	return Get(i, s, false);
}

bool RichTxt::IsEmpty() const
{
	return part.IsEmpty() || part.GetCount() == 1 && ComputeLength() == 0;
}

int RichTxt::ComputeLength() const
{
	int length = part.GetCount() - 1;
	for(int i = 0; i < part.GetCount(); i++)
		length += GetPartLength(i);
	return length;
}

int RichTxt::GetLength() const
{ // expects int to be atomic, worst 'race' is that it gets computed more times...
	if(length < 0)
		length = ComputeLength();
	return length;
}

int  RichTxt::GetTableCount() const
{ // expects int to be atomic, worst 'race' is that it gets computed more times...
	if(tabcount < 0) {
		int n = 0;
		for(int i = 0; i < part.GetCount(); i++)
			if(IsTable(i))
				n += GetTable(i).GetTableCount() + 1;
		tabcount = n;
	}
	return tabcount;
}

bool RichTxt::EvaluateFields(const RichStyles& s, VectorMap<String, Value>& vars)
{
	bool b = false;
	for(int i = 0; i < part.GetCount(); i++)
		if(IsTable(i)) {
			RichTable& tab = part[i].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j) && tab[i][j].text.EvaluateFields(s, vars)) {
						tab.InvalidateRefresh(i, j);
						b = true;
					}
		}
		else {
			RichPara p = Get(i, s);
			if(p.EvaluateFields(vars)) {
				b = true;
				Set(i, p, s);
			}
		}
	return b;
}

void RichTxt::ClearSpelling()
{
	for(int i = 0; i < part.GetCount(); i++) {
		if(IsTable(i)) {
			RichTable& tab = part[i].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j)) {
						tab[i][j].text.ClearSpelling();
						tab.InvalidateRefresh(i, j);
					}
		}
		else {
			Para& p = part[i].Get<Para>();
			p.spellerrors.Clear();
			p.checked = false;
		}
	}
}

void RichTxt::SetParaStyle(int i, const Uuid& id)
{
	ASSERT(IsPara(i));
	Para& p = part[i].Get<Para>();
	p.styleid = id;
	p.Invalidate();
	SetRefreshFrom(i);
}

void RichTxt::GetRichPos(int pos, RichPos& rp, int ti, int maxlevel, const RichStyles& st) const
{
	int p = pos;
	int pti = ti;
	for(int i = 0; i < part.GetCount(); i++) {
		int l = GetPartLength(i) + 1;
		if(pos < l) {
			if(IsTable(i)) {
				const RichTable& tab = GetTable(i);
				rp.level++;
				rp.tabtextparti = i;
				rp.tabtextpartcount = part.GetCount();
				rp.tabtextlen = GetLength();
				rp.tabposintabtext = p - pos;
				rp.posintab = pos;
				rp.cell = tab.FindCell(pos);
				rp.posincell = pos;
				rp.tabsize = tab.GetSize();
				rp.tablen = tab.GetLength();
				const RichTxt& ct = tab[rp.cell].text;
				rp.celllen = ct.GetLength();
				rp.parenttab = pti;
				rp.table = ti + 1;
				if(rp.level < maxlevel)
					ct.GetRichPos(pos, rp, ti + 1 + tab.GetTableCount(rp.cell), maxlevel, st);
				return;
			}
			else {
				rp.posinpara = pos;
				rp.parai = i;
				rp.partcount = part.GetCount();
				rp.paralen = l - 1;
				Get(i, st, true).GetRichPos(rp, pos);
				return;
			}
		}
		pos -= l;
		if(IsTable(i))
			ti += 1 + GetTable(i).GetTableCount();
	}
	rp.parai = part.GetCount();
	rp.posinpara = 0;
	rp.partcount = part.GetCount();
	rp.paralen = 0;
}

RichPara::Format RichTxt::GetFirstFormat(const RichStyles& style) const
{
	if(part.IsEmpty())
		return RichPara::Format();
	if(IsTable(0)) {
		const RichTable& tab = GetTable(0);
		return tab[0][0].text.GetFirstFormat(style);
	}
	RichPos rp;
	Get(0, style).GetRichPos(rp, 0);
	return rp.format;
}

RichCellPos RichTxt::GetCellPos(int table, int row, int column) const
{
	int pos = 0;
	for(int i = 0;; i++) {
		if(IsTable(i)) {
			table--;
			const RichTable& tab = part[i].Get<RichTable>();
			if(table <= tab.GetTableCount()) {
				if(table == 0) {
					RichCellPos p;
					p.tabsize = tab.GetSize();
					p.tablen = tab.GetLength();
					p.cellpos = tab.GetCellPos(row, column);
					p.pos = p.tabpos = pos;
					p.pos += p.cellpos;
					p.textlen = GetLength();
					p.level = 1;
					return p;
				}
				for(int i = 0; i < tab.GetRows(); i++)
					for(int j = 0; j < tab.GetColumns(); j++)
						if(tab(i, j)) {
							const RichTxt& txt = tab[i][j].text;
							if(table <= txt.GetTableCount()) {
								RichCellPos p = txt.GetCellPos(table, row, column);
								p.pos += pos + tab.GetCellPos(i, j);
								p.level++;
								return p;
							}
							table -= txt.GetTableCount();
						}
				NEVER();
			}
			else
				table -= tab.GetTableCount();
		}
		pos += GetPartLength(i) + 1;
	}
	NEVER();
	return RichCellPos();
}

RichCellPos RichTxt::GetCellPos(int table, Point p) const
{
	return GetCellPos(table, p.y, p.x);
}

bool RichTxt::Iterate(UpdateIterator& r, int gpos, const RichStyles& s)
{
	for(int pi = 0; pi < part.GetCount(); pi++)
		if(IsTable(pi)) {
			RichTable& tab = part[pi].Get<RichTable>();
			for(int i = 0; i < tab.GetRows(); i++)
				for(int j = 0; j < tab.GetColumns(); j++)
					if(tab(i, j)) {
						if(tab[i][j].text.Iterate(r, gpos, s))
							return true;
						gpos += tab[i][j].text.GetLength() + 1;
					}
		}
		else {
			RichPara p = Get(pi, s);
			int q = r(gpos, p);
			if(q & UpdateIterator::UPDATE)
				Set(pi, p, s);
			if(q & UpdateIterator::STOP)
				return true;
			gpos += GetPartLength(pi) + 1;
		}
	return false;
}

struct sIter__ : RichTxt::UpdateIterator {
	RichTxt::Iterator *iter;
	virtual int operator()(int pos, RichPara& para) {
		return iter->operator()(pos, para) ? STOP : CONTINUE;
	}
};

bool RichTxt::Iterate(Iterator& r, int gpos, const RichStyles& s) const
{
	sIter__ it;
	it.iter = &r;
	return const_cast<RichTxt *>(this)->Iterate(it, gpos, s);
}

void RichTxt::Init()
{
	r_type = ALL;
	r_parti = 0;
	r_paraocx = r_paraocy = -1;
	tabcount = length = 0;
}

void RichTxt::Clear()
{
	part.Clear();
	Init();
}

RichTxt::RichTxt()
{
	Init();
}

RichTxt::RichTxt(const RichTxt& src, int)
{
	Init();
	part <<= src.part;
	length = src.length;
	tabcount = src.tabcount;
}

#ifdef _DEBUG

#define DMP(x)   s << #x << "=" << x << ", "

String RichPos::ToString() const
{
	String s;
	DMP(tabtextparti);
	DMP(tabtextpartcount);
	DMP(tabposintabtext);
	DMP(tabtextlen);
	DMP(table);
	DMP(tabsize);
	DMP(cell);
	DMP(tablen);
	DMP(posintab);
	DMP(celllen);
	DMP(posincell);
	DMP(parai);
	DMP(partcount);
	DMP(posinpara);
	DMP(paralen);
	DMP(level);
	DMP(parenttab);
	s << "char: " << (char)chr;
	return s;
}

String RichCellPos::ToString() const
{
	String s;
	s << "pos: " << pos << ", textlen: " << textlen << ", size: " << tabsize << ", tabpos: " << tabpos
	  << ", tablen: " << tablen << ", cellpos: " << cellpos << ", celllen: " << celllen << ", level: " << level;
	return s;
}

#endif

}
