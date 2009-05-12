//////////////////////////////////////////////////////////////////////
// OldTreeCtrl: tree control.

#include "TCtrlLib.h"
#pragma hdrstop

#include "OldTreeCtrl.h"

NAMESPACE_UPP

#define IMAGECLASS OldTreeCtrlImg
#define IMAGEFILE  <TCtrlLib/OldTreeCtrl.iml>
#include <Draw/iml_header.h>

enum { SCREEN_ZOOM = 140 };

#ifdef _DEBUG
//#define DBG_PAINT(co, dbgco) dbgco // debug version
#define DBG_PAINT(co, dbgco) co    // release version
#else
#define DBG_PAINT(co, dbgco) co    // release version
#endif

#define IMAGECLASS OldTreeCtrlImg
#define IMAGEFILE  <TCtrlLib/OldTreeCtrl.iml>
#include <Draw/iml_source.h>

enum
{
	ICONGAP     = 6,
	TEXTGAP     = 5,
	TSGAP       = 3,
	BUTTON_SIZE = 9,
	BUTTON_HALF = BUTTON_SIZE >> 1,
};

//////////////////////////////////////////////////////////////////////
// OldTreeCursor::

OldTreeCursor::OldTreeCursor()
: root(0)
{
}

OldTreeCursor::OldTreeCursor(const OldTreeItem& item)
: root(0)
{
	Find(item);
}

OldTreeCursor::OldTreeCursor(const OldTreeCursor& another, int)
: root(another.root)
, cursor(another.cursor, 0)
{
}

OldTreeCursor& OldTreeCursor::Adjust()
{
	if(cursor.IsEmpty() || !root)
		return *this;
	const OldTreeItem *ptr = root;
	int *b = cursor.Begin(), *e = cursor.End(), *p;
	for(p = b; p < e; p++)
		if(ptr->IsEmpty())
			break;
		else {
			*p = minmax(*p, 0, ptr->GetCount() - 1);
			ptr = &ptr->children[*p];
		}
	cursor.SetCount(p - b);
	return *this;
}

OldTreeCursor& OldTreeCursor::WeakAdjust()
{
	if(cursor.IsEmpty() || !root)
		return *this;
	const OldTreeItem *ptr = root;
	int *b = cursor.Begin(), *e = cursor.End(), *p;
	for(p = b; p < e; p++) {
		*p = minmax(*p, 0, ptr->GetCount());
		if(*p == ptr->GetCount()) {
			p++;
			break;
		}
		ptr = &ptr->children[*p];
	}
	cursor.SetCount(p - b);
	return *this;
}

OldTreeCursor& OldTreeCursor::AddAfter(const OldTreeCursor& c, int count)
{
	int top = c.cursor.GetCount() - 1;
	if(!root || root != c.root || top < 0 || cursor.GetCount() <= top
		|| memcmp(cursor.Begin(), c.cursor.Begin(), sizeof(int) * top))
		return *this;
	int d = cursor[top] - c.cursor[top];
	if(d >= 0)
		cursor[top] += max(count, -d);
	return *this;
}

OldTreeCursor& OldTreeCursor::AdjustAfter(const OldTreeCursor& c, int count)
{
	return AddAfter(c, count).Adjust();
}

OldTreeCursor& OldTreeCursor::WeakAdjustAfter(const OldTreeCursor& c, int count)
{
	return AddAfter(c, count).WeakAdjust();
}

OldTreeItem *OldTreeCursor::Get() const
{
	OldTreeItem *ptr = root;
	for(const int *p = cursor.Begin(), *e = cursor.End(); ptr && p < e; p++)
		ptr = (*p >= 0 && *p < ptr->children.GetCount() ? &ptr->children[*p] : NULL);
	return ptr;
}

OldTreeItem *OldTreeCursor::GetParent() const
{
	if(!root || cursor.IsEmpty())
		return NULL;
	OldTreeItem *ptr = root;
	for(const int *p = cursor.Begin(), *e = cursor.End() - 1; ptr && p < e; p++)
		ptr = (*p >= 0 && *p < ptr->children.GetCount() ? &ptr->children[*p] : NULL);
	return ptr;
}

const OldTreeCursor& OldTreeCursor::Open() const
{
	if(!root)
		return *this;
	OldTreeItem *i = root;
	for(const int *p = cursor.Begin(), *e = cursor.End(); p < e; p++) {
		if(!i->IsOpenItem())
			i->Open().TouchLayout();
		if(*p < 0 || *p >= i->GetCount())
			break;
		i = &(*i)[*p];
	}
	return *this;
}

OldTreeCursor& OldTreeCursor::BeginAll()
{
	cursor.Clear();
	return *this;
}

OldTreeCursor& OldTreeCursor::EndAll()
{
	cursor.Clear();
	ASSERT(root);
	cursor.Add(root->GetCount());
	return *this;
}

bool OldTreeCursor::IsBeginAll() const
{
	return cursor.IsEmpty();
}

bool OldTreeCursor::IsEndAll() const
{
	ASSERT(root);
	return !cursor.IsEmpty() && cursor[0] >= root->GetCount();
}

bool OldTreeCursor::IsBegin() const
{
	return cursor.IsEmpty() || cursor.Top() == 0;
}

