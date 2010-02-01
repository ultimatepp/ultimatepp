#include "Draw.h"

NAMESPACE_UPP

String DumpLanguage(int language)
{
	if(IsNull(language))
		return "Null";
	char out[5];
	out[0] = (char)(language >> 24);
	out[1] = (char)(language >> 16);
	out[2] = '-';
	out[3] = (char)(language >> 8);
	out[4] = (char)(language >> 0);
	return String(out, 5);
}

String DumpAlign(int align)
{
	String ss;
	switch(align)
	{
	case ALIGN_NULL:    return "ALIGN_NULL";
	case ALIGN_LEFT:    return "ALIGN_LEFT";
	case ALIGN_CENTER:  return "ALIGN_CENTER";
	case ALIGN_RIGHT:   return "ALIGN_RIGHT";
	case ALIGN_JUSTIFY: return "ALIGN_JUSTIFY";
	default:            return ss << "ALIGNMENT(" << (int)align << ")";
	}
}

String DumpColor(Color color)
{
	if(IsNull(color))
		return "Null";
	int r = color.GetR(), g = color.GetG(), b = color.GetB();
	byte xr = -r, xg = -g, xb = -b;
	static const char *stdc[] = { "Black", "LtBlue", "LtGreen", "LtCyan", "LtRed", "LtMagenta", "Yellow", "White" };
	if((xr | xg | xb) <= 1)
		return stdc[4 * xr + 2 * xg + xb];
	String ss;
	return ss << "Color(" << r << ", " << g << ", " << b << ")";
}

String DumpFont(Font font)
{
	String out;
	if(IsNull(font))
		out << "Font(Null)";
	else
	{
		out << "Font()";
		out << ".Face("       << font.GetFace()     << ")";
		out << ".FaceName(\"" << font.GetFaceName() << "\")";
		out << ".Height("     << font.GetHeight()   << ")";
		out << ".Width("      << font.GetWidth()    << ")";

		if(font.IsItalic())    out << ".Italic()";
		if(font.IsBold())      out << ".Bold()";
		if(font.IsUnderline()) out << ".Underline()";
		if(font.IsStrikeout()) out << ".Strikeout()";
	}
	return out;
}

String DumpFontInfo(FontInfo fi)
{
	String out;
	out << "FontInfo(font = " << fi.GetFont() << "\n"
		"\tempty = " << fi.IsEmpty() << "\n"
		"\tascent = " << fi.GetAscent() << "\n"
		"\tdescent = " << fi.GetDescent() << "\n"
		"\texternal leading = " << fi.GetExternal() << "\n"
		"\tinternal leading = " << fi.GetInternal() << "\n"
		"\theight = " << fi.GetHeight() << "\n"
		"\tline height = " << fi.GetLineHeight() << "\n"
		"\toverhang = " << fi.GetOverhang() << "\n"
		"\taverage width = " << fi.GetAveWidth() << "\n"
		"\tmaximum width = " << fi.GetMaxWidth() << "\n"
		"\tfixed pitch = " << fi.IsFixedPitch() << "\n"
		"\tscaleable = " << fi.IsScaleable() << "\n"
		"\tfont height = " << fi.GetFontHeight() << "\n";

	out << "\n\twidth:\n";
	int i;
	for(i = 0; i < 256; i++)
		out << NFormat(i & 15 ? "  " : "\n\t%02x\t", i) << NFormat("%2>d", fi[i]);
	out << "\n\n\tleft space:\n";
	for(i = 0; i < 256; i++)
		out << NFormat(i & 15 ? "  " : "\n\t%02x\t", i) << NFormat("%2>d", fi.GetLeftSpace(i));
	out << "\n\n\tright space:\n";
	for(i = 0; i < 256; i++)
		out << NFormat(i & 15 ? "  " : "\n\t%02x\t", i) << NFormat("%2>d", fi.GetRightSpace(i));
	out << "\n\n\tkerning:\n";
	int kp = 0;
	for(i = 0; i < 256; i++)
		for(int j = 0; j < 256; j++)
			if(int k = fi.GetKerning(i, j))
				out << (kp++ & 7 ? "  " : "\n\t") << NFormat("(%02x,%02x)=%2>d", i, j, k);
	out << (kp ? "\n" : "\tnone\n");
	return out;
}

