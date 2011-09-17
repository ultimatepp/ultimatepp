#include "FTGLCtrl.h"

FTGLCtrl::FTGLCtrl() : trackBall(this)
{
	mode = INTERACTIVE;
	
	currentFont = FTGL_EXTRUDE;

	myString =
	"OpenGL is a powerful software interface for graphics "
	"hardware that allows graphics programmers to produce high-quality "
	"color images of 3D objects. abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL"
	"MNOPQRSTUVWXYZ0123456789";
	
	totalFonts = 0;

	layouts.Add(new FTSimpleLayout);
	currentLayout = 0;

	OX = -100;
	OY = 200;

#ifdef  PLATFORM_OSX11
	SetFontFiles(Vector<String>()
		<< "/System/Library/Fonts/Helvetica.dfont",
		<<	"/System/Library/Fonts/Geneva.dfont"
	;
#elif defined(WIN32)
	SetFontFiles(Vector<String>()
    	<< "C:\\WINDOWS\\Fonts\\arial.ttf"
    );
#elif defined(PLATFORM_LINUX)
	SetFontFiles(Vector<String>()
    	<< "/usr/share/fonts/truetype/msttcorefonts/arial.ttf"
    	<< "/usr/share/fonts/truetype/msttcorefonts/cour.ttf"
    );
#else
#error "UNSUPPORTED PLATFORM -- ADD HERE DEFAULT FONTS"
#endif

	initialLineLength = 300.0f;

	FTSimpleLayout &sl = (FTSimpleLayout &)layouts.Top();
	sl.SetLineLength(initialLineLength);
	sl.SetFont(&fonts[currentFont]);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.13, 0.17, 0.32, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(1.0, 1.0); // ????
	
	SetCamera();
}

Image FTGLCtrl::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	return trackBall.MouseEvent(event, p, zdelta, keyflags);
}

bool FTGLCtrl::Key(dword key, int count)
{
    FTSimpleLayout *l = NULL;

    // If the currentLayout is a SimpleLayout store a pointer in l
    if(hasLayout() && (dynamic_cast <FTSimpleLayout *>(&layouts[currentLayout])))
        l = (FTSimpleLayout *)&layouts[currentLayout];

	switch (key)
	{
		case K_ESCAPE:
			GetMainWindow()->Break();
			break;

		case K_RETURN:
			if(mode == EDITING)
				mode = INTERACTIVE;
			else
				mode = EDITING;
			break;

		case K_TAB:
			if(l)
			{
				// Decrement the layout
				switch (l->GetAlignment())
				{
					case FTGL::ALIGN_LEFT:
						l->SetAlignment(FTGL::ALIGN_RIGHT);
						break;

					case FTGL::ALIGN_RIGHT:
						l->SetAlignment(FTGL::ALIGN_CENTER);
						break;

					case FTGL::ALIGN_CENTER:
						l->SetAlignment(FTGL::ALIGN_JUSTIFY);
						break;

					case FTGL::ALIGN_JUSTIFY:
						l->SetAlignment(FTGL::ALIGN_LEFT);
						break;
				}
			}
			break;

		case K_UP:
			currentFont = (GetFace()*NumStyles + (currentFont + 1) % NumStyles) % totalFonts;
			break;

		case K_DOWN:
			currentFont = (GetFace()*NumStyles + (currentFont + NumStyles - 1) % NumStyles) % totalFonts;
			break;

		case K_LEFT:
			fonts[currentFont].FaceSize(fonts[currentFont].FaceSize() - 1);
			break;

		case K_RIGHT:
			fonts[currentFont].FaceSize(fonts[currentFont].FaceSize() + 1);
			break;

		case K_PAGEUP:
			currentFont = (currentFont + NumStyles) % totalFonts;
			break;

		case K_PAGEDOWN:
			currentFont = (currentFont + totalFonts - NumStyles) % totalFonts;
			break;

		case K_HOME:
			currentLayout = (currentLayout + 1) % layouts.GetCount();
			break;

		case K_END:
			currentLayout = (currentLayout + layouts.GetCount() - 1) % layouts.GetCount();
			break;

		case K_F1:
		case K_F10:
		// If the current layout is simple decrement its line length
			if (l)
				l->SetLineLength(l->GetLineLength() - 10.0f);
			break;

		case K_F2:
		case K_F11:
			// If the current layout is simple increment its line length
			if (l)
				l->SetLineLength(l->GetLineLength() + 10.0f);
			break;

		default:
			if(mode == EDITING)
			{
				if(isprint(key))
					myString << (char)key;
				else if(key == K_BACKSPACE && myString.GetCount() > 0)
					myString.Trim(myString.GetCount() - 1);
			}
			break;
	}
	
	// If the current layout is a SimpleLayout, update its font.
	if(l)
		l->SetFont(&fonts[currentFont]);

	Refresh();
	return true;
}

