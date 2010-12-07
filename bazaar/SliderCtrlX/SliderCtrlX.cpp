#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "SliderCtrlX.h"


#define			BORDER_SIZE			2
#define			BORDER1				BORDER_SIZE
#define			BORDER2				( 2 * BORDER_SIZE )


SliderCtrlX::SliderCtrlX()
: m_nMin(0)
, m_nMax(100)
, m_nStep(1)
, m_bRound_step(true)
, m_bJump(false)
, m_bUseCustomThumbs( 0 )
, m_nMajorTicks( 0 )
, m_nMinorTicks( 0 )
, m_nMajorTickSize( 30 )
, m_nMinorTickSize( 20 )
, m_TickPosition( TOP )
{
	SetValue( 0 );

	Transparent();
	NoWantFocus();
}

SliderCtrlX::~SliderCtrlX() {}


bool SliderCtrlX::IsVert() const
{
	return GetSize().cx < GetSize().cy;
}

int  SliderCtrlX::HoVe(int  x, int  y) const
{
	return IsVert() ? y : x;
}

int& SliderCtrlX::HoVeR(int& x, int& y) const
{
	return IsVert() ? y : x;
}

void SliderCtrlX::Paint(Draw& w)
{
	Size size = GetSize();

	// draw gradations	
	for( 	int i = Min(); 
			( m_nMajorTicks > 0 ) && ( i <= Max() ) ;
			i += ( m_nMinorTicks == 0 ? m_nMajorTicks : m_nMinorTicks ) ) {
		
		int nPos = SliderToClient( i );
		
		if( ( m_nMajorTicks != 0 ) && ( i % m_nMajorTicks ) == 0 )
			DrawTick( w, MAJOR, (HOVE)HoVe( HORZ, VERT ), nPos, i );
		else if( ( m_nMinorTicks != 0 ) && ( i % m_nMinorTicks ) == 0 )
			DrawTick( w, MINOR, (HOVE)HoVe( HORZ, VERT ), nPos, i );
	}

	if(IsVert()) { // Vertical slider
		// Draw Line Border
		int half = size.cx >> 1;
		DrawBorder(w, half - BORDER1, BORDER1, BORDER2, size.cy - BORDER2, InsetBorder);

		// Draw thumbs
		for( int i = m_vValues.GetCount() - 1 ; i >= 0 ; i-- ) {
			if(!IsNull(m_vValues[i]))
				w.DrawImage((size.cx - ( m_bUseCustomThumbs ? m_vThumbImgs[i].GetSize().cx : CtrlImg::vthumb().GetSize().cx ) ) >> 1,
					max( SliderToClient(m_vValues[i]) - ( ( m_bUseCustomThumbs ? m_vThumbImgs[i].GetSize().cy : CtrlImg::vthumb().GetSize().cy ) >> 1 ), 0 ),
				    HasCapture() || HasFocus() ? ( m_bUseCustomThumbs ? m_vThumbImgsFocus[i] : CtrlImg::vthumb1() ) : ( m_bUseCustomThumbs ? m_vThumbImgs[i] : CtrlImg::vthumb() ) );
		}
	}
	else { // Horz slider
		// Draw Line Border
		int half = size.cy >> 1;
		DrawBorder(w, BORDER1, half - BORDER1, size.cx - BORDER2, BORDER2, InsetBorder);
		
		// draw thumbs
		for( int i = m_vValues.GetCount() - 1 ; i >= 0 ; i-- ) {
			if(!IsNull(m_vValues[i]))
				w.DrawImage(max( SliderToClient(m_vValues[i]) - ( ( m_bUseCustomThumbs ? m_vThumbImgs[i].GetSize().cx : CtrlImg::hthumb().GetSize().cx ) >> 1 ), 0),
						(size.cy - ( m_bUseCustomThumbs ? m_vThumbImgs[i].GetSize().cy : CtrlImg::hthumb().GetSize().cy )) >> 1,
			            HasCapture() || HasFocus() ? (m_bUseCustomThumbs ? m_vThumbImgsFocus[i] : CtrlImg::hthumb1()) : (m_bUseCustomThumbs ? m_vThumbImgs[i] : CtrlImg::hthumb() ));
		}
	}
	if(HasFocus())
		DrawFocus(w, size);
}

