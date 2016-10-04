#include "robot.h"

int OpenGL::InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}

void OpenGL::GLPaint()
{
	//StdView();



	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Size sz = GetSize();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();





	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glRotatef(45,1.0f,0,0);
//glRotatef(sceneroty,0,1.0,0);
		glTranslatef(-0.0f,-50.0f,-50.0f);						// Move Right 1.5 Units And Into The Screen 7.0
	//glRotatef(rquad,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis ( NEW )

glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);									// Draw A Quad

		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glTexCoord2f(0.0f, 1.0f);glVertex3f( szerokosc/2, 0.0f,-dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-szerokosc/2, 0.0f,-dlugosc/2);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f, 1.0f);glVertex3f(-szerokosc/2, 0.0f, dlugosc/2);					// Bottom Left Of The Quad (Top)
		glTexCoord2f(0.0f, 1.0f);glVertex3f( szerokosc/2, 0.0f, dlugosc/2);					// Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( szerokosc/2, 0.0f,-dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 1.0f);glVertex3f( szerokosc/2, wysokosc,-dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 0.0f);glVertex3f(-szerokosc/2, wysokosc,-dlugosc/2);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-szerokosc/2, 0.0f,-dlugosc/2);					// Top Left Of The Quad (Top)

		glColor3f(1.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( szerokosc/2, 0.0f, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 1.0f);glVertex3f( szerokosc/2, wysokosc, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 0.0f);glVertex3f(-szerokosc/2, wysokosc, dlugosc/2);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-szerokosc/2, 0.0f, dlugosc/2);					// Top Left Of The Quad (Top)


		glColor3f(1.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( szerokosc/2, 0.0f, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 1.0f);glVertex3f( szerokosc/2, wysokosc, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 0.0f);glVertex3f(szerokosc/2, wysokosc, -dlugosc/2);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f);glVertex3f(szerokosc/2, 0.0f, -dlugosc/2);					// Top Left Of The Quad (Top)

		glColor3f(0.0f,1.0f,1.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -szerokosc/2, 0.0f, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 1.0f);glVertex3f( -szerokosc/2, wysokosc, dlugosc/2);					// Top Right Of The Quad (Top)
		glTexCoord2f(20.0f, 0.0f);glVertex3f(-szerokosc/2, wysokosc, -dlugosc/2);					// Top Left Of The Quad (Top)
		glTexCoord2f(1.0f, 0.0f);glVertex3f(-szerokosc/2, 0.0f, -dlugosc/2);					// Top Left Of The Quad (Top)


	/*	glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
		*/
	glEnd();
	for (int i=0;i<ilP;i++)
	{
		glBegin(GL_QUADS);									// Draw A Quad
			glColor3f(1.0f,1.0f,1.0f);						// Set The Color To Green
			glVertex3f( listaP[i].x1, wysokosc,listaP[i].y1);					// Top Right Of The Quad (Top)
			glVertex3f(listaP[i].x2, wysokosc,listaP[i].y1);					// Top Left Of The Quad (Top)
			glVertex3f(listaP[i].x2, wysokosc,listaP[i].y2);					// Bottom Left Of The Quad (Top)
			glVertex3f(listaP[i].x1, wysokosc,listaP[i].y2);					// Bottom Right Of The Quad (Top)

			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Green
			glVertex3f( listaP[i].x1, wysokosc,listaP[i].y1);					// Top Right Of The Quad (Top)
			glVertex3f(listaP[i].x2, wysokosc,listaP[i].y1);					// Top Left Of The Quad (Top)
			glVertex3f(listaP[i].x2, 0,listaP[i].y1);					// Top Left Of The Quad (Top)
			glVertex3f( listaP[i].x1, 0,listaP[i].y1);					// Top Right Of The Quad (Top)

			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Green
			glVertex3f( listaP[i].x1, wysokosc,listaP[i].y2);					// Top Right Of The Quad (Top)
			glVertex3f(listaP[i].x2, wysokosc,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f(listaP[i].x2, 0,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f( listaP[i].x1, 0,listaP[i].y2);					// Top Right Of The Quad (Top)

			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Green
			glVertex3f( listaP[i].x1, wysokosc,listaP[i].y1);					// Top Right Of The Quad (Top)
			glVertex3f(listaP[i].x1, wysokosc,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f(listaP[i].x1, 0,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f( listaP[i].x1, 0,listaP[i].y1);					// Top Right Of The Quad (Top)

			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Green
			glVertex3f( listaP[i].x2, wysokosc,listaP[i].y1);					// Top Right Of The Quad (Top)
			glVertex3f(listaP[i].x2, wysokosc,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f(listaP[i].x2, 0,listaP[i].y2);					// Top Left Of The Quad (Top)
			glVertex3f( listaP[i].x2, 0,listaP[i].y1);					// Top Right Of The Quad (Top)



		glEnd();
	}

		glTranslatef(ry,1.0f,rx);						// Move Right 1.5 Units And Into The Screen 7.0
	//glRotatef(rquad*100,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis ( NEW )
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,1.0f,1.0f);						// Set The Color To Green
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
	glEnd();											// Done Drawing The Quad


	sceneroty+=1;
};

AUX_RGBImageRec *OpenGL::LoadBMP(char *Filename)                // Loads A Bitmap Image
{
        FILE *File=NULL;                                // File Handle

        if (!Filename)                                  // Make Sure A Filename Was Given
        {
                return NULL;                            // If Not Return NULL
        }

        File=fopen(Filename,"r");                       // Check To See If The File Exists

        if (File)                                       // Does The File Exist?
        {
                fclose(File);                           // Close The Handle
                return auxDIBImageLoad(Filename);       // Load The Bitmap And Return A Pointer
        }
        return NULL;                                    // If Load Failed Return NULL
}

int OpenGL::LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("data/nehe.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture[0]);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;										// Return The Status
}


GLvoid OpenGL::ReSizeGLScene(GLsizei width, GLsizei height)
{
 	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
};

void OpenGL::NextMove()
{
	if ((rx+dx+1>dlugosc/2) || (rx+dx-1<-dlugosc/2)) dx=-dx;
	if ((ry+dy+1>szerokosc/2) || (ry+dy-1<-szerokosc/2)) dy=-dy;
	for (int i=0;i<ilP;i++)
	{
		if ((rx+dx+1>listaP[i].x1) && (rx+dx+1<listaP[i].x2)&&(ry+dy+1>listaP[i].y1) && (ry+dy+1<listaP[i].y2)) dx=-dx;
		//if ((ry+dy+1>listaP[i].y1) && (ry+dy+1<listaP[i].x2)) dy=-dy;
	}

	rx+=dx;
	ry+=dy;

}


robot::robot()
{

	gl.SetFrame(InsetFrame());
	Add(gl.HSizePos(10, 10).VSizePos(110, 10));
	gl.wysokosc=2;
	gl.lookupdown=0;
	gl.sceneroty=0;
	gl.dx=1.3f;
	gl.dy=1.3f;
	gl.rx=0;
	gl.ry=0;
	gl.InitGL();
	CtrlLayout(zadanie);
    tab.Add(zadanie, "Parametry zadania");
	zadanie.SizePos();
    CtrlLayout(teren);
    tab.Add(teren, "Parametry terenu");
	teren.SizePos();

	teren.lista.AddColumn("x1",0).Edit(x1);
	teren.lista.AddColumn("y1",0).Edit(y1);
	teren.lista.AddColumn("x2",0).Edit(x2);
	teren.lista.AddColumn("y2",0).Edit(y2);
	teren.lista.Appending().Removing();

	CtrlLayout(probota);
    tab.Add(probota, "Parametry robota");
	probota.SizePos();
	SetTimeCallback(-1, THISBACK(Change));
	teren.Bteren <<= THISBACK(UstawTeren);
	teren.ilP<<=1;
	teren.Edlugosc<<=60;
	teren.Eszerokosc<<=80;
	teren.lista.Set(0,0,-25);
	teren.lista.Set(0,1,-20);
	teren.lista.Set(0,2,-5);
	teren.lista.Set(0,3,20);
	//teren.lista.OddRowColor(SWhite,SWhite);
//	teren.lista.SetEditable();
//	teren.lista.StartEdit(0);
//	teren.lista.IsEdit=true;
	teren.ilP.WhenAction=THISBACK(ilPrzeszkod);
	ilPrzeszkod();
	UstawTeren();
	Sizeable().Zoomable();
	CtrlLayout(*this, "Symulacja ruchu robota");

}

void robot::Change()
{
	gl.NextMove();
	gl.Refresh();
}
void robot::ilPrzeszkod()
{
	if (teren.ilP>0 && teren.ilP<100) teren.lista.SetCount(teren.ilP);
}
void robot::UstawTeren()
{
    _przeszkoda P;
    gl.ilP=teren.ilP;
    gl.listaP.Clear();
    for (int i=0;i<gl.ilP;i++)
    {
        P.x1=(double)teren.lista.Get(i,0);
        P.y1=(double)teren.lista.Get(i,1);
        P.x2=(double)teren.lista.Get(i,2);
        P.y2=(double)teren.lista.Get(i,3);
        gl.listaP.Add(P);
    }
	gl.szerokosc=(GLfloat)teren.Eszerokosc;
	gl.dlugosc=(GLfloat)teren.Edlugosc;
	gl.Refresh();
}

GUI_APP_MAIN
{
	robot().Run();
}