void FTGLCtrl::SetUpLighting()
{
	// Set up lighting.
	float light1_ambient[4]  = { 1.0, 1.0, 1.0, 1.0 };
	float light1_diffuse[4]  = { 1.0, 0.9, 0.9, 1.0 };
	float light1_specular[4] = { 1.0, 0.7, 0.7, 1.0 };
	float light1_position[4] = { -1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);
	
	float light2_ambient[4]  = { 0.2, 0.2, 0.2, 1.0 };
	float light2_diffuse[4]  = { 0.9, 0.9, 0.9, 1.0 };
	float light2_specular[4] = { 0.7, 0.7, 0.7, 1.0 };
	float light2_position[4] = { 1.0, -1.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	//glEnable(GL_LIGHT2);
	
	float front_emission[4] = { 0.3, 0.2, 0.1, 0.0 };
	float front_ambient[4]  = { 0.2, 0.2, 0.2, 0.0 };
	float front_diffuse[4]  = { 0.95, 0.95, 0.8, 0.0 };
	float front_specular[4] = { 0.6, 0.6, 0.6, 0.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, front_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, front_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, front_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
	glColor4fv(front_diffuse);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glEnable(GL_CULL_FACE);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}


void FTGLCtrl::SetFontFiles(Vector<String> const &files)
{
	// copy font file names, it's needed later...
	fontFiles <<= files;
	
	// The total number of fonts is styles * faces
	totalFonts = files.GetCount() * NumStyles;
	
	// Instantiate and configure named fonts
	for(int i = 0; i < files.GetCount(); i++)
	{
		fonts.Add(new FTBitmapFont(files[i]));
		fonts.Add(new FTPixmapFont(files[i]));
		fonts.Add(new FTOutlineFont(files[i]));
		fonts.Add(new FTPolygonFont(files[i]));
		fonts.Add(new FTExtrudeFont(files[i]));
		fonts.Add(new FTTextureFont(files[i]));

		for(int x = 0; x < NumStyles; ++x)
		{
			int j = i * NumStyles + x;
			if(fonts[j].Error())
			{
				Exclamation(Format("Failed to open font %s", fontFiles[i]));
				exit(1);
			}
			
			if(!fonts[j].FaceSize(24))
			{
				Exclamation("Failed to set size");
				exit(1);
			}
			fonts[j].Depth(20);
			fonts[j].CharMap(ft_encoding_unicode);
		}
	}
	
	infoFont = new FTPixmapFont(files[0]);
	if(infoFont->Error())
	{
		Exclamation(Format("Failed to open font %s\n", files[0]));
		exit(1);
	}
	
	infoFont->FaceSize(18);
}


void FTGLCtrl::RenderFontmetrics()
{
	FTBBox bbox;
	float x1, y1, z1, x2, y2, z2;
	
	// If there is a layout, use it to compute the bbox, otherwise query as
	// a string.
	if(hasLayout())
		bbox = layouts[currentLayout].BBox(myString);
	else
		bbox = fonts[currentFont].BBox(myString);
	
	x1 = bbox.Lower().Xf(); y1 = bbox.Lower().Yf(); z1 = bbox.Lower().Zf();
	x2 = bbox.Upper().Xf(); y2 = bbox.Upper().Yf(); z2 = bbox.Upper().Zf();
	
	// Draw the bounding box
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
	
	glColor3f(0.0, 1.0, 0.0);
	// Draw the front face
	glBegin(GL_LINE_LOOP);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	glEnd();
	// Draw the back face
	if((GetStyle() == FTGL_EXTRUDE) && (z1 != z2))
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y1, z2);
		glEnd();
		// Join the faces
		glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y1, z2);
		
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y2, z2);
		
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2);
		
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y1, z2);
		glEnd();
	}
	
	// Render layout-specific metrics
	if(!hasLayout())
	{
		// There is no layout. Draw the baseline, Ascender and Descender
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(fonts[currentFont].Advance(myString), 0.0, 0.0);
		glVertex3f(0.0, fonts[currentFont].Ascender(), 0.0);
		glVertex3f(0.0, fonts[currentFont].Descender(), 0.0);
		glEnd();
	}
	else if (dynamic_cast <FTSimpleLayout *>(&layouts[currentLayout]))
	{
		float lineWidth = ((FTSimpleLayout const &)layouts[currentLayout]).GetLineLength();
	
		// The layout is a SimpleLayout.  Render guides that mark the edges
		// of the wrap region.
		glColor3f(0.5, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0, 10000, 0);
		glVertex3f(0, -10000, 0);
		glVertex3f(lineWidth, 10000, 0);
		glVertex3f(lineWidth, -10000, 0);
		glEnd();
	}
	
	// Draw the origin
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void FTGLCtrl::RenderFontInfo()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, GetSize().cx, 0, GetSize().cy);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// draw mode
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(20.0f , GetSize().cy - (20.0f + infoFont->Ascender()));
	
	switch(mode)
	{
		case EDITING:
			infoFont->Render("Edit Mode");
			break;

		case INTERACTIVE:
			break;
	}
	
	// draw font type
	glRasterPos2i(20 , 20);
	switch(GetStyle())
	{
		case FTGL_BITMAP:
			infoFont->Render("Bitmap Font");
			break;

		case FTGL_PIXMAP:
			infoFont->Render("Pixmap Font");
			break;

		case FTGL_OUTLINE:
			infoFont->Render("Outline Font");
			break;

		case FTGL_POLYGON:
			infoFont->Render("Polygon Font");
			break;

		case FTGL_EXTRUDE:
			infoFont->Render("Extruded Font");
			break;

		case FTGL_TEXTURE:
			infoFont->Render("Texture Font");
			break;
	}
	
	glRasterPos2f(20.0f , 20.0f + infoFont->Ascender() - infoFont->Descender());
	infoFont->Render(fontFiles[GetFace()]);
	
	// If the current layout is a SimpleLayout, output the alignemnt mode
	if(hasLayout() && (dynamic_cast <FTSimpleLayout *>(&layouts[currentLayout])))
	{
		glRasterPos2f(20.0f , 20.0f + 2*(infoFont->Ascender() - infoFont->Descender()));
		
		// Output the alignment mode of the layout
		switch (((FTSimpleLayout const &)layouts[currentLayout]).GetAlignment())
		{
			case FTGL::ALIGN_LEFT:
				infoFont->Render("Align Left");
				break;

			case FTGL::ALIGN_RIGHT:
				infoFont->Render("Align Right");
				break;

			case FTGL::ALIGN_CENTER:
				infoFont->Render("Align Center");
				break;

			case FTGL::ALIGN_JUSTIFY:
				infoFont->Render("Align Justified");
				break;
		}
	}
}

