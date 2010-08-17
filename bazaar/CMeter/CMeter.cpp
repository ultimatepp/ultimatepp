#include "CMeter.h"

CH_STYLE(CMeter, Style, StyleDefault)
{
	backcol = SColorFace();
	textcol = SColorText();
	forecol = LtBlue();
	forecol2 = LtRed();
	cmarksize = 3;
}

CMeter::CVMark::CVMark(float _v, String _name, Color _col)
	:  v(_v)
	, col(_col)
	, name(_name)
{
}

//CMeter

CMeter::CMeter(float _min, float _max, bool _vertical, bool _reverse)
	: min(_min)
	, max(_max)
	, val(min)
	, inc(0.1f)

	, vertical(_vertical)
	, reverse(_reverse)
	, stepping(false)
	, steplin(false)
	, showvalue(true)
	, showpercent(false)
	, clickincdec(true)
	, incdectostep(false)
	, drawmode(IMGCACHE)
	, increment(0)
	, incv(0.0f)
	, inci(0)
	, __val(min)
	, __ii(-1)

	, px(0)
	, py(0)
	, _px(0)
	, _py(0)
	
{
	Transparent();

	vsteps.Add(max); //max needs to be there for sure
	cvmarks.Add(CVMark(0.0f));

	SetStyle(StyleDefault());

	Set((max-min)/2.0f);		
}

CMeter& CMeter::SetStyle(const Style& s)
{
	style = &s;
	RefreshLayout();
	return *this;
}

int CMeter::FindStep(float v) const
{
	float _v = min;
	
	int i = vsteps.GetCount()-1;
	while(i>=0)
	{
		if(vsteps[i] <= v)
		{
			_v = vsteps[i];
			break;
		}
		--i;
	}
	return i;
}

CMeter & CMeter::Set(float _val)
{
	val = _val; //first try the desired value
	
	if(val < min)
		val = min;
	if(val > max)
		val = max;

	__val = val; //for cache

	//determine the val
	if(stepping)
	{
		int i = FindStep(val);
		val = min;
		pp = 0;
		__ii = i; //needed for inc dec, NO oszillates
		if(i>=0)
		{
			val = vsteps[i];
			if(steplin)
				pp = (float)(i+1)/(float)vsteps.GetCount();
			else
				pp = (val - min) / (max - min);
		}
	}
	else
	{
		pp = (val - min) / (max - min);
	}
	
	Update();

	Refresh();	
	return *this;
}

CMeter & CMeter::Min(float _min)
{
	min = _min;

	if(val < min)
		val = min;
	
	Update();

	return *this;
}

CMeter & CMeter::Max(float _max)
{
	max = _max;

	if(val > max)
		val = max;

	Update();

	return *this;
}

CMeter & CMeter::MinMax(float _min, float _max)
{
	min = _min;
	max = _max;

	Update();

	return *this;
}

CMeter & CMeter::Reverse(bool _reverse)
{
	reverse = _reverse;

	CreateBack();

	return *this;
}

CMeter & CMeter::Vertical(bool _vertical)
{
	vertical = _vertical;

	Layout();

	return *this;
}

CMeter & CMeter::Stepping(bool _stepping)
{
	stepping = _stepping;

	Set(Get());

	return *this;
}

CMeter & CMeter::StepLinear(bool _steplin)
{
	steplin = _steplin;

	Set(Get());

	return *this;
}

CMeter & CMeter::Steps(const Vector<float> & _vsteps, bool reverse)
{
	vsteps <<= _vsteps;
	
	if(vsteps.IsEmpty())
		vsteps << max; //at least

	if(!reverse)
		return *this;
	
	int i = 0, j = vsteps.GetCount()-1;
	while(i<j)
		vsteps.Swap(i++, j--);

	return *this;
}

CMeter & CMeter::ShowValue(bool _showvalue)
{
	showvalue = _showvalue;

	return *this;
}

CMeter & CMeter::ShowPercent(bool _showpercent)
{
	showpercent = _showpercent;

	Update();

	return *this;
}

