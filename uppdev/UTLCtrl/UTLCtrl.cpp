#include <CtrlLib/CtrlLib.h>
#include "UTLCtrl.h"
#define IMAGECLASS UTLImag
#define IMAGEFILE <UTLCtrl/UTLCtrl.iml>
//#include <Draw/iml_header.h>
//#include <Draw/iml_source.h>
#include <Draw/iml.h>

UTLCtrl::Node::Node()
{
	Init();
}

UTLCtrl::Node::Node(const Image& img, const Value& v)
{
	Init();
	image = img;
	key = value = v;
}

UTLCtrl::Node::Node(const Image& img, const Value& v, const Value& t)
{
	Init();
	image = img;
	key = v;
	value = t;
}

UTLCtrl::Node::Node(const Value& v)
{
	Init();
	key = value = v;
}

UTLCtrl::Node::Node(const Value& v, const Value& t)
{
	Init();
	key = v;
	value = t;
}



UTLCtrl::UTLCtrl()
{
	levelcx = 16;
	rowHeight = 32;
	colCnt=1;
	colWidth.Add(180);
	colTitle.Add("Tree");
	nocursor = false;
//	Col=1;
	dragPtx=-10;//
	dragCol=-1;//which col is dragging
	isdrag=false;//Is dragging column line
	RMargin=30;//Margin at right side of the table,by Ulti
	dirty = true;
	isselection = false;
	multiselect = false;
	Clear();
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	BackPaint();//Add this to avoid flicker,very useful
}

UTLCtrl::~UTLCtrl() {}
//此处之前均可理解
void   UTLCtrl::Layout()
{
	sb.SetPage(GetSize());
	sb.SetLine(rowHeight);
}

Size   UTLCtrl::Item::GetValueSize() const
{
	if(IsNull(size))
	  return display ? display->GetStdSize(value) : StdDisplay().GetStdSize(value);
	else
	  return size;
}

Size   UTLCtrl::Item::GetSize() const
{
	Size sz = GetValueSize();
	sz += Size(2 * margin, 2 * margin);
	Size isz = image.GetSize();
	sz.cx += isz.cx;
	sz.cy = max(sz.cy, isz.cy);
	return sz;
}

void   UTLCtrl::SetRoot(const UTLCtrl::Node& n)
{
	(UTLCtrl::Node &)item[0] = n;
	RefreshItem(0);
}

void   UTLCtrl::SetRoot(const Image& img, Value v)
{
	SetRoot(UTLCtrl::Node(img, v).CanOpen());
}

void   UTLCtrl::SetRoot(const Image& img, Value v, Value t)
{
	SetRoot(UTLCtrl::Node(img, v, t).CanOpen());
}

int    UTLCtrl::Insert(int parentid, int i, const UTLCtrl::Node& n)//新入节点在父亲的孩子节点中的位置
{
	int id;
	if(freelist >= 0) {   //首先看item数组是否有空闲位置,如有,新插入的Node放在该位置
		id = freelist;
		freelist = item[id].freelink;
	}
	else { //如果没有,就新加一个位置
		id = item.GetCount();
		item.Add();
	}
	Item& m = item[id];
	m.parent = parentid;
	(UTLCtrl::Node&)m = n;
	item[id].subItemText.SetCount(colCnt-1);
	if(parentid >= 0) {
		Item& parent = item[parentid];
		parent.child.Insert(i, id);
	}
	dirty = true;
	Dirty(parentid);
	return id;
}//此routine不难理解

int    UTLCtrl::Add(int parentid, const UTLCtrl::Node& n)
{
	return Insert(parentid, item[parentid].child.GetCount(), n);
}

int    UTLCtrl::Insert(int parentid, int i)
{
	return Insert(parentid, i, UTLCtrl::Node());
}

int    UTLCtrl::Add(int parentid)
{
	return Add(parentid, UTLCtrl::Node());
}

int    UTLCtrl::Insert(int parentid, int i, const Image& img, Value v, bool withopen)
{
	return Insert(parentid, i, UTLCtrl::Node(img, v).CanOpen(withopen));
}

