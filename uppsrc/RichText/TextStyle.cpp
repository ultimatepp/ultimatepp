#include "RichText.h"

namespace Upp {

const RichStyle& RichStyle::GetDefault()
{
	return Single<RichStyle>();
}

Uuid  RichStyle::GetDefaultId()
{
	Uuid id;
	id.SetNull();
	return id;
}

const RichStyle& GetStyle(const RichStyles& s, const Uuid& id)
{
	return s.Get(id, RichStyle::GetDefault());
}

int FindStyleWithName(const RichStyles& style, const String& name)
{
	for(int j = 0; j < style.GetCount(); j++)
		if(style[j].name == name)
			return j;
	return -1;
}

struct RichText::StyleChangeOp : RichTxt::ParaOp {
	Uuid id, nid;

	virtual bool operator()(RichTxt::Para& p) {
		if(p.styleid == id) {
			p.styleid = nid;
			p.Invalidate();
			return true;
		}
		return false;
	}
};

void RichText::SetStyle(const Uuid& id, const RichStyle& _style)
{
	Invalidate();
	RichStyle& s = style.GetAdd(id);
	s = _style;
	s.format.styleid = id;
	s.format.language = 0;
	s.format.newpage = false;
	StyleChangeOp op;
	op.nid = op.id = id;
	Update(op);
}

struct RichText::SetStylesOp : RichTxt::ParaOp {
	RichStyles *style;

	virtual bool operator()(RichTxt::Para& p) {
		p.Invalidate();
		if(style->Find(p.styleid) < 0)
			p.styleid = RichStyle::GetDefaultId();
		return true;
	}
};

void RichText::SetStyles(const RichStyles& _style)
{
	Invalidate();
	style <<= _style;
	SetStylesOp op;
	op.style = &style;
	Update(op);
}

Uuid RichText::GetStyleId(const String& name) const
{
	int i = FindStyleWithName(style, name);
	return i >= 0 ? style.GetKey(i) : RichStyle::GetDefaultId();
}

void  RichText::RemoveStyle(const Uuid& id)
{
	ASSERT(id != RichStyle::GetDefaultId());
	Invalidate();
	StyleChangeOp op;
	op.id = id;
	op.nid = RichStyle::GetDefaultId();
	Update(op);
	style.RemoveKey(id);
}

struct RichText::MergeStylesOp : RichTxt::ParaOp {
	RichText       *text;
	const RichText *stxt;

	virtual bool operator()(RichTxt::Para& p) {
		if(text->style.Find(p.styleid) < 0) {
			RichStyle& s = text->style.GetAdd(p.styleid);
			s = stxt->GetStyle(p.styleid);
			s.format.styleid = p.styleid;
		}
		return false;
	}
};

void RichText::MergeStyles(const RichText& text)
{
	Invalidate();
	MergeStylesOp op;
	op.stxt = &text;
	op.text = this;
	const_cast<RichText&>(text).Update(op);
}

struct RichText::OverrideStylesOp : RichTxt::ParaOp {
	VectorMap<Uuid, Uuid> cs;

	virtual bool operator()(RichTxt::Para& p) {
		p.styleid = cs.Get(p.styleid, RichStyle::GetDefaultId());
		p.Invalidate();
		return true;
	}
};

void  RichText::OverrideStyles(const RichStyles& ms, bool matchname, bool addmissing)
{
	Invalidate();
	RichStyles orig = pick(style);
	style <<= ms;
	OverrideStylesOp op;
	for(int i = 0; i < orig.GetCount(); i++) {
		Uuid pid = orig.GetKey(i);
		int q;
		if(ms.Find(pid) >= 0)
			op.cs.Add(pid, pid);
		else
		if(matchname && (q = FindStyleWithName(style, orig[i].name)) >= 0)
			op.cs.Add(pid, style.GetKey(q));
		else
		if(addmissing) {
			SetStyle(pid, orig[i]);
			op.cs.Add(pid, pid);
		}
	}

	Update(op);
}

}
