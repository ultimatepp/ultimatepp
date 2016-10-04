/*
YPuzzle copyright (c)2005 Arlen Albert Keshabian

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, excluding commercial applications. To use, alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#ifndef _YPuzzle_YPuzzle_h
#define _YPuzzle_YPuzzle_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <YPuzzle/YPuzzle.lay>
#include <CtrlCore/lay.h>

#include <vector>
#include <map>

using namespace std;

class YPuzzle;

#define IMAGEFILE <YPuzzle/YPuzzle.iml>
#include <Draw/iml_header.h>

class CYPuzzleCell : public Label
{
public:
	int m_iID;
	int m_iCurID;

	YPuzzle *m_pParent;

	CYPuzzleCell(int, YPuzzle*);
	void Paint(Draw&);
	void LeftDown(Point, dword);
};

class YPuzzle : public TopWindow {
public:

	typedef YPuzzle CLASSNAME;
	YPuzzle();
	~YPuzzle();

	void Paint(Draw&);
	bool Key(dword key, int count);

	void Process(int iIndex);
	void ShuffleVector();
	void ArrangeButtons(vector<int> &viShuffle);
	void BuildMatrix(int, int, int);
	void CheckAndFixParity(vector<int> &viShuffledVector);
	void OnOptions();

protected:
	Label m_StatusBar;

	int m_iCellSize;
	int m_iNumberOfCellsX, m_iNumberOfCellsY;
	int m_iMaxNumberOfCells;
	vector<CYPuzzleCell*> m_Cells;
	vector<int> m_viShuffle;
	int m_iCurEmptyID;
	int m_iNumberOfMoves, m_iBestNumberOfMoves;
	int m_iLanguage;
	String m_sConfigFilePath;
	map<String, int> m_msiScores;

	Button m_Options;
};

#endif
