#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

void OpenGLFont::LoadBrc(const byte* xml, const byte** imagesData, const int* imagesSize, int imagesCount)
{
	Parse((const char*) xml, false);
	for(int i = 0; i < imagesCount; i++)
	{
		MemStream ms((void*) imagesData[i], imagesSize[i]);
		Image img = StreamRaster::LoadAny(ms);
		images.Add(img);
		if(preload)
			resources.Add(img, true);
	}
}

void OpenGLFont::Load(const String& fileName, bool preload)
{
	String filePath = GetDataFile(fileName);
	String xml = LoadFile(filePath);
	Parse(xml, true);
}

void OpenGLFont::Parse(const char* xml, bool parsePages)
{
	chars.SetCount(512);
	
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
						Image img = StreamRaster::LoadFileAny(GetDataFile(fileName));
						images.Add(img);
						if(preload)
							resources.Add(img, true);
					}
					pages.Add(StrInt(p["id"]));
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
					int id = p.Int("id");
					CharInfo& ci = chars[id];
					
					ci.id = id;
					ci.x = (float) p.Double("x");
					ci.y = (float) p.Double("y");
					ci.width = (float) p.Double("width");
					ci.height = (float) p.Double("height");
					ci.xoffset = (float) p.Double("xoffset");
					ci.yoffset = (float) p.Double("yoffset");
					ci.xadvance = (float) p.Double("xadvance");
					ci.page = p.Int("page");
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
				
	for(int i = 0; i < images.GetCount(); i++)
	{
		const TextureResource& t = resources.Bind(images[i], Resources::LINEAR_FILTERING);
		pages[i] = t.serialId;
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
	OpenGLFont& fi = resources.GetFont(font);
	
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
	
	#ifdef COLOR_ALPHA
	float ac = alpha / 255.f;
	int inkA = ink.GetA();
	int outlineA = outlineColor.GetA();
	int glowA = glowColor.GetA();
	#else
	float ac = alpha;
	int inkA = 1;
	int outlineA = 1;
	int glowA = 1;
	#endif
	
	alphaMagProg.SetUniform("GlyphColor", ink.GetR() * ic, ink.GetG() * ic, ink.GetB() * ic, inkA * ic * ac);
	alphaMagProg.SetUniform("OutlineColor", outlineColor.GetR() * ic, outlineColor.GetG() * ic, outlineColor.GetB() * ic, outlineA * ic * ac);
	alphaMagProg.SetUniform("GlowColor", glowColor.GetR() * ic, glowColor.GetG() * ic, glowColor.GetB() * ic, glowA * ic * ac);
	alphaMagProg.SetUniform("Outline", outlineStrength > 0); //0.1 - 0.45
	alphaMagProg.SetUniform("Glow", glowStrength > 0); //0.55 - 0.095
	alphaMagProg.SetUniform("Shadow", false);
	alphaMagProg.SetUniform("OutlineCenter", outlineCenter);
	alphaMagProg.SetUniform("GlowCenter", glowCenter);
	
	float xp = (float) x;
	float yp = (float) y;

	int page = -1;

	float sw = (float) fi.scaleW;
	float sh = (float) fi.scaleH;
	
	float tw = 1.f / sw;
	float th = 1.f / sh;

	while(*s && n > 0)
	{
		int cn = *s;
		
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
	
			cn <<= 3;
	
			if(ci.page != page)
			{
				resources.Bind(fi.pages[ci.page], Resources::LINEAR_FILTERING);
				glActiveTexture(GL_TEXTURE0);
				alphaMagProg.SetUniform("Texture", 0);
				page = ci.page;
			}
			
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

				tl = (tl + TEXEL_OFFSET) * tw;
				tr = (tr - TEXEL_OFFSET) * tw;
				tt = (tt + TEXEL_OFFSET) * th;
				tb = (tb - TEXEL_OFFSET) * th;
				/*
				float vtx[] = {
					sx, dy,
					sx, sy,
					dx, dy,
					dx, sy
				};*/
				float vtx[12];
				SetVtx(vtx, sx, sy, dx, dy);

				float crd[] = {
					tl, tb,
					tl, tt,
					tr, tb,
					tr, tt
				};
			
				glTexCoordPointer(2, GL_FLOAT, 0, crd);
				glVertexPointer(projection_mode ? 3: 2, GL_FLOAT, 0, vtx);
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
	GuiLock __;
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
		int cn = *wtext++;
		if(cn >= 0 && cn < fi.chars.GetCount())
		{
			const OpenGLFont::CharInfo& ci = fi.chars[cn];
			sz.cx += int(ci.xadvance * fi.scale + 0.5f);
			int k = fi.kerns.Find(cn);
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