int    UTLCtrl::Insert(int parentid, int i, const Image& img, Value v, Value t, bool withopen)
{
	return Insert(parentid, i, UTLCtrl::Node(img, v, t).CanOpen(withopen));
}

int    UTLCtrl::Add(int parentid, const Image& img, Value v, bool withopen)
{
	return Add(parentid, UTLCtrl::Node(img, v).CanOpen(withopen));
}

int    UTLCtrl::Add(int parentid, const Image& img, Value v, Value t, bool withopen)
{
	return Add(parentid, UTLCtrl::Node(img, v, t).CanOpen(withopen));
}


Value  UTLCtrl::Get(int id) const //Get返回的是KEY
{
	const Item& m = item[id];
	return m.key;
}

Value  UTLCtrl::GetValue(int id) const //GetValue才返回值
{
	const Item& m = item[id];
	return m.value;
}

void  UTLCtrl::Set(int id, Value v) //设置KEY的值
{
	Item& m = item[id];
	m.value = m.key = v;
	RefreshItem(id);
}

void  UTLCtrl::Set(int id, Value k, Value v) //设置KEY和值
{
	Item& m = item[id];
    m.key = k;
    m.value = v;
	RefreshItem(id);
}

void   UTLCtrl::SetNode(int id, const UTLCtrl::Node& n)//设置节点
{
	(UTLCtrl::Node&)item[id] = n;
	Dirty(id);
}

void   UTLCtrl::RemoveChildren(int id)//删除id的孩子节点,不难理解
{
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveSubtree(m.child[i]);
	m.child.Clear();
	Dirty(id);
}

void   UTLCtrl::RemoveSubtree(int id)//删除id极其子节点,未处理其在父节点中的信息
{
	Item& m = item[id];
	if(m.linei == cursor)
		cursor = item[m.parent].linei;//先把光标移到父节点
	m.value = Null;
	m.image = Null;
	for(int i=0;i<colCnt-1;i++)
		m.subItemText[i]="";
	RemoveChildren(id);//删除其孩子节点
	m.freelink = freelist;//该内存位置加到空闲列表
	freelist = id;
}

void   UTLCtrl::Remove(int id)//删除id极其子节点,同时删除其在父节点中的信息
{
	ASSERT(id > 0);
	int pi = item[id].parent;
	Item& parent = item[item[id].parent];
	parent.child.Remove(FindIndex(parent.child, id));//删除其在父亲中的信息
	RemoveSubtree(id);
	Dirty(pi);
}

void   UTLCtrl::Clear()//清除全部,重置为空,加item[0]
{
	item.Clear();
	item.Add();
	item[0].linei = -1;
	item[0].parent = -1;
	freelist = -1;
	Dirty();
	cursor = -1;
}


// revised version by Ulti,used only for fixed height rows treelist
void UTLCtrl::ReLine(int itemi, int level)
{
	int ii = line.GetCount();
	Item& m = item[itemi];
	  Line& l = line.Add();
	  l.level = level;
	  l.itemi = itemi;
      l.ll = -1;

	m.linei = ii;
	level++;

	if(m.isopen)
	{
	     for(int i = 0; i < m.child.GetCount(); i++)
		 {
			line[ii].ll = line.GetCount();
			ReLine(m.child[i], level);
		 }
	}
}

//revised version by Ulti,used only for fixed height rows treelist
void UTLCtrl::SyncTree()//同步树
{
	if(!dirty)//没有更新,不需同步
		return;
	//if(noroot)
		Open(0);
	int cursorid = GetCursor();
	for(int i = 0; i < item.GetCount(); i++)
		item[i].linei = -1;
	line.Clear();//行与项的关联重置
	ReLine(0, -1);
	Size treesize = GetSize();//treesize置0
	if(GetWidth()+RMargin<treesize.cx&&GetWidth()>0)
	{
		float ratio=((float)(treesize.cx-RMargin))/((float)GetWidth());
		for(int n=0;n<colCnt;n++)
			colWidth[n]=(int)colWidth[n]*ratio;
	}
	//treesize.cy=(line.GetCount()+1)*rowHeight;
	//treesize.cx=GetWidth()+rowHeight;
	//sb.SetTotal(treesize);//这里是让滚动条工作的关键
    SetSBSize();
	cursor = -1;
	dirty = false;
	if(cursorid >= 0)
		SetCursor(cursorid, false);
	Refresh();
}