const OldTreeCursor& OldTreeCursor::Select(bool select, bool subtree) const
{
	OldTreeItem *ptr = Get();
	if(ptr)
		if(subtree)
			ptr->SelectTree(select);
		else
			ptr->Select(select);
	return *this;
}

OldTreeCursor& OldTreeCursor::Find(const OldTreeItem& item)
{
	cursor.Clear();
	if(root = item.owner) {
		for(OldTreeItem *i = const_cast<OldTreeItem *>(&item); i->parent; i = i->parent) {
			int pos = i->parent->children.GetIndex(*i);
			ASSERT(pos >= 0);
			cursor.Insert(0, pos);
		}
	}
	return *this;
}

OldTreeItem *OldTreeCursor::Find(int clss, int flags)
{
	for(OldTreeItem *i; i = Get(); Down(flags))
		if(i->GetClass() == clss)
			return i;
	return NULL;
}

bool OldTreeCursor::Up(int flags)
{
	OldTreeItem *parent = GetParent();
	if(!parent)
		return false;
	int pos = min(cursor.Top(), parent->GetCount());
	if(pos <= 0) { // drop out of child into parent
		if(flags & LEFT)
			cursor.Drop();
		return true;
	}
	cursor.Top() = (pos -= 1);
	if(flags & (RIGHT | RIGHT_CLOSED))
		for(OldTreeItem *temp; temp = &parent->children[pos],
			!temp->IsEmpty() && (temp->IsOpenItem() || flags & RIGHT_CLOSED);
			cursor.Add(pos = temp->GetCount() - 1), parent = temp)
			;
	return true;
}

bool OldTreeCursor::Down(int flags)
{
	if(cursor.IsEmpty()) { // step into 1st item
		if(flags & (RIGHT | RIGHT_CLOSED) && root && !root->IsEmpty()
			&& (root->IsOpenItem() || flags & RIGHT_CLOSED)) {
			cursor.Add(0);
			return true;
		}
		return false;
	}

	OldTreeItem *parent = GetParent();
	if(!parent)
		return false;
	int pos = cursor.Top();
	if(pos < parent->GetCount() && flags & (RIGHT | RIGHT_CLOSED)) {
		OldTreeItem *ptr = &parent->children[pos];
		if(!ptr->IsEmpty() && (ptr->IsOpenItem() || flags & RIGHT_CLOSED)) { // descend into child
			cursor.Add(0);
			return true;
		}
	}
	do {
		if(++cursor.Top() < parent->GetCount()) // move to next item in current row
			return true;
		if(!(flags & LEFT) || cursor.GetCount() <= 1) { // not moving out or global last item encountered - move to end item
			cursor.Top() = parent->GetCount();
			return true;
		}
		cursor.Drop();
	}
	while(parent = parent->parent);
	return true;
}

OldTreeCursor& OldTreeCursor::Right()
{
	if(!IsEnd())
		cursor.Add(0);
	return *this;
}

OldTreeCursor& OldTreeCursor::Left()
{
	if(!cursor.IsEmpty())
		cursor.Drop();
	return *this;
}

OldTreeCursor& OldTreeCursor::Begin()
{
	cursor.Top() = 0;
	return *this;
}

OldTreeCursor& OldTreeCursor::End()
{
	OldTreeItem *parent = GetParent();
	if(parent)
		cursor.Top() = parent->GetCount();
	return *this;
}

int TreeCursor_Compare(const OldTreeCursor& a, const OldTreeCursor& b)
{
	return OldTreeCursor::Compare(a, b);
}

int OldTreeCursor::Compare(const OldTreeCursor& a, const OldTreeCursor& b)
{
	if(a.root != b.root)
		return cmp(a.root, b.root);
	const int *pa = a.cursor.Begin(), *ea = pa + min(a.cursor.GetCount(), b.cursor.GetCount());
	const int *pb = b.cursor.Begin();
	for(; pa < ea; pa++, pb++)
		if(*pa != *pb)
			return cmp(*pa, *pb);
	return cmp(a.cursor.GetCount(), b.cursor.GetCount());
}

String OldTreeCursor::ToString() const
{
	String out;
	out << '[';
	for(int i = 0; i < cursor.GetCount(); i++) {
		if(i)
			out << ", ";
		out << cursor[i];
	}
	out << ']';
	return out;
}

bool OldTreeCursor::IsSubtreeOf(const OldTreeCursor& top) const
{
	return top.root == root
		&& top.cursor.GetCount() <= cursor.GetCount()
		&& !memcmp(top.cursor.Begin(), cursor.Begin(), top.cursor.GetCount() * sizeof(int));
}

//////////////////////////////////////////////////////////////////////
// SimpleTreeItem::

SimpleTreeItem::SimpleTreeItem()
{
	Zero(*this);
	indent = 0;
	type = TEXT;
	layout = false;
	enabled = true;
	branch = true;
	leaf = true;
	auto_size = true;
	auto_indent = true;
}

//////////////////////////////////////////////////////////////////////
// OldTreeItem::

OldTreeItem::OldTreeItem()
: text(String::GetVoid())
{
}

OldTreeItem::OldTreeItem(const OldTreeItem& item, int)
: SimpleTreeItem(item)
, text(item.text)
, value(item.value)
, subtree(item.subtree)
, children(item.children, 0)
{
}

