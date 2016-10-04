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

#include "YPuzzle.h"

#define TFILE <YPuzzle/YPuzzle.t>
#include <Core/t.h>

#define IMAGEFILE <YPuzzle/YPuzzle.iml>
#include <Draw/iml_source.h>

#include "TinyXML/tinyxml.h"

template<class T> void YSwap(T &A, T &B)
{
	A ^= B ^= A ^= B;
}

CYPuzzleCell::CYPuzzleCell(int iID, YPuzzle *pParent)
{
	m_iID = iID;
	m_iCurID = m_iID;

	m_pParent = pParent;

	NoIgnoreMouse();
	NoTransparent();

	SetInk(Color(0, 0, 203));

#ifdef PLATFORM_WIN32
	SetFont(Tahoma(18).Bold());
#else
	SetFont(Arial(18).Bold());
#endif

	SetLabel(FormatInt(m_iID + 1));
	SetAlign(ALIGN_CENTER);
	SetVAlign(ALIGN_CENTER);
}

void CYPuzzleCell::LeftDown(Point p, dword keyflags)
{
	m_pParent->Process(m_iID);
}

void CYPuzzleCell::Paint(Draw &draw)
{
	Size l_Size = GetSize();
	draw.DrawImage(0, 0, l_Size.cx, l_Size.cy, backimage);
	PaintLabel(draw, 0, 0, l_Size.cx, l_Size.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());

	//UltimateCPP lacks 'DrawRoundedRect' function.
	//So, the next lines of code roughly emulate it.

	int l_iOffset = 4;
	int l_iLineWidth = 2;
	Color l_Color(0, 0, 203);

	Rect l_Rect = Rect(GetSize());

	draw.DrawLine(l_Rect.left + l_iOffset, l_Rect.top, l_Rect.right - l_iOffset, l_Rect.top, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.right - l_iOffset, l_Rect.top, l_Rect.right, l_Rect.top + l_iOffset, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.right, l_Rect.top + l_iOffset, l_Rect.right, l_Rect.bottom - l_iOffset, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.right, l_Rect.bottom - l_iOffset, l_Rect.right - l_iOffset, l_Rect.bottom, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.right - l_iOffset, l_Rect.bottom, l_Rect.left + l_iOffset, l_Rect.bottom, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.left + l_iOffset, l_Rect.bottom, l_Rect.left, l_Rect.bottom - l_iOffset, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.left, l_Rect.bottom - l_iOffset, l_Rect.left, l_Rect.top + l_iOffset, l_iLineWidth, l_Color);
	draw.DrawLine(l_Rect.left, l_Rect.top + l_iOffset, l_Rect.left + l_iOffset, l_Rect.top, l_iLineWidth, l_Color);
}

