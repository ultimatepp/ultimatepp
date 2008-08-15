#include <CtrlLib/CtrlLib.h>

using namespace Upp;

enum { EMPTY, BLACK, WHITE };
enum { BOARD = 6 };
enum { STALL = -100000 };

struct Game {
	Point black[BOARD];
	Point white[BOARD];
	byte  board[BOARD][BOARD];

	Game Reversed();
	void Move(int figure, Point pos);
	void Update();
	void Paint(Draw& w);
	void Reset();
	int  Evaluate();
	void EvaluateMove(int fig, Point pos, int& best, int& bfig, Point& bpos, int depth);
	int  GetBest(int& fig, Point& pos, int depth);
	void Dump();
};

void Game::Dump()
{
	LOG("-----------------");
	for(int i = 0; i < BOARD; i++) {
		String ln;
		for(int j = 0; j < BOARD; j++)
			switch(board[j][i]) {
			case 0:     ln << '.'; break;
			case BLACK: ln << '#'; break;
			case WHITE: ln << 'O'; break;
			}
		LOG(ln);
	}
}


int  Game::Evaluate()
{
	int q = 0;
	for(int i = 0; i < BOARD; i++) {
		if(black[i].y == BOARD - 1)
			return -1000;
		if(black[i].y >= 0)
			q--;
		if(white[i].y == 0)
			return  1000;
		if(white[i].y >= 0)
			q++;
	}
	return q;
}

void Game::EvaluateMove(int fig, Point pos, int& best, int& bfig, Point& bpos, int depth)
{
	Game g2 = *this;
	g2.Move(fig, pos);
	int q;
	q =	g2.Evaluate();
	if(depth > 0 && q != 1000 && q != -1000) {
		LOGBEGIN();
		LOG(">>>");
		Game op = g2.Reversed();
		Point d;
		int   b;
		b = op.GetBest(b, d, q > best ? depth - 1 : depth - 2);
		if(b != STALL)
			if(b > 100)
				q = -b + 1;
			else
			if(b < -100)
				q = -b - 1;
			else
				q = -b;
		LOGEND();
	}
	if(q > best) {
		best = q;
		bfig = fig;
		bpos = pos;
	}
}

int Game::GetBest(int& fig, Point& pos, int depth)
{
	int best = STALL;
	for(int i = 0; i < BOARD; i++)
		if(white[i].y > 0) {
			Point p = white[i];
			Game game = *this;
			p.y -= 2;
			if(p.y == BOARD - 1 - 2 && game.board[p.x][p.y] == 0)
				EvaluateMove(i, p, best, fig, pos, depth);
			p.y++;
			if(game.board[p.x][p.y] == 0)
				EvaluateMove(i, p, best, fig, pos, depth);
			p.x--;
			if(p.x >= 0 && game.board[p.x][p.y] == BLACK)
				EvaluateMove(i, p, best, fig, pos, depth);
			p.x += 2;
			if(p.x <= BOARD - 1 && game.board[p.x][p.y] == BLACK)
				EvaluateMove(i, p, best, fig, pos, depth);
		}
	if(best == STALL)
		LOG("STALL");
	else {
		LOG("BEST");
		Game game = *this;
		game.Move(fig, pos);
		game.Dump();
	}
	return best;
}

inline Point Reverse(Point p)
{
	return Point(p.x, p.y >= 0 ? BOARD - 1 - p.y : -1);
}

Game Game::Reversed()
{
	Game game;
	for(int i = 0; i < BOARD; i++) {
		game.black[i] = Reverse(white[i]);
		game.white[i] = Reverse(black[i]);
	}
	game.Update();
	return game;
}

void Game::Update()
{
	memset(board, 0, sizeof(board));
	for(int i = 0; i < BOARD; i++) {
		if(black[i].y >= 0)
			board[black[i].x][black[i].y] = BLACK;
		if(white[i].y >= 0)
			board[white[i].x][white[i].y] = WHITE;
	}
}

void Game::Move(int i, Point pos)
{
	if(board[pos.x][pos.y] == BLACK)
		for(int i = 0; i < BOARD; i++)
			if(black[i] == pos) {
				black[i].y = -1;
				break;
			}
	board[white[i].x][white[i].y] = 0;
	white[i] = pos;
	board[pos.x][pos.y] = WHITE;
}

void Game::Reset()
{
	for(int i = 0; i < BOARD; i++) {
		black[i].x = white[i].x = i;
		white[i].y = BOARD - 1;
		black[i].y = 0;
	}
	Update();
}

void Game::Paint(Draw& w)
{
	for(int i = 0; i < BOARD; i++)
		for(int j = 0; j < BOARD; j++) {
			w.DrawRect(i * 32, j * 32, 32, 32, (i ^ j) & 1 ? White : LtGray);
			if(board[i][j]) {
				Rect r = RectC(i * 32 + 8, j * 32 + 8, 16, 16);
				w.DrawRect(r, Black);
				w.DrawRect(r.Deflated(2, 2), board[i][j] == WHITE ? White : Gray);
			}
		}
}

struct Chess : TopWindow {
	Game game;
	int  moveno;

	virtual void Paint(Draw& w) {
		game.Paint(w);
		w.DrawText(0, 0, AsString(moveno + 1));
	}

	void Result()
	{
		int q = game.Evaluate();
		PromptOK(q == 0 ? "Tie" : q > 0 ? "White has won" : "Black has won");
		game.Reset();
		Refresh();
		moveno = 0;
		if(rand() & 1)
			BlackMove();
	}

	void BlackMove()
	{
		int fig;
		Point pos;
		if(game.Evaluate() == 1000 || game.GetBest(fig, pos, 0) == STALL) {
			Result();
			return;
		}
		Game g = game.Reversed();
		if(g.GetBest(fig, pos, moveno++ ? 9 : 0) != STALL) {
			g.Move(fig, pos);
			game = g.Reversed();
			if(game.Evaluate() == -1000 || game.GetBest(fig, pos, 0) == STALL)
				Result();
		}
		else
			Result();
		Refresh();
	}

	void WhiteMove(int fig, Point pos)
	{
		game.Move(fig, pos);
		Refresh();
		Sync();
		BlackMove();
	}

	virtual void LeftDown(Point p, dword flags)
	{
		p /= 32;
		if(p.x >= 0 && p.x <= BOARD && p.y >= 0 && p.y <= BOARD) {
			for(int i = 0; i < BOARD; i++) {
				Point pp = game.white[i];
				if(pp.y > 0) {
					pp.y -= 2;
					if(p.y == BOARD - 1 - 2 && pp == p && game.board[pp.x][pp.y] == 0) {
						WhiteMove(i, p);
						return;
					}
					pp.y++;
					if(pp == p && game.board[pp.x][pp.y] == 0) {
						WhiteMove(i, p);
						return;
					}
					pp.x -= 1;
					if(pp == p && p.x >= 0 && game.board[pp.x][pp.y] == BLACK) {
						WhiteMove(i, p);
						return;
					}
					pp.x += 2;
					if(pp == p && p.x <= BOARD - 1 && game.board[pp.x][pp.y] == BLACK) {
						WhiteMove(i, p);
						return;
					}
				}
			}
		}
		BeepExclamation();
	}

	Chess() {
		SetRect(0, 0, 32 * BOARD, 32 * BOARD);
		game.Reset();
		moveno = 0;
		BlackMove();
	}
};

GUI_APP_MAIN
{
	Chess chess;
	chess.Run();
}