CMeter & CMeter::SetMode(int mode)
{
	drawmode = mode;
	
	return *this;
}

//updates all local cache info, depends on correct layout, uses already set val / min / max
void CMeter::Update()
{
	if(showpercent)
	{
		(s = Format("%0n", (100.0f * pp))) << " %";
		tlen = GetTextSize(s,StdFont(),s.GetLength());
	}
	else
	{
		s = Format("%1!n", val);
		tlen = GetTextSize(s,StdFont(),s.GetLength());
	}

	Size size = GetSize();

	//drawing will use waht it needs
	dx = (int)(size.cx * pp);
	lx = (int)(size.cx - dx);
	dy = (int)(size.cy * pp);
	ly = (int)(size.cy - dy);
}

void CMeter::Layout()
{
	Size size = GetSize();

	if(cvmarks.GetCount()>0)
	{
		if(vertical)
		{
			py = 0; _px = 0;
			px = style->cmarksize;
			_py = px;
		}
		else
		{
			px = 0; _py = 0;
			_px = style->cmarksize;
			py = _px;
		}
	}
	else
	{
		px = 0; py = 0;
		_px = 0; _py = 0;	
	}

	Update();

	CreateBack();
}

void CMeter::PaintBarGradient(Painter & sw, bool staticgradient)
{
	Size size = GetSize();

	if(vertical)
	{
		if(!reverse)
		{
			sw.DrawRect(px,py,size.cx,ly, style->backcol);//.Fill(px,py, backcol, px+size.cx,py+ly,backcol);
			sw.Rectangle(px,py+ly,size.cx,dy);
			if(staticgradient)
				sw.Fill(px,py   ,style->forecol2,px/*+size.cx*/,py+size.cy,style->forecol);
			else
				sw.Fill(px,py+ly,style->forecol2,px/*+size.cx*/,py+size.cy,style->forecol);
		}
		else
		{
			sw.DrawRect(px,py+dy,size.cx,ly,style->backcol);
			sw.Rectangle(px,py,size.cx,dy);
			if(staticgradient)
				sw.Fill(px,py,style->forecol,px/*+size.cx*/,py+size.cy,style->forecol2);
			else
				sw.Fill(px,py,style->forecol,px/*+size.cx*/,py+dy,style->forecol2);
		}
	}
	else //horizontal
	{
		if(!reverse)
		{
			sw.DrawRect(px+dx,py,lx,size.cy, style->backcol);//.Fill(px+dx,py,backcol,px+size.cx,py+size.cy,backcol);
			sw.Rectangle(px,py,dx,size.cy);
			if(staticgradient)
				sw.Fill(px,py,style->forecol,px+size.cx,py/*+size.cy*/,style->forecol2);
			else
				sw.Fill(px,py,style->forecol,px+dx,py/*+size.cy*/,style->forecol2);
		}
		else
		{
			sw.DrawRect(px,py,lx,size.cy,style->backcol);
			sw.Rectangle(px+lx,py,dx,size.cy);
			if(staticgradient)
				sw.Fill(px   ,py,style->forecol2,px+size.cx,py/*+size.cy*/,style->forecol);
			else
				sw.Fill(px+lx,py,style->forecol2,px+size.cx,py/*+size.cy*/,style->forecol);
		}
	}
}

void CMeter::PaintBarStatic(Painter & sw)
{
	Size size = GetSize();

	if(vertical)
	{
		if(!reverse)
		{
			sw.DrawRect(px,py   ,size.cx,ly,style->backcol);
			sw.DrawRect(px,py+ly,size.cx,dy,style->forecol);
		}
		else
		{
			sw.DrawRect(px,py+dy,size.cx,ly,style->backcol);
			sw.DrawRect(px,py   ,size.cx,dy,style->forecol);
		}
	}
	else //horizontal
	{
		if(!reverse)
		{
			sw.DrawRect(px+dx,py,lx,size.cy,style->backcol);
			sw.DrawRect(px,py   ,dx,size.cy,style->forecol);
		}
		else
		{
			sw.DrawRect(px,py   ,lx,size.cy,style->backcol);
			sw.DrawRect(px+lx,py,dx,size.cy,style->forecol);
		}
	}
}

