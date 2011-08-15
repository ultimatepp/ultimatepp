#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL

NAMESPACE_UPP

void OpenGLFont::LoadBrc(const byte* xml, const byte* image)
{
	Parse((const char*) xml, false);
	compiledFiles.Add((byte*) image);
	pages.Add();
}

void OpenGLFont::Load(const String& fileName)
{
	String filePath = GetDataFile(fileName);
	String xml = LoadFile(filePath);
	Parse(xml, true);
}

void OpenGLFont::Parse(const char* xml, bool parsePages)
{
	XmlParser p(xml);

	while(!p.IsTag())
		p.Skip();

	p.PassTag("font");
	
	while(!p.End())
	{
		if(p.TagE("common"))
		{
			scaleW = (float) p.Double("scaleW");
			scaleH = (float) p.Double("scaleH");
			lineHeight = (float) p.Double("lineHeight");
			base = (float) p.Double("base");
		}
		else if(p.Tag("pages"))
		{
			while(!p.End())
			{
				if(p.TagE("page"))
				{
					if(parsePages)
					{
						String fileName = p["file"];
						files.Add(fileName);
					}
					pages.Add(-1);
				}
				else
					p.Skip();
			}
			
		}
		else if(p.Tag("chars"))
		{
			while(!p.End())
			{
				if(p.TagE("char"))
				{
					CharInfo& ci = chars.Add();
					
					int page = p.Int("page");
				
					ci.id = p.Int("id");
					ci.x = (float) p.Double("x");
					ci.y = (float) p.Double("y");
					ci.width = (float) p.Double("width");
					ci.height = (float) p.Double("height");
					ci.xoffset = (float) p.Double("xoffset");
					ci.yoffset = (float) p.Double("yoffset");
					ci.xadvance = (float) p.Double("xadvance");
					ci.page = page;					
				}
				else
					p.Skip();
			}
		}
		else if(p.Tag("kernings"))
		{
			while(!p.End())
			{
				if(p.TagE("kerning"))
				{
					int first = p.Int("first"); 
					int second = p.Int("second");
					float amount = (float) p.Double("amount");
					
					VectorMap<int, float>& vm = kerns.GetAdd(first);
					vm.Add(second, amount);
				}
				else
					p.Skip();
			}
		}		
		else
			p.Skip();
	}
}

void OpenGLFont::UpdateTextures()
{
	if(texturesUpdated)
		return;
				
	for(int i = 0; i < files.GetCount(); i++)
	{
		Image img = StreamRaster::LoadFileAny(GetDataFile(files[i]));
		int64 serialId = Resources::Bind(img, true);
		pages[i] = serialId;
	}
	
	for(int i = 0; i < compiledFiles.GetCount(); i++)
	{
/*		DUMP(compiledFiles[i]);
		MemStream ms((void*) resTahoma14Img, resTahoma14Img_length);
		Image img = StreamRaster::LoadAny(ms);
		int64 serialId = Resources::Bind(img, true);
		pages[i] = serialId;*/
		int64 serialId = Resources::Bind(WinGlImg::tahoma(), true);
		pages[i] = serialId;
	}

	texturesUpdated = true;
}

float ConvStrength(float min, float max, float p)
{
	return min + (max - min) * p / 100.f;
}