OldTreeItem& OldTreeItem::Open(bool _open)
{
	if((open = _open) && !up_to_date && subtree) { // expand subtree as necessary
		subtree(*this);
		if(is_static)
			up_to_date = true;
		TouchLayout();
	}
	return *this;
}

void OldTreeItem::SetCursor()
{
	if(this) {
		ASSERT(owner);
		owner->SetCursor(this);
	}
}

bool OldTreeItem::IsSubtree() const
{
	return !IsEmpty() || (subtree && !up_to_date);
}

bool OldTreeItem::IsOpenFull() const
{
	return IsOpenItem() && !IsEmpty();
}

String OldTreeItem::GetTextOrValue() const
{
	if(!text.IsVoid())
		return text;
	return StdFormat(value);
}

OldTreeItem& OldTreeItem::Select(bool _sel)
{
	if(this && selected != _sel) {
		selected = _sel;
		Touch();
	}
	return *this;
}

OldTreeItem& OldTreeItem::SelectTree(bool _sel)
{
	if(this) {
		Select(_sel);
		for(int i = 0; i < GetCount(); i++)
			children[i].SelectTree(_sel);
	}
	return *this;
}

int OldTreeItem::GetCount(int clss) const
{
	int count = (item_class == clss ? 1 : 0);
	for(int i = 0; i < children.GetCount(); i++)
		count += children[i].GetCount(clss);
	return count;
}

int OldTreeItem::GetCountDeep() const
{
	if(this == 0)
		return 0;
	int count = 1;
	for(int i = 0; i < children.GetCount(); i++)
		count += children[i].GetCountDeep();
	return count;
}

const OldTreeItem *OldTreeItem::FindValue(Value _value) const
{
	if(value == _value)
		return this;
	for(int i = 0; i < children.GetCount(); i++) {
		const OldTreeItem *p = children[i].FindValue(_value);
		if(p)
			return p;
	}
	return 0;
}

OldTreeItem *OldTreeItem::FindValue(Value _value)
{
	if(value == _value)
		return this;
	for(int i = 0; i < children.GetCount(); i++) {
		OldTreeItem *p = children[i].FindValue(_value);
		if(p)
			return p;
	}
	return 0;
}

void OldTreeItem::Touch()
{
	if(this && !owner->updating && !owner->IsFullRefresh())
		owner->Refresh(GetClientRect());
}

void OldTreeItem::TouchLayout()
{
	if(this && !layout) {
		OldTreeItem *p = this;
		while(p && !p->layout) {
			p->layout = true;
			p = p->parent;
		}
		if(!p && owner)
			owner->Refresh();
	}
}

Rect OldTreeItem::GetRect(int hittest) const
{
	Rect sum = Null;
	if(item_size.cy <= 0)
		return sum;

	if((hittest & HBUTTON) && IsLeaf() && IsSubtree())
		sum = RectC(-BUTTON_HALF, (item_size.cy >> 1) - BUTTON_HALF, BUTTON_SIZE, BUTTON_SIZE);

	Size iconsize(0, 0);
	if(type == TEXT && image)
		iconsize = image.GetSize();
	int left = 0;
	if(leaf)
		left += BUTTON_HALF + 1 + ICONGAP;
	if(type == TEXT && image) {
		if(hittest & HICON)
			sum |= RectC(left, 0, iconsize.cx, item_size.cy);
		left += iconsize.cx + TEXTGAP;
	}
	if(hittest & (HTEXT | HPICT | HCTRL))
		sum |= Rect(left, 0, item_size.cx ? item_size.cx : tree_size.cx, item_size.cy);

	return sum;
}

OldTreeItem& OldTreeItem::Clear()
{
	children.Clear();
	TouchLayout();
	return *this;
}

OldTreeItem& OldTreeItem::SetCount(int n)
{
	if(children.GetCount() == n)
		return *this;

	while(children.GetCount() < n)
		Add();
	children.SetCount(n);
	TouchLayout();
	return *this;
}

OldTreeItem *OldTreeItem::Detach(int pos)
{
	children[pos].TouchLayout();
	return children.Detach(pos);
}

OldTreeItem& OldTreeItem::Add()
{
	return Insert(GetCount(), new OldTreeItem);
}

OldTreeItem& OldTreeItem::Insert(int pos)
{
	return Insert(pos, new OldTreeItem);
}

OldTreeItem& OldTreeItem::Add(OldTreeItem *item)
{
	return Insert(GetCount(), item);
}

static void SetOwner(OldTreeItem& item)
{
	for(int i = 0; i < item.GetCount(); i++)
		SetOwner(item[i].SetOwner(item.GetOwner()));
}

OldTreeItem& OldTreeItem::Insert(int pos, OldTreeItem *item)
{
	ASSERT(owner && item);
	children.Insert(pos, item);
	item->parent = this;
	item->owner  = owner;
	UPP::SetOwner(*item);
	item->TouchLayout();
	return *item;
}

void OldTreeItem::Remove(int pos, int count)
{
	ASSERT(owner);
	ASSERT(pos >= 0 && pos < GetCount());
	if(pos >= 0 && pos < GetCount()) {
		children.Remove(pos, count);
		TouchLayout();
	}
}

