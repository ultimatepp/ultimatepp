#include "Chessboard.h"


Chessboard::Chessboard()
{
	CtrlLayout(*this, "Window title");
	a=140;
	b=200;
	D=50;
	Add(piece);
	piece.SetImage(ChessPiece::qw()).Transparent();
	piece.LeftPosZ(a, D).TopPosZ(b, D);
}

void Chessboard::LeftDown(Point pos, dword flags) 
{   if (!piece.HasCapture()) piece.SetCapture();
	else piece.ReleaseCapture();      
}

 
void Chessboard::MouseMove(Point pos, dword flags) 
{     
    p = pos;    
 	if (piece.HasCapture()) piece.LeftPosZ(p.x - D/2, D).TopPosZ(p.y - D/2, D);
 	Refresh();
}

void Chessboard::MouseLeave()
{
	//Refresh();
}

GUI_APP_MAIN
{
	Chessboard().Run();
}

