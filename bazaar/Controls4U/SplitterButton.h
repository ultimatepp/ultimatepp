#ifndef _Controls4U_SplitterButton_h_
#define _Controls4U_SplitterButton_h_

NAMESPACE_UPP

class SplitterButton : public StaticRect {
public:
	typedef SplitterButton CLASSNAME;

	SplitterButton();
	
	SplitterButton& Horz(Ctrl &left, Ctrl &right);
	SplitterButton& Vert(Ctrl& top, Ctrl& bottom);
	
	SplitterButton &SetPositions(Vector<int> &_positions);
	SplitterButton &SetPositions(int pos1, int pos2);
	SplitterButton &SetPositions(int pos1, int pos2, int pos3);
	
	SplitterButton &SetInitialPositionId(int id);
		
private:
	bool movingRight;		
	int buttonWidth;
	Vector<int> positions;
	int positionId;
	
	struct SplitterLay : Splitter {
		Callback1<int> WhenLayout;
		
		virtual void Layout() {
			Splitter::Layout();
			WhenLayout(PosToClient(GetPos()));
		}
	};
	
	SplitterLay splitter;
	Button button;
	
	void OnLayout(int pos);
	void OnButton();
	void SetArrows();
};

END_UPP_NAMESPACE

#endif