void FTGLCtrl::Paint0(void)
{
	switch(GetStyle())
	{
		case FTGL_BITMAP:
		case FTGL_PIXMAP:
		case FTGL_OUTLINE:
			break;

		case FTGL_POLYGON:
			glDisable(GL_BLEND);
			SetUpLighting();
			break;

		case FTGL_EXTRUDE:
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			SetUpLighting();
			break;

		case FTGL_TEXTURE:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			SetUpLighting();
			glNormal3f(0.0, 0.0, 1.0);
			break;
	}
	
	glColor3f(1.0, 1.0, 1.0);
	// If you do want to switch the color of bitmaps rendered with glBitmap,
	// you will need to explicitly call glRasterPos (or its ilk) to lock
	// in a changed current color.
	
	// If there is an active layout use it to render the font
	if (hasLayout())
		layouts[currentLayout].Render(myString);
	else
		fonts[currentFont].Render(myString);
	RenderFontmetrics();
	RenderFontInfo();
}

void FTGLCtrl::SetCamera(void)
{
	switch(GetStyle())
	{
		case FTGL_BITMAP:
		case FTGL_PIXMAP:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, GetSize().cx, 0, GetSize().cy);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			break;

		case FTGL_OUTLINE:
		case FTGL_POLYGON:
		case FTGL_EXTRUDE:
		case FTGL_TEXTURE:
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity ();
			gluPerspective(90, (float)GetSize().cx / (float)GetSize().cy, 1, 1000);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0.0, 0.0, (float)GetSize().cy / 2.0f, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
			break;
	}
}

void FTGLCtrl::Layout()
{
	GLCtrl::Layout();
	glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, GetSize().cx, GetSize().cy);
	glLoadIdentity();
	SetCamera();
	trackBall.Reshape(GetSize().cx, GetSize().cy);
}

void FTGLCtrl::GLPaint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SetCamera();
	
	switch(GetStyle())
	{
		case FTGL_BITMAP:
		case FTGL_PIXMAP:
			glRasterPos2i((long)(GetSize().cx / 2 + OX), (long)(GetSize().cy / 2 + OY));
			glTranslatef(GetSize().cx / 2 + OX, GetSize().cy / 2 + OY, 0.0);
			break;

		case FTGL_OUTLINE:
		case FTGL_POLYGON:
		case FTGL_EXTRUDE:
		case FTGL_TEXTURE:
			glTranslatef(OX, OY, 0);
			trackBall.Matrix();
			break;
	}

	glPushMatrix();
	Paint0();
	glPopMatrix();
}