void OldTreeItem::LayoutItem()
{
	ASSERT(owner);
	if(!layout)
		return;
	Size iconsize = image.GetSize();
	int leafgap = (leaf ? BUTTON_HALF + 1 + ICONGAP : 0);
	if(auto_indent)
		indent = leafgap + (iconsize.cx >> 1);
	if(auto_size) {
		Size textsize(0, 0);
		item_size = Size(0, 0);

		switch(type) {
		case TEXT: {
				String display = GetTextOrValue();
				if(!IsNull(display)) {
/*					if(*display == '\xFF') {
						Document doc(display.Begin() + 1);
						Size size;
						textsize.cx = doc.GetWidth(DOC_SCREEN_ZOOM, ScreenInfo());
						textsize.cy = doc.GetHeight(DOC_SCREEN_ZOOM, ScreenInfo(), textsize.cx);
					}
					else*/
						textsize = GetSmartTextSize(display, owner->font);
				}
			}
			break;

		case PICT:
			textsize = picture->GetStdSize(value);
			break;

		case CTRL:
			textsize = ctrl->GetRect().Size();
			break;
		}
		item_size.cx = iconsize.cx + textsize.cx + leafgap + TSGAP;
		item_size.cy = max(iconsize.cy, textsize.cy);
		if(iconsize.cx && textsize.cx)
			item_size.cx += TEXTGAP;
	}

	tree_size = item_size;

/*	if(subtree && open) { // expand subtree as necessary
		if(!up_to_date) {
			subtree(*this);
			if(is_static)
				up_to_date = true;
		}
	}
*/
	if(open) { // include children in raw size
		for(int i = 0; i < children.GetCount(); i++) {
			OldTreeItem& child = children[i];
			child.LayoutItem();
			tree_size.cx = max(tree_size.cx, child.tree_size.cx + indent);
			tree_size.cy += child.tree_size.cy;
		}
	}

	layout = false;
}

void OldTreeItem::PaintItem(Draw& draw, Point pos) const
{
	Rect clip(0, 0, 9999, 9999);
	Rect rc = Rect(pos.x - BUTTON_HALF, pos.y, clip.right, pos.y + tree_size.cy) & clip;
	if(rc.IsEmpty())
		return;
	int daddy = pos.x;

	Color bg = (owner->IsShowEnabled() ? SWhite : SLtGray);
 { // clear branch area
		int lb = pos.x + (leaf ? BUTTON_HALF + 1 : 0);
		int rb = pos.x + indent + BUTTON_HALF + 1;
		draw.DrawRect(lb, pos.y, clip.right - lb, item_size.cy, DBG_PAINT(bg, LtRed));
		draw.DrawRect(lb, pos.y, rb - lb, tree_size.cy, DBG_PAINT(bg, LtMagenta));
/*
		int left = parent ? prev + 1 : clip.left;
		int right = pos.x + branch_indent + 1;
		if(right - left)
			draw.DrawRect(left, rc.top, right - left, rc.Height(), bg);
*/
	}

	if(IsBranch() && IsOpenFull()) { // generate branch
		int top = pos.y + item_size.cy;
		int bot = Null, bbot = Null;
		int tmp = top;
		for(int i = 0; i < children.GetCount(); i++) {
			const OldTreeItem& child = children[i];
			if(child.item_size.cy > 0 && child.leaf) {
				bot = tmp + (child.item_size.cy >> 1) + 1;
				bbot = tmp + child.item_size.cy;
			}
			tmp += child.tree_size.cy;
		}
		if(!IsNull(bot) && item_size.cy > 0) { // vertical joint
			int x = pos.x + indent;
			DrawRect(draw, Rect(x, top, x + 1, bot), OldTreeCtrlImg::dither());
		}
	}

	if(item_size.cy > 0) {
		Rect item(pos, GetItemSize());
//		if(prev + 1 < clip.right)
//			draw.DrawRect(prev + 1, item.top, clip.right - prev - 1, item.Height(), bg);
		if(item.Width() == 0)
			item.right = item.left + tree_size.cx;
		if(item.bottom > rc.top && item.top < rc.bottom) {
			if(leaf) { // draw leaf
				int mid = (item.top + item.bottom) >> 1;
				DrawRect(draw, pos.x, mid, BUTTON_SIZE + ICONGAP, 1, OldTreeCtrlImg::dither());
				if(IsSubtree()) { // draw expand/collapse button
					draw.DrawImage(pos.x - BUTTON_HALF, mid - BUTTON_HALF, IsOpenItem() ? OldTreeCtrlImg::minus() : OldTreeCtrlImg::plus());
				}
				item.left += BUTTON_HALF + 1 + ICONGAP;
			}

			Color ink = SBlack;
			Color bgnd = bg;
			if(IsSelected())
				if(owner->HasFocus()) {
					bgnd = SBlue;
					ink = SWhite;
				}
				else {
					bgnd = SLtGray;
					ink = SBlack;
				}

			if(image) {
				Size iconsize = image.GetSize();
				Point iconpos(item.left, (item.top + item.bottom - iconsize.cy) >> 1);
			#ifdef NEWIMAGE
				draw.DrawImage(iconpos.x, iconpos.y, enabled ? image : MakeImage(image, Etched));
			#else
				draw.DrawImage(iconpos.x, iconpos.y, image, enabled ? 0 : image.ETCHED);
			#endif
				item.left += iconsize.cx + TEXTGAP;
			}

			switch(type) {
			case TEXT: {
					String display = GetTextOrValue();
					WString wdisplay;
					if(!IsNull(display)) {
						Size size;
						if(*display == '\xFF') {
							Document doc;
							doc.Qtf(display.Begin() + 1);
							size.cx = doc.GetWidth(SCREEN_ZOOM);
							size.cy = doc.GetHeight(SCREEN_ZOOM, size.cx);
						}
						else
							size = GetTLTextSize(wdisplay = display.ToWString(), owner->font);

						Rect trc = item;
						trc.left -= TSGAP;
//						trc.top = (item.top + item.bottom - size.cy) >> 1;
//						trc.bottom = trc.top + size.cy;
						draw.DrawRect(trc, bgnd);
						trc.left += TSGAP;
						trc.top = (item.top + item.bottom - size.cy) >> 1;
						if(*display == '\xFF') {
							Document doc;
							String s;
							s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") "
								<< display.Begin() + 1;
							doc.Qtf(s);
							doc.Paint(SCREEN_ZOOM, draw, trc.left, trc.top, trc.Width());
						}
						else
							DrawTLText(draw, trc.left, trc.top, trc.Width(), wdisplay, owner->font, ink);
					}
				}
				break;

			case PICT:
				picture->Paint(draw, item, value, ink, bgnd, 0);
				break;

			case CTRL:
				if(item != ctrl->GetRect())
					ctrl->SetRect(item);
				break;
			}
		}
		pos.y += item_size.cy;
	}
	if(IsOpenFull()) {
		pos.x += indent;
		int i = 0;
		for(; i < children.GetCount() && pos.y + children[i].tree_size.cy <= rc.top; i++)
			pos.y += children[i].tree_size.cy;
		for(; i < children.GetCount() && pos.y < rc.bottom; i++) {
			children[i].PaintItem(draw, pos);
			pos.y += children[i].tree_size.cy;
		}
	}
}

