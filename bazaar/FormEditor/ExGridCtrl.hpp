#ifndef EX_GRID_CTRL_HPP
#define EX_GRID_CTRL_HPP

#include <GridCtrl/GridCtrl.h>
using namespace Upp;

class ExGridCtrl : public GridCtrl
{
	typedef ExGridCtrl CLASSNAME;

public:
	ExGridCtrl();
	virtual void Paint(Draw &w);
	void SetInfo(const String& info, Font font = StdFont(), Color ink = Black());

	virtual bool Key(dword key, int count);
	virtual void LeftDown(Point p, dword keyflags);

	void NotUseKeys() { _UseKeys = false; }
	void OnLeftClick();
	void SetInfoOffset(int offset) { _Offset = offset; Refresh(); }

private:
	String _Info;
	Color _Ink;
	Font _Font;
	bool _UseKeys;
	int _X;
	int _Y;
	int _Offset;
};

#endif // EX_GRID_CTRL_HPP
