#ifndef _Controls4U_SplitterButton_h_
#define _Controls4U_SplitterButton_h_


class SplitterButton : public StaticRect {
public:
	typedef SplitterButton CLASSNAME;

	SplitterButton();
	virtual ~SplitterButton() {};
	
	SplitterButton& Horz(Ctrl &left, Ctrl &right);
	SplitterButton& Vert(Ctrl& top, Ctrl& bottom);
	
	SplitterButton &SetPositions(Vector<int> &_positions);
	SplitterButton &SetPositions(int pos1);
	SplitterButton &SetPositions(int pos1, int pos2);
	SplitterButton &SetPositions(int pos1, int pos2, int pos3);
	
	SplitterButton &SetInitialPositionId(int id);
	
	SplitterButton &SetButtonNumber(int _buttonNumber) {
		ASSERT(_buttonNumber > 0 && _buttonNumber <= 2);
		buttonNumber = _buttonNumber;
		button2.Show(buttonNumber > 1);
		return *this;
	}
		
private:
	bool movingRight;		
	int buttonWidth;
	Vector<int> positions;
	int positionId;
	int buttonNumber;
	
	struct SplitterLay : Splitter {
		Callback1<int> WhenLayout;
		
		virtual void Layout() {
			Splitter::Layout();
			WhenLayout(PosToClient(GetPos()));
		}
	};
	
	SplitterLay splitter;
	Button button1, button2;
	
	void OnLayout(int pos);
	void OnButton(int id);
	void SetArrows();
};


#endif