OldTreeItem::HITTEST OldTreeItem::FindItem(Point pt, OldTreeItem *& item) const
{
	if(pt.y < 0 || pt.y >= tree_size.cy) {
		item = NULL;
		return HNIL;
	}
	if(item_size.cy > 0 && pt.y < item_size.cy) { // check this item
		item = (OldTreeItem*)this;
		if(GetRect(HBUTTON).Contains(pt))
			return HBUTTON;

		switch(type) {
		case TEXT:
			if(GetRect(HICON).Contains(pt))
				return HICON;
			if(GetRect(HTEXT).Contains(pt))
				return HTEXT;
			break;

		case PICT:
			if(GetRect(HPICT).Contains(pt))
				return HPICT;
			break;

		case CTRL:
			if(GetRect(HCTRL).Contains(pt))
				return HCTRL;
			break;
		}
		return HOTHER;
	}


	if(IsOpenFull() && (pt.y -= item_size.cy) >= 0) {
		pt.x -= indent;
		for(int i = 0; i < children.GetCount() && pt.y >= 0; pt.y -= children[i++].tree_size.cy) {
			HITTEST result;
			if((result = children[i].FindItem(pt, item)) != HNIL)
				return result;
		}
	}

	item = NULL;
	return HNIL;
}

int OldTreeItem::GetOpenCount() const
{
	int count = 0;
	if(item_size.cy > 0)
		count++;
	if(IsOpenFull())
		for(List::ConstIterator p = children.Begin(), e = children.End(); p < e; p++)
			count += p->GetOpenCount();
	return count;
}

Rect OldTreeItem::GetItemRect(int hittest) const
{
	Rect rc = GetRect(hittest);
	for(const OldTreeItem *c = this, *p; p = c->parent; c = p) {
		int i;
		if(!p->IsOpenFull() || (i = p->children.GetIndex(*c)) < 0)
			return Null; // error
		int y = p->item_size.cy;
		for(int t = 0; t < i; t++)
			y += p->children[t].tree_size.cy;
		rc.Offset(p->indent, y);
	}
	return rc;
}

Rect OldTreeItem::GetClientRect(int hittest) const
{
	return GetItemRect(hittest) + (owner->offset - owner->old_scroll);
}

Rect OldTreeItem::GetTreeRect() const
{
	Rect rc(0, 0, tree_size.cx, tree_size.cy);
	for(const OldTreeItem *c = this, *p; p = c->parent; c = p) {
		int i;
		if(!p->IsOpenFull() || (i = p->children.GetIndex(*c)) < 0)
			return Null; // error
		int y = p->item_size.cy;
		for(int t = 0; t < i; t++)
			y += p->children[t].tree_size.cy;
		rc.Offset(p->indent, y);
	}
	return rc;

}

Rect OldTreeItem::GetClientTreeRect() const
{
	return GetTreeRect() + (owner->offset - owner->old_scroll);
}

OldTreeItem& OldTreeItem::Set(Proc proc)
{
	subtree.Clear();
	proc(*this);
	return *this;
}

Bar::Item& OldTreeItem::BarOpenClose(Bar& bar)
{
	return bar.Add(enabled && IsSubtree(),
		IsOpenItem() ? t_("Close") : t_("Open"), callback(this, &OldTreeItem::OnOpenClose))
		.Help(IsOpenItem() ? t_("Close child items") : t_("Open child items"));
}

Bar::Item& OldTreeItem::BarOpenSubtree(Bar& bar)
{
	return bar.Add(enabled && IsSubtree(),
		t_("Open all"), callback(this, &OldTreeItem::OnOpenSubtree))
		.Help(t_("Open the whole subtree"));
}

