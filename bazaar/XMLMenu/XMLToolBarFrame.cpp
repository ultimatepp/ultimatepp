#include "XMLToolBarFrame.h"
#include "WithXMLMenu.h"

NAMESPACE_UPP

// recalculates relative toolbar's positions
// needed after adding or removing a toolbar
void XMLToolBarFrame::Reposition(void)
{
	// clears position mapper
	posMapper.Clear();
	
	// setup a dummy frame size, just in case frame is empty
	frameSize = 3;
	
	// if no toolbars inside, just do nothing else
	if(!toolBars.GetCount())
		return;
	
	// fixup 'vertical' positions, i.e. rows on which toolbars belong
	Array<int>vPos;
	for(int iPos = 0; iPos < relativePositions.GetCount(); iPos++)
	{
		Size const &sz = relativePositions[iPos];
		vPos.Add(sz.cy);
	}
	Sort(vPos);

	VectorMap<int, int>vMap;
	int v = 0;
	for(int iPos = 0; iPos < vPos.GetCount(); iPos++)
	{
		if(vMap.Find(vPos[iPos]) < 0)
		{
			vMap.Add(vPos[iPos], v);
			v ++;
		}
	}
	for(int iPos = 0; iPos < relativePositions.GetCount(); iPos++)
	{
		Size &sz = relativePositions[iPos];
		sz.cy = vMap.Get(sz.cy);
	}
	
	// now, scans each row, fixing up positions inside it and
	// building true toolbars positions
	int curVPos = 0;
	for(int iRow = 0; iRow < vMap.GetCount(); iRow++)
	{
		Vector<int>ps, idx;
		int rowHeight = 0;
		for(int iPos = 0; iPos < relativePositions.GetCount(); iPos++)
		{
			Size &sz = relativePositions[iPos];
			if(sz.cy == iRow)
			{
				ps.Add(sz.cx);
				idx.Add(iPos);
			}
		}
		IndexSort(ps, idx);
		int minNextPos = 0;
		for(int i = 0; i < ps.GetCount(); i++)
		{
			if(ps[i] < minNextPos)
				ps[i] = minNextPos;
			XMLToolBarCtrl &tb = *toolBars[idx[i]];
			Size sz;
			switch(toolBarState)
			{
				case TOOLBAR_LEFT :
				case TOOLBAR_RIGHT :
					sz = tb.GetVertSize();
					rowHeight = max(rowHeight, sz.cx);
					minNextPos = ps[i] + sz.cy + 2; // 2 pixels gap between columns
					break;

				case TOOLBAR_TOP :
				case TOOLBAR_BOTTOM :
					sz = tb.GetHorzSize();
					rowHeight = max(rowHeight, sz.cy);
					minNextPos = ps[i] + sz.cx + 2; // 2 pixels gap between columns
					break;

				default:
					NEVER();
			}
		}
		VectorMap<int, int> &rowMap = posMapper.Add(curVPos);
		for(int i = 0; i < ps.GetCount(); i++)
			rowMap.Add(ps[i], idx[i]);
		curVPos += rowHeight;
	}
	
	// store total frame size
	frameSize = curVPos;

	// if reversed direction, fixup cols
	if(toolBarState == TOOLBAR_RIGHT || toolBarState == TOOLBAR_BOTTOM)
	{
		int lastPos = posMapper.GetKey(posMapper.GetCount()-1);
		for(int iRow = 0; iRow < posMapper.GetCount(); iRow++)
			posMapper.SetKey(iRow, lastPos - posMapper.GetKey(iRow));
	}
}

void XMLToolBarFrame::FrameLayout(Rect& r)
{
	// stores frame rectangle -- it's needed to layout toolbars
	frameRect = r;
	Reposition();

	switch(toolBarState)
	{
		case TOOLBAR_LEFT:
			r.left += frameSize + 4;
			frameRect.right = r.left;
			frameP11 = Point(frameRect.left, frameRect.top);
			frameP12 = Point(frameRect.left, frameRect.bottom);
			frameP21 = Point(frameRect.right-1, frameRect.top);
			frameP22 = Point(frameRect.right-1, frameRect.bottom);
			frameRect.left += 2;
			frameRect.right -= 2;
			break;
			
		case TOOLBAR_RIGHT:
			r.right -= frameSize + 4;
			frameRect.left = r.right;
			frameP11 = Point(frameRect.left, frameRect.top);
			frameP12 = Point(frameRect.left, frameRect.bottom);
			frameP21 = Point(frameRect.right-1, frameRect.top);
			frameP22 = Point(frameRect.right-1, frameRect.bottom);
			frameRect.left += 2;
			frameRect.right -= 2;
			break;

		case TOOLBAR_TOP:
			r.top += frameSize + 4;
			frameRect.bottom = r.top;
			frameP11 = Point(frameRect.left, frameRect.top);
			frameP12 = Point(frameRect.right, frameRect.top);
			frameP21 = Point(frameRect.left, frameRect.bottom-1);
			frameP22 = Point(frameRect.right, frameRect.bottom-1);
			frameRect.top += 2;
			frameRect.bottom -= 2;
			break;

		case TOOLBAR_BOTTOM:
			r.bottom -= frameSize + 4;
			frameRect.top = r.bottom;
			frameP11 = Point(frameRect.left, frameRect.top);
			frameP12 = Point(frameRect.right, frameRect.top);
			frameP21 = Point(frameRect.left, frameRect.bottom-1);
			frameP22 = Point(frameRect.right, frameRect.bottom-1);
			frameRect.top += 2;
			frameRect.bottom -= 2;
			break;
			
		default:
			NEVER();
	}
	
	// fixup control inside frame
	toolBarContainer.SetFrameRect(
		frameRect.left,
		frameRect.top,
		frameRect.GetSize().cx,
		frameRect.GetSize().cy
	);
	
	// layouts toolbars inside frame
	Layout();
}

