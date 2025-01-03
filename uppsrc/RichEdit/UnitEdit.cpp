#include "RichEdit.h"

namespace Upp {

double UnitMultiplier(int unit) {
	static double m[] =
	{
		1, // DOT
		72.0 / 600, // POINT
		1.0 / 600, // INCH
		25.4 / 600, // mm
		2.54 / 600, // cm
		1 / 8.0, // PIXELMODE
	};
	ASSERT(unit >= UNIT_DOT && unit <= UNIT_PIXELMODE);
	return m[unit];
}

const char *UnitText(int unit) {
	static const char *txt[] =
	{
		"dt",
		"pt",
		"\"",
		"mm",
		"cm"
		"",
	};
	return txt[unit];
}

void UnitEdit::Read(double& d, int& u) const
{
	String txt = GetText().ToString();
	const char *s = txt;
	u = unit;
	d = Null;
	if(s && *s) {
		const char *e;
		int sign = 1;
		for(;;) {
			if(*s == '-' && sgn)
				sign = -1;
			else
			if(*s != ' ')
				break;
			s++;
		}
		d = ScanDouble(s, &e);
		if(IsNull(d))
			return;
		d *= sign;
		if(e == s) {
			d = Null;
			return;
		}
		while(*e == ' ') e++;
		if(*e == '\"' || *e == 'i')
			u = UNIT_INCH;
		if(*e == 'm')
			u = UNIT_MM;
		if(*e == 'p' || *e == 'b')
			u = UNIT_POINT;
		if(*e == 'c')
			u = UNIT_CM;
		if(*e == 'd')
			u = UNIT_DOT;
	}
}

Value UnitEdit::GetData() const
{
	double q;
	int u;
	Read(q, u);
	return IsNull(q) ? Null : int(q / UnitMultiplier(u) + 0.5);
}

String UnitEdit::AsText(double d, int unit)
{
	if(IsNull(d))
		return Null;
	if(unit == UNIT_PIXELMODE)
		return AsString(d);
	String utxt = UnitText(unit);
	if(unit == UNIT_POINT)
		d = floor(10 * d + 0.5) / 10;
	return FormatF(d, unit == UNIT_DOT ? 0 : unit == UNIT_MM ? 1 : 2) + ' ' + utxt;
}

String UnitEdit::DotAsText(int dot, int unit)
{
	if(IsNull(dot)) return Null;
	return AsText(dot * UnitMultiplier(unit), unit);
}

Value UnitEdit::Format(const Value& v) const
{
	return DotAsText(v, unit);
}

void UnitEdit::SetData(const Value& v)
{
	SetText(DotAsText(v, unit).ToWString());
}

bool UnitEdit::Key(dword key, int repcnt)
{
	if(key == K_UP)   { Spin(+1); return true; }
	if(key == K_DOWN) { Spin(-1); return true; }
	return EditField::Key(key, repcnt);
}

void UnitEdit::MouseWheel(Point p, int zdelta, dword keyflags)
{
	Spin(zdelta < 0 ? -1 : 1);
}

void UnitEdit::Spin(int delta)
{
	double q;
	int u;
	Read(q, u);
	if(IsNull(q))
		q = 0;
	else {
		double h = 10;
		switch(u) {
		case UNIT_DOT:   h = 10; break;
		case UNIT_POINT: h = 0.5; break;
		case UNIT_MM:    h = 0.5; break;
		case UNIT_CM:
		case UNIT_INCH:  h = 0.05; break;
		case UNIT_PIXELMODE: h = 1; break;
		default:         NEVER();
		}
		h *= delta;
		q = ceil(q / h + 1e-2) * h;
		if(!sgn && q < 0)
			q = 0;
	}
	SetText(AsText(q, unit).ToWString());
	UpdateAction();
}

int CharFilterUnitEdit(int c)
{
	return IsAlpha(c) ? ToLower(c) : IsDigit(c) || c == ' ' || c == '\"' || c == '.' ? c : 0;
}

int CharFilterUnitEditSgn(int c)
{
	return c == '-' ? c : CharFilterUnitEdit(c);
}

int CharFilterUnitEditPx(int c)
{
	return IsDigit(c) || c == '.' ? c : 0;
}

int CharFilterUnitEditSgnPx(int c)
{
	return c == '-' ? c : CharFilterUnitEditPx(c);
}

void UnitEdit::SyncFilter()
{
	if(unit == UNIT_PIXELMODE)
		SetFilter(sgn ? CharFilterUnitEditSgnPx : CharFilterUnitEditPx);
	else
		SetFilter(sgn ? CharFilterUnitEditSgn : CharFilterUnitEdit);
}

UnitEdit& UnitEdit::WithSgn(bool b)
{
	sgn = b;
	SyncFilter();
	return *this;
}

UnitEdit::UnitEdit()
{
	AddFrame(spin);
	spin.inc.WhenRepeat = spin.inc.WhenPush = THISBACK1(Spin, 1);
	spin.dec.WhenRepeat = spin.dec.WhenPush = THISBACK1(Spin, -1);
	unit = UNIT_DOT;
	WithSgn(false);
}

}