Bar::Item& OldTreeItem::BarCloseSubtree(Bar& bar)
{
	return bar.Add(enabled && IsSubtree(),
		t_("Close all"), callback(this, &OldTreeItem::OnCloseSubtree))
		.Help(t_("Close the whole subtree"));
}

void OldTreeItem::OnOpenClose()
{
	if(enabled && IsSubtree()) {
		Open(!IsOpenItem());
		TouchLayout();
		LayoutItem();
		ScrollInto();
	}
}

void OldTreeItem::OnOpenSubtreeRaw()
{
	if(enabled) {
		Open();
		TouchLayout();
		for(int i = 0; i < GetCount(); i++)
			children[i].OnOpenSubtree();
	}
}

void OldTreeItem::OnOpenSubtree()
{
	OnOpenSubtreeRaw();
	ScrollInto();
}

void OldTreeItem::OnCloseSubtree()
{
	for(int i = 0; i < GetCount(); i++)
		children[i].OnCloseSubtree();
	Close();
	TouchLayout();
}

void OldTreeItem::ScrollInto()
{
	if(this && owner/* && owner->IsOpen()*/) {
		owner->Layout();
		Rect rc = owner->GetOffset() + GetItemRect();
		rc.Inflate(4);
		owner->ScrollInto(rc);
	}
}

//////////////////////////////////////////////////////////////////////
// OldTreeEdit::

OldTreeEdit& OldTreeEdit::Perform()
{
	if(!cursor)
		return *this;
	item.TouchLayout();
	OldTreeItem *pos;
	switch(op & ~DEEP) {
	case INS:
		if(pos = cursor.GetParent()) {
			int index = minmax(cursor.cursor.Top(), 0, pos->GetCount());
			if(op & DEEP)
				pos->Insert(index) <<= item;
			else
				pos->Insert(index) = item;
			pos->TouchLayout();
		}
		else
			NEVER();
		break;
	case INS | KIDS:
		if(pos = cursor.Get()) {
			if(!item.IsEmpty()) {
				if(!pos->IsEmpty()) {
					int count = pos->GetCount();
					pos->children.SetCount(count + item.GetCount());
					OldTreeItem::List::Iterator d = pos->children.GetIter(count);
					for(OldTreeItem::List::Iterator p = item.children.Begin(), e = item.children.End(); p != e;)
						if(op & DEEP)
							*d++ <<= *p++;
						else
							*d++ = *p++;
				}
				else {
					if(op & DEEP)
						pos->children <<= item.children;
					else
						pos->children = item.children;
				}
				pos->TouchLayout();
			}
		}
		else
			NEVER();
		break;
	case UPD:
		if(pos = cursor.Get()) {
			if(op & DEEP)
				*pos <<= item;
			else
				*pos = item;
			pos->TouchLayout();
		}
		else
			NEVER();
		break;
	case UPD | KIDS:
		if(pos = cursor.Get()) {
			if(op & DEEP)
				pos->children <<= item.children;
			else
				pos->children = item.children;
			pos->TouchLayout();
		}
		else
			NEVER();
		break;
	case DEL:
		if(pos = cursor.GetParent()) {
			int index = cursor.cursor.Top();
			if(index >= 0 && index < pos->GetCount()) {
				pos->children.Remove(index);
				pos->TouchLayout();
				cursor.Adjust();
			}
			else
				NEVER();
		}
		else
			NEVER();
		break;
	case DEL | KIDS:
		if(pos = cursor.Get()) {
			pos->Clear();
			pos->TouchLayout();
		}
		else
			NEVER();
		break;
	}
	return *this;
}

OldTreeEdit& OldTreeEdit::Adjust(OldTreeCursor& pos)
{
	switch(op & ~DEEP) {
	case INS:        pos.AdjustAfter(cursor, 1); break;
	case INS | KIDS: break;
	case UPD:        break;
	case UPD | KIDS: pos.Adjust(); break;
	case DEL:
		if(pos.IsSubtreeOf(cursor))
			pos <<= cursor;
		pos.AdjustAfter(cursor, -1);
		break;

	case DEL | KIDS:
		if(pos.IsSubtreeOf(cursor))
			pos <<= cursor;
		pos.Adjust();
		break;
	}
	return *this;
}

OldTreeEdit& OldTreeEdit::Adjust(const Vector<OldTreeCursor *>& list)
{
	for(OldTreeCursor *const *p = list.Begin(), *const *e = list.End(); p < e; p++)
		Adjust(**p);
	return *this;
}

//////////////////////////////////////////////////////////////////////
// OldTreeCtrl::

OldTreeCtrl::OldTreeCtrl()
: updating(false)
, font(StdFont())
, offset(3 + BUTTON_HALF, 3)
, old_scroll(Null)
, open_menu(true)
, hide_top(false)
, hittest(0)
, multiselect(false)
{
//	WheelScroll(scroll);
	drag_state = 0;

	owner = this;
	AddFrame(scroll);
	scroll.AutoHide();
//	scroll/*.AutoHide()*/.AddTo(*this);
	scroll.WhenScroll = THISBACK(OnScroll);

	WhenOpenClose  = THISBACK(OnOpenClose);
	WhenPush       = THISBACK(OnPush);
	WhenRight      = THISBACK(OnRight);
	WhenKey        = THISBACK(OnKey);
	WhenBar        = THISBACK(StdBar);
	WhenLeftDouble = THISBACK(OnOpenClose);

	SetFrame(InsetFrame());
}