bool UTLCtrl::IsOpen(int id) const//是否打开
{
	return item[id].isopen;
}

void UTLCtrl::Dirty(int id)
{
	ClearSelection();
	Size sz = GetSize();
	dirty = true;
	while(id >= 0) {
		int q = item[id].linei;
		if(q >= 0) {
			int y = q*rowHeight+(rowHeight>>1)- sb.GetY();//revised by Ulti
			Refresh(0, y, sz.cx, sz.cy - y);
			return;
		}
		id = item[id].parent;
	}
	Refresh();
}

void UTLCtrl::Open(int id, bool open)
{
	Item& m = item[id];
	if(m.isopen != open) {
		m.isopen = open;
		int q = GetCursor();
		while(q >= 0) {
			if(q == id) {
				SetCursor(id);
				break;
			}
			q = GetParent(q);
		}
		Dirty(id);
		if(open)
			WhenOpen(id);
		else
			WhenClose(id);
	}
}

void UTLCtrl::OpenDeep(int id, bool open)
{
	Open(id);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		OpenDeep(m.child[i], open);
}

void UTLCtrl::MakeVisible(int id)
{
	id = GetParent(id);
	while(id >= 0) {
		Open(id);
		id = GetParent(id);
	}
}

int UTLCtrl::FindLine(int y) const
{
	int l=y/rowHeight;
	return l> 0 ? l : 0;//这里需要再研究一下
}

int UTLCtrl::FindCol()
{
	int i;
	for(i=1;i<colCnt;i++)
	{
	  if(GetWidth(i)-sb.GetX()>colWidth[0]+(colWidth[i]-4)) break;
	}
	return i==colCnt?0:i;
}
void UTLCtrl::RefreshLine(int i)
{
	SyncTree();
	if(i >= 1&&i<line.GetCount()) {
		Size sz = GetSize();
		int y = i*rowHeight-sb.GetY();//revised by Ulti
		Refresh(0, y, sz.cx,rowHeight);
	}
}

void UTLCtrl::RefreshItem(int id)
{
	SyncTree();
	RefreshLine(item[id].linei);
}

int  UTLCtrl::GetItemAtLine(int i)
{
	SyncTree();
	return line[i].itemi;
}

int  UTLCtrl::GetLineAtItem(int id)
{
	SyncTree();
	return item[id].linei;
}

int  UTLCtrl::GetLineCount()
{
	SyncTree();
	return line.GetCount();
}

void UTLCtrl::SetCursorLine(int i, bool sc)
{
	if(nocursor)
		return;
	if(i != cursor) {
		i = minmax(i, 0, line.GetCount() - 1);
		if(i < 0) return;
		Item& m = item[line[i].itemi];
		if(sc)
			sb.ScrollIntoY(i*rowHeight, rowHeight);//revised by Ulti
		RefreshLine(cursor);
		cursor = i;
		RefreshLine(cursor);
		WhenCursor();
	}
}

void UTLCtrl::SetCursorLine(int i)
{
	SetCursorLine(i, true);
}

void UTLCtrl::SetCursorLineSync(int i)
{
	if(nocursor)
		return;
	if(i != cursor) {
		if(cursor < 0)
			Refresh();
		RefreshLine(cursor);
		if(i < 0)
			cursor = -1;
		else
			cursor = minmax(i,1, line.GetCount() - 1);
		RefreshLine(cursor);
		Item& m = item[line[cursor].itemi];
		if(cursor < 0)
			Refresh();
		else {
			Sync();
			sb.ScrollIntoY(cursor*rowHeight, rowHeight);//revised by Ulti
		}
		SetFocus();
		WhenCursor();
	}
}