void CMeter::CreateBack()
{
	Size size = GetSize();
	ImageDraw ddraw(size); //create a background image that will be shown
	//ddraw.DrawRect(0,0,size.cx,size.cy,LtGreen);

	if(vertical)
	{
		if(!reverse)
			for(int i = 0; i < size.cy; i++)
			{
				Color col = Blend(style->forecol2, style->forecol, (256 * i / size.cy));
				ddraw.DrawLine(0,i,size.cx,i,1,((i%2)?(style->backcol):(col)));
			}
		else	
			for(int i = 0; i < size.cy; i++)
			{
				Color col = Blend(style->forecol, style->forecol2, (256 * i / size.cy));
				ddraw.DrawLine(0,i,size.cx,i,1,((i%2)?(style->backcol):(col)));
			}
	}
	else
	{
		if(!reverse)
			for(int i = 0; i < size.cx; i++)
			{
				Color col = Blend(style->forecol, style->forecol2, (256 * i / size.cx));
				ddraw.DrawLine(i,0,i,size.cy,1,((i%2)?(style->backcol):(col)));
			}
		else	
			for(int i = 0; i < size.cx; i++)
			{
				Color col = Blend(style->forecol2, style->forecol, (256 * i / size.cx));
				ddraw.DrawLine(i,0,i,size.cy,1,((i%2)?(style->backcol):(col)));
			}
	}
	
	draw = ddraw; //make the image
}

void CMeter::PaintBarImage(Painter & sw)
{
	Size size = GetSize();

	if(draw.GetSize() != size)
		CreateBack();
	
	if(vertical)
	{
		if(!reverse)
		{
			sw.DrawRect(px,py   ,size.cx,ly,style->backcol);

			//sw.DrawRect(px,py+ly,size.cx,dy,forecol);
			Point p(px, py+ly);
			Size s(size.cx, dy);
			Rect r(p,s);
			sw.ClipOp(r);
			sw.DrawImage(0,0, draw);
			sw.End();
		}
		else
		{
			sw.DrawRect(px,py+dy,size.cx,ly,style->backcol);

			//sw.DrawRect(px,py   ,size.cx,dy,forecol);
			Point p(px, py);
			Size s(size.cx, dy);
			Rect r(p,s);
			sw.ClipOp(r);
			sw.DrawImage(0,0, draw);
			sw.End();
		}
	}
	else //horizontal
	{
		if(!reverse)
		{
			sw.DrawRect(px+dx,py,lx,size.cy,style->backcol);

			//sw.DrawRect(px,py   ,dx,size.cy,forecol);
			Point p(px, py);
			Size s(dx, size.cy);
			Rect r(p,s);
			sw.ClipOp(r);
			sw.DrawImage(0,0, draw);
			sw.End();
		}
		else
		{
			sw.DrawRect(px,py   ,lx,size.cy,style->backcol);

			//sw.DrawRect(px+lx,py,dx,size.cy,forecol);
			Point p(px+lx, py);
			Size s(dx, size.cy);
			Rect r(p,s);
			sw.ClipOp(r);
			sw.DrawImage(0,0, draw);
			sw.End();
		}
	}
}