#ifdef PLATFORM_X11
enum
{
	GCAllMask = GCFunction
	| GCPlaneMask
	| GCForeground
	| GCBackground
	| GCLineWidth
	| GCLineStyle
	| GCCapStyle
	| GCJoinStyle
	| GCFillStyle
	| GCFillRule
	| GCArcMode
	| GCTile
	| GCStipple
	| GCTileStipXOrigin
	| GCTileStipYOrigin
	| GCFont
	| GCSubwindowMode
	| GCGraphicsExposures
	| GCClipXOrigin
	| GCClipYOrigin
//	| GCClipMask
	| GCDashOffset
//	| GCDashList
};
#endif

#ifdef PLATFORM_X11
String DumpPixmap(Pixmap pixmap)
{
	String out;
	if(pixmap == Pixmap(-1))
		out << "NULL";
	else
	{
		Window root;
		int x, y;
		unsigned width, height, border_width, depth;
		XGetGeometry(Xdisplay, pixmap, &root, &x, &y, &width, &height, &border_width, &depth);
		out << (int)width << " x " << (int)height << ", depth = " << (int)depth;
	}
	return out;
}
#endif

#ifdef PLATFORM_X11
String DumpXFont(XFont font)
{
	XFontStruct *fs = XQueryFont(Xdisplay, font);
	String out = DumpXFontStruct(fs);
	if(fs)
		XFreeFontInfo(NULL, fs, 0);
	return out;
}
#endif

#ifdef PLATFORM_X11
String DumpXFontStruct(XFontStruct *fs)
{
	String out;
	if(!fs)
		out << "NULL\n";
	else
	{
		out <<
		"direction="         << (int)fs -> direction         << "\n"
		"min_char_or_byte2=" << (int)fs -> min_char_or_byte2 << "\n"
		"max_char_or_byte2=" << (int)fs -> max_char_or_byte2 << "\n"
		"min_byte1="         << (int)fs -> min_byte1         << "\n"
		"max_byte1="         << (int)fs -> max_byte1         << "\n"
		"all_chars_exist="   << (int)fs -> all_chars_exist   << "\n"
		"default_char="      << (int)fs -> default_char      << "\n"
		"n_properties="      << (int)fs -> n_properties      << "\n";
		for(int i = 0; i < fs -> n_properties; i++)
		{
			char *name = XGetAtomName(Xdisplay, fs -> properties[i].name);
			out << "properties[" << i << "]=" << name << ", card32 = " << (int)fs -> properties[i].card32 << "\n";
			XFree(name);
		}
		out <<
		"min_bounds=" << (int)fs -> min_bounds.width << " x "
		<< (int)fs -> min_bounds.ascent << " + " << (int)fs -> min_bounds.descent << "\n"
		"max_bounds=" << (int)fs -> max_bounds.width << " x "
		<< (int)fs -> max_bounds.ascent << " + " << (int)fs -> max_bounds.descent << "\n";
		out <<
		"ascent=" << (int)fs -> ascent << "\n"
		"descent=" << (int)fs -> descent << "\n";
	}
	return out;
}
#endif

#ifdef PLATFORM_X11
String DumpGC(GC gc)
{
	XGCValues values;
	Zero(values);
	XGetGCValues(Xdisplay, gc, GCAllMask & ~GCFont, &values);
	String out = DumpGCValues(values);
	XFontStruct *gc_font = XQueryFont(Xdisplay, (XID)gc);
	out << "GCFont=" << DumpXFontStruct(gc_font) << "\n";
	if(gc_font)
		XFreeFontInfo(NULL, gc_font, 0);
	return out;
}
#endif

