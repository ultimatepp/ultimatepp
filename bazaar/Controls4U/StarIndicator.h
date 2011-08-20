#include <CtrlLib/CtrlLib.h>

class StarIndicator : public ProgressIndicator {
private:
	bool	m_bMouseInside;	// True if mouse over the control
	Color	m_FontColor;	// Font color
	int		m_nVotes;		// Number of votes
	int		m_nVotesHigh;	// Number of Votes for Green
	int		m_nVotesLow;	// Number of Votes for Red
	bool	votes;			// Shows number of votes or not

public:
	StarIndicator();
	virtual ~StarIndicator();

	virtual void	MouseEnter(Point p, dword keyflags);
	virtual void	MouseLeave();
	virtual void	MouseMove(Point p, dword keyflags)	{ Refresh(); }
	virtual void	Paint(Draw& draw);
	virtual void	Layout();
	virtual StarIndicator&	SetFontColor(Color c)	{ m_FontColor=c; return *this; }
	virtual StarIndicator&	SetVotes(int n);
	virtual StarIndicator&	SetVotesHigh(int n)		{ m_nVotesHigh=n; return *this; }
	virtual StarIndicator&	SetVotesLow(int n)		{ m_nVotesLow=n; return *this; }
	virtual StarIndicator&	ShowVotes(bool b)		{ votes=b; return *this; }
};