OldTreeCtrl::~OldTreeCtrl()
{
}

Value OldTreeCtrl::GetData() const
{
	const OldTreeItem *item = GetCursorItem();
	if(item)
		return item->GetData();
	return Value();
}

void OldTreeCtrl::SetData(const Value& value)
{
	SetCursor(FindValue(value));
}

OldTreeItem::HITTEST OldTreeCtrl::Find(Point pt, const OldTreeItem *& item) const
{
	return FindItem(pt - offset + old_scroll, (OldTreeItem*&)item);
}

OldTreeItem::HITTEST OldTreeCtrl::Find(Point pt, OldTreeItem *& item)
{
	return FindItem(pt - offset + old_scroll, item);
}

OldTreeCursor OldTreeCtrl::Find(int y) const
{
	OldTreeItem *item;
	y += old_scroll.y - offset.cy;
	HITTEST ht = FindItem(Point(0, y), item);
	if(ht != HNIL && item)
		return OldTreeCursor(*item);
	OldTreeCursor c(*this);
	if(y >= tree_size.cy)
		c.EndAll();
	return c;
}

OldTreeCtrl& OldTreeCtrl::HideTop()
{
	hide_top = true;
	Indent(4);
	Open();
	EnableItem();
	return *this;
}

void OldTreeCtrl::Layout()
{
//	if(!IsOpen())
//		return;
/*
	if(IsDesignMode() && IsEmpty()) {
		Text("OldTreeCtrl", CtrlImg::new_doc()).Open();
		Add().Text("Child1", CtrlImg::save());
		OldTreeItem& item = Add().Text("Child2", CtrlImg::save_as()).Open();
		item.Add().Text("Child3", CtrlImg::open());
	}
*/
	LayoutItem();
	old_scroll = scroll;
	rowht = tree_size.cy / max(GetOpenCount(), 1);
	Size total = GetTreeSize() + 2 * offset;
	scroll.Set(old_scroll, scroll.GetReducedViewSize(), total);
	scroll.SetLine((scroll.GetReducedViewSize().cx >> 3) + 1, rowht);
}

void OldTreeCtrl::Paint(Draw& draw)
{
	if(layout) {
		Layout();
		Refresh();
	}
	Size size = GetSize();
	Rect rc = GetClientTreeRect();
	Rect rc_butt = rc;
	rc_butt.left += BUTTON_HALF + 1 + ICONGAP;
	Color bg = (IsShowEnabled() ? SWhite : SLtGray);
	DrawRectMinusRect(draw, GetSize(), rc_butt, DBG_PAINT(bg, LtGreen));
	PaintItem(draw, rc.TopLeft());
}

void OldTreeCtrl::OnScroll()
{
	if((Point)scroll != old_scroll) {
		Size delta = old_scroll - (Point)scroll;
		Size view = GetSize();
		old_scroll = scroll;
		if(IsNull(old_scroll.x) || 2 * tabs(delta.cx) >= view.cx || 2 * tabs(delta.cy) >= view.cy || IsFullRefresh())
			Refresh();
		else
			ScrollView(delta);
	}
}

void OldTreeCtrl::LeftDown(Point pt, dword keyflags)
{
	SetFocus();
	SetCapture();
	OldTreeItem *item;
	hittest = Find(pt, item);

	if(multiselect && hittest == HNIL && !(keyflags & (K_CTRL | K_SHIFT)))
		UnselectTree();
	else if(item && item->IsEnabledItem()) {
		OldTreeCursor new_cursor(*item);

		switch(hittest) {
		case HBUTTON:
			item->Open(!item->IsOpenItem());
			item->TouchLayout();
			break;

		case HNIL:
			break;

		default:
			if(multiselect && (keyflags & K_CTRL))
				PickCursor(new_cursor, XOR_SEL);
			else if(multiselect && (keyflags & K_SHIFT) && anchor) // anchor selection
				PickCursor(new_cursor, DRAG_SEL);
			else if(!item->IsSelected())
				PickCursor(new_cursor, SET_SEL);
			else
				PickCursor(new_cursor, KEEP_SEL);
			break;
		}
	}

	drag_start = pt;
	drag_state = 2;

	WhenPush();
}

void OldTreeCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
	if(drag_state == 1)
		WhenDrag(pt + GetScreenView().TopLeft(), keyflags, 3);
	else {
	}
//	OldTreeItem *item;
//	hittest = Find(pt, item);
//	SetCursor(item);
	Action();
	drag_state = 0;
}

void OldTreeCtrl::LeftDouble(Point pt, dword keyflags)
{
	IgnoreMouseUp();
	OldTreeItem *item;
	int hit = Find(pt, item);
	if(item == GetCursor().Get() && hit == hittest)
		WhenLeftDouble();
}

void OldTreeCtrl::RightDown(Point pt, dword keyflags)
{
	if(drag_state == 1) {
		WhenDrag(pt + GetScreenView().TopLeft(), keyflags, 2);
		drag_state = 0;
	}
	SetFocus();
	OldTreeItem *item;
	hittest = Find(pt, item);
	SetCursor(item);
	Action();
	WhenRight();
}

