/***********************************************************************************************
 * SliderCtrlX
 *
 * Enhanced Slider Control / Slider indicator
 *
 * Autors : avpavp, jerson, jibe, koldo
 * Last modified : June 09, 2011
 *
 * 2do : adjust default values when changing CONTROL/INDICATOR
 *		Better .usc file
 * Known bugs : Invalid memory access when choosing FillColor with .lay designer (seems to be
 *      only with ubuntu. See http://www.ultimatepp.org/forum/index.php?t=msg&th=6022&start=0&
 **********************************************************************************************/

#include <CtrlLib/CtrlLib.h>
#include "SliderCtrlX.h"

NAMESPACE_UPP

#define IMAGECLASS SliderThumbsImg
#define IMAGEFILE <Controls4U/SliderCtrlX.iml>
#include <Draw/iml.h>

SliderCtrlX::SliderCtrlX()
: m_nMin(0)
, m_nMax(100)
, m_bInverted(false)
, m_nStep(1)
, m_bRound_step(false)
, m_bJump(false)
, m_bUseCustomThumbs( 0 )
, m_nMajorTicks( 10 )
, m_nMinorTicks( 2 )
, m_nMajorTickSize( 30 )
, m_nMinorTickSize( 20 )
, m_nTickPosition( TOP )
, m_nThickness( 2 )
, m_nSliderType( 0 )
, m_nThumbNumber( 1 )
/***********************************************************************************************
 * Constructor
 * public
 **********************************************************************************************/
{
	SetValue( 0 );

	border2 = m_nThickness * BORDER_SIZE;
	Transparent();

	m_ThumbImg.Clear();
}

SliderCtrlX::~SliderCtrlX() 
/***********************************************************************************************
 * Destructor
 * virtual - public
 **********************************************************************************************/
{
}

SliderCtrlX& SliderCtrlX::AddOutCtrl( Ctrl* c )
/***********************************************************************************************
 * public
 **********************************************************************************************/
{
	m_vctrlOutput.Add( c );
	
	return *this;
}

int SliderCtrlX::ClientToSlider(int p) const
/***********************************************************************************************
 * Converts Slider values to canvas coordinates
 * private
 **********************************************************************************************/
{
	p -= HoVe(m_ThumbSize.cx/2, m_ThumbSize.cy/2);

	return minmax(m_nMin + iscale(p, m_nMax - m_nMin,
	                           HoVe(GetSize().cx - border2 - m_ThumbSize.cx, GetSize().cy - border2 - m_ThumbSize.cy)), Min(), Max());
}

void SliderCtrlX::Dec()
/***********************************************************************************************
 * decrements the slider value
 * public
 **********************************************************************************************/
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