#ifdef PLATFORM_X11
String DumpGCValues(const XGCValues& gc_values, int mask)
{
	String out;
	if(mask & GCFunction)
	{
		static const char *names[] =
		{
			"X11_ROP2_ZERO",
			"X11_ROP2_AND",
			"X11_ROP2_AND_NOT",
			"X11_ROP2_COPY",
			"X11_ROP2_NOT_AND",
			"X11_ROP2_NOP",
			"X11_ROP2_XOR",
			"X11_ROP2_OR",
			"X11_ROP2_NOT_AND_NOT",
			"X11_ROP2_NOT_XOR",
			"X11_ROP2_INVERT",
			"X11_ROP2_OR_NOT",
			"X11_ROP2_NOT_COPY",
			"X11_ROP2_NOT_OR",
			"X11_ROP2_NOT_OR_NOT",
			"X11_ROP2_ONE",
		};
		ASSERT(__countof(names) == 16);
		out << "GCFunction=" << (int)gc_values.function << ", " << names[gc_values.function & 15] << "\n";
	}
	if(mask & GCPlaneMask)
		out << "GCPlaneMask=" << Format("%08x", (int)gc_values.plane_mask) << "\n";
	if(mask & GCForeground)
		out << "GCForeground=" << Format("%08x", (int)gc_values.foreground) << "\n";
	if(mask & GCBackground)
		out << "GCBackground=" << Format("%08x", (int)gc_values.background) << "\n";
	if(mask & GCLineWidth)
		out << "GCLineWidth=" << (int)gc_values.line_width << "\n";
	if(mask & GCLineStyle)
	{
		out << "GCLineStyle=" << (int)gc_values.line_style << ", ";
		switch(gc_values.line_style)
		{
		case LineSolid:      out << "LineSolid"; break;
		case LineOnOffDash:  out << "LineOnOffDash"; break;
		case LineDoubleDash: out << "LineDoubleDash"; break;
		default:             out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCCapStyle)
	{
		out << "GCCapStyle=" << (int)gc_values.cap_style << ", ";
		switch(gc_values.cap_style)
		{
		case CapNotLast:    out << "CapNotLast"; break;
		case CapButt:       out << "CapButt"; break;
		case CapRound:      out << "CapRound"; break;
		case CapProjecting: out << "CapProjecting"; break;
		default:            out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCJoinStyle)
	{
		out << "GCJoinStyle=" << (int)gc_values.join_style << ", ";
		switch(gc_values.join_style)
		{
		case JoinMiter: out << "JoinMiter"; break;
		case JoinRound: out << "JoinRound"; break;
		case JoinBevel: out << "JoinBevel"; break;
		default:        out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCFillStyle)
	{
		out << "GCFillStyle=" << (int)gc_values.fill_style << ", ";
		switch(gc_values.fill_style)
		{
		case FillSolid:          out << "FillSolid"; break;
		case FillTiled:          out << "FillTiled"; break;
		case FillStippled:       out << "FillStippled"; break;
		case FillOpaqueStippled: out << "FillOpaqueStippled"; break;
		default:                 out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCFillRule)
	{
		out << "GCFillRule=" << (int)gc_values.fill_rule << ", ";
		switch(gc_values.fill_rule)
		{
		case EvenOddRule: out << "EvenOddRule"; break;
		case WindingRule: out << "WindingRule"; break;
		default:          out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCArcMode)
	{
		out << "GCArcMode=" << (int)gc_values.arc_mode << ", ";
		switch(gc_values.arc_mode)
		{
		case ArcChord:    out << "ArcChord"; break;
		case ArcPieSlice: out << "ArcPieSlice"; break;
		default:          out << "unknown"; break;
		}
		out << "\n";
	}
	if(mask & GCTile)
		out << "GCTile=" << DumpPixmap(gc_values.tile) << "\n";
	if(mask & GCStipple)
		out << "GCStipple=" << DumpPixmap(gc_values.stipple) << "\n";
	if(mask & GCTileStipXOrigin)
		out << "GCTileStipXOrigin=" << (int)gc_values.ts_x_origin << "\n";
	if(mask & GCTileStipYOrigin)
		out << "GCTileStipYOrigin=" << (int)gc_values.ts_y_origin << "\n";
	if(mask & GCFont)
		out << "GCFont=" << DumpXFont(gc_values.font) << "\n";
	if(mask & GCSubwindowMode)
	{
		out << "GCSubwindowMode=" << (int)gc_values.subwindow_mode << ", ";
		switch(gc_values.subwindow_mode)
		{
		case ClipByChildren:   out << "ClipByChildren"; break;
		case IncludeInferiors: out << "IncludeInferiors"; break;
		default:               out << "unknown"; break;
		}
	}
	if(mask & GCGraphicsExposures)
		out << "GCGraphicsExposures=" << (gc_values.graphics_exposures ? "true" : "false") << "\n";
	if(mask & GCClipXOrigin)
		out << "GCClipXOrigin=" << (int)gc_values.clip_x_origin;
	if(mask & GCClipYOrigin)
		out << "GCClipYOrigin=" << (int)gc_values.clip_y_origin;
//	if(mask & GCClipMask)
//		out << "GCClipMask=" << DumpPixmap(gc_values.clip_mask);
	if(mask & GCDashOffset)
		out << "GCDashOffset=" << (int)gc_values.dash_offset;
//	if(mask & GCDashList)
//		out << "GCDashList=" << Format("%02x\n", gc_values.dashes);
	return out;
}
#endif

#ifdef PLATFORM_X11
String DumpEvent(XEvent *event)
{
	String out;
	if(!event)
		out << "NULL";
	else
	{
		out << "event_type=" << (int)event -> type << ", ";
		switch(event -> type)
		{
		case KeyPress:
			out << "KeyPress, state=" << (int)event -> xkey.state << ", keycode=" << (int)event -> xkey.keycode;
			break;

		case KeyRelease:
			out << "KeyRelease, state=" << (int)event -> xkey.state << ", keycode=" << (int)event -> xkey.keycode;
			break;

		case ButtonPress:
			out << "ButtonPress, x=" << (int)event -> xbutton.x << ", y=" << (int)event -> xbutton.y << ", state=" << (int)event -> xbutton.state << ", button=" << (int)event -> xbutton.button;
			break;

		case ButtonRelease:
			out << "ButtonRelease, x=" << (int)event -> xbutton.x << ", y=" << (int)event -> xbutton.y << ", state=" << (int)event -> xbutton.state << ", button=" << (int)event -> xbutton.button;
			break;

		case MotionNotify:
			out << "MotionNotify, x=" << (int)event -> xmotion.x << ", y=" << (int)event -> xmotion.y << ", state=" << (int)event -> xmotion.state;
			break;

		case EnterNotify:
		case LeaveNotify:
			out << (event -> type == EnterNotify ? "EnterNotify" : "LeaveNotify") << ", mode=";
			switch(event -> xcrossing.mode)
			{
			case NotifyNormal: out << "NotifyNormal"; break;
			case NotifyGrab:   out << "NotifyGrab"; break;
			case NotifyUngrab: out << "NotifyUngrab"; break;
			default:           out << "unknown"; break;
			}
			out << ", detail=";
			switch(event -> xcrossing.detail)
			{
			case NotifyAncestor:         out << "NotifyAncestor"; break;
			case NotifyVirtual:          out << "NotifyVirtual"; break;
			case NotifyInferior:         out << "NotifyInferior"; break;
			case NotifyNonlinear:        out << "NotifyNonlinear"; break;
			case NotifyNonlinearVirtual: out << "NotifyNonlinearVirtual"; break;
			default:                     out << "unknown (" << (int)event -> xcrossing.detail << ")"; break;
			}
			out << ", same_screen=" << (event -> xcrossing.same_screen ? "true" : "false")
			<< ", focus=" << (event -> xcrossing.focus ? "true" : "false")
			<< ", state=" << (int)event -> xcrossing.state;
			break;

		case FocusIn:
		case FocusOut:
			out << (event -> type == FocusIn ? "FocusIn" : "FocusOut") << ", mode=";
			switch(event -> xfocus.mode)
			{
			case NotifyNormal: out << "NotifyNormal"; break;
			case NotifyGrab:   out << "NotifyGrab"; break;
			case NotifyUngrab: out << "NotifyUngrab"; break;
			default:           out << "unknown"; break;
			}
			out << ", detail=";
			switch(event -> xfocus.detail)
			{
			case NotifyAncestor:         out << "NotifyAncestor"; break;
			case NotifyVirtual:          out << "NotifyVirtual"; break;
			case NotifyInferior:         out << "NotifyInferior"; break;
			case NotifyNonlinear:        out << "NotifyNonlinear"; break;
			case NotifyNonlinearVirtual: out << "NotifyNonlinearVirtual"; break;
			case NotifyPointer:          out << "NotifyPointer"; break;
			case NotifyPointerRoot:      out << "NotifyPointerRoot"; break;
			case NotifyDetailNone:       out << "NotifyDetailNone"; break;
			default:                     out << "unknown (" << (int)event -> xfocus.detail << ")"; break;
			}
			break;

		case KeymapNotify:
			out << "KeymapNotify";
			break;

		case Expose:
			out << "Expose [" << event -> xexpose.x << ", " << event -> xexpose.y
			<< " - " << (event -> xexpose.x + event -> xexpose.width)
			<< ", " << (event -> xexpose.y + event -> xexpose.height) << "], count=" << event -> xexpose.count;
			break;

		case GraphicsExpose:
			out << "GraphicsExpose [" << event -> xgraphicsexpose.x << ", " << event -> xgraphicsexpose.y
			<< " - " << (event -> xgraphicsexpose.x + event -> xgraphicsexpose.width)
			<< ", " << (event -> xgraphicsexpose.y + event -> xgraphicsexpose.height) << "], "
			"count=" << event -> xgraphicsexpose.count
			<< ", major_code=" << (int)event -> xgraphicsexpose.major_code
			<< ", minor_code=" << (int)event -> xgraphicsexpose.minor_code;
			break;

		case NoExpose:
			out << "NoExpose"
			<< ", major_code=" << (int)event -> xnoexpose.major_code
			<< ", minor_code=" << (int)event -> xnoexpose.minor_code;
			break;

		case VisibilityNotify:
			out << "VisibilityNotify, state=" << (int)event -> xvisibility.state;
			break;

		case CreateNotify:
			out << "CreateNotify [" << event -> xcreatewindow.x << ", " << event -> xcreatewindow.y
			<< " - " << (event -> xcreatewindow.x + event -> xcreatewindow.width)
			<< ", " << (event -> xcreatewindow.y + event -> xcreatewindow.height) << "], "
			"border_width=" << event -> xcreatewindow.border_width << ", "
			"override_redirect=" << (event -> xcreatewindow.override_redirect ? "true" : "false");
			break;

		case DestroyNotify:
			out << "DestroyNotify";
			break;

		case UnmapNotify:
			out << "UnmapNotify";
			break;

		case MapNotify:
			out << "MapNotify";
			break;

		case MapRequest:
			out << "MapRequest";
			break;

		case ReparentNotify:
			out << "ReparentNotify";
			break;

		case ConfigureNotify:
			out << "ConfigureNotify";
			break;

		case ConfigureRequest:
			out << "ConfigureRequest";
			break;

		case GravityNotify:
			out << "GravityNotify";
			break;

		case ResizeRequest:
			out << "ResizeRequest";
			break;

		case CirculateNotify:
			out << "CirculateNotify";
			break;

		case CirculateRequest:
			out << "CirculateRequest";
			break;

		case PropertyNotify:
			out << "PropertyNotify";
			break;

		case SelectionClear:
			out << "SelectionClear";
			break;

		case SelectionRequest:
			out << "SelectionRequest";
			break;

		case SelectionNotify:
			out << "SelectionNotify";
			break;

		case ColormapNotify:
			out << "ColormapNotify";
			break;

		case ClientMessage:
			out << "ClientMessage";
			break;

		case MappingNotify:
			out << "MappingNotify";
			break;
		}
	}
	return out;
}
#endif

#ifdef PLATFORM_WIN32
int GdiGetFreeSpace()
{
	Vector<HPEN> objects;
	for(HPEN pen; pen = CreatePen(PS_SOLID, 0, LtGray()); objects.Add(pen))
		;
	for(int i = 0; i < objects.GetCount(); DeleteObject(objects[i++]))
		;
	return objects.GetCount();
}
#endif

END_UPP_NAMESPACE