void OldTreeCtrl::MouseMove(Point pt, dword keyflags)
{
	Point scrn = pt + GetScreenView().TopLeft();
	if(drag_state == 2 && abs2(pt - drag_start) >= 50) { // start d&d
		WhenDrag(scrn, keyflags, 0);
		drag_state = 1;
	}
	if(drag_state == 1)
		WhenDrag(scrn, keyflags, 1);
}

void OldTreeCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	scroll.WheelY(zdelta);
}

Image OldTreeCtrl::CursorImage(Point pt, dword keyflags)
{
	Image image = Image::Arrow();
	if(drag_state == 1)
		WhenDragImage(image);
	return image;
}

//////////////////////////////////////////////////////////////////////
// default handlers

void OldTreeCtrl::OnOpenClose()
{
	const OldTreeCursor& cursor = GetCursor();
	if(cursor) {
		cursor->Open(!cursor->IsOpenItem());
		cursor.TouchLayout();
	}
}

void OldTreeCtrl::OnPush()
{
}

void OldTreeCtrl::OnRight()
{
	MenuBar menu;
	WhenBar(menu);
	menu.Execute();
}

void OldTreeCtrl::StdBar(Bar& bar)
{
	const OldTreeCursor& cursor = GetCursor();
	if(cursor && cursor->IsSubtree()) {
		cursor->BarOpenClose(bar);
		cursor->BarOpenSubtree(bar);
		cursor->BarCloseSubtree(bar);
	}
}

bool OldTreeCtrl::Key(dword key, int repcnt)
{
	if(WhenKey(key))
		return true;
	if(key == K_INSERT)
		if(OldTreeItem *item = GetCursorItem()) {
			item->OnOpenClose();
			return true;
		}

	return false;
}

bool OldTreeCtrl::OnKey(dword key)
{
	OldTreeCursor move = cursor.Copy();
	Rect rc(0, 0, 0, 0);
	const OldTreeItem *item = move.Get();
	if(item)
		rc = item->GetClientRect();

	int ops = SET_SEL;
	if(key & K_CTRL)
		ops = XOR_SEL;
	else if(key & K_SHIFT)
		ops = DRAG_SEL;

	switch(key & ~(K_SHIFT | K_CTRL)) {
	default:
		return Bar::Scan(WhenBar, key);

	case K_UP:
		if(move)
			move.Up(OldTreeCursor::HIERARCHY);
		else
			move = OldTreeCursor(*this);
		break;

	case K_DOWN:
		if(move)
			move.Down(OldTreeCursor::HIERARCHY);
		else
			move = OldTreeCursor(*this);
		break;

	case K_PAGEUP:
		move = Find(rc.bottom - GetSize().cy);
		break;

	case K_PAGEDOWN:
		move = Find(rc.top + GetSize().cy);
		break;

	case K_CTRL_PAGEUP:
	case K_HOME:
		move = OldTreeCursor(*this);
		break;

	case K_CTRL_PAGEDOWN:
	case K_END:
		move = OldTreeCursor(*this).EndAll();
		break;
	}

	if(move.IsEnd())
		move.Up(OldTreeCursor::HIERARCHY);
	if(move.IsBegin() && move && hide_top)
		move.Down(OldTreeCursor::HIERARCHY);
	if(move != cursor) {
		PickCursor(move, ops);
		Action();
	}
	return true;
}

OldTreeCtrl& OldTreeCtrl::PickCursor(pick_ OldTreeCursor& nc, int selection_op)
{
	OldTreeCursor new_cursor = nc;
	new_cursor.Adjust();
	cursor.Touch();
	if(selection_op == DRAG_SEL && anchor) {
		UnselectTree();
		bool anchorup = anchor >= new_cursor;
		const OldTreeCursor& b = (anchorup ? new_cursor : anchor);
		const OldTreeCursor& l = (anchorup ? anchor : new_cursor);
		for(OldTreeCursor p(b, 0); p <= l; p.Down(p.HIERARCHY))
			if(p->IsEnabledItem())
				p->Select();
	}
	else if(selection_op != KEEP_SEL) {
		anchor <<= new_cursor;
		if(selection_op == SET_SEL)
			UnselectTree();
		if(new_cursor)
			new_cursor->Select(selection_op == SET_SEL ? true : !new_cursor->IsSelected());
	}
	cursor = new_cursor;
	cursor.Touch().Open().ScrollInto();
	WhenSetCursor();
	return *this;
}

OldTreeCtrl& OldTreeCtrl::SetCursor(const OldTreeItem *item)
{
	if(!item)
		return PickCursor(OldTreeCursor());
	ASSERT(item->owner == this);
	return PickCursor(OldTreeCursor(*item));
}

void OldTreeCtrl::GotFocus()
{
	Refresh();
}

void OldTreeCtrl::LostFocus()
{
	Refresh();
}

Point OldTreeCtrl::GetCursorSc() const
{
	Point pt = scroll;
	OldTreeItem *i = GetCursorItem();
	if(i)
		pt -= i->GetTreeRect().TopLeft();
	return pt;
}

void OldTreeCtrl::ScCursor(Point pt)
{
	OldTreeItem *i = GetCursorItem();
	if(i)
		pt += i->GetTreeRect().TopLeft();
	scroll = pt;
}

END_UPP_NAMESPACE