void CMeter::PaintCVMarks(Painter & sw)
{
	Size size = GetSize();

	if(cvmarks.GetCount()>0)
	{
		Vector<Point> p;
		if(vertical)
		{
			sw.DrawRect(0,0,px,size.cy,style->backcol);
			for(int i = 0; i<cvmarks.GetCount(); i++)
			{
				CVMark & cm = cvmarks[i];
				if(cm.v > max || cm.v < min) continue;
				int pos;
				if(stepping && steplin)
				{
					//pos = ValToPix((float)i);
				}
				else
					pos = ValToPix(cm.v);
				p.Clear(); p << Point(px, pos) << Point(0, pos-(_py>>1) ) << Point(0, pos-(_py>>1)+_py);
				sw.DrawPolygon(p, cm.col);
			}
		}
		else
		{
			sw.DrawRect(0,0,size.cx,py,style->backcol);
			for(int i = 0; i<cvmarks.GetCount(); i++)
			{
				CVMark & cm = cvmarks[i];
				if(cm.v > max || cm.v < min) continue;
				int pos;
				if(stepping && steplin)
				{
					//pos = ValToPix((float)i);
				}
				else
					pos = ValToPix(cm.v);
				p.Clear(); p << Point(pos, py) << Point(pos-(_px>>1), 0) << Point(pos-(_px>>1)+_px, 0);
				sw.DrawPolygon(p, cm.col);
			}
		}
		
	}
}

