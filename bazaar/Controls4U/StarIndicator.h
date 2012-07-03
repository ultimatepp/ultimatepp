#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class StarIndicator : public ProgressIndicator {
private:
	bool	m_bAlwaysShowValue;	// True if value/votes must be always shown, not only if mouse over
	bool	m_bMouseInside;	// True if mouse over the control
	bool	m_bTotal;		// Shows the total (5) or not
	bool	m_bVotes;		// Shows number of votes or not
	Color	m_FontColor;	// Font color
	int		m_nVotes;		// Number of votes
	int		m_nVotesHigh;	// Number of Votes for Green
	int		m_nVotesLow;	// Number of Votes for Red

public:
	StarIndicator();
	virtual ~StarIndicator();

	virtual StarIndicator&	AlwaysShowValue(bool b)		{ m_bAlwaysShowValue=b; return *this; }
	virtual void	MouseEnter(Point p, dword keyflags);
	virtual void	MouseLeave();
	virtual void	MouseMove(Point p, dword keyflags)	{ Refresh(); }
	virtual void	Paint(Draw& draw);
	virtual void	Layout();
	virtual StarIndicator&	SetFontColor(Color c)	{ m_FontColor=c; return *this; }
	virtual void	Set(double n);
	virtual void	Set(int n)						{ return Set((double)n); }
	virtual void	Set(int n, int tot)				{ return ProgressIndicator::Set(n, tot); }
	virtual StarIndicator&	SetVotes(int n);
	virtual StarIndicator&	SetVotesHigh(int n)		{ m_nVotesHigh=n; return *this; }
	virtual StarIndicator&	SetVotesLow(int n)		{ m_nVotesLow=n; return *this; }
	virtual StarIndicator&	ShowPercent(bool b)		{ percent=b; return *this; }
	virtual StarIndicator&	ShowTotal(bool b)		{ m_bTotal=b; return *this; }
	virtual StarIndicator&	ShowVotes(bool b)		{ m_bVotes=b; return *this; }
};

END_UPP_NAMESPACE