#include "AggCtrl.h"

#ifdef PLATFORM_WIN32

static void ConvertPmap(agg::rendering_buffer* dst,const agg::rendering_buffer* src,agg::pix_format_e format){
	switch(format){
		case agg::pix_format_gray8:  break;
		case agg::pix_format_gray16: agg::color_conv(dst, src, agg::color_conv_gray16_to_gray8()); break;
		case agg::pix_format_rgb565: agg::color_conv(dst, src, agg::color_conv_rgb565_to_rgb555());break;
		case agg::pix_format_rgbAAA: agg::color_conv(dst, src, agg::color_conv_rgbAAA_to_bgr24()); break;
		case agg::pix_format_bgrAAA: agg::color_conv(dst, src, agg::color_conv_bgrAAA_to_bgr24()); break;
		case agg::pix_format_rgbBBA: agg::color_conv(dst, src, agg::color_conv_rgbBBA_to_bgr24()); break;
		case agg::pix_format_bgrABB: agg::color_conv(dst, src, agg::color_conv_bgrABB_to_bgr24()); break;
		case agg::pix_format_rgb24:  agg::color_conv(dst, src, agg::color_conv_rgb24_to_bgr24());  break;
		case agg::pix_format_rgb48:  agg::color_conv(dst, src, agg::color_conv_rgb48_to_bgr24());  break;
		case agg::pix_format_bgr48:  agg::color_conv(dst, src, agg::color_conv_bgr48_to_bgr24());  break;
		case agg::pix_format_abgr32: agg::color_conv(dst, src, agg::color_conv_abgr32_to_bgra32());break;
		case agg::pix_format_argb32: agg::color_conv(dst, src, agg::color_conv_argb32_to_bgra32());break;
		case agg::pix_format_rgba32: agg::color_conv(dst, src, agg::color_conv_rgba32_to_bgra32());break;
		case agg::pix_format_bgra64: agg::color_conv(dst, src, agg::color_conv_bgra64_to_bgra32());break;
		case agg::pix_format_abgr64: agg::color_conv(dst, src, agg::color_conv_abgr64_to_bgra32());break;
		case agg::pix_format_argb64: agg::color_conv(dst, src, agg::color_conv_argb64_to_bgra32());break;
		case agg::pix_format_rgba64: agg::color_conv(dst, src, agg::color_conv_rgba64_to_bgra32());break;
	}
};