void	SliderCtrlX::DrawTick( Draw &w, MAJORMINOR Type, HOVE Orientation, int nPos, int nVal )
{
	Size size = GetSize();
	int		nMajorWidth = 3;

	String txt;
	
	if (LabelFormat)
		LabelFormat(txt, nVal);
	else
		txt = AsString(nVal);
	
	if( Orientation == HORZ ) {
		if( Type == MAJOR ) {
			int nSize = (int)( m_nMajorTickSize / 100.0f * size.cy * .5 + 0.5f );
			w.DrawLine( max( nPos, nMajorWidth >> 1 ), (int)( size.cy * .5 + .5), max( nPos, nMajorWidth >> 1 ), (int)(size.cy * .5 + ( m_TickPosition == TOP ? -nSize : nSize ) + 0.5f), nMajorWidth );
			
			Size sz = GetTextSize( txt, StdFont() );
			int nTextPos = nPos - (int)( sz.cx / 2.0f + 0.5f );
			nTextPos = min( max( 0, nTextPos ), size.cx - sz.cx );
			w.DrawText( nTextPos, 0, txt );
		}
		else {
			int nSize = (int)( m_nMinorTickSize / 100.0f * size.cy * .5 + 0.5f );
			w.DrawLine( nPos, size.cy / 2, nPos, (int)(size.cy * .5 + ( m_TickPosition == TOP ? -nSize : nSize ) + 0.5f) );
		}
	}
	else {// vert
		if( Type == MAJOR ) {
			int nSize = (int)( m_nMajorTickSize / 100.0f * size.cx * .5 + 0.5f );
			w.DrawLine( (int)( size.cx * .5 + .5), max( nPos, nMajorWidth >> 1 ), (int)(size.cx * .5 + ( m_TickPosition == TOP ? -nSize : nSize ) + 0.5f), max( nPos, nMajorWidth >> 1 ), nMajorWidth );
			
			Size sz = GetTextSize( txt, StdFont() );
			int nTextPos = nPos - (int)( sz.cy / 2.0f + 0.5f );
			nTextPos = min( max( 0, nTextPos ), size.cy - sz.cy );
			w.DrawText( 0, nTextPos, txt );
		}
		else {
			int nSize = (int)( m_nMinorTickSize / 100.0f * size.cx * .5 + 0.5f );
			w.DrawLine( size.cx / 2, nPos, (int)(size.cx * .5 + ( m_TickPosition == TOP ? -nSize : nSize ) + 0.5f), nPos );
		}
	}
}

bool SliderCtrlX::Key(dword key, int repcnt)
{
	if(IsEditable())
		switch(key) {
		case K_LEFT:
		case K_UP:
			Dec();
			return true;
		case K_RIGHT:
		case K_DOWN:
			Inc();
			return true;
		}
	return Ctrl::Key(key, repcnt);
}

void SliderCtrlX::LeftDown(Point pos, dword keyflags)
{
	if(!IsEditable())
		return;
	SetWantFocus();
	int thumbPos = SliderToClient(m_vValues[0]);
	int p = HoVe(pos.x, pos.y);
	int nHalfThumb = HoVe(m_bUseCustomThumbs ? m_vThumbImgs[0].GetSize().cx : CtrlImg::hthumb().GetSize().cx,
			 m_bUseCustomThumbs ? m_vThumbImgs[0].GetSize().cy : CtrlImg::vthumb().GetSize().cy) >> 1;
			 
	if(IsNull(thumbPos)) {
		SetValue( ClientToSlider(p) );
		WhenSlideFinish();
		UpdateActionRefresh();
	}
	// Did we click on the thumb?
	else if( ( p >= ( thumbPos - nHalfThumb ) ) && 
		( p < ( thumbPos + nHalfThumb ) ) )
		SetCapture();
	
	else if( m_bJump ) 
	{
		m_vValues[0] = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();		
	}
	else 
	{		
		if( ( ( p < thumbPos) && (m_nMin == Min() ) ) || ( (p > thumbPos) && ( m_nMin == Max() ) ) )
			Dec();
		else
			Inc();
	}
	
	Refresh();
}