void XMLToolBarFrame::FrameAddSize(Size& sz)
{
	Reposition();
	switch(toolBarState)
	{
		case TOOLBAR_LEFT:
		case TOOLBAR_RIGHT:
			sz.cx += frameSize + 4;
			break;
			
		case TOOLBAR_TOP:
		case TOOLBAR_BOTTOM:
			sz.cy += frameSize + 4;
			break;
			
		default:
			NEVER();
			break;
	}
}

// frame painting
void XMLToolBarFrame::FramePaint(Draw& w, const Rect& r)
{
//	w.DrawRect(r, Red());
	w.DrawRect(r, SColorLtFace());
//	w.DrawLine(frameP11, frameP12, 0, SColorLight());
//	w.DrawLine(frameP21, frameP22, 0, SColorLtFace());

	// if predocking, paint the docking toolBar placeholder
/*
	if(preDocking)
		w.DrawRect(preDockRect, Yellow());
*/
}

// lays toolbars inside frame
void XMLToolBarFrame::Layout(void)
{
	// don't layout if still no parent
	if(!parent)
		return;
	for(int iRow = 0; iRow < posMapper.GetCount(); iRow++)
	{
		int rowPos = posMapper.GetKey(iRow);
		VectorMap<int, int> &rowMapper = posMapper[iRow];
		for(int iCol = 0; iCol < rowMapper.GetCount(); iCol++)
		{
			int colPos = rowMapper.GetKey(iCol);
			int idx = rowMapper[iCol];
			XMLToolBarCtrl &tb = *toolBars[idx];
			Ctrl::LogPos pos;
			Size sz;
			int x1, y1, x2, y2;
			switch(toolBarState)
			{
				case TOOLBAR_LEFT :
				case TOOLBAR_RIGHT :
					sz = tb.GetVertSize();
					x1 = rowPos;
					y1 = colPos;
					break;
					
				case TOOLBAR_TOP :
				case TOOLBAR_BOTTOM :
					sz = tb.GetHorzSize();
					x1 = colPos;
					y1 = rowPos;
					break;
					
				default : 
					NEVER();
					break;
			}
			x2 = x1 + sz.cx;
			y2 = y1 + sz.cy;

			// don't add last toolbar as a child if predocking
			if(!preDocking || &tb != toolBars.Top())
			{
				pos = Ctrl::LogPos(
					Ctrl::PosLeft(x1, x2 - x1),
					Ctrl::PosTop(y1, y2 - y1)
				);
				tb.SetPos(pos);
			}
			if(preDocking && &tb == toolBars.Top())
				preDockRect = Rect(x1, y1, x2, y2);
		}
	}
}

// frame insertion/removing handlers
void XMLToolBarFrame::FrameAdd(Ctrl &_parent)
{
	parent = &_parent;
	parent->AddChild(&toolBarContainer);
	Layout();
}

void XMLToolBarFrame::FrameRemove(void)
{
	toolBars.Clear();
	relativePositions.Clear();
	parent->RemoveChild(&toolBarContainer);
	posMapper.Clear();
	parent = NULL;
}

XMLToolBarFrame::XMLToolBarFrame(XMLToolBarState _toolBarState)
{
	parent = NULL;
	toolBarState = _toolBarState;
	
	preDocking = false;
}

XMLToolBarFrame::~XMLToolBarFrame()
{
}

// find index of a docked XMLToolBarFrame; -1 if not found
int XMLToolBarFrame::FindIndex(XMLToolBarCtrl &tb)
{
	int n = toolBars.GetCount();

	// skip last toolbar if predocking, it's there
	// just as a placeholder
	if(preDocking)
		n--;
	
	for(int i = 0; i < n; i++)
		if(&tb == toolBars[i])
			return i;
	return -1;
}

// docks a toolbar into this frame
// internal function -- called by XMLToolBar one
XMLToolBarFrame &XMLToolBarFrame::Dock(XMLToolBarCtrl &tb, Point p)
{
	// if already docked here, just do nothing
	if(FindIndex(tb) >= 0)
		return *this;
	
	// undocks/unfloats from previous position asking owning frame, if any, to do it
	if(tb.toolBarFrame)
		tb.toolBarFrame->Undock(tb);
	
	// dock here the frame
	toolBars.Add(&tb);
	toolBarContainer.AddChild(&tb);
	relativePositions.Add(p);
	Reposition();
	Layout();
	tb.toolBarPos = Point(relativePositions.Top().cx, relativePositions.Top().cy);
	return *this;
}

