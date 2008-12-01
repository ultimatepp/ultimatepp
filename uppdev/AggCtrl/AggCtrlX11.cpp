#include "AggCtrl.h"

#ifdef PLATFORM_X11
AggCtrl::AggCtrl(agg::pix_format_e format):
pixformat(format), bpp(0),
sysformat(agg::pix_format_undefined),
byte_order(LSBFirst),
flip_y(false),
cx(16),cy(16),
r_mask(Upp::Xvisual->red_mask), g_mask(Upp::Xvisual->green_mask), b_mask(Upp::Xvisual->blue_mask)
{
	ASSERT_(!(Upp::Xdepth<15||r_mask==0||g_mask==0||b_mask==0),"AGG requires at least 15-bit color depth and True- or DirectColor class.");
	// Determine bpp from pixel format
	switch(pixformat){
		default: break;
		case agg::pix_format_gray8:
			bpp = 8; break;
		case agg::pix_format_rgb565:
		case agg::pix_format_rgb555:
			bpp = 16; break;
		case agg::pix_format_rgb24:
		case agg::pix_format_bgr24:
			bpp = 24; break;
		case agg::pix_format_bgra32:
		case agg::pix_format_abgr32:
		case agg::pix_format_argb32:
		case agg::pix_format_rgba32:
			bpp = 32; break;
		}
	
	// Pre-determine the byte order
	int t = 1; 
	int hw_byte_order = LSBFirst;
	if (*(char*)&t == 0) hw_byte_order = MSBFirst;

	// Perceive system pixel format by mask
	switch (Upp::Xdepth) {
		case 15:
			sysbpp=16;
			if (r_mask == 0x7C00 && g_mask == 0x3E0 && b_mask == 0x1F) {
				sysformat = agg::pix_format_rgb555;
				byte_order = hw_byte_order;
			}
			break;
		case 16:
			sysbpp=16;
			if (r_mask==0xF800&&g_mask==0x7E0&&b_mask==0x1F) {
				sysformat = agg::pix_format_rgb565;
				byte_order = hw_byte_order;
			}
			break;
		case 24:
		case 32:
			sysbpp=32;
			if (g_mask==0xFF00){
				if (r_mask==0xFF&&b_mask==0xFF0000) {
					switch(pixformat) {
						case agg::pix_format_rgba32:
							sysformat = agg::pix_format_rgba32;
							byte_order = LSBFirst;
							break;
						case agg::pix_format_abgr32:
							sysformat = agg::pix_format_abgr32;
							byte_order = MSBFirst;
							break;
						default:
							byte_order = hw_byte_order;
							sysformat = (hw_byte_order==LSBFirst)?agg::pix_format_rgba32:agg::pix_format_abgr32;
							break;
					}
				}
				if (r_mask == 0xFF0000 && b_mask == 0xFF) {
					switch (pixformat) {
						case agg::pix_format_argb32:
							sysformat = agg::pix_format_argb32;
							byte_order = MSBFirst;
							break;
						case agg::pix_format_bgra32:
							sysformat = agg::pix_format_bgra32;
							byte_order = LSBFirst;
							break;
						default:
							byte_order = hw_byte_order;
							sysformat = (hw_byte_order == MSBFirst)?agg::pix_format_argb32:agg::pix_format_bgra32;
							break;
					}
				}
			}
			break;
	}

	ASSERT_(sysformat!=agg::pix_format_undefined,"RGB masks are not compatible with AGG pixel formats");

	// Allocate buffer
	buf=new unsigned char[cx*cy*(bpp/8)];
	// Connect rendering buffer
	rbuf.attach(buf,cx,cy,(flip_y?-1:1)*cx*(bpp/8));
	// Initialize XImage for sending the data to the X server
	ximg=XCreateImage(Upp::Xdisplay,Upp::Xvisual,Upp::Xdepth,ZPixmap,0,(char*)buf,cx,cy,sysbpp,cx*(sysbpp/8));
	ximg->byte_order = byte_order;
	// User initialization
	onInit();
};