void	SliderCtrlX::DrawTick( Draw &w, MAJORMINOR Type, HOVE Orientation, int nPos, int nVal )
/***********************************************************************************************
 * draws a tickmark at nPos on canvas
 * protected
 **********************************************************************************************/
{
	Size size = GetSize();
	int		sz = Orientation == HORZ ? size.cy : size.cx;
	int		nMajorSize = (int)( m_nMajorTickSize / 100.0f * sz * .5 + 0.5f );
	int		nMinorSize = (int)( m_nMinorTickSize / 100.0f * sz * .5 + 0.5f );
	int		nMajorWidth = 3;
	int		TickBottomMaj, TickBottomMin, TickTop;

	String txt;
	
	if (LabelFormat)
		LabelFormat(txt, nVal);
	else
		txt = AsString(nVal);
	
	switch (m_nTickPosition) {
		case TOP : TickTop = -2;
				TickBottomMaj = -nMajorSize -2;
				TickBottomMin = -nMinorSize -2;
				break;
		case MIDDLE_TOP : TickTop = +2 +m_nThickness;
				TickBottomMaj = -nMajorSize +2 +m_nThickness;
				TickBottomMin = -nMinorSize +2 +m_nThickness;
				break;
		case MIDDLE_BOTTOM : TickTop = 0;
				TickBottomMaj = nMajorSize;
				TickBottomMin = nMinorSize;
				break;
		case BOTTOM : TickTop = +2 +m_nThickness;
				TickBottomMaj = nMajorSize +2 +m_nThickness;
				TickBottomMin = nMinorSize +2 +m_nThickness;
				break;
	}
	
	if( Orientation == HORZ ) {
		if( Type == MAJOR ) {
			w.DrawLine( max( nPos, nMajorWidth >> 1 ),
				(int)( size.cy * .5 + TickTop), 
				max( nPos, nMajorWidth >> 1 ), 
				(int)(size.cy * .5 + TickBottomMaj), 
				nMajorWidth );
			
			Size sz = GetTextSize( txt, StdFont() );
			int nTextPos = nPos - (int)( sz.cx / 2.0f + 0.5f );
			nTextPos = min( max( 0, nTextPos ), size.cx - sz.cx );
			w.DrawText( nTextPos, 0, txt );
		}
		else {
			w.DrawLine( nPos,
				(int)( size.cy * .5 + TickTop),
				nPos,
				(int)(size.cy * .5 + TickBottomMin ) );
		}
	}
	else { // vert
		if( Type == MAJOR ) {
			w.DrawLine( (int)( size.cx * .5 + TickTop), 
				max( nPos, nMajorWidth >> 1 ), 
				(int)(size.cx * .5 + TickBottomMaj), 
				max( nPos, nMajorWidth >> 1 ), 
				nMajorWidth );
			
			Size sz = GetTextSize( txt, StdFont() );
			int nTextPos = nPos - (int)( sz.cy / 2.0f + 0.5f );
			nTextPos = min( max( 0, nTextPos ), size.cy - sz.cy );
			w.DrawText( 0, nTextPos, txt );
		}
		else {
			w.DrawLine( (int)(size.cx / 2 + TickTop), 
				nPos, 
				(int)(size.cx * .5 + TickBottomMin), 
				nPos );
		}
	}
}

Value  SliderCtrlX::GetData() const
/***********************************************************************************************
 * default read of slider value
 * virtual - public
 **********************************************************************************************/
{
	return m_vValues[0];
}

Value SliderCtrlX::GetData( int nIndex ) const
/***********************************************************************************************
 * reads the slider[n] value
 * virtual - public
 **********************************************************************************************/
{
	return m_vValues[nIndex];
}

void SliderCtrlX::GotFocus()
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
{
	Refresh();
}

int  SliderCtrlX::HoVe(int  x, int  y) const
/***********************************************************************************************
 * returns x for Hslider or y for Vslider
 * private
 **********************************************************************************************/
{
	return IsVert() ? y : x;
}

int& SliderCtrlX::HoVeR(int& x, int& y) const
/***********************************************************************************************
 * returns &x for Hslider or &y for Vslider
 * private
 **********************************************************************************************/
{
	return IsVert() ? y : x;
}

void SliderCtrlX::Inc()
/***********************************************************************************************
 * increments
 * virtual - public
 **********************************************************************************************/
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

bool SliderCtrlX::IsVert() const
/***********************************************************************************************
 * Checks if the slider is vertically oriented
 * public
 **********************************************************************************************/
{
	return GetSize().cx < GetSize().cy;
}

bool SliderCtrlX::Key(dword key, int repcnt)
/***********************************************************************************************
 * handles keys addressed to the slider (GotFocussed)
 * only if it is a CONTROL
 * virtual - public
 **********************************************************************************************/
{
	// No thumb action for indicator
	if (m_nSliderType == INDICATOR)	return Ctrl::Key(key, repcnt);

	if(IsEditable())
		switch(key) {
		case K_LEFT:
		case K_DOWN:
			Dec();
			return true;
		case K_RIGHT:
		case K_UP:
			Inc();
			return true;
		}
	return Ctrl::Key(key, repcnt);
}

