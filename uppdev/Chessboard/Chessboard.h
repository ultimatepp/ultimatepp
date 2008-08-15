#ifndef _Chessboard_Chessboard_h
#define _Chessboard_Chessboard_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Chessboard/Chessboard.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS ChessPiece
#define IMAGEFILE <Chessboard/images.iml>

#include <Draw/iml.h>

class Chessboard : public WithChessboardLayout<TopWindow> {
Label piece;
int a, b, D;
Point p;
public:
	typedef Chessboard CLASSNAME;
	Chessboard();
	virtual void  LeftDown(Point pos, dword flags);
	virtual void  MouseMove(Point pos, dword flags);
//	virtual void  LeftDrag(Point pos, dword keyflags);
	virtual void  MouseLeave();
};


#endif