AggCtrl::AggCtrl(agg::pix_format_e format):
pixformat(format), bpp(0),
sysformat(agg::pix_format_undefined),
flip_y(false),
cx(16),cy(16)
{
	// Set bpp and sysbpp from pixel format
	switch(pixformat)
	{
		case agg::pix_format_bw:
			sysformat = agg::pix_format_bw;
			bpp = 1;
			sysbpp = 1;
			break;
		case agg::pix_format_gray8:
			sysformat = agg::pix_format_gray8;
			bpp = 8;
			sysbpp = 8;
			break;
		case agg::pix_format_gray16:
			sysformat = agg::pix_format_gray8;
			bpp = 16;
			sysbpp = 8;
			break;
		case agg::pix_format_rgb565:
		case agg::pix_format_rgb555:
			sysformat = agg::pix_format_rgb555;
			bpp = 16;
			sysbpp = 16;
			break;
		case agg::pix_format_rgbAAA:
		case agg::pix_format_bgrAAA:
		case agg::pix_format_rgbBBA:
		case agg::pix_format_bgrABB:
			sysformat = agg::pix_format_bgr24;
			bpp = 32;
			sysbpp = 24;
			break;
		case agg::pix_format_rgb24:
		case agg::pix_format_bgr24:
			sysformat = agg::pix_format_bgr24;
			bpp = 24;
			sysbpp = 24;
			break;
		case agg::pix_format_rgb48:
		case agg::pix_format_bgr48:
			sysformat = agg::pix_format_bgr24;
			bpp = 48;
			sysbpp = 24;
			break;
		case agg::pix_format_bgra32:
		case agg::pix_format_abgr32:
		case agg::pix_format_argb32:
		case agg::pix_format_rgba32:
			sysformat = agg::pix_format_bgra32;
			bpp = 32;
			sysbpp = 32;
			break;
		case agg::pix_format_bgra64:
		case agg::pix_format_abgr64:
		case agg::pix_format_argb64:
		case agg::pix_format_rgba64:
			sysformat = agg::pix_format_bgra32;
			bpp = 64;
			sysbpp = 32;
			break;
	}
	CreatePmap(cx,cy,&rbuf);
	// User initialization code
	onInit();
};
void AggCtrl::CreatePmap(unsigned width,unsigned height,agg::rendering_buffer* rb){
	// Create a pixmap and attach a buffer to it
	pmap.create(width,height,agg::org_e(bpp));
	rb->attach(pmap.buf(),pmap.width(),pmap.height(),flip_y ?pmap.stride():-pmap.stride());
};
void AggCtrl::PaintAgg(Upp::Draw& dest){PaintAgg(&rbuf,dest);};
void AggCtrl::PaintAgg(const agg::rendering_buffer* src,Upp::Draw& dest){
	if(sysformat==pixformat){
		// Pixmap is in the right format, put it on display
		pmap.draw(dest.GetHandle());
	}else{
		// Pixmap must be converted to the right pixel format first
		agg::pixel_map pmap_tmp;
		pmap_tmp.create(pmap.width(),pmap.height(),agg::org_e(sysbpp));
		agg::rendering_buffer rbuf_tmp;
		rbuf_tmp.attach(pmap_tmp.buf(),pmap_tmp.width(),pmap_tmp.height(),flip_y?pmap_tmp.stride():-pmap_tmp.stride());
		ConvertPmap(&rbuf_tmp, src, pixformat);
		pmap_tmp.draw(dest.GetHandle());
	}
};
void AggCtrl::Resized(){
	// Update the dimensions and reinitialize the pixmap with new size
	cx=GetSize().cx;cy=GetSize().cy;
	CreatePmap(cx,cy,&rbuf);
	// User resize code
	onResize(cx,cy);
};
AggCtrl::~AggCtrl() {
	pmap.destroy();
};
void AggCtrl::Paint(Upp::Draw& draw) {
	// Check if the control was resized
	if(cx!=GetSize().cx||cy!=GetSize().cy){Resized();}
	// User Drawing code
	onDraw(draw);
	// Draw to screen
	PaintAgg(draw);
};
void AggCtrl::SetPixFmt(agg::pix_format_e format){
	pixformat=format;
	switch(pixformat)
	{
		case agg::pix_format_bw:
			sysformat = agg::pix_format_bw;
			bpp = 1;
			sysbpp = 1;
			break;
		case agg::pix_format_gray8:
			sysformat = agg::pix_format_gray8;
			bpp = 8;
			sysbpp = 8;
			break;
		case agg::pix_format_gray16:
			sysformat = agg::pix_format_gray8;
			bpp = 16;
			sysbpp = 8;
			break;
		case agg::pix_format_rgb565:
		case agg::pix_format_rgb555:
			sysformat = agg::pix_format_rgb555;
			bpp = 16;
			sysbpp = 16;
			break;
		case agg::pix_format_rgbAAA:
		case agg::pix_format_bgrAAA:
		case agg::pix_format_rgbBBA:
		case agg::pix_format_bgrABB:
			sysformat = agg::pix_format_bgr24;
			bpp = 32;
			sysbpp = 24;
			break;
		case agg::pix_format_rgb24:
		case agg::pix_format_bgr24:
			sysformat = agg::pix_format_bgr24;
			bpp = 24;
			sysbpp = 24;
			break;
		case agg::pix_format_rgb48:
		case agg::pix_format_bgr48:
			sysformat = agg::pix_format_bgr24;
			bpp = 48;
			sysbpp = 24;
			break;
		case agg::pix_format_bgra32:
		case agg::pix_format_abgr32:
		case agg::pix_format_argb32:
		case agg::pix_format_rgba32:
			sysformat = agg::pix_format_bgra32;
			bpp = 32;
			sysbpp = 32;
			break;
		case agg::pix_format_bgra64:
		case agg::pix_format_abgr64:
		case agg::pix_format_argb64:
		case agg::pix_format_rgba64:
			sysformat = agg::pix_format_bgra32;
			bpp = 64;
			sysbpp = 32;
			break;
	}
	Resized();
};

void AggCtrl::onDraw(Upp::Draw& dest){};
void AggCtrl::onInit(){};
void AggCtrl::onResize(int width,int height){};

#endif