void SliderCtrlX::LeftDown(Point pos, dword keyflags)
/***********************************************************************************************
 * handles the left mouse button if the slider is s CONTROL
 * virtual - public
 **********************************************************************************************/
{
	if(!IsEditable())
		return;

	// No thumb action for indicator
	if (m_nSliderType == INDICATOR)	return;
	
	SetWantFocus();
	int thumbPos = SliderToClient(m_vValues[0]);
	int p = HoVe(pos.x, pos.y);
			 
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
			IsInverted() ? Inc() : Dec();
		else
			IsInverted() ? Dec() : Inc();
	}
	
	Refresh();
}

void SliderCtrlX::LeftRepeat(Point p, dword f)
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
{
	if(!HasCapture())
		LeftDown(p, f);
}

void SliderCtrlX::LeftUp(Point pos, dword keyflags)
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
{
	if (HasCapture())
		WhenSlideFinish();
	Refresh();
}

void SliderCtrlX::LostFocus()
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
{
	Refresh();
}

SliderCtrlX& SliderCtrlX::MinMax(int _min, int _max)
/***********************************************************************************************
 * Restricts slider to Min, Max values
 * public
 **********************************************************************************************/
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

void SliderCtrlX::MouseMove(Point pos, dword keyflags)
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
{
	if(HasCapture()) {
		int n;
		int p = HoVe(pos.x, pos.y);
		int thumbPos = SliderToClient(m_vValues[0]);
		if (IsInverted())
			n = ClientToSlider(HoVe(GetSize().cx-pos.x, GetSize().cy-pos.y));
		else
			n = ClientToSlider(HoVe(pos.x, pos.y));
		if(n != m_vValues[0]) {
			SetValue( n );
			UpdateActionRefresh();
		}
	}
}

void SliderCtrlX::Paint(Draw& w)
/***********************************************************************************************
 * Draws the slider on the canvas w
 * virtual - public
 **********************************************************************************************/
{
	Size size = GetSize();
	if (m_ThumbImg.IsEmpty())
		SetThumbType(m_nThumbNumber);

	if(IsVert()) { // Vertical slider
		// Draw Line Border
		int half = size.cx >> 1;
		DrawBorder(w, half - BORDER1, BORDER1, border2, size.cy - border2, InsetBorder);

		if (IsInverted())
			// draw the fill where bottom is the min value
			w.DrawRect(half - BORDER1+1, SliderToClient(m_vValues[0])+1,
				   border2-2, size.cy-SliderToClient(m_vValues[0])-BORDER1-2,
				   m_FillColor);
		else
			// draw the fill where top is the min value
			w.DrawRect(half - BORDER1+1, BORDER1+1,
				   border2-2, SliderToClient(m_vValues[0])-BORDER1-2,
				   m_FillColor);
	}
	else { // Horz slider
		// Draw Line Border
		int half = size.cy >> 1;
		DrawBorder(w, BORDER1, half - BORDER1, size.cx - border2, border2, InsetBorder);
		
		if (IsInverted())
			// draw the fill where left is the min value
			w.DrawRect(SliderToClient(m_vValues[0]), half - BORDER1+2, size.cx-SliderToClient(m_vValues[0])-2, border2-3, m_FillColor);
		else
			// draw the fill where right is the min value
			w.DrawRect(BORDER1, half - BORDER1+2, SliderToClient(m_vValues[0])-2, border2-3, m_FillColor);
	}

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

	// Draw thumbs
	if(IsVert()) { // Vertical slider
		for( int i = m_vValues.GetCount() - 1 ; i >= 0 ; i-- ) {
			if(!IsNull(m_vValues[i])) {
				w.DrawImage((size.cx - m_ThumbSize.cx ) >> 1,
					SliderToClient(m_vValues[i]) - ( m_ThumbSize.cy >> 1 ),
				    HasCapture() || HasFocus() ? ( m_bUseCustomThumbs ? m_vThumbImgsFocus[i] : m_ThumbFImg ) : ( m_bUseCustomThumbs ? m_vThumbImgs[i] : m_ThumbImg ) );
			}
		}
	}
	else { // Horz slider
		for( int i = m_vValues.GetCount() - 1 ; i >= 0 ; i-- ) {
			if(!IsNull(m_vValues[i])) {
				w.DrawImage(SliderToClient(m_vValues[i]) - ( m_ThumbSize.cx >> 1 ),
					m_nThickness + ((size.cy - m_ThumbSize.cy) >> 1),
					HasCapture() || HasFocus() ? (m_bUseCustomThumbs ? m_vThumbImgsFocus[i] : m_ThumbFImg) : (m_bUseCustomThumbs ? m_vThumbImgs[i] : m_ThumbImg));
			}
		}
	}
	
	if(HasFocus())
		DrawFocus(w, size);
}