void SystemDraw::Text(int x, int y, int angle, const wchar *text, Font font, Color ink, int outlineStrength, Color outlineColor, int glowStrength, Color glowColor, int n, const int *dx)
{
	if(!text)
		return;

	const wchar* s = text;	
	OpenGLFont& fi = Resources::GetFont(font);
	
	glEnable(GL_TEXTURE_2D);
	
	#if CLIP_MODE == 3
	float cl = (float) clip.left;
	float ct = (float) clip.top;
	float cr = (float) clip.right;
	float cb = (float) clip.bottom;
	#endif
	fi.UpdateTextures();	

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	alphaMagProg.Start();

	const float ic = 1.f / 255.f;

	float outlineCenter = ConvStrength(0.1f, 0.45f, (float) 100 - min(outlineStrength, 100));
	float glowCenter = ConvStrength(0.55f, 0.95f, (float) 100 - min(glowStrength, 100));
	
	float a = alpha * ic;
	
	alphaMagProg.Set("GlyphColor", ink.GetR() * ic, ink.GetG() * ic, ink.GetB() * ic, a);
	alphaMagProg.Set("OutlineColor", outlineColor.GetR() * ic, outlineColor.GetG() * ic, outlineColor.GetB() * ic, a);
	alphaMagProg.Set("GlowColor", glowColor.GetR() * ic, glowColor.GetG() * ic, glowColor.GetB() * ic, a);
	alphaMagProg.Set("Outline", outlineStrength > 0); //0.1 - 0.45
	alphaMagProg.Set("Glow", glowStrength > 0); //0.55 - 0.095
	alphaMagProg.Set("Shadow", false);
	alphaMagProg.Set("OutlineCenter", outlineCenter);
	alphaMagProg.Set("GlowCenter", glowCenter);
	
	float xp = (float) x;
	float yp = (float) y;
	
	while(*s && n > 0)
	{
		int ch = *s;
		int cn = ch - 32;
		
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
	
			cn <<= 3;
	
			Resources::Bind(fi.pages[ci.page], true);
			glActiveTexture(GL_TEXTURE0);
			alphaMagProg.Set("Texture", 0);
			
			float sx = (float) ci.xoffset * fi.scale + xp + drawing_offset.x;
			float sy = (float) ci.yoffset * fi.scale + yp + drawing_offset.y;
			float dx = sx + ci.width * fi.scale;
			float dy = sy + ci.height * fi.scale;

			#if CLIP_MODE == 3
			if(sx <= clip.right && sy <= clip.bottom && dx >= clip.left && dy >= clip.top)
			#endif
			{
				float tl = (float) ci.x;
				float tt = (float) ci.y;
				float tr = (float) ci.x + ci.width;
				float tb = (float) ci.y + ci.height;
				
				float sw = (float) fi.scaleW;
				float sh = (float) fi.scaleH;
				
				#if CLIP_MODE == 3
				if(sx < cl)
				{
					tl += (cl - sx);
					sx = cl;
				}
				
				if(sy < ct)
				{
					tt += (ct - sy);
					sy = ct;
				}
				
				if(dx > cr)
				{
					tr -= dx - cr;
					dx = cr;
				}
				
				if(dy > cb)
				{
					tb -= dy - cb;
					dy = cb;
				}
				#endif
				
				float tw = 1.f / sw;
				float th = 1.f / sh;
				
				tl *= tw;
				tt *= th;
				tr *= tw;
				tb *= th;
		
				float vtx[] = {
					sx, dy,
					sx, sy,
					dx, dy,
					dx, sy
				};
			
				float crd[] = {
					tl, tb,
					tl, tt,
					tr, tb,
					tr, tt
				}; 

				glTexCoordPointer(2, GL_FLOAT, 0, crd);
				glVertexPointer(2, GL_FLOAT, 0, vtx);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
	
			xp += int(ci.xadvance * fi.scale + 0.5f);
			
			int k = fi.kerns.Find(*s);
			if(k >= 0)
				xp += int(fi.kerns[k].Get(*(s + 1), 0) * fi.scale);
		}
		
		++s;
		--n;
	}

	alphaMagProg.Stop();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	Text(x, y, angle, text, font, ink, 0, White, 0, White, n, dx);
}

Size GetTextSize(const wchar *text, const OpenGLFont& fi, int n)
{
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		int ch = *wtext++;
		int cn = ch - 32;
		
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
			sz.cx += int(ci.xadvance * fi.scale + 0.5f);
			int k = fi.kerns.Find(ch);
			if(k >= 0)
				sz.cx += int(fi.kerns[k].Get(*wtext, 0) * fi.scale);
		}
		n--;
	}
	sz.cy = int(fi.lineHeight * fi.scale + 0.5f);
	return sz;
}

END_UPP_NAMESPACE

#endif