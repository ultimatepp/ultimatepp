#ifndef _plugin_cairo_CairoCtrl_h_
#define _plugin_cairo_CairoCtrl_h_

#include <CtrlLib/CtrlLib.h>
#include "Cairo.h"
#include "CairoDraw.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#include "lib/cairo-win32.h"

class CairoCtrl : public DHCtrl 
{
private:
	Drawing const *_drawing;
	Color const *_backgroundColor;
	HDC _hDC;
	Cairo _cairo;
	bool _doubleBuffering;
	
	void DoPaint()
	{
		if (!_cairo.HasContext())
			return;
		
		CairoDraw draw(_cairo);
		
		if (_drawing == NULL)
		{
			if (_backgroundColor != NULL)
				_cairo.Clear(*_backgroundColor);
			
			CairoPaint(_cairo);
			UppPaint(draw);
		}
		else
		{
			_cairo.Clear(_backgroundColor == NULL ? White() : *_backgroundColor);
			
			Size drawingSize = _drawing->GetSize();
			Size size = GetSize();
			Size drawSize = size;
			int dx = 0, dy = 0;
			
			if(drawingSize.cy * size.cx < size.cy * drawingSize.cx) 
			{
				drawSize.cy = drawingSize.cy * size.cx / drawingSize.cx;
				dy = (size.cy - drawSize.cy) / 2;
			}
			else 
			{
				drawSize.cx = drawingSize.cx * size.cy / drawingSize.cy;
				dx = (size.cx - drawSize.cx) / 2;
			}
			
			draw.Clipoff(dx, dy, drawSize.cx, drawSize.cy);
			draw.DrawDrawing(drawSize, *_drawing);
			draw.End();
		}
		
		if (_doubleBuffering)
			BitBlt(_hDC, Point(0, 0), cairo_win32_surface_get_dc(_cairo.GetSurface()), GetSize());
	}

protected:
	// Called on paint events
	virtual void CairoPaint(Cairo &cairo) { WhenCairoPaint(cairo); }
	virtual void UppPaint(Draw &draw) { WhenPaint(draw); }

public:
	Callback1<Cairo &> WhenCairoPaint;
	Callback1<Draw &> WhenPaint;

	CairoCtrl()
		: _drawing(NULL), 
		  _hDC(NULL), 
		  _doubleBuffering(true), 
		  _backgroundColor(NULL)
	{}
	
	~CairoCtrl()
	{
		if (_hDC != NULL)
			ReleaseDC(GetHWND(), _hDC);
		
		if (_backgroundColor != NULL)
			delete _backgroundColor;
	}
	
	void Set(Drawing const * const drawing) 
	{
		_drawing = drawing;
		Refresh();
	}
	
	void Background(Color const &color)
	{
		if (_backgroundColor != NULL)
			delete _backgroundColor;
		
		_backgroundColor = new Color(color);
	}
	
	void SetDoubleBuffering(bool buffer = true)
	{
		_doubleBuffering = buffer;
		State(LAYOUTPOS);
	}
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(message == WM_PAINT) 
		{
			PAINTSTRUCT ps;
			BeginPaint(GetHWND(), &ps);
			DoPaint();
			EndPaint(GetHWND(), &ps);
			return 0;
		}
		else if(message == WM_ERASEBKGND)
			return 1;
		
		return DHCtrl::WindowProc(message, wParam, lParam);
	}
	
	virtual void State(int reason)
	{
		DHCtrl::State(reason);
		
		if (reason == OPEN || reason == LAYOUTPOS)
		{
			HWND hwnd = GetHWND();
			if (hwnd != NULL)
				_hDC = ::GetDC(hwnd);
			
			if (_hDC != NULL)
			{
				if (_doubleBuffering)
					_cairo.SetSurface(cairo_win32_surface_create_with_ddb(_hDC, CAIRO_FORMAT_RGB24, GetSize().cx, GetSize().cy), GetSize().cx, GetSize().cy);
				else
					_cairo.SetSurface(cairo_win32_surface_create(_hDC), GetSize().cx, GetSize().cy);
			}
		}
		else if (reason == CLOSE && _hDC != NULL)
		{
			ReleaseDC(GetHWND(), _hDC);
			_hDC = NULL;
		}
	}
};

#else

class CairoCtrl : public Ctrl
{
private:
	Drawing const *_drawing;
	Color const *_backgroundColor;
	Cairo _cairo;
	ImageBuffer _buffer;
	
	void DoPaint()
	{
		if (!_cairo.HasContext())
			return;
		
		CairoDraw draw(_cairo);
		
		if (_drawing == NULL)
		{
			if (_backgroundColor != NULL)
				_cairo.Clear(*_backgroundColor);
			
			CairoPaint(_cairo);
			UppPaint(draw);
		}
		else
		{
			_cairo.Clear(_backgroundColor == NULL ? White() : *_backgroundColor);
			
			Size drawingSize = _drawing->GetSize();
			Size size = GetSize();
			Size drawSize = size;
			int dx = 0, dy = 0;
			
			if(drawingSize.cy * size.cx < size.cy * drawingSize.cx) 
			{
				drawSize.cy = drawingSize.cy * size.cx / drawingSize.cx;
				dy = (size.cy - drawSize.cy) / 2;
			}
			else 
			{
				drawSize.cx = drawingSize.cx * size.cy / drawingSize.cy;
				dx = (size.cx - drawSize.cx) / 2;
			}
			
			draw.Clipoff(dx, dy, drawSize.cx, drawSize.cy);
			draw.DrawDrawing(drawSize, *_drawing);
			draw.End();
		}
	}
	
	virtual void Paint(Draw& draw)
	{
		if (GetSize() != _buffer.GetSize())
		{
			_buffer.Create(GetSize());
			_cairo.SetSurface(_buffer);
		}
		
		if (_buffer.IsEmpty())
			return;
		
		DoPaint();
		
		Image i = _buffer;
		draw.DrawImage(0, 0, i);
		_buffer = i;
	}

protected:
	// Called on paint events
	virtual void CairoPaint(Cairo &cairo) { WhenCairoPaint(cairo); }
	virtual void UppPaint(Draw &draw) { WhenPaint(draw); }

public:
	Callback1<Cairo &> WhenCairoPaint;
	Callback1<Draw &> WhenPaint;

	CairoCtrl()
		: _drawing(NULL), 
		  _backgroundColor(NULL)
	{}
	
	~CairoCtrl()
	{
		if (_backgroundColor != NULL)
			delete _backgroundColor;
	}
	
	void Set(Drawing const * const drawing) 
	{
		_drawing = drawing;
		Refresh();
	}
	
	void Background(Color const &color)
	{
		if (_backgroundColor != NULL)
			delete _backgroundColor;
		
		_backgroundColor = new Color(color);
	}
	
	void SetDoubleBuffering(bool buffer = true) {}
};

#endif

END_UPP_NAMESPACE

#endif
