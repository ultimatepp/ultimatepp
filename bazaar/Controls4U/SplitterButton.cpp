#include <CtrlLib/CtrlLib.h>
#include "SplitterButton.h"

NAMESPACE_UPP

SplitterButton::SplitterButton() {
	Add(splitter.SizePos());
	Add(button.LeftPosZ(80, 10).TopPosZ(30, 40));
	
	splitter.WhenLayout = THISBACK(OnLayout);
	button.WhenAction = THISBACK(OnButton);
	
	movingRight = true;
	buttonWidth = int(2.5*splitter.GetSplitWidth());
	
	splitter.SetPos(5000);
}

SplitterButton& SplitterButton::Horz(Ctrl &left, Ctrl &right) {
	splitter.Horz(left, right);	
	
	SetArrows();
	
	return *this;
}

SplitterButton& SplitterButton::Vert(Ctrl& top, Ctrl& bottom) {
	splitter.Vert(top, bottom);
	
	SetArrows();
	
	return *this;
}

SplitterButton &SplitterButton::SetPositions(Vector<int> &_positions) {
	ASSERT(_positions.GetCount() > 1);
	positions = pick(_positions);
	Sort(positions);
	if (positionId >= positions.GetCount())
		positionId = 0;
	splitter.SetPos( positions[positionId]);
	return *this;
}

SplitterButton &SplitterButton::SetPositions(int pos1, int pos2) {
	Vector<int> positions;
	
	positions << pos1 << pos2;
	SetPositions(positions);
	return *this;
}

SplitterButton &SplitterButton::SetPositions(int pos1, int pos2, int pos3) {
	Vector<int> positions;
	
	positions << pos1 << pos2 << pos3;
	SetPositions(positions);
	return *this;
}

SplitterButton &SplitterButton::SetInitialPositionId(int id) {
	positionId = id;
	splitter.SetPos(positions[positionId]);
	
	SetArrows();
	
	return *this;
}

void SplitterButton::OnLayout(int pos) {
	int cwidth, cheight;
	
	if (splitter.IsVert()) {
		cwidth = GetSize().cy;
		cheight = GetSize().cx;
	} else {
		cwidth = GetSize().cx;
		cheight = GetSize().cy;
	}
	
	int posx = max(0, pos - buttonWidth/2);
	posx = min(cwidth - buttonWidth, posx);
	int posy = (2*cheight)/5;	
	int widthy = cheight/5;
	
	if (splitter.IsVert()) 
		button.SetPos(PosLeft(posy, widthy), PosTop(posx, buttonWidth));
	else
		button.SetPos(PosLeft(posx, buttonWidth), PosTop(posy, widthy));
}

void SplitterButton::OnButton() {
	ASSERT(positions.GetCount() > 1);
	
	int pos = splitter.GetPos();
	
	int closerPositionId = Null;
	int closerPosition = 10000;
	for (int i = 0; i < positions.GetCount(); ++i)
		if (abs(positions[i] - pos) < closerPosition) {
			closerPosition = abs(positions[i] - pos);
			closerPositionId = i;
		}
	
	bool arrowRight;
	if (movingRight) {
		if (closerPositionId == (positions.GetCount() - 1)) {
			movingRight = false;
			positionId = positions.GetCount() - 2;
			if (positionId == 0)
				arrowRight = true;
			else	
				arrowRight = false;
		} else {
			positionId = closerPositionId + 1;
			if (positionId == (positions.GetCount() - 1)) 
				arrowRight = false;
			else
				arrowRight = true;
		}
	} else {
		if (closerPositionId == 0) {
			movingRight = true;
			positionId = 1;
			if (positionId == (positions.GetCount() - 1)) 
				arrowRight = false;
			else	
				arrowRight = true;
		} else {
			positionId = closerPositionId - 1;
			if (positionId == 0) 
				arrowRight = true;
			else
				arrowRight = false;
		}
	}
	splitter.SetPos(positions[positionId]);
	SetArrows();
}

void SplitterButton::SetArrows() {
	bool arrowRight = movingRight;
	if (positionId == (positions.GetCount() - 1))
		arrowRight = false;
	if (positionId == 0)
		arrowRight = true;
	
	if (arrowRight)
		button.SetImage(splitter.IsVert() ? CtrlImg::smalldown() : CtrlImg::smallright());
	else
		button.SetImage(splitter.IsVert() ? CtrlImg::smallup() : CtrlImg::smallleft());
}
		
END_UPP_NAMESPACE