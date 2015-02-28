#include "CtrlLib.h"

#ifdef PLATFORM_X11
#ifndef flagNOGTK

#include "ChGtk.h"

#define LLOG(x)  // DLOG(x)
#define LDUMP(x) // DDUMP(x)

NAMESPACE_UPP

#define IMAGECLASS AmbientCtrlsImg
#define IMAGEFILE <CtrlLib/AmbientCtrls.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS AmbientCtrlsImg
#define IMAGEFILE <CtrlLib/AmbientCtrls.iml>
#include <Draw/iml_source.h>

extern int    gtk_antialias;
extern int    gtk_hinting;
extern String gtk_hintstyle;
extern String gtk_rgba;

extern void ClearFtFaceCache();

void SetDefTrough(ScrollBar::Style& s)
{
	for(int i = 0; i < 4; i++) {
		Image m = CtrlsImg::Get(CtrlsImg::I_SBVU + i);
		ImageBuffer ib(m);
		Size sz = ib.GetSize();
		ib[0][sz.cx - 1] = ib[0][0] = SColorShadow();
		ib.SetHotSpot(Point(0, 0));
		ib.Set2ndSpot(Point(sz.cx - 1, 0));
		m = ib;
		s.vlower[i] = s.vupper[i] = m;
		m = CtrlsImg::Get(CtrlsImg::I_SBHU + i);
		ib = m;
		sz = ib.GetSize();
		ib[sz.cy - 1][0] = ib[0][0] = SColorShadow();
		ib.SetHotSpot(Point(0, 0));
		ib.Set2ndSpot(Point(0, sz.cy - 1));
		m = ib;
		s.hupper[i] = s.hlower[i] = m;
	}
}

extern void (*chgtkspy__)(const char *name, int state, int shadow, const char *detail, int type, int cx, int cy, const Value& look);

extern String CurrentSoundTheme;

