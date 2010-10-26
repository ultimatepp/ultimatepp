#include "CMeter.h"

CH_STYLE(CMeter, Style, StyleDefault)
{
	backcol = SColorFace();
	textcol = SColorText();
	forecol = LtBlue();
	forecol2 = LtRed();
	cmarksize = 3;
}

CMeter::CVMark::CVMark(double _v, String _name, Color _col)
	:  v(_v)
	, col(_col)
	, name(_name)
{
}

CMeter::CMeter(double _min, double _max, bool _vertical, bool _reverse)
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

	, incv(0.0)
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
	cvmarks.Add(CVMark(0.0));
	SetStyle(StyleDefault());
	Set((max-min)/2.0);		
}

CMeter& CMeter::SetStyle(const Style& s)
{
	style = &s;
	RefreshLayout();
	return *this;
}

int CMeter::FindStep(double v) const
{
	double _v = min;
	int i = vsteps.GetCount()-1;
	while(i>=0)
	{
		if(vsteps[i] <= v)
		{
			_v = vsteps[i]; break;
		}
		--i;
	}
	return i;
}

CMeter & CMeter::Set(double _val)
{
	val = minmax(_val, min, max);
	__val = val; //for cache

	if(stepping)
	{
		int i = FindStep(val);
		val = min;
		pp = 0;
		__ii = i; //needed for inc dec, NO oszillates
		if(i>=0)
		{
			val = vsteps[i];
			if(steplin) pp = (double)(i+1)/(double)vsteps.GetCount();
			else pp = (val - min) / (max - min);
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

CMeter & CMeter::Min(double _min)
{
	min = _min;
	if(val < min) val = min;
	Update();
	return *this;
}

CMeter & CMeter::Max(double _max)
{
	max = _max;
	if(val > max) val = max;
	Update();
	return *this;
}

CMeter & CMeter::MinMax(double _min, double _max)
{
	min = _min;
	max = _max;
	val = minmax(val, min, max);
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

CMeter & CMeter::Steps(const Vector<double> & _vsteps, bool reverse)
{
	vsteps <<= _vsteps;
	if(vsteps.IsEmpty()) vsteps << max; //needs max at least
	if(reverse)
	{
		int i = 0, j = vsteps.GetCount()-1;
		while(i<j) vsteps.Swap(i++, j--);
	}
	Layout();
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

void CMeter::Update()
{
	if(showpercent)
	{
		(s = Format("%0n", (100.0 * pp))) << " %";
		tlen = GetTextSize(s,font,s.GetLength());
	}
	else
	{
		s = Format("%1!n", val);
		tlen = GetTextSize(s,font,s.GetLength());
	}
	Size size = GetSize();

	//drawing will use what it needs
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
			if(staticgradient) sw.Fill(px,py   ,style->forecol2,px/*+size.cx*/,py+size.cy,style->forecol);
			else sw.Fill(px,py+ly,style->forecol2,px/*+size.cx*/,py+size.cy,style->forecol);
		}
		else
		{
			sw.DrawRect(px,py+dy,size.cx,ly,style->backcol);
			sw.Rectangle(px,py,size.cx,dy);
			if(staticgradient) sw.Fill(px,py,style->forecol,px/*+size.cx*/,py+size.cy,style->forecol2);
			else sw.Fill(px,py,style->forecol,px/*+size.cx*/,py+dy,style->forecol2);
		}
	}
	else //horizontal
	{
		if(!reverse)
		{
			sw.DrawRect(px+dx,py,lx,size.cy, style->backcol);//.Fill(px+dx,py,backcol,px+size.cx,py+size.cy,backcol);
			sw.Rectangle(px,py,dx,size.cy);
			if(staticgradient) sw.Fill(px,py,style->forecol,px+size.cx,py/*+size.cy*/,style->forecol2);
			else sw.Fill(px,py,style->forecol,px+dx,py/*+size.cy*/,style->forecol2);
		}
		else
		{
			sw.DrawRect(px,py,lx,size.cy,style->backcol);
			sw.Rectangle(px+lx,py,dx,size.cy);
			if(staticgradient) sw.Fill(px   ,py,style->forecol2,px+size.cx,py/*+size.cy*/,style->forecol);
			else sw.Fill(px+lx,py,style->forecol2,px+size.cx,py/*+size.cy*/,style->forecol);
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
	ImageDraw ddraw(size);
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
	if(draw.GetSize() != size) CreateBack();
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
	if(cvmarks.GetCount()<=0) return;
	Size size = GetSize();
	Vector<Point> p;
	if(vertical)
	{
		sw.DrawRect(0,0,px,size.cy,style->backcol);
		for(int i = 0; i<cvmarks.GetCount(); i++)
		{
			CVMark & cm = cvmarks[i];
			if((!stepping || !steplin) && (cm.v > max || cm.v < min)) continue;
			int pos = ValToPix(cm.v);
			p.Clear(); p << Point(px, pos) << Point(0, pos-(_py>>1) ) << Point(0, pos+(_py>>1));
			sw.DrawPolygon(p, cm.col);

			String st = ((cm.name.IsEmpty())?(FormatDouble(cm.v)):(cm.name));
			Size tsz = GetTextSize(st, font);
			
			int dd = 0;
			if(pos < (tsz.cy>>1)) dd = 0;
			else if((size.cy - pos) < (tsz.cy>>1)) dd = tsz.cy;
			else dd = (tsz.cy>>1);
			
			sw.DrawText(px, pos-dd, st, font);
		}
	}
	else
	{
		sw.DrawRect(0,0,size.cx,py,style->backcol);
		for(int i = 0; i<cvmarks.GetCount(); i++)
		{
			CVMark & cm = cvmarks[i];
			if((!stepping || !steplin) && (cm.v > max || cm.v < min)) continue;
			int pos = ValToPix(cm.v);
			p.Clear(); p << Point(pos, py) << Point(pos-(_px>>1), 0) << Point(pos+(_px>>1), 0);
			sw.DrawPolygon(p, cm.col);

			String st = ((cm.name.IsEmpty())?(FormatDouble(cm.v)):(cm.name));
			Size tsz = GetTextSize(st, font);

			int dd = 0;
			if(pos < (tsz.cy>>1)) dd = tsz.cy;
			else if((size.cx - pos) < (tsz.cy>>1)) dd = 0;
			else dd = (tsz.cy>>1);

			sw.DrawText(pos+dd, py, 2700, st, font);
		}
	}
}

void CMeter::PaintValue(Painter & sw)
{
	Size size = GetSize();

	if(showpercent)
	{
		if(size.cx >= tlen.cx && size.cy >= tlen.cy) //only if fits into window
			sw.DrawText(px+(size.cx>>1)-(tlen.cx>>1),py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		else sw.DrawText(2, py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
	}
	else
	{
		if(size.cx >= tlen.cx && size.cy >= tlen.cy) //only if fits into window
			sw.DrawText(px+(size.cx>>1)-(tlen.cx>>1),py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
		else sw.DrawText(2, py+(size.cy>>1)-(tlen.cy>>1),s, font, style->textcol);
	}
}

void CMeter::DoPaint0(Painter& sw)
{
	switch(drawmode)
	{
		case IMGCACHE: PaintBarImage(sw); break;
		case STATIC: PaintBarStatic(sw); break;
		case GRADIENT: PaintBarGradient(sw, true); break;
		default:
		case DYNGRADIENT: PaintBarGradient(sw, false); break;
	}
	PaintCVMarks(sw);
	if(showvalue) PaintValue(sw);
}

void CMeter::DoPaint(Painter& sw)
{
	//sw.Translate(~ctrl.translate_x, ~ctrl.translate_y);
	//sw.Rotate(~ctrl.rotate);
	//sw.Scale(~ctrl.scale, (double)~ctrl.scale * (double)~ctrl.scale_x);
	//sw.Opacity(~ctrl.opacity);
	//sw.LineCap(~ctrl.linecap);
	//sw.LineJoin(~ctrl.linejoin);
	//{
	//	//PAINTER_TIMING("FILL");
	//	if(ctrl.transparent)
	//		sw.Clear(RGBAZero());
	//	else
	//		sw.Clear(White());
	//}

	DoPaint0(sw);
}

void CMeter::Paint(Draw& w)
{
	Size sz = GetSize();
	DrawPainter dp(w, sz);
	DoPaint(dp);
	if(HasFocus()) DrawFocus(w, sz);
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

int CMeter::ValToPix(double _val)
{
	double pix = 0; 
	Size s = GetSize();
	if(stepping && steplin)
	{
		if(vertical)
		{
			if(!reverse) pix = (double)s.cy - (_val+1) * (double)(s.cy)/(vsteps.GetCount());
			else pix = (_val+1) * (double)(s.cy)/(vsteps.GetCount());
		}
		else
		{
			if(!reverse) pix = (_val+1) * (double)(s.cx)/(vsteps.GetCount());
			else pix = (double)s.cx - (_val+1) * (double)(s.cx)/(vsteps.GetCount());
		}
		return fround(pix);
	}

	if(vertical)
	{
		if(!reverse) pix = (double)s.cy - (_val - min) * (double)(s.cy)/(max - min);
		else pix = (_val - min) * (double)(s.cy)/(max - min);
	}
	else
	{
		if(!reverse) pix = (_val - min) * (double)(s.cx)/(max - min);
		else pix = (double)s.cx - (_val - min) * (double)(s.cx)/(max - min);
	}
	
	return fround(pix);
}

double CMeter::PointToVal(Point & p)
{
	Size s = GetSize();
	
	if(stepping && steplin)
	{
		double _i = 0;
		if(vertical)
		{
			if(!reverse) _i = -1 + (double)vsteps.GetCount() * (double)(s.cy - p.y)/(double)s.cy;
			else _i = -1 + (double)vsteps.GetCount() * (double)p.y/(double)s.cy;
		}
		else
		{
			if(!reverse) _i = -1 + (double)vsteps.GetCount() * (double)p.x/(double)s.cx;
			else _i = -1 + (double)vsteps.GetCount() * (double)(s.cx - p.x)/(double)s.cx;
		}
		return _i;
	}

	double _val = min;
	if(vertical)
	{
		if(!reverse) _val = min + (max - min) * (double)(s.cy - p.y)/(double)s.cy;
		else _val = min + (max - min) * (double)p.y/(double)s.cy;
	}
	else
	{
		if(!reverse) _val = min + (max - min) * (double)p.x/(double)s.cx;
		else _val = min + (max - min) * (double)(s.cx - p.x)/(double)s.cx;
	}
	return _val;
}

bool CMeter::SetDirect(Point p, dword keyflags)
{
	double newval = PointToVal(p);
	if(stepping && steplin)
	{
		int i = (int)newval;//not fround(newval);
		if(i<0) Set(min);
		else if(i>=vsteps.GetCount()) Set(max);
		else Set(vsteps[i]);
	}
	else Set(newval);
	return true;
}

bool CMeter::SetFollow(double _val)
{
	double newval;
	if(stepping && steplin)
	{
		int _ii = fround(_val);
		int newii;
		
		if(__ii < _ii)
		{
			newii = __ii + inci;
			if(newii>_ii) { newii = _ii; increment = 1; } //dont pass target, leave repeat active
		}
		else
		if(__ii > _ii)
		{
			newii = __ii - inci;
			if(newii<_ii) { newii = _ii; increment = 1; }
		}
		else return false;
		__ii = newii; //cache
		if(newii<0) newval = min;
		else if(newii>=vsteps.GetCount()) newval = max;
		else newval = vsteps[newii];	
	}
	else
	{
		if(__val < _val)
		{
			newval = __val + incv;
			if(newval > _val) { newval = _val; increment = 1; } //dont pass target, leave repeat active
		}
		else
		if(__val > _val)
		{
			newval = __val - incv;
			if(newval < _val) { newval = _val; increment = 1; }
		}
		else return false;
		__val = newval; //cache against oszillation, save unstepped, desired value,
	}
	Set(newval);
	return true;
}

bool CMeter::SetFollow(Point p, dword keyflags)
{
	double _val = PointToVal(p);
	return SetFollow(_val);
}

void CMeter::LeftDown(Point p, dword keyflags)
{
	if(!IsEditable()) return;
	if(!HasFocus()) SetFocus();
	SetCapture();
}

double CMeter::CalcIncV()
{
	switch(increment++)
	{
		case 0: incv = (max - min) * (double)1 / 1000.0f; inci = 1; break;
		case 10: incv = (max - min) * (double)increment / 1000.0f; inci = 2; break;
		case 20: incv = (max - min) * (double)increment *2.0f / 1000.0f; inci = 3; break;
		default: break; //keep last
	}
	return incv;	
}

void CMeter::LeftRepeat(Point p, dword keyflags)
{
	if(!IsEditable()) return;
	if(increment < 0) return; //we finished inc already
	CalcIncV();
	if(SetFollow(p, keyflags)) Action();
}

void CMeter::LeftUp(Point p, dword keyflags)
{
	if(!IsEditable()) return;
	ReleaseCapture();
	if(increment==0) //we had no inc, so we just set
	{
		if(clickincdec && !(keyflags & K_CTRL))
		{
			LeftRepeat(p, keyflags);			
		}
		else
		{
			if(SetDirect(p, keyflags)) Action();
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
	if(!HasFocus()) return;
	if(!IsEditable()) return;
	if(!(keyflags & K_MOUSELEFT)) return;
	
	if(clickincdec) return;
	if(increment <=0) //had no inc or is done or we desabled it here due to early move
	{
		increment = -1; //once we move, we dont inc
		if(SetDirect(p, keyflags)) Action();
		return;
	}
	//else //inc already started, no action then, keep incing
}

void CMeter::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(!IsEditable()) return;
	if(increment != 0) return; //inc in progress, ignore

	//if(!HasFocus()) SetFocus();
	if(zdelta>0) Key(keyflags | K_DOWN, zdelta/120);
	else Key(keyflags | K_UP, -zdelta/120);
}

bool CMeter::Key0(dword key, int rep)
{
	if(!IsEditable()) return false;
	if(increment != 0) return false; //inc in progress, ignore

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
	default: return false;
	}

	if(incb)
	{
	if(stepping && steplin) return SetFollow((double)0);
	else return SetFollow(min);
	}
	else
	{
	if(stepping && steplin) return SetFollow((double)vsteps.GetCount());
	else return SetFollow(max);
	}
	return true;
}

bool CMeter::Key(dword key, int rep)
{
	bool ret = Key0(key, rep);
	if(ret) Action();
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
