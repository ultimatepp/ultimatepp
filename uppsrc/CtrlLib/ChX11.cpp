#include "CtrlLib.h"

#ifdef GUI_X11

namespace Upp {
	
#define LLOG(x)

VectorMap<String, Value> GetXSettings()
{
	VectorMap<String, Value> map;

	Window owner = XGetSelectionOwner(Xdisplay, XAtom("_XSETTINGS_S0"));

	if(owner == None) {
		LLOG("No XSettings manager found for screen " << screen);
		return map;
	}

	Atom settings_atom = XAtom("_XSETTINGS_SETTINGS");
	String data = GetProperty(owner, settings_atom, settings_atom);

	if(data.IsEmpty()) {
		LLOG("Failed to read XSettings property or property is empty.");
		return map;
	}

	const char *ptr = data.Begin();
	const char *lim = data.End();

	if(data.GetCount() < 12)
		return map;
	
	byte be = ptr[0];

	auto Peek32 = [&](const char *p) {
		return be ? Peek32be(p) : Peek32le(p);
	};
	
	auto Peek16 = [&](const char *p) {
		return be ? Peek16be(p) : Peek16le(p);
	};

	int n_settings = Peek32(ptr + 8);
	ptr += 12;

	for(int i = 0; i < n_settings && ptr < lim; i++) {
		if(lim - ptr < 4)
			break;
		
		byte type = *ptr++; // 0 = Integer, 1 = String, 2 = Color
		ptr++; // unused
		
		int name_len = Peek16(ptr);
		ptr += 2;
		if(lim - ptr < name_len)
			break;
		String name(ptr, name_len);
		ptr += (name_len + 3) & ~3;
		
		ptr += 4; // Last-change serial

		if(type == 0) { // Integer
			if(lim - ptr < 4)
				break;
			map.GetAdd(name) = (int)Peek32(ptr);
			ptr += 4;
		}
		else
		if(type == 1) { // String
			if(lim - ptr < 4)
				break;
			int str_len = Peek32(ptr);
			ptr += 4;
			if(lim - ptr < str_len)
				break;
			map.GetAdd(name) = String(ptr, str_len);
			ptr += (str_len + 3) & ~3; // Align to 4 bytes
		}
		else
		if(type == 2) { // Color - ignored
			if(lim - ptr < 8)
				break;
			ptr += 8;
		}
	}
	
	return map;
}

void ChHostSkin()
{
	VectorMap<String, Value> map = GetXSettings();
	try {
		String font_name = map.Get("Gtk/FontName", "");
		String theme = map.Get("Net/ThemeName", "");
		//int scaling = map.Get("Gdk/WindowScalingFactor", 1);
		int xdpi = map.Get("Xft/DPI", 98347);
	
		int fontface = Font::ARIAL;
		int fontheight = 13;
		bool bold = false;
		bool italic = false;
	
		const char *q = strrchr(font_name, ' ');
		if(q) {
			int h = atoi(q);
			if(h)
				fontheight = h;
			String face(font_name, q);
			fontface = Font::FindFaceNameIndex(face);
	
			if(fontface == 0) {
				for(;;) {
					const char *q = strrchr(face, ' ');
					if(!q) break;
					const char *s = q + 1;
					if(stricmp(s, "Bold") == 0 || stricmp(s, "Heavy") == 0)
						bold = true;
					else
					if(stricmp(s, "Italic") == 0 || stricmp(s, "Oblique") == 0)
						italic = true;
					else
					if(stricmp(s, "Regular") == 0 || stricmp(s, "Light") || stricmp(s, "Medium"))
						;
					else
						continue;
					face = String(~face, q);
				}
				fontface = Font::FindFaceNameIndex(face);
				if(fontface == 0) {
					if(ToUpper(face[0]) == 'M')
						fontface = Font::COURIER;
					else
					if(ToUpper(face[0]) == 'S' && ToUpper(face[1]) == 'e')
						fontface = Font::ROMAN;
					else
						fontface = Font::ARIAL;
				}
			}
		}
		
		Font gui_font = Font(fontface, fround(fontheight * xdpi / (72*1024.0))).Bold(bold).Italic(italic);
		Font::SetDefaultFont(gui_font);
	
		SColorFace_Write(Color(242, 241, 240));
		SColorMenu_Write(Color(242, 241, 240));
		SColorHighlight_Write(Color(50, 50, 250));
	
		auto ThemeHasWord = [&](const char *text) {
			int q = ToLower(theme).Find(text);
			if(q >= 0) {
				if(q > 0) {
					int pc = theme[q - 1];
					if(!(IsUpper(theme[q]) && IsLower(pc) || !IsLetter(pc)))
						return false;
				}
				int l = strlen(text);
				int nc = theme[q + l];
				if(!(IsLower(text[l - 1]) && IsUpper(nc) || !IsLetter(nc)))
					return false;
				return true;
			}
			return false;
		};
		
		if(ThemeHasWord("dark") || ThemeHasWord("inverse") || ThemeHasWord("black"))
			ChDarkSkin();
		else
			ChStdSkin();
	}
	catch(ValueTypeError) {}

	SwapOKCancel_Write(Environment().Get("KDE_FULL_SESSION", String()) != "true");
}

};

#endif
