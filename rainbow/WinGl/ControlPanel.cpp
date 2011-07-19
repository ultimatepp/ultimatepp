#include "ControlPanel.h"

NAMESPACE_UPP

ValueSlider::ValueSlider()
{
	pos = 0;
	shaded = true;
	immediate = false;
	src = Color(255, 255, 0);
	dst = Color(255, 153, 51);
}

void ValueSlider::Paint(Draw &w)
{
	Size sz = GetSize();

	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Black);
	w.DrawRect(0, 0, 1, sz.cy, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
		
	int t = (int) (((pos - minValue) * sz.cx) / (maxValue - minValue));
	if(t < 1) t = 1;
	if(t > sz.cx - 1) t = sz.cx - 1;
	
	if(shaded)
	{
		for(int i = 1; i < t; i++)
			w.DrawRect(i, 1, 1, sz.cy - 2, Blend(src, dst, 256 * i / (sz.cx - 1)));
	}
	else
	{
		w.DrawRect(Rect(1, 1, t, sz.cy - 1), dst);
	}

	if(t < sz.cx - 1)
		w.DrawRect(Rect(t, 1, sz.cx - 1, sz.cy - 1), Color(245, 245, 255));

	String s = Format("%s : %.2f", text, pos);
	Size tsz = GetTextSize(s, StdFont());
	w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, s);
}

void ValueSlider::LeftDown(Point p, dword keyflags)
{
	pos = minValue + (p.x * (maxValue - minValue)) / (float) GetSize().cx;
	Refresh();
	SetCapture();
	if(immediate)
		WhenAction();
}

void ValueSlider::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	if(!immediate)
		WhenAction();
}

void ValueSlider::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		pos = minValue + (p.x * (maxValue - minValue)) / (float) GetSize().cx;
		if(pos > maxValue) pos = maxValue;
		if(pos < minValue) pos = minValue;
		Refresh();
		if(immediate)
			WhenAction();
	}	
}

void ValueSlider::SetPos(float p, float minv, float maxv)
{
	pos = p;
	minValue = minv;
	maxValue = maxv;
	Refresh();
}

float ValueSlider::GetPos()
{
	return pos;
}

InfoPanel::InfoPanel() : init(true)
{
	Add(alphaSlider.RightPos(215, 100).BottomPos(2, 24));
	Add(angleSlider.RightPos(110, 100).BottomPos(2, 24));
	Add(scaleSlider.RightPos(5, 100).BottomPos(2, 24));
	
	alphaSlider.text = "Alpha";
	angleSlider.text = "Angle";
	scaleSlider.text = "Scale";
	
	alphaSlider.shaded = true;
	angleSlider.shaded = true;
	scaleSlider.shaded = true;
	alphaSlider.immediate = true;
	angleSlider.immediate = true;
	scaleSlider.immediate = true;
}

void InfoPanel::Paint(Draw& w)
{
	Size sz = GetSize();
	Size wsz = screenRect.GetSize();
	Color frameColor = Color(183, 183, 183);
	Color bgColor = Color(102, 102, 102);
	w.DrawRect(sz, bgColor);
	w.DrawRect(0, 0, 1, sz.cy, frameColor);
	w.DrawRect(0, 0, sz.cx, 1, frameColor);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, frameColor);
	String info = Format("FPS: %.2f, Textures: %d, Size: %d, %d", GetFps(), Resources::textures.GetCount(), wsz.cx, wsz.cy);
	w.DrawText(5, sz.cy - 22, info, StdFont(), White);
}

void InfoPanel::Init(Ctrl& parent, float angle, float scale, float alpha)
{
	int screenWidth = screenRect.Width();
	int width = 585;
	int distance = (screenWidth - width) / 2;
	HSizePos(distance, distance).BottomPos(0, 30);

	if(!init)
		return;
	init = false;
	
	//parent.Add(*this);
	PopUp(&parent, true, false);
	alphaSlider.SetPos(alpha, 0.f, 255.f);
	angleSlider.SetPos(angle, 0.f, 360.f);
	scaleSlider.SetPos(scale, 1.f, 5.f);
}

float InfoPanel::GetAlpha()
{
	return alphaSlider.GetPos();
}

float InfoPanel::GetAngle()
{
	return angleSlider.GetPos();
}

float InfoPanel::GetScale()
{
	return scaleSlider.GetPos();
}



END_UPP_NAMESPACE