YPuzzle::YPuzzle()
{
	m_iLanguage = LNG_ENGLISH;
	m_sConfigFilePath = "YPuzzle.config.xml";

	int l_iX = 4, l_iY = 4, l_iCellSize = 64;

	TiXmlDocument l_XMLDoc(m_sConfigFilePath);
	if(l_XMLDoc.LoadFile())
	{
		TiXmlElement *l_pXMLRoot = l_XMLDoc.FirstChildElement("Configuration");
		if(l_pXMLRoot)
		{
			TiXmlElement *l_pXMLLanguage = l_pXMLRoot->FirstChildElement("Language");
			if(l_pXMLLanguage)
			{
				string l_sLanguage(l_pXMLLanguage->Attribute("id"));
				if(l_sLanguage.length() == 5 && l_sLanguage[2] == '-')
					m_iLanguage = LNG_(l_sLanguage[0], l_sLanguage[1], l_sLanguage[3], l_sLanguage[4]);
			}

			TiXmlElement *l_pXMLDimension = l_pXMLRoot->FirstChildElement("Dimension");
			if(l_pXMLDimension)
			{
				l_pXMLDimension->Attribute("x", &l_iX);
				l_pXMLDimension->Attribute("y", &l_iY);
				l_pXMLDimension->Attribute("cell", &l_iCellSize);

				if(l_iX < 3)
					l_iX = 3;
				if(l_iX > 15)
					l_iX = 15;

				if(l_iY < 3)
					l_iY = 3;
				if(l_iY > 15)
					l_iY = 15;

				if(l_iCellSize < 32)
					l_iCellSize = 32;
				if(l_iCellSize > 64)
					l_iCellSize = 64;
			}

			TiXmlElement *l_pXMLScores = l_pXMLRoot->FirstChildElement("Scores");
			if(l_pXMLScores)
			{
				TiXmlElement *l_XMLPlane = l_pXMLScores->FirstChildElement("Plane");
				while(l_XMLPlane)
				{
					String l_sDimension = l_XMLPlane->Attribute("dimension");
					map<String, int>::iterator l_It = m_msiScores.find(l_sDimension);

					int l_iBestNumberOfMoves = -1;
					l_XMLPlane->Attribute("best", &l_iBestNumberOfMoves);

					if(l_iBestNumberOfMoves < -1)
						l_iBestNumberOfMoves = -1;

					if(l_It != m_msiScores.end())
						(*l_It).second = l_iBestNumberOfMoves;
					else
						m_msiScores.insert(make_pair(l_sDimension, l_iBestNumberOfMoves));

					l_XMLPlane = l_XMLPlane->NextSibling("Plane")->ToElement();
				}
			}
		}
	}

	SetLanguage(m_iLanguage);

	String l_sTitle("YPuzzle ");
	l_sTitle += t_("game");
	Title(l_sTitle);
	Icon(smallicon);
	MinimizeBox(true);

	srand(GetTickCount());

	m_iCellSize = 0;
	m_iNumberOfCellsX = 0;
	m_iNumberOfCellsY = 0;
	m_iMaxNumberOfCells = m_iNumberOfCellsX * m_iNumberOfCellsY;
	m_iCurEmptyID = m_iMaxNumberOfCells - 1;

	String l_sScore = FormatInt(l_iX) + 'x' + FormatInt(l_iY);

	map<String, int>::iterator l_It = m_msiScores.find(l_sScore);
	if(l_It != m_msiScores.end())
		m_iBestNumberOfMoves = (*l_It).second;
	else
		m_iBestNumberOfMoves = -1;

	m_iNumberOfMoves = 0;

	AddChild(&m_StatusBar);
	m_StatusBar.NoTransparent();

	AddChild(&m_Options);
	m_Options.SetLabel("Y");
	m_Options.NoWantFocus();
	m_Options <<= THISBACK(OnOptions);

	BuildMatrix(l_iX, l_iY, l_iCellSize);
}

bool YPuzzle::Key(dword key, int count)
{
	bool l_bRes = TopWindow::Key(key, count);

	if(key == K_SPACE)
	{
		ShuffleVector();
		ArrangeButtons(m_viShuffle);
		return true;
	}

	return l_bRes;
}