void SliderCtrlX::LeftRepeat(Point p, dword f)
{
	if(!HasCapture())
		LeftDown(p, f);
}

void SliderCtrlX::LeftUp(Point pos, dword keyflags)
{
	if (HasCapture())
		WhenSlideFinish();
	Refresh();
}

void SliderCtrlX::MouseMove(Point pos, dword keyflags)
{
	if(HasCapture()) {
		int n = ClientToSlider(HoVe(pos.x, pos.y));
		if(n != m_vValues[0]) {
			SetValue( n );
			UpdateActionRefresh();
		}
	}
}

SliderCtrlX& SliderCtrlX::MinMax(int _min, int _max)
{
	if(m_nMin != _min || m_nMax != _max) {
		m_nMin = _min;
		m_nMax = _max;
		if(!IsNull(m_vValues[0])) {
			int v = minmax(m_vValues[0], Min(), Max());
			if(m_vValues[0] != v) {
				SetValue( v );
				Update();
			}
		}
		Refresh();
	}
	return *this;
}

int SliderCtrlX::SliderToClient(int v) const
{
	if(IsNull(v))
		return Null;
	v = minmax(v, Min(), Max());

	if( v < 0 )
		v = iscalefloor(v - m_nMin, HoVe(GetSize().cx - BORDER2,
		                         GetSize().cy - BORDER2), m_nMax - m_nMin);
	else
		v = iscaleceil(v - m_nMin, HoVe(GetSize().cx - BORDER2,
		                         GetSize().cy - BORDER2), m_nMax - m_nMin);
	return v;
}

int SliderCtrlX::ClientToSlider(int p) const
{
	return minmax(m_nMin + iscale(p, m_nMax - m_nMin,
	                           HoVe(GetSize().cx - BORDER2, GetSize().cy - BORDER2)), Min(), Max());
}

void SliderCtrlX::Dec()
{
	int n = m_vValues[0];
	if(IsNull(m_vValues[0]))
		n = Max();
	else
	if(n > Min()) {
		if(m_bRound_step && m_nStep > 1)
			n = idivfloor(n - 1, m_nStep) * m_nStep;
		else
			n -= m_nStep;
		if(n < Min())
			n = Min();
	}
	if(n != m_vValues[0]) {
		SetValue( n );
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}

void SliderCtrlX::Inc()
{
	int n = m_vValues[0];
	if(IsNull(m_vValues[0]))
		n = Min();
	else
	if(n < Max()) {
		if(m_bRound_step && m_nStep > 1)
			n = idivceil(n + 1, m_nStep) * m_nStep;
		else
			n += m_nStep;
		if(n > Max())
			n = Max();
	}
	if(n != m_vValues[0]) {
		SetValue( n );
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}


SliderCtrlX&		SliderCtrlX::AddOutCtrl( Ctrl* c )
{
	m_vctrlOutput.Add( c );
	
	return *this;
}


void SliderCtrlX::SetData(const Value& v)
{
	SetValue( v );
/*	int i = v;
	if(!IsNull(i))
		i = minmax(i, Min(), Max() );

	if(i != m_vValues[0]) {
		SetValue( i );
		UpdateRefresh();
	}*/
}

Value SliderCtrlX::GetData( int nIndex ) const
{
	return m_vValues[nIndex];
}

Value  SliderCtrlX::GetData() const
{
	return m_vValues[0];
}

Value			SliderCtrlX::SetValue( const Value& v, int nIndex /*= 0 */ )
{
	int n = v;
	
	if(!IsNull(n))
	{
		n = minmax(n, Min(), Max() );

		if( m_vValues.At(nIndex) != n )
		{
			m_vValues.At(nIndex) = n;
		
			if( m_vctrlOutput.GetCount() > nIndex )
				m_vctrlOutput[nIndex]->SetData( n );
			
			UpdateRefresh();
		}
	}
	
	return m_vValues.At(nIndex);
}

void SliderCtrlX::GotFocus()
{
	Refresh();
}

void SliderCtrlX::LostFocus()
{
	Refresh();
}