bool XMLToolBarFrame::GetDockTarget(XMLToolBarCtrl &tb, Point p, int &dockLine, bool &insert, int &col)
{
	// nothing if point outside frame
	if(!toolBarContainer.GetScreenRect().Contains(p))
		return false;
	p -= toolBarContainer.GetScreenRect().TopLeft();
	Size sz = toolBarContainer.GetRect().GetSize();
	
	// ok, it's inside the frame, we must now search for
	// row, column and if we must dock on existing row or
	// inserting a new one
	int framePos;
	switch(toolBarState)
	{
		case TOOLBAR_LEFT :
			framePos = p.x;
			col = p.y;
			break;
			
		case TOOLBAR_RIGHT :
			framePos = sz.cx - p.x;
			col = p.y;
			break;
			
		case TOOLBAR_TOP :
			framePos = p.y;
			col = p.x;
			break;
			
		case TOOLBAR_BOTTOM :
			framePos = sz.cy - p.y;
			col = p.x;
			break;
			
		default :
			NEVER();
			return false; 
	}

	int numLines = posMapper.GetCount();
	if(!numLines)
	{
		dockLine = 0;
		insert = false;
		return true;
	}
	ASSERT(numLines);
	int lineHeight = frameSize / numLines;
	ASSERT(lineHeight);
	dockLine = framePos / lineHeight;
	int rem = framePos - dockLine * lineHeight;
	if(rem <= lineHeight / 4)
		insert = true;
	else if(rem >= 3 * lineHeight / 4)
	{
		dockLine++;
		insert = true;
	}
	else
		insert = false;
	
	return true;
}

XMLToolBarFrame &XMLToolBarFrame::DockAt(XMLToolBarCtrl &tb, Point p)
{
	// should not happen, but....
	if(FindIndex(tb) >= 0)
		return *this;
	
	// get dock position
	int dockLine, col;
	bool insert;
	if(!GetDockTarget(tb, p, dockLine, insert, col))
		return *this;

	// if needed, shift all positions to make place for this toolbar line
	if(insert)
	{
		for(int i = 0; i < relativePositions.GetCount(); i++)
			if(relativePositions[i].cy >= dockLine)
				relativePositions[i].cy++;
	}
		
	// docks the toolbar there
	relativePositions.Add(Size(col, dockLine));
	toolBars.Add(&tb);
	toolBarContainer.AddChild(&tb);
	Reposition();
	Layout();
	tb.toolBarPos = Point(relativePositions.Top().cx, relativePositions.Top().cy);
	return *this;
}

// closes (undocking it) an XMLToolBar from this frame
XMLToolBarFrame &XMLToolBarFrame::Undock(XMLToolBarCtrl &tb)
{
	// if already docked here, just do nothing
	int i = FindIndex(tb);
	if(i < 0)
		return *this;

	// store inside toolbar last docked position
	Size &sz = relativePositions[i];
	toolBars[i]->toolBarPos= Point(sz.cx, sz.cy);
	
	// remove from toolbars and positions list
	toolBars.Remove(i);
	toolBarContainer.RemoveChild(&tb);
	relativePositions.Remove(i);
	
	// signals closed toolbar
	tb.toolBarState = TOOLBAR_CLOSED;
	tb.toolBarFrame = NULL;

	// reposition the frame
	Reposition();
	Layout();
	
	// frees toolbar from pointing here
	return *this;
}

// pre-docking handling -- Point p in SCREEN COORDINATES
XMLToolBarFrame &XMLToolBarFrame::PreDock(XMLToolBarCtrl &tb, Point p)
{
	if(preDocking)
		return *this;

	// get dock position
	int dockLine, col;
	bool insert;
	if(!GetDockTarget(tb, p, dockLine, insert, col))
		return *this;
	preDocking = true;

	// if needed, shift all positions to make place for this toolbar line
	if(insert)
	{
		for(int i = 0; i < relativePositions.GetCount(); i++)
			if(relativePositions[i].cy >= dockLine)
				relativePositions[i].cy++;
	}
		
	// docks the toolbar there
	relativePositions.Add(Size(col, dockLine));
	toolBars.Add(&tb);
	Reposition();
	Layout();
	return *this;
}

XMLToolBarFrame &XMLToolBarFrame::UnPreDock(XMLToolBarCtrl &tb)
{
	if(!preDocking)
		return *this;
	relativePositions.Trim(relativePositions.GetCount() - 1);
	toolBars.Trim(toolBars.GetCount() - 1);
	Reposition();
//	parent->Refresh(preDockRect);
	Layout();
	preDocking = false;
	return *this;
}

// check whether a point (in SCREEN coordinates) is inside the frame
bool XMLToolBarFrame::Contains(Point ps)
{
	if(!this->parent)
		return false;
	ps -= parent->GetScreenRect().TopLeft();
	return frameRect.Contains(ps);
}


END_UPP_NAMESPACE
