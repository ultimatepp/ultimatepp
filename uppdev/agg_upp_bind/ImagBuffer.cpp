#include "agg_upp_bind.h"
//#include "ImagBuffer.h"

NAMESPACE_UPP

RGBA ImagBuffer::GetPixel(int x, int y) const
{
	const RGBA *p=this[0] + y*GetSize().cx + x;
	return *p;
}

void ImagBuffer::SetPixel(int x, int y, RGBA color)
{
	RGBA *p = this[0] + y*GetSize().cx + x;
	*p=color;
}


void ImagBuffer::SetPixel(int x, int y, Color color, int alpha)
{
	RGBA ba;
	ba.a = alpha;
	ba.b = color.GetB();
	ba.r = color.GetR();
	ba.g = color.GetG();
	RGBA *p = this[0] + y*GetSize().cx + x;
//	p+=y*GetSize().cx+x;
	p->a=ba.a;
	p->r=ba.r;
	p->g=ba.g;
	p->b=ba.b;
}

bool ImagBuffer::IsEqualColumn(int x, int y, int height, RGBA rgba)
{
	RGBA *p = this[0] + (y+height-1)*GetSize().cx + x;
	
	while(height>0) {
		if (*p!=rgba) {
			return false;
		}
		p-=GetSize().cx;
		--height;
	}
	return true;
}

void ImagBuffer::SetLineV(int x, int y, int height, RGBA rgba)
{
	ASSERT(height>0);   //don't make height <=0 !!! later change to out of bounds...
	RGBA *p = this[0] + (y+height-1)*GetSize().cx + x;
	while(height>0) {
		*p=rgba;
		p-=GetSize().cx;
		--height;
	}
}

Image ImagBuffer::CreateCopy(int x, int y, int width, int height)
{
	ImageBuffer imgb(width,height);
	RGBA *src = this[0] + y *GetSize().cx+ x;
	int dy=0;
	while(--height >= 0) {
		memcpy(imgb[dy++] , src, width * sizeof(RGBA));
		src+=GetSize().cx;
	}
	return imgb;
}


void ImagBuffer::Rectangalize(Draw& w, bool norect) //showrect for DEBUG 
{
	Size bsz=GetSize();
	int cx=bsz.cx;
	int cy=bsz.cy;
	
	RGBA back=RGBA(Color(255,200,200));   //for background blending
//	memset(uibuf, 255, cx * cy * 4); //!!!change or remove this for the background...


int x=0;
int y=0;
	int band=1;
int yy = 0;

	while(yy < cy) {  //going through each full horizontal 16 pix strip
		int ccy = min(cy - yy, 16); 
//		Image m = imgA.InitRender(aggTriangle(m_x,m_y), 0, -yy, 600, ccy);  //render some lines (rectangle of image)
		int x2 = 0;
		int x1 = 0;
		int ximg1 = -1;
		int ximg2 = -1;
		RGBA c;

		while(x2 < cx) {  //while inside a horizontal strip - going through each several times!!!
			c = GetPixel(x1, yy);

			while(IsEqualColumn(x2, yy, ccy, c) && x2 < cx) {
				++x2;  //end of a mono-rectangle
				}

			if(  (x2 - x1 > 0) && (x2 - x1 <= 16) ) { //first small mono-rectangle
				if( ximg2-ximg1 == 0 ) {
					ximg1=x1;  //start the very first small mono-rectangle as image series
				}
				ximg2=x2;
			}
		
			else  { 
				  if( x2 - x1 > 16 ) {
					if (ximg2-ximg1>0) {   // Step1 draw as accumulated image
						w.DrawImage( x + ximg1, y + yy, CreateCopy(ximg1, yy, ximg2 - ximg1, ccy) );
						ximg1=ximg2=-1;
					}
					//Step 2 draw mono-rectangle
					
					if (norect) {  //if false - it paints only near the img, other area is 100 % transparent
					//	RGBA c1=back;
						RGBA c1=c;												
//						AlphaBlend(&c1,&c, 1, Color(c.r, c.g, c.b)); //Aris FIXME
						w.DrawRect(x + x1, y + yy, x2 - x1, ccy, 
							Color(c1.r, c1.g, c1.b)); //Mirek's old TODO - Color RGBA conversion?
					}
				}
			}
			
			x1 = x2;  //x counter forward because we processed the solid part

			//below means pure image
			while(!IsEqualColumn( x2, yy, ccy, GetPixel(x2, yy)) && x2 < cx)
				x2++;
					
			if( x2 - x1 > 0) { //piece of image found
				if ( ximg2-ximg1 == 0 ) {
					ximg1 = x1;
					}
				ximg2 = x2;
				x1 = x2;  //x counter forward
				}

		}  //end of a strip

		if (ximg2-ximg1>0) {  //after the end of strip paint the image
			w.DrawImage( x + ximg1, y + yy, CreateCopy(ximg1, yy, ximg2 - ximg1, ccy));
			}
	
		yy += ccy;  //next strip
//		if (yy>=8) break;
	}  //end of all strips

}

#if 0
		typedef agg::pixfmt_bgra32 pixfmt;
		typedef agg::renderer_base<pixfmt> renderer_base;
		typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;		
		agg::rendering_buffer rbuf;
	
	//	sz-=50;	
		uibuf.Create(sz);  //aris002
			rbuf.attach((agg::int8u *)uibuf[0], sz.cx, sz.cy, sz.cx*4); //THIS IS THE MAIN agg and upp CONNECTION!!!
		pixfmt pixf(rbuf);
		
		renderer_base rb(pixf);
		renderer_solid ren(rb);

		rb.clear(agg::rgba(1,0.8,0.8));

		agg::rasterizer_scanline_aa<> ras;
		agg::scanline_p8 sl;
		agg::trans_affine mtx;

		
//		mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
//		mtx *= agg::trans_affine_scaling(g_scale, g_scale);
//		mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
//		mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
//		mtx *= agg::trans_affine_translation(sz.cx/2, sz.cy/2);
		mtx *= agg::trans_affine_translation(300, 0);
		mtx *= agg::trans_affine_rotation(agg::deg2rad(time.second*6));
//		mtx *= agg::trans_affine_translation(300, -sz.cy/2);


        // This code renders the lion:
	//	agg::conv_transform<agg::path_storage, agg::trans_affine> trans(g_path, mtx);
	//	agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);

		
		m_path.render(ras, sl, ren, mtx, rb.clip_box(), 1.0);
	//	double tm; // = elapsed_time();
		unsigned vertex_count = m_path.vertex_count();
	//	m_path.bounding_rect(double* x1¸ double* y1¸ double* x2¸ double* y2)

		String txt;
		agg::gsv_text t;
		t.size(12.0);
		t.flip(true);

		agg::conv_stroke<agg::gsv_text> pt(t);
		pt.width(1.5);

//		sprintf(buf, "Vertices=%d Time=%.3f ms", vertex_count, tm);
		txt<<"Vertices="<<vertex_count<<" Time="<<time; 

		t.start_point(10.0, 40.0);
		t.text(txt);

		ras.add_path(pt);
		ren.color(agg::rgba(0,0,0));
		agg::render_scanlines(ras, sl, ren);
#endif

END_UPP_NAMESPACE