void YPuzzle::OnOptions()
{
	WithYPuzzleDimensionLayout<TopWindow> l_OptionsDlg;

	l_OptionsDlg.ok.Ok();
	l_OptionsDlg.cancel.Cancel();
	CtrlLayoutOKCancel(l_OptionsDlg, t_("Options"));

	l_OptionsDlg.applang.SetData(m_iLanguage);
	l_OptionsDlg.Xdim <<= m_iNumberOfCellsX;
	l_OptionsDlg.Ydim <<= m_iNumberOfCellsY;
	l_OptionsDlg.CellSize <<= m_iCellSize;

	int l_iRet = l_OptionsDlg.Run();
	if(l_iRet == IDCANCEL)
		return;

	int l_iNumberOfCellsX = l_OptionsDlg.Xdim.GetData();
	int l_iNumberOfCellsY = l_OptionsDlg.Ydim.GetData();
	int l_iLanguage = l_OptionsDlg.applang.GetData();
	int l_iCellSize = l_OptionsDlg.CellSize.GetData();

	String l_sScore = FormatInt(m_iNumberOfCellsX) + 'x' + FormatInt(m_iNumberOfCellsY);
	map<String, int>::iterator l_It = m_msiScores.find(l_sScore);
	if(l_It != m_msiScores.end())
		(*l_It).second = m_iBestNumberOfMoves;
	else
		m_msiScores.insert(make_pair(l_sScore, m_iBestNumberOfMoves));

	l_sScore = FormatInt(l_iNumberOfCellsX) + 'x' + FormatInt(l_iNumberOfCellsY);
	l_It = m_msiScores.find(l_sScore);
	if(l_It != m_msiScores.end())
		m_iBestNumberOfMoves = (*l_It).second;
	else
		m_iBestNumberOfMoves = -1;

	if(m_iLanguage != l_iLanguage)
	{

		m_iLanguage = l_iLanguage;
		SetLanguage(l_iLanguage);

		String l_sTitle("YPuzzle ");
		l_sTitle += t_("game");
		Title(l_sTitle);

		if(l_iNumberOfCellsX == m_iNumberOfCellsX && l_iNumberOfCellsY == m_iNumberOfCellsY)
		{
			String l_sFormat = t_(" Moves: ");
			l_sFormat += "%d";
			String l_sStr(Format(l_sFormat, m_iNumberOfMoves));
			if(m_iBestNumberOfMoves != -1)
			{
				String l_sStr2(Format(" (%d)", m_iBestNumberOfMoves));
				l_sStr += l_sStr2;
			}

			m_StatusBar.SetLabel(l_sStr);
		}
	}

	BuildMatrix(l_iNumberOfCellsX, l_iNumberOfCellsY, l_iCellSize);
}

void YPuzzle::Paint(Draw &draw)
{
	Rect l_Rect = Rect(GetSize());
	draw.DrawRect(l_Rect, Color(0, 0, 0));
}

YPuzzle::~YPuzzle()
{
	vector<CYPuzzleCell*>::iterator Idx = m_Cells.begin();
	for(; Idx != m_Cells.end(); Idx++)
		delete *Idx;

	String l_sScore = FormatInt(m_iNumberOfCellsX) + 'x' + FormatInt(m_iNumberOfCellsY);
	map<String, int>::iterator l_It = m_msiScores.find(l_sScore);
	if(l_It != m_msiScores.end())
		(*l_It).second = m_iBestNumberOfMoves;
	else
		m_msiScores.insert(make_pair(l_sScore, m_iBestNumberOfMoves));

	TiXmlDeclaration l_XMLDeclaration("1.0", "UTF-8", "yes");
	TiXmlDocument l_XMLDoc(m_sConfigFilePath);
	l_XMLDoc.InsertEndChild(l_XMLDeclaration);

	TiXmlElement l_XMLConfig("Configuration");
	TiXmlElement l_XMLLanguage("Language");
	l_XMLLanguage.SetAttribute("id", LNGAsText(m_iLanguage));
	TiXmlElement l_XMLDimension("Dimension");
	l_XMLDimension.SetAttribute("x", m_iNumberOfCellsX);
	l_XMLDimension.SetAttribute("y", m_iNumberOfCellsY);
	l_XMLDimension.SetAttribute("cell", m_iCellSize);
	TiXmlElement l_XMLScores("Scores");

	TiXmlElement l_XMLScore("Plane");
	l_It = m_msiScores.begin();
	for(; l_It != m_msiScores.end(); l_It++)
	{
		l_XMLScore.SetAttribute("dimension", (*l_It).first);
		l_XMLScore.SetAttribute("best", (*l_It).second);
		l_XMLScores.InsertEndChild(l_XMLScore);
	}

	l_XMLConfig.InsertEndChild(l_XMLLanguage);
	l_XMLConfig.InsertEndChild(l_XMLDimension);
	l_XMLConfig.InsertEndChild(l_XMLScores);
	l_XMLDoc.InsertEndChild(l_XMLConfig);

	l_XMLDoc.SaveFile();
}