void UTLCtrl::KillCursor()
{
	RefreshLine(cursor);
	cursor = -1;
	Refresh();
	WhenCursor();
}

void UTLCtrl::SetCursor(int id, bool sc)
{
	while(id > 0) {
		ASSERT(id >= 0 && id < item.GetCount());
		MakeVisible(id);
		SyncTree();
		const Item& m = item[id];
		if(m.linei >= 0) {
			SetCursorLine(m.linei, sc);
			return;
		}
		id = m.parent;
	}
	SetCursorLine(0, sc);
}

void UTLCtrl::SetCursor(int id)
{
	SetCursor(id, true);
}

int  UTLCtrl::GetCursor() const
{
	return cursor >= 0 ? line[cursor].itemi : -1;
}

Value  UTLCtrl::Get() const
{
	return IsCursor() ? Get(GetCursor()) : Value();
}

Value  UTLCtrl::GetValue() const
{
	return IsCursor() ? GetValue(GetCursor()) : Value();
}

int    UTLCtrl::Find(Value key)
{
	for(int i = 0; i < item.GetCount(); i++)
		if(Get(i) == key)
			return i;
	return -1;
}

bool   UTLCtrl::FindSetCursor(Value key)
{
	int q = Find(key);
	if(q < 0)
		return false;
	SetCursor(q);
	return true;
}

void UTLCtrl::ShiftSelect(int l1, int l2)
{
	if(!multiselect)
		return;
	bool b = false;
	if(l1 > l2)
		Swap(l1, l2);
	for(int i = l1; i <= l2; i++)
		SelectOne(line[i].itemi, true);
	UpdateSelect();
}

void UTLCtrl::LeftDown(Point p, dword flags)
{
	Point org = sb;

    if(p.y<0||p.y>-org.y+(line.GetCount())*rowHeight||p.x>GetActureWidth(colCnt-1))return;
    if(p.y<=rowHeight)
    {
        if(ShowDragArrow(p)) SetCapture();
    }
    else
    {
 	   int i = FindLine(p.y + org.y);
	   const Line& l = line[i];
	   int x = levelcx + l.level * levelcx - (levelcx >> 1);

	   if(p.x > x - 5 && p.x < x + 5)
		   Open(l.itemi, !IsOpen(l.itemi));
	   else {//后面的选择太慢,需要检查
		    SetFocus();
		     int q = cursor;
		     SetCursorLine(i);
		     if(cursor != q)
			 WhenAction();
		     if(multiselect)
		     {
			   if(flags & K_CTRL)
			   {
				 int id = GetCursor();
				 SelectOne(id, !IsSelected(id));
			   }
			   else {
				ClearSelection();
				if(flags & K_SHIFT) {
					ShiftSelect(q, cursor);
					cursor = q;}}
		       }
	         }
    }
}

void UTLCtrl::LeftDouble(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	if(FindLine(p.y + org.y) == GetCursorLine())
		WhenLeftDouble();
}

void UTLCtrl::RightDown(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y < sb.GetTotal().cy) {
		int i = FindLine(p.y + org.y);
		if(i >= 0) {
			SetFocus();
			SetCursorLine(i);
		}
	}
	if(WhenBar)
		MenuBar::Execute(WhenBar);
}