void AggCtrl::PaintAgg(Upp::Draw& dest){PaintAgg(&rbuf,dest);};
void AggCtrl::PaintAgg(const agg::rendering_buffer* src,Upp::Draw& dest){
	if(ximg==0) return;
	ximg->data=(char*)(flip_y?(src->row_ptr(src->height())-1):src->row_ptr(0));
	if(pixformat==sysformat){
		XPutImage(Upp::Xdisplay,dest.GetDrawable(),dest.GetGC(),ximg,0,0,GetRect().left,GetRect().top,cx,cy);
		XFlush(Upp::Xdisplay);
		XSync(Upp::Xdisplay,false);
	}else{
		// Conversion to pixel format compatible with current system
		int row_len = cx*sysbpp/8;
		unsigned char* buf_tmp = new unsigned char[row_len*cy];
		agg::rendering_buffer rbuf_tmp;
		rbuf_tmp.attach(buf_tmp,cx,cy,flip_y?-row_len:row_len);
		switch (sysformat){
			default: break;
			case agg::pix_format_rgb555:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgb555());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgb555());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgb555());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgb555());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgb555());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgb555());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgb555());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgb555());break;
				}
				break;
			case agg::pix_format_rgb565:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgb565());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgb565());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgb565());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgb565());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgb565());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgb565());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgb565());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgb565());break;
				}
				break;
			case agg::pix_format_rgba32:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_rgba32());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_rgba32());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_rgba32());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_rgba32());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_rgba32());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_rgba32());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_rgba32());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_rgba32());break;
				}
				break;
			case agg::pix_format_abgr32:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_abgr32());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_abgr32());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_abgr32());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_abgr32());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_abgr32());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_abgr32());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_abgr32());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_abgr32());break;
				}
				break;
			case agg::pix_format_argb32:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_argb32());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_argb32());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_argb32());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_argb32());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_argb32());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_argb32());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_argb32());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_argb32());break;
				}
				break;
			case agg::pix_format_bgra32:
				switch (pixformat) {
					default: break;
					case agg::pix_format_rgb555:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb555_to_bgra32());break;
					case agg::pix_format_rgb565:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb565_to_bgra32());break;
					case agg::pix_format_rgb24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgb24_to_bgra32());break;
					case agg::pix_format_bgr24:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgr24_to_bgra32());break;
					case agg::pix_format_rgba32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_rgba32_to_bgra32());break;
					case agg::pix_format_argb32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_argb32_to_bgra32());break;
					case agg::pix_format_abgr32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_abgr32_to_bgra32());break;
					case agg::pix_format_bgra32:agg::color_conv(&rbuf_tmp, src, agg::color_conv_bgra32_to_bgra32());break;
				}
				break;
		}
		ximg->data=(char*)buf_tmp;
		XPutImage(Upp::Xdisplay,dest.GetDrawable(),dest.GetGC(),ximg,0,0,GetRect().left,GetRect().top,cx,cy);
		XFlush(Upp::Xdisplay);
		XSync(Upp::Xdisplay,false);
		delete [] buf_tmp;
	}
};
void AggCtrl::Resized(){
	// Update the dimensions, reallocate the buffer with new size, get a bigger XImage
	cx=GetSize().cx;cy=GetSize().cy;
	delete [] buf;
	ximg->data = 0;
	XDestroyImage(ximg);
	buf=new unsigned char[cx*cy*(bpp/8)];
	rbuf.attach(buf,cx,cy,(flip_y?-1:1)*cx*(bpp/8));
	ximg=XCreateImage(Upp::Xdisplay,Upp::Xvisual,Upp::Xdepth,ZPixmap,0,(char*)buf,cx,cy,sysbpp,cx*(sysbpp/8));
	ximg->byte_order=byte_order;
	// User resize code
	onResize(cx,cy);
};
AggCtrl::~AggCtrl() {
	delete [] buf;
	ximg->data = 0;
	XDestroyImage(ximg);
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
	switch(format){
		default: break;
		case agg::pix_format_gray8:
			bpp = 8; break;
		case agg::pix_format_rgb565:
		case agg::pix_format_rgb555:
			bpp = 16; break;
		case agg::pix_format_rgb24:
		case agg::pix_format_bgr24:
			bpp = 24; break;
		case agg::pix_format_bgra32:
		case agg::pix_format_abgr32:
		case agg::pix_format_argb32:
		case agg::pix_format_rgba32:
			bpp = 32; break;
	}
	Resized();
};

void AggCtrl::onDraw(Upp::Draw& dest){};
void AggCtrl::onInit(){};
void AggCtrl::onResize(int width,int height){};

#endif