GUI_APP_MAIN
{
	YPuzzle().Run();
}

void YPuzzle::ShuffleVector()
{
	m_viShuffle.erase(m_viShuffle.begin(), m_viShuffle.end());
	for(int iIndex = 0; iIndex < m_iMaxNumberOfCells - 1; iIndex++)
		m_viShuffle.push_back(iIndex);

	random_shuffle(m_viShuffle.begin(), m_viShuffle.end());

	m_iCurEmptyID = m_iMaxNumberOfCells - 1;

	CheckAndFixParity(m_viShuffle);
}

void YPuzzle::CheckAndFixParity(vector<int> &viShuffledVector)
{
	int l_iParity = 0;

	for(vector<int>::iterator Idx = viShuffledVector.begin(); Idx != viShuffledVector.end(); Idx++)
	{
		for(vector<int>::iterator Idx1 = Idx + 1; Idx1 != viShuffledVector.end(); Idx1++)
			if((*Idx) > (*Idx1))
				l_iParity++;
	}

	if(l_iParity & 1)
		YSwap(viShuffledVector[0], viShuffledVector[1]);
}

void YPuzzle::BuildMatrix(int CX, int CY, int iCellSize)
{
	if(CX == m_iNumberOfCellsX &&
		CY == m_iNumberOfCellsY &&
		iCellSize == m_iCellSize &&
		m_Cells.size())
		return;

	m_iNumberOfCellsX = CX;
	m_iNumberOfCellsY = CY;
	m_iCellSize = iCellSize;
	m_iMaxNumberOfCells = m_iNumberOfCellsX * m_iNumberOfCellsY;
	m_iCurEmptyID = m_iMaxNumberOfCells - 1;

	int l_iStatusBarHeight = 16;
	Rect l_WindowRect(0, 0, m_iCellSize * m_iNumberOfCellsX, m_iCellSize * m_iNumberOfCellsY + l_iStatusBarHeight);

	Hide();

	Rect l_Desktop = Ctrl::GetWorkArea();
	Rect l_CenteredRect = l_Desktop.CenterRect(min(l_Desktop.Size(), l_WindowRect.Size()));
	SetRect(l_CenteredRect);

	Rect l_ClientRect(l_CenteredRect);
	l_ClientRect.Offset(-l_CenteredRect.left, -l_CenteredRect.top);

	int l_iStatusTop = l_ClientRect.bottom - l_iStatusBarHeight;
	m_StatusBar.SetRect(Rect(0, l_iStatusTop, l_ClientRect.right - l_iStatusBarHeight * 2, l_ClientRect.bottom));
	m_Options.SetRect(Rect(l_ClientRect.right - l_iStatusBarHeight * 2, l_iStatusTop, l_ClientRect.right, l_ClientRect.bottom));

	Show();

	int Idx = 0;

	Rect l_CellRect(0, 0, m_iCellSize, m_iCellSize);
	CYPuzzleCell *l_pCell = NULL;

	if((int)m_Cells.size() < m_iMaxNumberOfCells - 1)
	{
		for(int Idx = (int)m_Cells.size(); Idx < m_iMaxNumberOfCells - 1; Idx++)
		{
			l_pCell = new CYPuzzleCell(Idx, this);

			AddChild(l_pCell);

			m_Cells.push_back(l_pCell);
		}
	}
	else
	{
		for(vector<CYPuzzleCell*>::iterator Idx = m_Cells.begin() + m_iMaxNumberOfCells - 1; Idx != m_Cells.end(); Idx++)
			delete *Idx;

		m_Cells.erase(m_Cells.begin() + m_iMaxNumberOfCells - 1, m_Cells.end());
	}

	ShuffleVector();
	ArrangeButtons(m_viShuffle);
}