void UTLCtrl::Paint(Draw& w)
{
	SyncTree();
	Size sz = GetSize();
	Point org = sb;
	scroller.Set(org);
	w.DrawRect(sz, SColorPaper);


	w.DrawRect(0,rowHeight-org.y,colWidth[0],(line.GetCount()-1)*rowHeight,SWhiteGray());


	int levelcx2 = levelcx >> 1;
	int halfRow = rowHeight >> 1;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& l = line[i];
		if(l.ll >= 0) {
			int yl = i*rowHeight+halfRow - org.y;
			int yh = l.ll*rowHeight+halfRow - org.y;
			if(yh >= 0 && yl < sz.cy) {
				int x = levelcx + levelcx * l.level + levelcx2;
				w.DrawRect(x, yl, 1, yh - yl, SGreen()/*SBlack()/*SColorShadow*/);
			}
		}
	}//this part done


    int Col=FindCol();

	for(int i = FindLine(org.y+rowHeight); i < line.GetCount(); i++) {
		Line& l = line[i];
		w.DrawRect(0,-org.y+(i+1)*rowHeight,GetActureWidth(colCnt-1),1,SLtGray());//by Ulti,for test
		const Item& m = item[l.itemi];
		const Display *d = m.display;
		if(!d)
			d = &StdDisplay();
		Size msz = m.GetSize();
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize();
		int y = i*rowHeight+halfRow- org.y;//let the tree item is in the middle of a row
		if(y > sz.cy)//do not draw things out of range
			break;
		if(w.IsPainting(0, y-halfRow,sz.cx,rowHeight )) {
			int x = 0;
			x = levelcx + l.level * levelcx;
			Point op = Point(x - levelcx2, y );//draw horiz lines
			w.DrawRect(op.x, op.y, levelcx2, 1,SGreen() /*SBlack()/*SColorShadow*/);
			if(m.canopen || m.child.GetCount()) {
				Image im = m.isopen ? /*CtrlImg::treeminus() : CtrlImg::treeplus()*/UTLImag::treeexpend():UTLImag::treecollapse();
				op -= im.GetSize() / 2;
				w.DrawImage(op.x, op.y, im);
			}
			w.DrawImage(x, y - isz.cy / 2, m.image);
			x += isz.cx;
			dword st = 0;
			Color fg = SColorText;
			Color bg = SWhiteGray()/*SColorPaper*/;
			Color tbg=SColorPaper;
			Color tfg = SColorText;
			bool hasfocus = HasFocus();
			if(IsReadOnly())
				st |= Display::READONLY;
			if(m.sel) {
				st |= Display::SELECT;
				fg = hasfocus ? SColorMark() : SColorText;
				tfg = hasfocus ? SColorHighlightText : SColorText;
				//bg = hasfocus ? SColorHighlight : SColorFace;
				tbg = hasfocus ? SColorHighlight : SColorPaper;;
			}
			if(i == cursor && !nocursor) {
				st |= Display::CURSOR;
				fg = hasfocus ? SColorMark() : SColorText;
				tfg = hasfocus ? SColorHighlightText : SColorText;
				//bg = hasfocus ? (isselection ? Blend(SColorLight, SColorHighlight) : SColorHighlight)
				//              : SColorFace;
				tbg = hasfocus?(isselection ? Blend(SColorLight, SColorHighlight) : SColorHighlight)
				              : SColorPaper;
			}
			if(hasfocus) st |= Display::FOCUS;

			w.DrawRect(x, y, vsz.cx + 2 * m.margin, msz.cy, bg);
			d->Paint(w, RectC(x + m.margin, y - vsz.cy / 2, vsz.cx, vsz.cy), m.value,fg, bg, st);


			if(Col!=0)
			{
			  int cellx=colWidth[0];
		      for(int j=Col;j<colCnt;j++)
		     {
		        cellx+=colWidth[j];
		        d->Paint(w,Rect(cellx-colWidth[j]+1,i*rowHeight+1-org.y,cellx-1,(i+1)*rowHeight-1-org.y),
		                 m.subItemText[j-1],tfg,tbg,st);
		     }
			}
		}

	}
	w.DrawRect(0,0,GetActureWidth(colCnt-1),rowHeight,SColorFace);//title row
	w.DrawRect(0,rowHeight,GetActureWidth(colCnt-1),1,SLtGray());//by Ulti,for test
	w.DrawRect(colWidth[0],-org.y,1,line.GetCount()*rowHeight,SLtGray());//just for test
	const Display *dd;
	dd = &StdDisplay();

	dd->Paint(w,Rect(1,0,colWidth[0]-1,rowHeight),colTitle[0],SColorText,SColorFace,0);//first cell in title row
	if(Col!=0)
	{
 	  int lx=colWidth[0];
	  for(int i=FindCol();i<colCnt;i++)
	  {
	    lx+=colWidth[i];
	    w.DrawRect(/*GetWidth(i)-org.x*/lx,-org.y,1,line.GetCount()*rowHeight,SLtGray());//just for test
	    dd->Paint(w,Rect(lx-colWidth[i]+1,0,lx-1,rowHeight),colTitle[i],SColorText,SColorFace,0);
	  }
	}
	w.DrawRect(Rect(GetActureWidth(colCnt-1)+1,0,sz.cx,line.GetCount()*rowHeight-org.y),SColorPaper);
    if(isdrag){w.DrawRect(dragPtx,0,1,sz.cy,SColorHighlight());isdrag=false;}
}