void CMeter::PaintValue(Painter & sw)
{
	Size size = GetSize();

	if(showpercent)
	{
		if(size.cx >= tlen.cx && size.cy >= tlen.cy) //only if fits into window
		{
			sw.DrawText(px+(size.cx>>1)-(tlen.cx>>1),py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		}
		else
		{
			sw.DrawText(2, py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		}
	}
	else
	{
		if(size.cx >= tlen.cx && size.cy >= tlen.cy) //only if fits into window
		{
			sw.DrawText(px+(size.cx>>1)-(tlen.cx>>1),py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		}
		else
		{
			sw.DrawText(2, py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		}
	}
}

void CMeter::DoPaint0(Painter& sw)
{
	PaintCVMarks(sw);

	switch(drawmode)
	{
		case IMGCACHE:
			PaintBarImage(sw);
			break;
		case STATIC:
			PaintBarStatic(sw);	
			break;
		case GRADIENT:
			PaintBarGradient(sw, true);
			break;
		default:
		case DYNGRADIENT:
			PaintBarGradient(sw, false);
			break;
	}
	
	if(showvalue)
		PaintValue(sw);
}

void CMeter::DoPaint(Painter& sw)
{
	//sw.Translate(~ctrl.translate_x, ~ctrl.translate_y);
	//sw.Rotate(~ctrl.rotate);
	//sw.Scale(~ctrl.scale, (double)~ctrl.scale * (double)~ctrl.scale_x);
	//sw.Opacity(~ctrl.opacity);
	//sw.LineCap(~ctrl.linecap);
	//sw.LineJoin(~ctrl.linejoin);
	{ 
	//	//PAINTER_TIMING("FILL");
	//	if(ctrl.transparent)
	//		sw.Clear(RGBAZero());
	//	else
			//sw.Clear(White());
	}

	DoPaint0(sw);
}

void CMeter::Paint(Draw& w)
{
	Size sz = GetSize();
/*
	ImageBuffer ib(sz);
	BufferPainter sw(ib, 0);
	DoPaint(sw);
	w.DrawImage(0, 0, ib);
*/
	DrawPainter dp(w, sz);
	DoPaint(dp);
	
	if(HasFocus())
		DrawFocus(w, sz);
}

void CMeter::GotFocus()
{
	Refresh();
}

void CMeter::LostFocus()
{
	Refresh();
}

//controlling

int CMeter::ValToPix(float _val)
{
	float pix = 0; 
	Size s = GetSize();

	if(stepping && steplin)
	{
		if(vertical)
		{
			if(!reverse)
			{
				pix = (float)s.cy - (_val+1) * (float)(s.cy)/(vsteps.GetCount());
			}
			else
			{
				pix = (_val+1) * (float)(s.cy)/(vsteps.GetCount());
			}
		}
		else
		{
			if(!reverse)
			{
				pix = (_val+1) * (float)(s.cx)/(vsteps.GetCount());
			}
			else
			{
				pix = (float)s.cx - (_val+1) * (float)(s.cx)/(vsteps.GetCount());
			}
		}
		return fround(pix);
	}

	if(vertical)
	{
		if(!reverse)
		{
			pix = (float)s.cy - (_val - min) * (float)(s.cy)/(max - min);
		}
		else
		{
			pix = (_val - min) * (float)(s.cy)/(max - min);
		}
	}
	else
	{
		if(!reverse)
		{
			pix = (_val - min) * (float)(s.cx)/(max - min);
		}
		else
		{
			pix = (float)s.cx - (_val - min) * (float)(s.cx)/(max - min);
		}
	}
	
	return fround(pix);
}

float CMeter::PointToVal(Point & p)
{
	Size s = GetSize();
	
	if(stepping && steplin)
	{
		float _i = 0;
		if(vertical)
		{
			if(!reverse)
			{
				_i = -1 + (float)vsteps.GetCount() * (float)(s.cy - p.y)/(float)s.cy;
			}
			else
			{
				_i = -1 + (float)vsteps.GetCount() * (float)p.y/(float)s.cy;
			}
		}
		else
		{
			if(!reverse)
			{
				_i = -1 + (float)vsteps.GetCount() * (float)p.x/(float)s.cx;
			}
			else
			{
				_i = -1 + (float)vsteps.GetCount() * (float)(s.cx - p.x)/(float)s.cx;
			}
		}
		return _i;
	}

	float _val = min;
	if(vertical)
	{
		if(!reverse)
		{
			_val = min + (max - min) * (float)(s.cy - p.y)/(float)s.cy;
		}
		else
		{
			_val = min + (max - min) * (float)p.y/(float)s.cy;
		}
	}
	else
	{
		if(!reverse)
		{
			_val = min + (max - min) * (float)p.x/(float)s.cx;
		}
		else
		{
			_val = min + (max - min) * (float)(s.cx - p.x)/(float)s.cx;
		}
	}
	
	return _val;
}

bool CMeter::SetDirect(Point p, dword keyflags)
{
	float newval = PointToVal(p);
	if(stepping && steplin)
	{
		int i = (int)newval;//fround(newval);
		if(i<0)
			Set(min);
		else
		if(i>=vsteps.GetCount())
			Set(max);
		else
			Set(vsteps[i]);
	}
	else
		Set(newval);
	return true;
}

bool CMeter::SetFollow(float _val)
{
	float newval;
	
	if(stepping && steplin)
	{
//		if(steplin)
//		{
			int _ii = fround(_val);
			int newii;
			
			if(__ii < _ii)
			{
				//inc
				newii = __ii + inci;
				if(newii>_ii) { newii = _ii; increment = 1; }
			}
			else
			if(__ii > _ii)
			{
				//dec
				newii = __ii - inci;
				if(newii<_ii) { newii = _ii; increment = 1; }
			}
			else
				return false;
			__ii = newii; //cache

			if(newii<0)
				newval = min;
			else
			if(newii>=vsteps.GetCount())
				newval = max;
			else
			newval = vsteps[newii];	
//		}
/*		else
		{
			if(__val < _val)
			{
				//inc
				ii += inci;
				if(ii>=vsteps.GetCount()) { ii = vsteps.GetCount()-1; increment = 1; }
			}
			else
			if(__val > _val)
			{
				//dec
				ii -= inci;
				if(ii<-1) { ii = -1; increment = 1; }
			}
			else
				return false;
			__val = newval; //cache against oszillation, save unstepped, desired value, compared against in SetFollow

			if(ii<0)
				newval = min;
			else
			if(ii>=vsteps.GetCount())
				newval = max;
			else
			newval = vsteps[ii];	
		}
*/
	}
	else
	{
		//normal
		if(__val < _val)
		{
			//inc
			newval = __val + incv;
			if(newval > _val) { newval = _val; increment = 1; } //dont pass target, leave repeat active
		}
		else
		if(__val > _val)
		{
			//dec
			newval = __val - incv;
			if(newval < _val) { newval = _val; increment = 1; } //dont pass target, leave repeat active
		}
		else
			return false;
		//__val verwendung hier nur wegen stepping gemeinsam
		__val = newval; //cache against oszillation, save unstepped, desired value, compared against in SetFollow

	}

	Set(newval); //set ourselves again, so refreshing is done
	return true;
}

bool CMeter::SetFollow(Point p, dword keyflags)
{
	//stepping but need to set the stepped values
	float _val = PointToVal(p); //targetet value
	return SetFollow(_val);
}
void CMeter::LeftDown(Point p, dword keyflags)
{
	if(!IsEditable()) return;
	if(!HasFocus())
		SetFocus();
	SetCapture();
}

float CMeter::CalcIncV()
{
	switch(increment++)
	{
		//case 5: increment = -1; //stop
		//	return;
		case 0:
			incv = (float)(max - min) * (float)1 / 1000.0f;
			inci = 1;
			break;
		case 10:
			incv = (float)(max - min) * (float)increment / 1000.0f;
			inci = 2;
			break;
		case 20:
			incv = (float)(max - min) * (float)increment *2.0f / 1000.0f;
			inci = 3;
			break;
		default:
			break; //keep last
	}
	return incv;	
}

void CMeter::LeftRepeat(Point p, dword keyflags)
{
	if(!IsEditable()) return;
	//here we increment

	if(increment < 0)
		return; //we finished inc already

	CalcIncV();

	if(SetFollow(p, keyflags))
		Action();
}

void CMeter::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	if(!IsEditable()) return;
	if(increment==0) //we had no inc, so we just set
	{
		if(clickincdec && !(keyflags & K_CTRL))
		{
			LeftRepeat(p, keyflags);			
		}
		else
		{
			if(SetDirect(p, keyflags))
				Action();
		}
	}
	else
	{
		//is incing or has finished, we may not change the value	
	}
	increment = 0; //may start another one
}

void CMeter::MouseMove(Point p, dword keyflags)
{
	//calculate value and set it, will cause a repaint
	if(!HasFocus()) return;
	if(!IsEditable()) return;
	if(!(keyflags & K_MOUSELEFT)) return;
	
	if(clickincdec) return;
	
	if(increment <=0) //had no inc or is done or we desabled it here due to early move
	{
		increment = -1; //once we move, we dont inc
		if(SetDirect(p, keyflags))
			Action();
		return;
	}
	//else //inc already started, no action then, keep incing
}

void CMeter::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(!IsEditable()) return;
	if(increment != 0) //inc in progress, ignore
	{
		return;
	}

	//if(!HasFocus()) SetFocus();
	
	if(zdelta>0)
		Key(keyflags | K_UP, zdelta/120);
	else
		Key(keyflags | K_DOWN, -zdelta/120); //always needs positive repeat
}

bool CMeter::Key0(dword key, int rep)
{
	if(!IsEditable()) return false;
	if(increment != 0) //inc in progress, ignore
	{
		return false;
	}

	bool select = key & K_SHIFT;
	bool incb = true;

	incv = inc * rep;
	inci = rep;
	if(select)
	{
		incv *= 10.0f;
		inci *= 2;
	}

	switch(key & ~K_SHIFT)
	{
	case K_RIGHT:
	case K_ADD:
	case K_UP:
		incb = true;
		break;
	case K_SUBTRACT:
	case K_LEFT:
	case K_DOWN:
		incb = false;
		break;
	case K_PAGEUP:
		incv *= 10.0f;
		inci *= 2;
		incb = true;
		break;
	case K_PAGEDOWN:
		incv *= 10.0f;
		inci *= 2;
		incb = false;
		break;
	default:
		return false;
	}

	if(incb)
	{
	if(stepping && steplin)
		return SetFollow((float)0);
	else
		return SetFollow(min);
	}
	else
	{
	if(stepping && steplin)
		return SetFollow((float)vsteps.GetCount());
	else
		return SetFollow(max);
	}
	return true;
}

bool CMeter::Key(dword key, int rep)
{
	bool ret = Key0(key, rep);
	if(ret)
		Action();
	return ret;
}

void CMeter::Inc()
{
	Key0(K_UP, 1);
}

void CMeter::Dec()
{
	Key0(K_DOWN, 1);
}