void SliderCtrlX::SetData(const Value& v)
/***********************************************************************************************
 * sets the slider value within its' range
 * virtual - public
 **********************************************************************************************/
{
	SetValue( v );
}

SliderCtrlX& SliderCtrlX::SetThumbType(int n)
/***********************************************************************************************
 * Selects the thumb according to the choosen type
 * public
 **********************************************************************************************/
{
	m_nThumbNumber = n;
	// This is called before size is known, so we cannot choose the right thumb...
	if (GetSize().IsEmpty()) {
		m_ThumbImg.Clear();
		return *this;
	}
	m_nThumbType = m_nThumbNumber*2;
	if (IsVert()) m_nThumbNumber = 0;
	m_ThumbImg = SliderThumbsImg::Get(m_nThumbType);
	m_ThumbFImg = SliderThumbsImg::Get(m_nThumbType+1);
	if (IsVert()) {
		m_ThumbImg = RotateAntiClockwise(m_ThumbImg);
		m_ThumbFImg = RotateAntiClockwise(m_ThumbFImg);
	}
	if (m_nTickPosition == BOTTOM) {
		IsVert() ? m_ThumbImg = MirrorHorz(m_ThumbImg) : m_ThumbImg = MirrorVert(m_ThumbImg);
		IsVert() ? m_ThumbFImg = MirrorHorz(m_ThumbFImg) : m_ThumbFImg = MirrorVert(m_ThumbFImg);
	}
	m_ThumbSize = m_bUseCustomThumbs ? m_vThumbImgs[0].GetSize() : m_ThumbImg.GetSize();
	nHalfThumb = HoVe(m_ThumbSize.cx, m_ThumbSize.cy) >> 1;
	return *this;
}

Value SliderCtrlX::SetValue( const Value& v, int nIndex /*= 0 */ )
/***********************************************************************************************
 * sets the value for the slider position
 * public
 **********************************************************************************************/
{
	int n = v;
	
	if(!IsNull(n))
	{
		n = minmax(n, Min(), Max() );

		if(n < Max() && n > Min()) {
			if(m_bRound_step && m_nStep > 1)
				n = idivceil(n, m_nStep) * m_nStep;
			if(n > Max())
				n = Max();
			if (n < Min())
				n = Min();
		}

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

int SliderCtrlX::SliderToClient(int v) const
/***********************************************************************************************
 * CONVERTS SLIDER VALUES TO Canvas coordinates
 * private 
 **********************************************************************************************/
{
	if(IsNull(v))
		return Null;
	v = minmax(v, Min(), Max());

	if( v < 0 )
		v = iscalefloor(v - m_nMin, HoVe(GetSize().cx - border2 - m_ThumbSize.cx,
		                         GetSize().cy - border2 - m_ThumbSize.cy), m_nMax - m_nMin);
	else
		v = iscaleceil(v - m_nMin, HoVe(GetSize().cx - border2 - m_ThumbSize.cx,
		                         GetSize().cy - border2 - m_ThumbSize.cy), m_nMax - m_nMin);

	v += HoVe(m_ThumbSize.cx/2, m_ThumbSize.cy/2);

	if (IsInverted())
		IsVert() ? v = GetSize().cy - v : v = GetSize().cx - v;
			
	return v;
}

END_UPP_NAMESPACE