void ChHostSkin()
{
	MemoryIgnoreLeaksBlock __;
	static struct { void (*set)(Color); int ii; } col[] = {
		{ SColorPaper_Write, 6*5 + 0 },
		{ SColorFace_Write, 1*5 + 0 },
		{ SColorText_Write, 5*5 + 0 },
		{ SColorHighlight_Write, 6*5 + 3 },
		{ SColorHighlightText_Write, 5*5 + 3 },
		{ SColorMenu_Write, 6*5 + 0 },
		{ SColorMenuText_Write, 5*5 + 0 },
		{ SColorDisabled_Write, 5*5 + 4 },
		{ SColorLight_Write, 2*5 + 0 },
		{ SColorShadow_Write, 3*5 + 0 },
	};
	for(int i = 0; i < __countof(col); i++)
		(*col[i].set)(ChGtkColor(col[i].ii, gtk__parent()));

	////////
		GtkWidget *label = gtk_label_new("Ch");
		Setup(label);
		Color ch_ink = ChGtkColor(0, label);
		gtk_widget_destroy(label);
		SColorLabel_Write(ch_ink);
		
	///////
	CtrlsImg::Reset();
	ColoredOverride(CtrlsImg::Iml(), ClassicCtrlsImg::Iml());
	
	ChLookFn(GtkLookFn);
	
	bool KDE = Environment().Get("KDE_FULL_SESSION", String()) == "true";

	String engine = GtkStyleString("gtk-theme-name");
	
	LDUMP(engine);
	
	bool Qt = engine == "Qt" || KDE;
	
	if(chgtkspy__)
		engine.Clear();

	int fontname = Font::ARIAL;
	int fontheight = 13;
	bool bold = false;
	bool italic = false;

	String font_name = GtkStyleString("gtk-font-name");
	int xdpi = Nvl(GtkStyleInt("gtk-xft-dpi"), 72 * 1024);
	gtk_antialias = Nvl(GtkStyleInt("gtk-xft-antialias"), -1);
	if(GetDesktopManager() != "gnome")
		gtk_antialias = -1;
	gtk_hinting = Nvl(GtkStyleInt("gtk-xft-hinting"), -1);
//	gtk_hintstyle = GtkStyleString("gtk-xft-hintstyle");
	gtk_hintstyle = gtk_hinting? "hintfull" : "hintnone"; // Gtk does not seem to follow its own rules...
	gtk_rgba = GtkStyleString("gtk-xft-rgba");

	const char *q = strrchr(font_name, ' ');
	if(q) {
		int h = atoi(q);
		if(h)
			fontheight = h;
		String face(font_name, q);
		fontname = Font::FindFaceNameIndex(face);

		if(fontname == 0) {
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
			fontname = Font::FindFaceNameIndex(face);
			if(fontname == 0) {
				if(ToUpper(face[0]) == 'M')
					fontname = Font::COURIER;
				else
				if(ToUpper(face[0]) == 'S' && ToUpper(face[1]) == 'e')
					fontname = Font::ROMAN;
				else
					fontname = Font::ARIAL;
			}
		}
	}

	Font::SetDefaultFont(Font(fontname, (fontheight * xdpi + 512*72) / (1024*72))
	                     .Bold(bold).Italic(italic));

	ClearFtFaceCache();

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

	int classiq = engine == "Redmond" || engine == "Raleigh" || engine == "Glider" || engine == "Simple";
	if(!classiq)
		ColoredOverride(CtrlsImg::Iml(), AmbientCtrlsImg::Iml());

	Color fc = Blend(SColorHighlight, SColorShadow);

	ChGtkIs().Clear();
	GtkWidget *w = Setup(gtk_radio_button_new(NULL));
	int is = GtkInt(w, "indicator-size") + 2;
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = false;
	GtkIml(CtrlsImg::I_S0, w, 2, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = true;
	GtkIml(CtrlsImg::I_S1, w, 1, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	gtk_widget_destroy(w);

	w = Setup(gtk_check_button_new());
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = false;
	GtkIml(CtrlsImg::I_O0, w, 2, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = true;
	GtkIml(CtrlsImg::I_O1, w, 1, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	GTK_TOGGLE_BUTTON(w)->active = false;
	GTK_TOGGLE_BUTTON(w)->inconsistent = true;
	GtkIml(CtrlsImg::I_O2, w, 3, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	gtk_widget_destroy(w);

	if(Qt) {
		for(int i = 0; i < 4; i++) {
			Image m = CtrlsImg::Get(CtrlsImg::I_O2 + i);
			ImageBuffer ib(m);
			Size sz = ib.GetSize();
			if(sz.cx > 6)
				for(int q = -2; q <= 2; q++)
					ib[sz.cy / 2 - 1][sz.cx / 2 - q - 1] = i == CTRL_DISABLED ? SColorDisabled() : SColorText();
			m = ib;
			CtrlsImg::Set(CtrlsImg::I_O2 + i, m);
		}
	}

	Point po(0, 0);
	
	{
		Button::Style& s = Button::StyleNormal().Write();
		s.overpaint = 3;
		static GtkWidget *button = gtk_button_new();
		ChGtkNew(button, "button", GTK_BOX|GTK_MARGIN3);
		GtkChButton(s.look);

		po.x = GtkInt("child-displacement-x");
		po.y = GtkInt("child-displacement-y");

		s.ok = GtkImage("gtk-ok", 4, 16);
		s.cancel = GtkImage("gtk-cancel", 4, 16);
		s.exit = GtkImage("gtk-quit", 4, 16);

		ChGtkColor(s.textcolor, 0 * 5);
		s.pressoffset = po;

		Color c = SColorFace();
		for(int i = 0; i < 4; i++)
			s.monocolor[i] = c.GetR() + c.GetG() + c.GetB() < 3 * 128 ? White() : Black();
		s.monocolor[3] = Gray();

		ToolBar::Style& ts = ToolBar::StyleDefault().Write();
		GtkChButton(ts.buttonstyle.look);
		ts.buttonstyle.look[CTRL_NORMAL] = Null;
		ts.buttonstyle.look[CTRL_DISABLED] = Null;
		GtkCh(ts.buttonstyle.look[CTRL_CHECKED], 1, 1);
		GtkCh(ts.buttonstyle.look[CTRL_HOTCHECKED], 1, 1);

		{
			HeaderCtrl::Style& hs = HeaderCtrl::StyleDefault().Write();
			if(classiq)
				for(int i = 0; i < 4; i++)
					hs.look[i] = s.look[i];
			else {
				ChGtkNew(button, "button", GTK_BOX);
				hs.look[0] = GtkMakeCh(2|GTKELEMENT_TABFLAG, 4, Rect(6, 3, 6, 0));
				hs.look[1] = GtkMakeCh(2|GTKELEMENT_TABFLAG, 2, Rect(6, 3, 6, 0));
				hs.look[2] = GtkMakeCh(1|GTKELEMENT_TABFLAG, 1, Rect(6, 3, 6, 0));
				hs.look[3] = GtkMakeCh(2|GTKELEMENT_TABFLAG, 4, Rect(6, 3, 6, 0));
				hs.pressoffset = po.x || po.y;
			}
		}
	}

	{
		Button::Style& s = Button::StyleOk().Write();
		static GtkWidget *def_button;
		if(!def_button) {
			def_button = gtk_button_new();
			Setup(def_button);
			gtk_widget_set(def_button, "can-default", true, NULL);
			gtk_window_set_default(GTK_WINDOW(gtk__parent()), def_button);
			ChGtkNew(def_button, "button", GTK_BOX|GTK_MARGIN3);
		}
		GtkChButton(s.look);
	}

	{
		ScrollBar::Style& s = ScrollBar::StyleDefault().Write();
		SetDefTrough(s);
		s.through = true;
		GtkObject *adj = gtk_adjustment_new(250, 0, 1000, 1, 1, 500);
		static GtkWidget *vscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adj));
		ChGtkNew(vscrollbar, "slider", GTK_SLIDER|GTK_VAL1);

		s.thumbmin = GTK_RANGE(vscrollbar)->min_slider_size;
		s.barsize = max(14, GtkInt("slider_width")); // 'max' - ugly fix for ThinIce theme
		s.arrowsize = max(s.barsize, GtkInt("stepper_size")); // 'max' - ugly fix for ThinIce theme

		/* The only theme with 3 buttons is Amaranth and it does not look good...
		s.isright2 = s.isdown2 = GtkInt("has-secondary-forward-stepper");
		s.isleft2 = s.isup2 = GtkInt("has-secondary-backward-stepper");
		*/

		for(int i = 0; i < 6; i++)
			CtrlsImg::Set(CtrlsImg::I_DA + i, CtrlsImg::Get(CtrlsImg::I_kDA + i));

		if((Environment().Get("GTK_MODULES", "").Find("overlay-scrollbar") < 0 ||
		    Environment().Get("LIBOVERLAY_SCROLLBAR", "1") == "0") &&
		    findarg(engine, "Glider", "Human", "DarkRoom", "Crux") >= 0) {
			GtkChScrollBar(s.up.look, s.up2.look, s.vlower, s.vthumb, s.vupper, s.down2.look, s.down.look,
			               CtrlsImg::I_UA, CtrlsImg::I_DA, false);

			if(IsEmptyImage(GetGTK(ChGtkLast(), 2, 2, "vscrollbar", GTK_BOX|GTK_TOP|GTK_RANGEA, 16, 16))) {
				static GtkWidget *btn = gtk_button_new();
				ChGtkNew(btn, "button", GTK_BOX);

				GtkChButton(Button::StyleScroll().Write().look);
				GtkChButton(Button::StyleEdge().Write().look);
				GtkChButton(Button::StyleLeftEdge().Write().look);

				{
					DropList::Style& s = DropList::StyleFrame().Write();
					GtkChButtonWith(s.look, CtrlsImg::DA());
					GtkChButtonWith(s.trivial, CtrlsImg::DA());
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					GtkChButtonWith(s.inc.look, CtrlImg::spinup2());
					GtkChButtonWith(s.dec.look, CtrlImg::spindown2());
				}
			}
			else {
				ChGtkNew("vscrollbar", GTK_BOX|GTK_VCENTER|GTK_RANGEB);
				GtkCh(Button::StyleScroll().Write().look, "02142222");

				int q = !classiq;

				GtkChImgWith(Button::StyleEdge().Write().look, Null, 1 * q);
				GtkChImgWith(Button::StyleLeftEdge().Write().look, Null, 2 * q);

				{
					DropList::Style& s = DropList::StyleFrame().Write();
					GtkChImgWith(s.look, CtrlsImg::DA(), 1 * q, po);
					GtkChImgWith(s.trivial, CtrlsImg::DA(), 1 * q, po);
					GtkChImgWith(s.left, CtrlsImg::DA(), 2 * q, po);
					GtkChImgWith(s.right, CtrlsImg::DA(), 1 * q, po);
					s.pressoffset = po;
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					GtkChImgWith(s.inc.look, q ? CtrlImg::spinup2() : CtrlImg::spinup3(), (1|GTK_BOTTOMLINE) * q, po);
					GtkChImgWith(s.dec.look, q ? CtrlImg::spindown2() : CtrlImg::spindown3(), 1 * q, po);
				}
			}

			static GtkWidget *hscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(adj));
			ChGtkNew(hscrollbar, "slider", GTK_SLIDER);
			GtkChScrollBar(s.left.look, s.left2.look, s.hlower, s.hthumb, s.hupper, s.right2.look, s.right.look,
			               CtrlsImg::I_LA, CtrlsImg::I_RA, true);
			
			int d = Diff(fc, SColorPaper());
			Image m = GtkGetLastImage();
			for(int x = 0; x < 4; x++)
				for(int y = 0; y < 4; y++) {
					RGBA c = m[x][y];
					if(c.a == 255) {
						int dd = Diff(c, SColorPaper());
						if(dd > d) {
							fc = c;
							d = dd;
						}
					}
				}
			FieldFrameColor_Write(fc);

			gtk_object_sink(adj);

			adj = gtk_adjustment_new(0, 0, 1000, 1, 1, 500);
			
			w = gtk_vscrollbar_new(NULL);
			Setup(w);
			s.overthumb = m != GetGTK(w, 0, 0, "slider", GTK_SLIDER|GTK_VAL1, 16, 32) && !Qt;
			gtk_widget_destroy(w);
			gtk_object_sink(adj);
			
			w = gtk_hscale_new_with_range(0.0, 100.0, 1.0);
			Setup(w);
			int cx = GtkInt(w, "slider-length");
			int cy = 15;
			if(engine == "Glider")
				cy += cx-3;
			if(engine == "Human" || engine == "DarkRoom" || engine == "Crux")
				cy = 13;
			CtrlImg::Set("hthumb",GetGTK(w, 0, 0, "hscale", GTK_SLIDER, cx, cy));
			CtrlImg::Set("hthumb1",GetGTK(w, 2, 0, "hscale", GTK_SLIDER, cx, cy));
			gtk_widget_destroy(w);
			w = gtk_hscale_new_with_range(0.0, 100.0, 1.0);
			Setup(w);
			CtrlImg::Set("vthumb",GetGTK(w, 0, 0, "vscale", GTK_SLIDER|GTK_VAL1, cy, cx));
			CtrlImg::Set("vthumb1",GetGTK(w, 2, 0, "vscale", GTK_SLIDER|GTK_VAL1, cy, cx));
			gtk_widget_destroy(w);
		}
		else {
			int r = Null;
			for(int i = 0; i < 4; i++) {
				ImageDraw iw(64, 64);
				iw.DrawRect(0, 0, 64, 64, SColorFace());
				ChPaint(iw, Size(64, 64), Button::StyleNormal().look[i == CTRL_HOT ? CTRL_NORMAL : i]);
				Image m = iw;
				if(IsNull(r))
					r = minmax(ImageMargin(m, 4, 50), 0, 2);
				m = Rescale(Crop(m, 4, 4, 56, 56), 16, 16);
				ChPartMaker pm(m);
				pm.tr = pm.tl = r;
				pm.bl = pm.br = 0;
				s.up.look[i] = ChLookWith(pm.Make(), CtrlsImg::UA(), ButtonMonoColor(i));
				pm.tr = pm.tl = 0;
				pm.t = false;
				s.down2.look[i] = ChLookWith(pm.Make(), CtrlsImg::DA(), ButtonMonoColor(i));
				pm.t = true;
				pm.bl = pm.br = r;
				s.down.look[i] = ChLookWith(pm.Make(), CtrlsImg::DA(), ButtonMonoColor(i));
				pm.br = pm.bl = 0;
				pm.b = false;
				s.up2.look[i] = ChLookWith(pm.Make(), CtrlsImg::UA(), ButtonMonoColor(i));
				pm.br = pm.bl = pm.tl = pm.tr = r;
				pm.b = true;
				Image bm = pm.Make();
				Button::StyleScroll().Write().look[i] = bm;
				pm.ResetShape();
				pm.t = pm.b = pm.l = false;
				Image lm = pm.Make();
				Button::StyleLeftEdge().Write().look[i] = lm;
				pm.r = false;
				pm.l = true;
				Image rm = pm.Make();
				Button::StyleEdge().Write().look[i] = rm;
				{
					DropList::Style& s = DropList::StyleFrame().Write();
					s.look[i] = s.trivial[i] = s.right[i] = ChLookWith(rm, CtrlsImg::DA());
					s.left[i] = ChLookWith(lm, CtrlsImg::DA());
					s.pressoffset = po;
				}
				{
					SpinButtons::Style& s = SpinButtons::StyleDefault().Write();
					s.dec.look[i] = ChLookWith(rm, CtrlImg::spindown2());
					pm.b = true;
					s.inc.look[i] = ChLookWith(pm.Make(), CtrlImg::spinup2());
				}
				pm.ResetShape();
				pm.tl = pm.bl = r;
				pm.tr = pm.br = 0;
				s.left.look[i] = ChLookWith(pm.Make(), CtrlsImg::LA(), ButtonMonoColor(i));
				pm.tl = pm.bl = 0;
				pm.l = false;
				s.right2.look[i] = ChLookWith(pm.Make(), CtrlsImg::RA(), ButtonMonoColor(i));
				pm.l = true;
				pm.tl = pm.bl = 0;
				pm.tr = pm.br = r;
				s.right.look[i] = ChLookWith(pm.Make(), CtrlsImg::RA(), ButtonMonoColor(i));
				pm.tr = pm.br = 0;
				pm.r = false;
				s.left2.look[i] = ChLookWith(pm.Make(), CtrlsImg::LA(), ButtonMonoColor(i));
			}
			ChGtkNew(vscrollbar, "slider", GTK_SLIDER|GTK_VAL1|GTK_MARGIN1|GTK_XMARGIN);
			GtkChSlider(s.vthumb);
			s.barsize += 2;
			s.arrowsize++;
			SetDefTrough(s);
			static GtkWidget *hscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(adj));
			ChGtkNew(hscrollbar, "slider", GTK_SLIDER|GTK_MARGIN1|GTK_XMARGIN);
			GtkChSlider(s.hthumb);
			s.overthumb = false;
		}
	}

	if(!Qt)
	{
		TabCtrl::Style& s = TabCtrl::StyleDefault().Write();
		static GtkWidget *tabctrl = gtk_notebook_new();
		ChGtkNew(tabctrl, "tab", GTK_EXT|GTK_VAL3|GTK_MARGIN3);
		ImageBuffer ib(9, 9);
		ImageBuffer ib1(9, 9);
		Image m = GetGTK(tabctrl, 0, 2, "tab", GTK_EXT|GTK_VAL3, 12, 24);
		for(int i = 0; i < 5; i++) {
			RGBA *t = ~ib + i * 9 + i;
			RGBA *t1 = ~ib1 + i * 9 + i;
			for(int n = 9 - 2 * i; n--; t += 9, t1 += 9) {
				Fill(t, m[21][i], 9 - 2 * i);
				Fill(t1, m[21][11 - i], 9 - 2 * i);
			}
		}
		{
			RGBA *t = ~ib + 9 + 8;
			RGBA *t1 = ~ib1 + 9 + 8;
			for(int i = 1; i < 9; i++) {
				memcpy(t, t1, i * sizeof(RGBA));
				t += 9 - 1;
				t1 += 9 - 1;
			}
		}
		ib.SetHotSpot(Point(4, 4));
		s.body = Image(ib);
		GtkCh(s.normal[0], 2, 1);
		GtkCh(s.normal[1], 2, 1);
		GtkCh(s.normal[2], 0x82, 0);
		GtkCh(s.normal[3], 2, 1);
		for(int i = 0; i < 4; i++)
			s.first[i] = s.last[i] = s.both[i] = s.normal[i];
		s.margin = 0;

		s.sel = Rect(0, 2, 0, 4);
		s.extendleft = 2;
		ChGtkColor(s.text_color, 0);
	}

	int efm = 0;
	if(engine.Find("oxygen") < 0)
	{
		EditField::Style& s = EditField::StyleDefault().Write();
		Image img;
		for(int i = 0; i < 4; i++) {
			GtkWidget *w = Setup(gtk_entry_new());
			if(i == CTRL_PRESSED)
				GTK_WIDGET_FLAGS (w) |= GTK_HAS_FOCUS;
			if(i == CTRL_DISABLED)
				GTK_WIDGET_FLAGS (w) &= GTK_SENSITIVE;
			img = GetGTK(w, GTK_STATE_NORMAL, GTK_SHADOW_IN, "entry", GTK_SHADOW, 20, 20);
			if(i == 0)	
				efm = max(ImageMargin(img, 4, 0), 1);
			if(!Qt)
				img = GetGTK(w, GTK_STATE_NORMAL, GTK_SHADOW_IN,
				             "entry", GTK_SHADOW, 2 * efm + 3, 2 * efm + 3);
			ImageBuffer eb(img);
			eb.SetHotSpot(Point(efm, efm));
			s.edge[i] = Image(eb);
			s.activeedge = true;
			gtk_widget_destroy(w);
		}
	}

	{
		MultiButton::Style& s = MultiButton::StyleDefault().Write();
		s.usetrivial = true;
		s.trivialsep = true;
		s.edge[0] = Null;
		s.overpaint = Button::StyleNormal().overpaint;
		s.margin.left = 3;
		for(int i = 0; i < 4; i++)
			s.left[i] = s.right[i] = s.lmiddle[i] = s.look[i] = Button::StyleNormal().look[i];
		s.loff = 1;
		s.roff = -1;
		s.trivialborder = s.border = 0;
		s.sep2 = SColorShadow();
		s.sep1 = SColorLight();
		s.sepm = 4;
		if(engine.Find("oxygen") < 0)
		{
			MultiButton::Style& s = MultiButton::StyleFrame().Write();
			for(int i = 0; i < 4; i++)
				s.edge[i] = EditField::StyleDefault().edge[i];
			s.activeedge = true;
			s.sep1 = Null;
			s.trivialborder = s.border = efm;
			s.usetrivial = true;
			s.sepm = 0;
			s.margin = Rect(efm, efm, efm, efm);
		}
	}

	ImageBuffer ib;
	ib.Create(3, 3);
	Fill(~ib, fc, ib.GetLength());
	ib[1][1] = Color(Null);
	ib.SetHotSpot(Point(1, 1));
	CtrlsImg::Set(CtrlsImg::I_EFE, ib);
	ib.Create(5, 5);
	Fill(~ib, fc, ib.GetLength());
	for(int x = 1; x < 4; x++)
		Fill(ib[x] + 1, SColorPaper(), 3);
	ib[2][2] = Color(Null);
	ib.SetHotSpot(Point(2, 2));
	CtrlsImg::Set(CtrlsImg::I_VE, ib);

	static GtkWidget *popup;
	static GtkWidget *bar = Setup(gtk_menu_bar_new());
	static int shadowtype;
	static GtkWidget *top_item;
	static GtkWidget *menu_item;
	if(!popup) {
		gtk_widget_style_get(bar, "shadow_type", &shadowtype, NULL);
		top_item = gtk_menu_item_new_with_label("M");
		gtk_menu_shell_append(GTK_MENU_SHELL(bar), top_item);
		gtk_widget_realize(top_item);
		gtk_widget_show(top_item);
		popup = gtk_menu_new();
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(top_item), popup);
		gtk_widget_realize(popup);
		gtk_widget_show(popup);
		GTK_MENU_SHELL(bar)->active = true;
		menu_item = gtk_menu_item_new_with_label("M");
		
		gtk_menu_shell_append(GTK_MENU_SHELL(popup), menu_item);
		gtk_widget_realize(menu_item);
		gtk_widget_show(menu_item);
//		gtk_widget_show(GTK_MENU(GTK_MENU_ITEM(top_item)->submenu)->toplevel);
	}
	Image mimg = GetGTK(popup, 0, 2, "menu", GTK_BGBOX, 32, 32);
	Color c = mimg[16][16];
	Value rlook;
	if(!IsNull(c))
		SColorMenu_Write(c);
	{
		MenuBar::Style& s = MenuBar::StyleDefault().Write();
		s.pullshift.y = 0;
		int m = ImageMargin(mimg, 4, 5);
		s.popupframe = WithHotSpot(mimg, m, m);
		s.popupbody = Crop(mimg, m, m, 32 - 2 * m, 32 - 2 * m);
		s.leftgap = 26;
		ChGtkNew(menu_item, "menuitem", GTK_BOX);
		int sw = GTK_SHADOW_OUT;
		if(gtk_check_version(2, 1, 0))
			sw = GtkInt("selected_shadow_type");
		GtkCh(s.item, sw, GTK_STATE_PRELIGHT);
		s.itemtext = ChGtkColor(2, menu_item);
		s.menutext = SColorMenuText();
		if(Diff(c, s.menutext) < 200) // menutext color too close to background color, fix it
			s.menutext = IsDark(c) ? White() : Black();

		ChGtkNew(top_item, "menuitem", GTK_BOX);
		if(gtk_check_version(2, 1, 0))
			sw = GtkInt("selected_shadow_type");
		
		s.topitemtext[0] = ChGtkColor(0, top_item);
		if(Qt)
			s.topitemtext[1] = ChGtkColor(2, top_item);
		else
			s.topitemtext[1] = ChGtkColor(0, top_item);
		s.topitemtext[2] = ChGtkColor(2, top_item);
		SColorMenuText_Write(s.topitemtext[1]);
		if(Qt)
			GtkCh(s.topitem[1], sw, GTK_STATE_PRELIGHT);
		else
			s.topitem[1] = s.topitem[0];
		GtkCh(s.topitem[2], sw, GTK_STATE_PRELIGHT);
		s.topitemtext[2] = ChGtkColor(2, top_item);
		if(engine == "Redmond") {
			s.topitem[1] = ChBorder(ThinOutsetBorder(), SColorFace());
			s.topitem[2] = ChBorder(ThinInsetBorder(), SColorFace());
		}
		if(engine == "Geramik" || engine == "ThinGeramik")
			s.topitemtext[2] = SColorText();
		ChGtkNew(bar, "menubar", GTK_BGBOX);
		sw = GtkInt("shadow_type");
		s.look = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 0, 0, 1));
		rlook = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 1, 0, 1));
		Image img = GetGTK(bar, GTK_STATE_NORMAL, sw, "menubar", GTK_BGBOX, 32, 32);
		s.breaksep.l1 = Color(img[31][15]);
		TopSeparator1_Write(s.breaksep.l1);
		s.breaksep.l2 = Null;
	}

	{
		ToolBar::Style& s = ToolBar::StyleDefault().Write();
		static GtkWidget *toolbar;
		if(!toolbar) {
			GtkWidget *handle = Setup(gtk_handle_box_new());
			toolbar = gtk_toolbar_new();
			gtk_container_add(GTK_CONTAINER(handle), toolbar);
		}
		ChGtkNew(toolbar, "toolbar", GTK_BGBOX);
		int sw = GtkInt("shadow_type");
		s.arealook = Null;
		s.look = GtkMakeCh(sw, GTK_STATE_NORMAL, Rect(0, 1, 0, 1));
		if(engine == "Redmond")
			s.look = rlook;
		s.breaksep.l1 = MenuBar::StyleDefault().breaksep.l1;
		Image img = GetGTK(toolbar, GTK_STATE_NORMAL, sw, "toolbar", GTK_BGBOX, 32, 32);
		MenuBar::StyleDefault().Write().breaksep.l2 = s.breaksep.l2 = Color(img[0][15]);
		TopSeparator2_Write(s.breaksep.l2);
	}

	if(engine != "Qt")
	{
		ProgressIndicator::Style& s = ProgressIndicator::StyleDefault().Write();
		static GtkWidget *pg = gtk_progress_bar_new();
		ChGtkNew(pg, "trough", GTK_BOX);
		int mx = pg->style->xthickness;
		int my = pg->style->ythickness;
		s.hlook = GtkMakeCh(GTK_SHADOW_IN, GTK_STATE_NORMAL, Rect(0, 0, 0, 0), Rect(mx, my, mx, my));
		ChGtkNew(pg, "bar", GTK_BOX);
		GtkCh(s.hchunk, GTK_SHADOW_OUT, GTK_STATE_PRELIGHT);
		s.bound = true;
	}

	static GtkWidget *ve = Setup(gtk_text_view_new());
	if(engine == "Redmond")
		ViewEdge_Write(EditFieldEdge());
	else
		ViewEdge_Write(WithHotSpot(GetGTK(ve, GTK_STATE_NORMAL, GTK_SHADOW_IN, "frame", GTK_BOX, 32, 32), 2, 2));

	ChCtrlImg(CtrlImg::I_information, "gtk-dialog-info", 6);
	ChCtrlImg(CtrlImg::I_question, "gtk-dialog-question", 6);
	ChCtrlImg(CtrlImg::I_exclamation, "gtk-dialog-warning", 6);
	ChCtrlImg(CtrlImg::I_error, "gtk-dialog-error", 6);

	static struct {
		void  (*set)(Image);
		const char *gtk;
	} bimg[] = {
		{ YesButtonImage_Write, "gtk-yes" },
		{ NoButtonImage_Write, "gtk-no" },
		{ AbortButtonImage_Write, "gtk-stop" },
		{ RetryButtonImage_Write, "gtk-refresh" },
	};

	for(int i = 0; i < __countof(bimg); i++)
		(*bimg[i].set)(GtkImage(bimg[i].gtk, 4, 16));

	if(engine != "Redmond")
		DropEdge_Write(ViewEdge());

	SwapOKCancel_Write(!Qt);

	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_DragFullWindow_Write(1);
	GUI_PopUpEffect_Write(Ctrl::IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
	GUI_DropShadows_Write(1);
	GUI_AltAccessKeys_Write(1);
	GUI_AKD_Conservative_Write(0);
	
	ClearFtFaceCache();
	
	CurrentSoundTheme = GtkStyleString("gtk-sound-theme-name");
}

END_UPP_NAMESPACE

#endif
#endif