void UTLCtrl::Scroll()
{
	SyncTree();
	scroller.Scroll(*this, sb);
	Refresh();
}

void UTLCtrl::MouseWheel(Point, int zdelta, dword)
{
	sb.WheelY(zdelta);
}

void UTLCtrl::ChildGotFocus()
{
	for(int i = 0; i < line.GetCount(); i++) {
		Item& m = item[line[i].itemi];
	}
}

bool UTLCtrl::Tab(int d)
{
	if(cursor < 0)
		return false;
	Item& m = item[line[cursor].itemi];

	int i = cursor;
	for(int i = cursor + d; i >= 0 && i < line.GetCount(); i += d) {
		Item& m = item[line[i].itemi];
	}
	return false;
}

bool UTLCtrl::Key(dword key, int)
{
	SyncTree();
	Size sz = GetSize();
	int cid = GetCursor();
	bool shift = key & K_SHIFT;
	key &= ~K_SHIFT;
	switch(key) {
	case K_TAB:
		return Tab(1);
	case K_SHIFT_TAB:
		return Tab(-1);
	case K_UP:
		SetCursorLineSync(cursor > 1 ? cursor - 1 : 1);
		break;
	case K_DOWN:
		SetCursorLineSync(cursor >= 1 ? cursor + 1 : line.GetCount() - 1);
		break;
	case K_PAGEDOWN:
		SetCursorLineSync(cursor >= 1 ? FindLine(cursor*rowHeight+1+ sz.cy) : line.GetCount() - 1);
		break;
	case K_PAGEUP:
		SetCursorLineSync(cursor >= 1? FindLine(cursor*rowHeight+1- sz.cy) : 1);
		break;
	case K_LEFT:
		if(cid >= 0)
			Close(cid);
		break;
	case K_RIGHT:
		if(cid >= 0)
			Open(cid);
		break;
	default:
		return false;
	}
	if(cid != cursor)
		if(shift && multiselect)
			ShiftSelect(cid, cursor);
		else
			ClearSelection();
	if(cursor != cid)
		WhenAction();
	return true;
}

void UTLCtrl::GotFocus()
{
	if(dirty)
		return;
	RefreshLine(cursor);
}

void UTLCtrl::LostFocus()
{
	if(dirty)
		return;
	RefreshLine(cursor);
}

void UTLCtrl::ChildRemoved(Ctrl *)
{
	Dirty();
}

void  UTLCtrl::SetData(const Value& data)
{
	FindSetCursor(data);
}

Value UTLCtrl::GetData() const
{
	return Get();
}

Point  UTLCtrl::GetScroll() const
{
	return sb;
}

void UTLCtrl::ScrollTo(Point sc)
{
	sb = sc;
}

void UTLCtrl::ClearSelTree(int id)
{
	Item& m = item[id];
	m.sel = false;
	for(int i = 0; i < m.child.GetCount(); i++)
		ClearSelTree(m.child[i]);
}

void UTLCtrl::ClearSelection()
{
	if(isselection) {
		ClearSelTree(0);
		isselection = false;
		WhenSelection();
		WhenAction();
		Refresh();
	}
}

bool UTLCtrl::UpdateSelTree(int id)
{
	Item& m = item[id];
	if(m.sel)
		return true;
	for(int i = 0; i < m.child.GetCount(); i++)
		if(UpdateSelTree(m.child[i]))
			return true;
	return false;
}

void UTLCtrl::UpdateSelect()
{
	isselection = UpdateSelTree(0);
	WhenSelection();
	WhenAction();
}