void YPuzzle::Process(int iIndex)
{
	int l_iCurDivY = m_Cells[iIndex]->m_iCurID / m_iNumberOfCellsX;
	int l_iCurDivX = m_Cells[iIndex]->m_iCurID % m_iNumberOfCellsX;

	int l_iEmptyDivY = m_iCurEmptyID / m_iNumberOfCellsX;
	int l_iEmptyDivX = m_iCurEmptyID % m_iNumberOfCellsX;

	int l_iDirection = 0;

	if(l_iCurDivX == l_iEmptyDivX)
	{
		if(l_iCurDivY + 1 == l_iEmptyDivY)
			l_iDirection = 3;
		else
			if(l_iCurDivY - 1 == l_iEmptyDivY)
				l_iDirection = 1;
	}
	else
	{
		if(l_iCurDivY == l_iEmptyDivY)
		{
			if(l_iCurDivX + 1 == l_iEmptyDivX)
				l_iDirection = 2;
			else
				if(l_iCurDivX - 1 == l_iEmptyDivX)
					l_iDirection = 4;
		}
	}

	if(l_iDirection)
	{
		Rect l_MoveRect = m_Cells[iIndex]->GetRect();
		if(l_iDirection & 1)
			l_MoveRect.OffsetVert((l_iDirection == 1) ? -m_iCellSize : m_iCellSize);
		else
			l_MoveRect.OffsetHorz((l_iDirection == 2) ? m_iCellSize : -m_iCellSize);

		m_Cells[iIndex]->SetRect(l_MoveRect);

		m_iNumberOfMoves += 1;

		String l_sFormat = t_(" Moves: ");
		l_sFormat += "%d";
		String l_sStr(Format(l_sFormat, m_iNumberOfMoves));
		if(m_iBestNumberOfMoves != -1)
		{
			String l_sStr2(Format(" (%d)", m_iBestNumberOfMoves));
			l_sStr += l_sStr2;
		}

		m_StatusBar.SetLabel(l_sStr);

		YSwap(m_Cells[iIndex]->m_iCurID, m_iCurEmptyID);

		if(m_iCurEmptyID == m_iMaxNumberOfCells - 1)
		{
			vector<CYPuzzleCell*>::iterator Idx = m_Cells.begin();
			for(; Idx != m_Cells.end(); Idx++)
				if((*Idx)->m_iID != (*Idx)->m_iCurID)
					break;

			if(Idx == m_Cells.end())
			{
				if(m_iBestNumberOfMoves == -1 || m_iBestNumberOfMoves > m_iNumberOfMoves)
					m_iBestNumberOfMoves = m_iNumberOfMoves;

				PromptOK(t_("You win!"));
				ShuffleVector();
				ArrangeButtons(m_viShuffle);
			}
		}
	}
}

void YPuzzle::ArrangeButtons(vector<int> &viShuffle)
{
	Rect l_CellRect;
	CYPuzzleCell *l_pCell = NULL;
	int l_iDivY = 0, l_iDivX = 0, l_iX = 0, l_iY = 0;
	String l_sStr;
	for(int Idx = 0; Idx < m_iMaxNumberOfCells - 1; Idx++)
	{
		l_iDivY = viShuffle[Idx] / m_iNumberOfCellsX;
		l_iDivX = viShuffle[Idx] % m_iNumberOfCellsX;
		l_iX = l_iDivX * m_iCellSize;
		l_iY = l_iDivY * m_iCellSize;
		l_CellRect.Set(l_iX, l_iY, l_iX + m_iCellSize, l_iY + m_iCellSize);

		m_Cells[Idx]->SetRect(l_CellRect);
		m_Cells[Idx]->m_iCurID = viShuffle[Idx];
	}

	m_iNumberOfMoves = 0;
	l_sStr = t_(" Moves: ");
	l_sStr += '0';
	if(m_iBestNumberOfMoves != -1)
	{
		String l_sStr2(Format(" (%d)", m_iBestNumberOfMoves));
		l_sStr += l_sStr2;
	}

	m_StatusBar.SetLabel(l_sStr);
}