void UTLCtrl::SelectOne(int id, bool sel)
{
	if(!multiselect) {
		ClearSelection();
		return;
	}
	if(item[id].canselect)
		item[id].sel = sel;
	UpdateSelect();
	RefreshItem(id);
}

void UTLCtrl::MouseMove(Point p, dword keyflags) {//deal with col drag
  if(!HasCapture())
  {
     for(int i=0;i<colCnt;i++)
     if(p.x>GetActureWidth(i)-2&&p.x<GetActureWidth(i)+2&&p.y>0&&p.y<rowHeight)
     {
       dragPtx=p.x;
       dragCol=i;
       break;
     }
  }
  else
  {
      if(dragCol==-1) return;
      Size sz;
      sz=GetSize();
      if(p.x>sz.cx-10) dragPtx=sz.cx-10;
       else
           if(p.x<GetActureWidth(dragCol)-colWidth[dragCol]+5)
             dragPtx=GetActureWidth(dragCol)-colWidth[dragCol]+5;
             else dragPtx=p.x;
      isdrag=true;
      if(dragCol>0)colWidth[dragCol]+=(dragPtx-GetActureWidth(dragCol));
      if(dragCol==0) colWidth[0]=dragPtx;
  }
  Refresh();
}

void UTLCtrl::LeftUp(Point p, dword keyflags) {//release left button
    SetSBSize();
	Refresh(dragPtx-2,0,4,GetSize().cy);
}
void UTLCtrl::MouseLeave()
{
  Refresh();
}

Image UTLCtrl::CursorImage(Point p, dword keyflags) {//show cursor
    return ShowDragArrow(p)?Image::SizeHorz():Image::Arrow();
}

int UTLCtrl::GetWidth()//get sum of col width
{
	return GetWidth(colCnt-1);
}
int UTLCtrl::GetWidth(int col)//get sum of colwith(col 0~col)
{
	int w=0;
	for(int i=0;i<=col;i++) w+=colWidth[i];
	return w;
}

int UTLCtrl::GetActureWidth(int col)//get col display width(col 0~col,some of cols not displayed)
{
	if(col<0||col>=colCnt) return -10;
	int Col=FindCol();
	int w=colWidth[0];
	if(col==0||col<Col||Col==0) return w;
	for(int i=Col;i<=col;i++) w+=colWidth[i];
	return w;
}

void UTLCtrl::SetColCount(int cnt)//set column count
{
	if(cnt<2||cnt>100){ PromptOK("Range exceed!");return;}

	Size sz=GetSize();
	for(int i=1;i<cnt;i++)
	{
		colWidth.Add((sz.cx-colWidth[0])/(cnt-1)>50?(sz.cx-colWidth[0])/(cnt-1):50);
		colTitle.Add("");
	}
	colCnt=cnt;
}

void UTLCtrl::SetColWidth(int col,int width)//set width of one of col
{
	if(col<0||col>colCnt-1) return;
	colWidth[col]=width>20?width:20;
}

void UTLCtrl::SetTitle(int col,String title)//set column title
{
	if(col<0||col>=colCnt) return;
	colTitle[col]=title;
}

void UTLCtrl::SetSubText(int id,int sub,String txt)//set tree subitem text
{
	if(sub<0||sub>=colCnt-1)return;
	item[id].subItemText[sub]=txt;
}

bool UTLCtrl::ShowDragArrow(Point p)//is that show drag cursor?
{
   return (p.x>dragPtx-2&&p.x<dragPtx+2)?true:false;
}

void UTLCtrl::SetSBSize()//set scroller total size,called when syntree and and when you drag column width
{
	Size size;
    size.cy=(line.GetCount()+1)*rowHeight;

	size.cx=GetWidth()+RMargin;
	sb.SetTotal(size);//这里是让滚动条工作的关键
}

int UTLCtrl::GetLineLevel(int i)
{
	return (i>0&&i<line.GetCount())?line[i].level:-1;
}

int UTLCtrl::GetLevel(int id)
{
	SyncTree();
	return line[item[id].linei].level;
}