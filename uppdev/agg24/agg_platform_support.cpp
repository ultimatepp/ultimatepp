//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// class platform_support. X11 version.
//
//----------------------------------------------------------------------------
#ifndef AGG_PLATFORM_SUPPORT
#define AGG_PLATFORM_SUPPORT

#include <Core/Core.h>

#ifdef PLATFORM_X11

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <agg24/agg_basics.h>
#include <agg24/agg_color_conv_rgb8.h>
#include <agg24/agg_platform_support.h>


namespace agg
{
    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);
        ~platform_specific();
        
        void caption(const char* capt);
        void put_image(const rendering_buffer* src);
       
        pix_format_e         m_format;
        pix_format_e         m_sys_format;
        int                  m_byte_order;
        bool                 m_flip_y;
        unsigned             m_bpp;
        unsigned             m_sys_bpp;
        Display*             m_display;
        int                  m_screen;
        int                  m_depth;
        Visual*              m_visual;
        Window               m_window;
        GC                   m_gc;
        XImage*              m_ximg_window;
        XSetWindowAttributes m_window_attributes;
        Atom                 m_close_atom;
        unsigned char*       m_buf_window;
        unsigned char*       m_buf_img[platform_support::max_images];
        unsigned             m_keymap[256];
       
        bool m_update_flag;
        bool m_resize_flag;
        bool m_initialized;
        //bool m_wait_mode;
        clock_t m_sw_start;
    };



    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
        m_format(format),
        m_sys_format(pix_format_undefined),
        m_byte_order(LSBFirst),
        m_flip_y(flip_y),
        m_bpp(0),
        m_sys_bpp(0),
        m_display(0),
        m_screen(0),
        m_depth(0),
        m_visual(0),
        m_window(0),
        m_gc(0),
        m_ximg_window(0),
        m_close_atom(0),

        m_buf_window(0),
        
        m_update_flag(true), 
        m_resize_flag(true),
        m_initialized(false)
        //m_wait_mode(true)
    {
        memset(m_buf_img, 0, sizeof(m_buf_img));

        unsigned i;
        for(i = 0; i < 256; i++)
        {
            m_keymap[i] = i;
        }

        m_keymap[XK_Pause&0xFF] = key_pause;
        m_keymap[XK_Clear&0xFF] = key_clear;

        m_keymap[XK_KP_0&0xFF] = key_kp0;
        m_keymap[XK_KP_1&0xFF] = key_kp1;
        m_keymap[XK_KP_2&0xFF] = key_kp2;
        m_keymap[XK_KP_3&0xFF] = key_kp3;
        m_keymap[XK_KP_4&0xFF] = key_kp4;
        m_keymap[XK_KP_5&0xFF] = key_kp5;
        m_keymap[XK_KP_6&0xFF] = key_kp6;
        m_keymap[XK_KP_7&0xFF] = key_kp7;
        m_keymap[XK_KP_8&0xFF] = key_kp8;
        m_keymap[XK_KP_9&0xFF] = key_kp9;

        m_keymap[XK_KP_Insert&0xFF]    = key_kp0;
        m_keymap[XK_KP_End&0xFF]       = key_kp1;   
        m_keymap[XK_KP_Down&0xFF]      = key_kp2;
        m_keymap[XK_KP_Page_Down&0xFF] = key_kp3;
        m_keymap[XK_KP_Left&0xFF]      = key_kp4;
        m_keymap[XK_KP_Begin&0xFF]     = key_kp5;
        m_keymap[XK_KP_Right&0xFF]     = key_kp6;
        m_keymap[XK_KP_Home&0xFF]      = key_kp7;
        m_keymap[XK_KP_Up&0xFF]        = key_kp8;
        m_keymap[XK_KP_Page_Up&0xFF]   = key_kp9;
        m_keymap[XK_KP_Delete&0xFF]    = key_kp_period;
        m_keymap[XK_KP_Decimal&0xFF]   = key_kp_period;
        m_keymap[XK_KP_Divide&0xFF]    = key_kp_divide;
        m_keymap[XK_KP_Multiply&0xFF]  = key_kp_multiply;
        m_keymap[XK_KP_Subtract&0xFF]  = key_kp_minus;
        m_keymap[XK_KP_Add&0xFF]       = key_kp_plus;
        m_keymap[XK_KP_Enter&0xFF]     = key_kp_enter;
        m_keymap[XK_KP_Equal&0xFF]     = key_kp_equals;

        m_keymap[XK_Up&0xFF]           = key_up;
        m_keymap[XK_Down&0xFF]         = key_down;
        m_keymap[XK_Right&0xFF]        = key_right;
        m_keymap[XK_Left&0xFF]         = key_left;
        m_keymap[XK_Insert&0xFF]       = key_insert;
        m_keymap[XK_Home&0xFF]         = key_delete;
        m_keymap[XK_End&0xFF]          = key_end;
        m_keymap[XK_Page_Up&0xFF]      = key_page_up;
        m_keymap[XK_Page_Down&0xFF]    = key_page_down;

        m_keymap[XK_F1&0xFF]           = key_f1;
        m_keymap[XK_F2&0xFF]           = key_f2;
        m_keymap[XK_F3&0xFF]           = key_f3;
        m_keymap[XK_F4&0xFF]           = key_f4;
        m_keymap[XK_F5&0xFF]           = key_f5;
        m_keymap[XK_F6&0xFF]           = key_f6;
        m_keymap[XK_F7&0xFF]           = key_f7;
        m_keymap[XK_F8&0xFF]           = key_f8;
        m_keymap[XK_F9&0xFF]           = key_f9;
        m_keymap[XK_F10&0xFF]          = key_f10;
        m_keymap[XK_F11&0xFF]          = key_f11;
        m_keymap[XK_F12&0xFF]          = key_f12;
        m_keymap[XK_F13&0xFF]          = key_f13;
        m_keymap[XK_F14&0xFF]          = key_f14;
        m_keymap[XK_F15&0xFF]          = key_f15;

        m_keymap[XK_Num_Lock&0xFF]     = key_numlock;
        m_keymap[XK_Caps_Lock&0xFF]    = key_capslock;
        m_keymap[XK_Scroll_Lock&0xFF]  = key_scrollock;

        switch(m_format)
        {
        default: break;
        case pix_format_gray8:
            m_bpp = 8;
            break;

        case pix_format_rgb565:
        case pix_format_rgb555:
            m_bpp = 16;
            break;

        case pix_format_rgb24:
        case pix_format_bgr24:
            m_bpp = 24;
            break;

        case pix_format_bgra32:
        case pix_format_abgr32:
        case pix_format_argb32:
        case pix_format_rgba32:
            m_bpp = 32;
            break;
        }
        m_sw_start = clock();
    }

    //------------------------------------------------------------------------
    platform_specific::~platform_specific()
    {
    }

    //------------------------------------------------------------------------
    void platform_specific::caption(const char* capt)
    {
        XTextProperty tp;
        tp.value = (unsigned char *)capt;
        tp.encoding = XA_WM_NAME;
        tp.format = 8;
        tp.nitems = strlen(capt);
        XSetWMName(m_display, m_window, &tp);
        XStoreName(m_display, m_window, capt);
        XSetIconName(m_display, m_window, capt);
        XSetWMIconName(m_display, m_window, &tp);
    }

    
    //------------------------------------------------------------------------
    void platform_specific::put_image(const rendering_buffer* src)
    {    
        if(m_ximg_window == 0) return;
        m_ximg_window->data = (char*)m_buf_window;
        
        if(m_format == m_sys_format)
        {
            XPutImage(m_display, 
                      m_window, 
                      m_gc, 
                      m_ximg_window, 
                      0, 0, 0, 0,
                      src->width(), 
                      src->height());
        }
        else
        {
            int row_len = src->width() * m_sys_bpp / 8;
            unsigned char* buf_tmp = 
                new unsigned char[row_len * src->height()];
            
            rendering_buffer rbuf_tmp;
            rbuf_tmp.attach(buf_tmp, 
                            src->width(), 
                            src->height(), 
                            m_flip_y ? -row_len : row_len);

            switch(m_sys_format)            
            {
                default: break;
                case pix_format_rgb555:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgb555()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb555()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgb555());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgb555());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgb555()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgb555()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgb555()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgb555()); break;
                    }
                    break;
                    
                case pix_format_rgb565:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgb565()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb565()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgb565());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgb565());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgb565()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgb565()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgb565()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgb565()); break;
                    }
                    break;
                    
                case pix_format_rgba32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgba32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgba32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgba32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgba32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgba32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgba32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgba32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgba32()); break;
                    }
                    break;
                    
                case pix_format_abgr32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_abgr32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_abgr32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_abgr32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_abgr32());  break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_abgr32()); break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_abgr32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_abgr32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_abgr32()); break;
                    }
                    break;
                    
                case pix_format_argb32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_argb32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_argb32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_argb32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_argb32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_argb32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_argb32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_argb32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_argb32()); break;
                    }
                    break;
                    
                case pix_format_bgra32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_bgra32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_bgra32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_bgra32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_bgra32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_bgra32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_bgra32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_bgra32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_bgra32()); break;
                    }
                    break;
            }
            
            m_ximg_window->data = (char*)buf_tmp;
            XPutImage(m_display, 
                      m_window, 
                      m_gc, 
                      m_ximg_window, 
                      0, 0, 0, 0,
                      src->width(), 
                      src->height());
            
            delete [] buf_tmp;
        }
    }
    

    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(format, flip_y)),
        m_format(format),
        m_bpp(m_specific->m_bpp),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "AGG Application");
    }

    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }



    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        if(m_specific->m_initialized)
        {
            m_specific->caption(cap);
        }
    }

   
    //------------------------------------------------------------------------
    enum xevent_mask_e
    { 
        xevent_mask =
            PointerMotionMask|
            ButtonPressMask|
            ButtonReleaseMask|
            ExposureMask|
            KeyPressMask|
            StructureNotifyMask
    };


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        m_window_flags = flags;
        
        m_specific->m_display = XOpenDisplay(NULL);
        if(m_specific->m_display == 0) 
        {
            fprintf(stderr, "Unable to open DISPLAY!\n");
            return false;
        }
        
        m_specific->m_screen = XDefaultScreen(m_specific->m_display);
        m_specific->m_depth  = XDefaultDepth(m_specific->m_display, 
                                             m_specific->m_screen);
        m_specific->m_visual = XDefaultVisual(m_specific->m_display, 
                                              m_specific->m_screen);
        unsigned long r_mask = m_specific->m_visual->red_mask;
        unsigned long g_mask = m_specific->m_visual->green_mask;
        unsigned long b_mask = m_specific->m_visual->blue_mask;
                
//printf("depth=%d, red=%08x, green=%08x, blue=%08x\n",
//       m_specific->m_depth,
//       m_specific->m_visual->red_mask,
//       m_specific->m_visual->green_mask,
//       m_specific->m_visual->blue_mask);
           

//         // NOT COMPLETED YET!
//         // Try to find an appropriate Visual if the default doesn't fit.
//         if(m_specific->m_depth < 15 ||
//            r_mask == 0 || g_mask == 0 || b_mask == 0)
//         {
//             
//             // This is an attempt to find an appropriate Visual if         
//             // the default one doesn't match the minumum requirements
//             static int depth[] = { 32, 24, 16, 15 };
//             int i;
//             for(int i = 0; i < 4; i++)
//             {
//                 XVisualInfo vi;
//                 if(XMatchVisualInfo(m_specific->m_display, 
//                                     m_specific->m_screen, 
//                                     depth[i], 
//                                     TrueColor, 
//                                     &vi)) 
//                 {
// //                     printf("TrueColor  depth=%d, red=%08x, green=%08x, blue=%08x, bits=%d\n",
// //                         vi.depth,
// //                         vi.visual->red_mask,
// //                         vi.visual->green_mask,
// //                         vi.visual->blue_mask,
// //                         vi.bits_per_rgb);
//                     m_specific->m_depth  = vi.depth;
//                     m_specific->m_visual = vi.visual;
//                     r_mask = m_specific->m_visual->red_mask;
//                     g_mask = m_specific->m_visual->green_mask;
//                     b_mask = m_specific->m_visual->blue_mask;
//                     break;
//                 }
//                 if(XMatchVisualInfo(m_specific->m_display, 
//                                     m_specific->m_screen, 
//                                     depth[i], 
//                                     DirectColor, 
//                                     &vi)) 
//                 {
// //                     printf("DirectColor depth=%d, red=%08x, green=%08x, blue=%08x, bits=%d\n",
// //                         vi.depth,
// //                         vi.visual->red_mask,
// //                         vi.visual->green_mask,
// //                         vi.visual->blue_mask,
// //                         vi.bits_per_rgb);
//                     m_specific->m_depth  = vi.depth;
//                     m_specific->m_visual = vi.visual;
//                     r_mask = m_specific->m_visual->red_mask;
//                     g_mask = m_specific->m_visual->green_mask;
//                     b_mask = m_specific->m_visual->blue_mask;
//                     break;
//                 }
//             }
//         }

        if(m_specific->m_depth < 15 ||
           r_mask == 0 || g_mask == 0 || b_mask == 0)
        {
            fprintf(stderr,
                   "There's no Visual compatible with minimal AGG requirements:\n"
                   "At least 15-bit color depth and True- or DirectColor class.\n\n");
            XCloseDisplay(m_specific->m_display);
            return false;
        }
        
        int t = 1;
        int hw_byte_order = LSBFirst;
        if(*(char*)&t == 0) hw_byte_order = MSBFirst;
        
        // Perceive SYS-format by mask
        switch(m_specific->m_depth)
        {
            case 15:
                m_specific->m_sys_bpp = 16;
                if(r_mask == 0x7C00 && g_mask == 0x3E0 && b_mask == 0x1F)
                {
                    m_specific->m_sys_format = pix_format_rgb555;
                    m_specific->m_byte_order = hw_byte_order;
                }
                break;
                
            case 16:
                m_specific->m_sys_bpp = 16;
                if(r_mask == 0xF800 && g_mask == 0x7E0 && b_mask == 0x1F)
                {
                    m_specific->m_sys_format = pix_format_rgb565;
                    m_specific->m_byte_order = hw_byte_order;
                }
                break;
                
            case 24:
            case 32:
                m_specific->m_sys_bpp = 32;
                if(g_mask == 0xFF00)
                {
                    if(r_mask == 0xFF && b_mask == 0xFF0000)
                    {
                        switch(m_specific->m_format)
                        {
                            case pix_format_rgba32:
                                m_specific->m_sys_format = pix_format_rgba32;
                                m_specific->m_byte_order = LSBFirst;
                                break;
                                
                            case pix_format_abgr32:
                                m_specific->m_sys_format = pix_format_abgr32;
                                m_specific->m_byte_order = MSBFirst;
                                break;

                            default:                            
                                m_specific->m_byte_order = hw_byte_order;
                                m_specific->m_sys_format = 
                                    (hw_byte_order == LSBFirst) ?
                                    pix_format_rgba32 :
                                    pix_format_abgr32;
                                break;
                        }
                    }
                    
                    if(r_mask == 0xFF0000 && b_mask == 0xFF)
                    {
                        switch(m_specific->m_format)
                        {
                            case pix_format_argb32:
                                m_specific->m_sys_format = pix_format_argb32;
                                m_specific->m_byte_order = MSBFirst;
                                break;
                                
                            case pix_format_bgra32:
                                m_specific->m_sys_format = pix_format_bgra32;
                                m_specific->m_byte_order = LSBFirst;
                                break;

                            default:                            
                                m_specific->m_byte_order = hw_byte_order;
                                m_specific->m_sys_format = 
                                    (hw_byte_order == MSBFirst) ?
                                    pix_format_argb32 :
                                    pix_format_bgra32;
                                break;
                        }
                    }
                }
                break;
        }
        
        if(m_specific->m_sys_format == pix_format_undefined)
        {
            fprintf(stderr,
                   "RGB masks are not compatible with AGG pixel formats:\n"
                   "R=%08x, R=%08x, B=%08x\n", r_mask, g_mask, b_mask);
            XCloseDisplay(m_specific->m_display);
            return false;
        }
                
        
        
        memset(&m_specific->m_window_attributes, 
               0, 
               sizeof(m_specific->m_window_attributes)); 
        
        m_specific->m_window_attributes.border_pixel = 
            XBlackPixel(m_specific->m_display, m_specific->m_screen);

        m_specific->m_window_attributes.background_pixel = 
            XWhitePixel(m_specific->m_display, m_specific->m_screen);

        m_specific->m_window_attributes.override_redirect = 0;

        unsigned long window_mask = CWBackPixel | CWBorderPixel;

        m_specific->m_window = 
            XCreateWindow(m_specific->m_display, 
                          XDefaultRootWindow(m_specific->m_display), 
                          0, 0,
                          width,
                          height,
                          0, 
                          m_specific->m_depth, 
                          InputOutput, 
                          CopyFromParent,
                          window_mask,
                          &m_specific->m_window_attributes);


        m_specific->m_gc = XCreateGC(m_specific->m_display, 
                                     m_specific->m_window, 
                                     0, 0); 
        m_specific->m_buf_window = 
            new unsigned char[width * height * (m_bpp / 8)];

        memset(m_specific->m_buf_window, 255, width * height * (m_bpp / 8));
        
        m_rbuf_window.attach(m_specific->m_buf_window,
                             width,
                             height,
                             m_flip_y ? -width * (m_bpp / 8) : width * (m_bpp / 8));
            
        m_specific->m_ximg_window = 
            XCreateImage(m_specific->m_display, 
                         m_specific->m_visual, //CopyFromParent, 
                         m_specific->m_depth, 
                         ZPixmap, 
                         0,
                         (char*)m_specific->m_buf_window, 
                         width,
                         height, 
                         m_specific->m_sys_bpp,
                         width * (m_specific->m_sys_bpp / 8));
        m_specific->m_ximg_window->byte_order = m_specific->m_byte_order;

        m_specific->caption(m_caption); 
        m_initial_width = width;
        m_initial_height = height;
        
        if(!m_specific->m_initialized)
        {
            on_init();
            m_specific->m_initialized = true;
        }

        trans_affine_resizing(width, height);
        on_resize(width, height);
        m_specific->m_update_flag = true;

        XSizeHints *hints = XAllocSizeHints();
        if(hints) 
        {
            if(flags & window_resize)
            {
                hints->min_width = 32;
                hints->min_height = 32;
                hints->max_width = 4096;
                hints->max_height = 4096;
            }
            else
            {
                hints->min_width  = width;
                hints->min_height = height;
                hints->max_width  = width;
                hints->max_height = height;
            }
            hints->flags = PMaxSize | PMinSize;

            XSetWMNormalHints(m_specific->m_display, 
                              m_specific->m_window, 
                              hints);

            XFree(hints);
        }


        XMapWindow(m_specific->m_display, 
                   m_specific->m_window);

        XSelectInput(m_specific->m_display, 
                     m_specific->m_window, 
                     xevent_mask);

        
        m_specific->m_close_atom = XInternAtom(m_specific->m_display, 
                                               "WM_DELETE_WINDOW", 
                                               false);

        XSetWMProtocols(m_specific->m_display, 
                        m_specific->m_window, 
                        &m_specific->m_close_atom, 
                        1);

        return true;
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        m_specific->put_image(&m_rbuf_window);
        
        // When m_wait_mode is true we can discard all the events 
        // came while the image is being drawn. In this case 
        // the X server does not accumulate mouse motion events.
        // When m_wait_mode is false, i.e. we have some idle drawing
        // we cannot afford to miss any events
        XSync(m_specific->m_display, m_wait_mode);
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
        XFlush(m_specific->m_display);
        
        bool quit = false;
        unsigned flags;
        int cur_x;
        int cur_y;

        while(!quit)
        {
            if(m_specific->m_update_flag)
            {
                on_draw();
                update_window();
                m_specific->m_update_flag = false;
            }

            if(!m_wait_mode)
            {
                if(XPending(m_specific->m_display) == 0)
                {
                    on_idle();
                    continue;
                }
            }

            XEvent x_event;
            XNextEvent(m_specific->m_display, &x_event);
            
            // In the Idle mode discard all intermediate MotionNotify events
            if(!m_wait_mode && x_event.type == MotionNotify)
            {
                XEvent te = x_event;
                for(;;)
                {
                    if(XPending(m_specific->m_display) == 0) break;
                    XNextEvent(m_specific->m_display, &te);
                    if(te.type != MotionNotify) break;
                }
                x_event = te;
            }

            switch(x_event.type) 
            {
            case ConfigureNotify: 
                {
                    if(x_event.xconfigure.width  != int(m_rbuf_window.width()) ||
                       x_event.xconfigure.height != int(m_rbuf_window.height()))
                    {
                        int width  = x_event.xconfigure.width;
                        int height = x_event.xconfigure.height;

                        delete [] m_specific->m_buf_window;
                        m_specific->m_ximg_window->data = 0;
                        XDestroyImage(m_specific->m_ximg_window);

                        m_specific->m_buf_window = 
                            new unsigned char[width * height * (m_bpp / 8)];

                        m_rbuf_window.attach(m_specific->m_buf_window,
                                             width,
                                             height,
                                             m_flip_y ? 
                                             -width * (m_bpp / 8) : 
                                             width * (m_bpp / 8));
            
                        m_specific->m_ximg_window = 
                            XCreateImage(m_specific->m_display, 
                                         m_specific->m_visual, //CopyFromParent, 
                                         m_specific->m_depth, 
                                         ZPixmap, 
                                         0,
                                         (char*)m_specific->m_buf_window, 
                                         width,
                                         height, 
                                         m_specific->m_sys_bpp,
                                         width * (m_specific->m_sys_bpp / 8));
                        m_specific->m_ximg_window->byte_order = m_specific->m_byte_order;

                        trans_affine_resizing(width, height);
                        on_resize(width, height);
                        on_draw();
                        update_window();
                    }
                }
                break;

            case Expose:
                m_specific->put_image(&m_rbuf_window);
                XFlush(m_specific->m_display);
                XSync(m_specific->m_display, false);
                break;

            case KeyPress:
                {
                    KeySym key = XLookupKeysym(&x_event.xkey, 0);
                    flags = 0;
                    if(x_event.xkey.state & Button1Mask) flags |= mouse_left;
                    if(x_event.xkey.state & Button3Mask) flags |= mouse_right;
                    if(x_event.xkey.state & ShiftMask)   flags |= kbd_shift;
                    if(x_event.xkey.state & ControlMask) flags |= kbd_ctrl;

                    bool left  = false;
                    bool up    = false;
                    bool right = false;
                    bool down  = false;

                    switch(m_specific->m_keymap[key & 0xFF])
                    {
                    case key_left:
                        left = true;
                        break;

                    case key_up:
                        up = true;
                        break;

                    case key_right:
                        right = true;
                        break;

                    case key_down:
                        down = true;
                        break;

                    case key_f2:                        
                        copy_window_to_img(max_images - 1);
                        save_img(max_images - 1, "screenshot");
                        break;
                    }

                    if(m_ctrls.on_arrow_keys(left, right, down, up))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    else
                    {
                        on_key(x_event.xkey.x, 
                               m_flip_y ? 
                                   m_rbuf_window.height() - x_event.xkey.y :
                                   x_event.xkey.y,
                               m_specific->m_keymap[key & 0xFF],
                               flags);
                    }
                }
                break;


            case ButtonPress:
                {
                    flags = 0;
                    if(x_event.xbutton.state & ShiftMask)   flags |= kbd_shift;
                    if(x_event.xbutton.state & ControlMask) flags |= kbd_ctrl;
                    if(x_event.xbutton.button == Button1)   flags |= mouse_left;
                    if(x_event.xbutton.button == Button3)   flags |= mouse_right;

                    cur_x = x_event.xbutton.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - x_event.xbutton.y :
                                       x_event.xbutton.y;

                    if(flags & mouse_left)
                    {
                        if(m_ctrls.on_mouse_button_down(cur_x, cur_y))
                        {
                            m_ctrls.set_cur(cur_x, cur_y);
                            on_ctrl_change();
                            force_redraw();
                        }
                        else
                        {
                            if(m_ctrls.in_rect(cur_x, cur_y))
                            {
                                if(m_ctrls.set_cur(cur_x, cur_y))
                                {
                                    on_ctrl_change();
                                    force_redraw();
                                }
                            }
                            else
                            {
                                on_mouse_button_down(cur_x, cur_y, flags);
                            }
                        }
                    }
                    if(flags & mouse_right)
                    {
                        on_mouse_button_down(cur_x, cur_y, flags);
                    }
                    //m_specific->m_wait_mode = m_wait_mode;
                    //m_wait_mode = true;
                }
                break;

                
            case MotionNotify:
                {
                    flags = 0;
                    if(x_event.xmotion.state & Button1Mask) flags |= mouse_left;
                    if(x_event.xmotion.state & Button3Mask) flags |= mouse_right;
                    if(x_event.xmotion.state & ShiftMask)   flags |= kbd_shift;
                    if(x_event.xmotion.state & ControlMask) flags |= kbd_ctrl;

                    cur_x = x_event.xbutton.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - x_event.xbutton.y :
                                       x_event.xbutton.y;

                    if(m_ctrls.on_mouse_move(cur_x, cur_y, (flags & mouse_left) != 0))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    else
                    {
                        if(!m_ctrls.in_rect(cur_x, cur_y))
                        {
                            on_mouse_move(cur_x, cur_y, flags);
                        }
                    }
                }
                break;
                
            case ButtonRelease:
                {
                    flags = 0;
                    if(x_event.xbutton.state & ShiftMask)   flags |= kbd_shift;
                    if(x_event.xbutton.state & ControlMask) flags |= kbd_ctrl;
                    if(x_event.xbutton.button == Button1)   flags |= mouse_left;
                    if(x_event.xbutton.button == Button3)   flags |= mouse_right;

                    cur_x = x_event.xbutton.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - x_event.xbutton.y :
                                       x_event.xbutton.y;

                    if(flags & mouse_left)
                    {
                        if(m_ctrls.on_mouse_button_up(cur_x, cur_y))
                        {
                            on_ctrl_change();
                            force_redraw();
                        }
                    }
                    if(flags & (mouse_left | mouse_right))
                    {
                        on_mouse_button_up(cur_x, cur_y, flags);
                    }
                }
                //m_wait_mode = m_specific->m_wait_mode;
                break;

            case ClientMessage:
                if((x_event.xclient.format == 32) &&
                (x_event.xclient.data.l[0] == int(m_specific->m_close_atom)))
                {
                    quit = true;
                }
                break;
            }           
        }


        unsigned i = platform_support::max_images;
        while(i--)
        {
            if(m_specific->m_buf_img[i]) 
            {
                delete [] m_specific->m_buf_img[i];
            }
        }

        delete [] m_specific->m_buf_window;
        m_specific->m_ximg_window->data = 0;
        XDestroyImage(m_specific->m_ximg_window);
        XFreeGC(m_specific->m_display, m_specific->m_gc);
        XDestroyWindow(m_specific->m_display, m_specific->m_window);
        XCloseDisplay(m_specific->m_display);
        
        return 0;
    }



    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".ppm"; }

    //------------------------------------------------------------------------
    const char* platform_support::full_file_name(const char* file_name)
    {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char buf[1024];
            strcpy(buf, file);
            int len = strlen(buf);
            if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
            {
                strcat(buf, ".ppm");
            }
            
            FILE* fd = fopen(buf, "rb");
            if(fd == 0) return false;

            if((len = fread(buf, 1, 1022, fd)) == 0)
            {
                fclose(fd);
                return false;
            }
            buf[len] = 0;
            
            if(buf[0] != 'P' && buf[1] != '6')
            {
                fclose(fd);
                return false;
            }
            
            char* ptr = buf + 2;
            
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            
            unsigned width = atoi(ptr);
            if(width == 0 || width > 4096)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            unsigned height = atoi(ptr);
            if(height == 0 || height > 4096)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(atoi(ptr) != 255)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            ptr++;
            fseek(fd, long(ptr - buf), SEEK_SET);
            
            create_img(idx, width, height);
            bool ret = true;
            
            if(m_format == pix_format_rgb24)
            {
                fread(m_specific->m_buf_img[idx], 1, width * height * 3, fd);
            }
            else
            {
                unsigned char* buf_img = new unsigned char [width * height * 3];
                rendering_buffer rbuf_img;
                rbuf_img.attach(buf_img,
                                width,
                                height,
                                m_flip_y ?
                                  -width * 3 :
                                   width * 3);
                
                fread(buf_img, 1, width * height * 3, fd);
                
                switch(m_format)
                {
                    case pix_format_rgb555:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgb555());
                        break;
                        
                    case pix_format_rgb565:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgb565());
                        break;
                        
                    case pix_format_bgr24:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_bgr24());
                        break;
                        
                    case pix_format_rgba32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgba32());
                        break;
                        
                    case pix_format_argb32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_argb32());
                        break;
                        
                    case pix_format_bgra32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_bgra32());
                        break;
                        
                    case pix_format_abgr32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_abgr32());
                        break;
                        
                    default:
                        ret = false;
                }
                delete [] buf_img;
            }
                        
            fclose(fd);
            return ret;
        }
        return false;
    }




    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        if(idx < max_images &&  rbuf_img(idx).buf())
        {
            char buf[1024];
            strcpy(buf, file);
            int len = strlen(buf);
            if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
            {
                strcat(buf, ".ppm");
            }
            
            FILE* fd = fopen(buf, "wb");
            if(fd == 0) return false;
            
            unsigned w = rbuf_img(idx).width();
            unsigned h = rbuf_img(idx).height();
            
            fprintf(fd, "P6\n%d %d\n255\n", w, h);
                
            unsigned y; 
            unsigned char* tmp_buf = new unsigned char [w * 3];
            for(y = 0; y < rbuf_img(idx).height(); y++)
            {
                const unsigned char* src = rbuf_img(idx).row_ptr(m_flip_y ? h - 1 - y : y);
                switch(m_format)
                {
                    default: break;
                    case pix_format_rgb555:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb555_to_rgb24());
                        break;
                        
                    case pix_format_rgb565:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb565_to_rgb24());
                        break;
                        
                    case pix_format_bgr24:
                        color_conv_row(tmp_buf, src, w, color_conv_bgr24_to_rgb24());
                        break;
                        
                    case pix_format_rgb24:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb24_to_rgb24());
                        break;
                       
                    case pix_format_rgba32:
                        color_conv_row(tmp_buf, src, w, color_conv_rgba32_to_rgb24());
                        break;
                        
                    case pix_format_argb32:
                        color_conv_row(tmp_buf, src, w, color_conv_argb32_to_rgb24());
                        break;
                        
                    case pix_format_bgra32:
                        color_conv_row(tmp_buf, src, w, color_conv_bgra32_to_rgb24());
                        break;
                        
                    case pix_format_abgr32:
                        color_conv_row(tmp_buf, src, w, color_conv_abgr32_to_rgb24());
                        break;
                }
                fwrite(tmp_buf, 1, w * 3, fd);
            }
            delete [] tmp_buf;
            fclose(fd);
            return true;
        }
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = rbuf_window().width();
            if(height == 0) height = rbuf_window().height();
            delete [] m_specific->m_buf_img[idx];
            m_specific->m_buf_img[idx] = 
                new unsigned char[width * height * (m_bpp / 8)];

            m_rbuf_img[idx].attach(m_specific->m_buf_img[idx],
                                   width,
                                   height,
                                   m_flip_y ? 
                                       -width * (m_bpp / 8) : 
                                        width * (m_bpp / 8));
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->m_update_flag = true;
    }


    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        m_specific->m_sw_start = clock();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        clock_t stop = clock();
        return double(stop - m_specific->m_sw_start) * 1000.0 / CLOCKS_PER_SEC;
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}



}

#endif

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// class platform_support
//
//----------------------------------------------------------------------------

#ifdef PLATFORM_WIN32
#include <windows.h>
#include <string.h>
#include "agg_platform_support.h"
#include "agg_win32_bmp.h"
#include "agg_color_conv_rgb8.h"
#include "agg_color_conv_rgb16.h"


namespace agg
{
    
    //------------------------------------------------------------------------
    HINSTANCE g_windows_instance = 0;
    int       g_windows_cmd_show = 0;


    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);

        void create_pmap(unsigned width, unsigned height, 
                         rendering_buffer* wnd);

        void display_pmap(HDC dc, const rendering_buffer* src);
        bool load_pmap(const char* fn, unsigned idx, 
                       rendering_buffer* dst);

        bool save_pmap(const char* fn, unsigned idx, 
                       const rendering_buffer* src);

        unsigned translate(unsigned keycode);

        pix_format_e  m_format;
        pix_format_e  m_sys_format;
        bool          m_flip_y;
        unsigned      m_bpp;
        unsigned      m_sys_bpp;
        HWND          m_hwnd;
        pixel_map     m_pmap_window;
        pixel_map     m_pmap_img[platform_support::max_images];
        unsigned      m_keymap[256];
        unsigned      m_last_translated_key;
        int           m_cur_x;
        int           m_cur_y;
        unsigned      m_input_flags;
        bool          m_redraw_flag;
        HDC           m_current_dc;
        LARGE_INTEGER m_sw_freq;
        LARGE_INTEGER m_sw_start;
    };


    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
        m_format(format),
        m_sys_format(pix_format_undefined),
        m_flip_y(flip_y),
        m_bpp(0),
        m_sys_bpp(0),
        m_hwnd(0),
        m_last_translated_key(0),
        m_cur_x(0),
        m_cur_y(0),
        m_input_flags(0),
        m_redraw_flag(true),
        m_current_dc(0)
    {
        memset(m_keymap, 0, sizeof(m_keymap));

        m_keymap[VK_PAUSE]      = key_pause;
        m_keymap[VK_CLEAR]      = key_clear;

        m_keymap[VK_NUMPAD0]    = key_kp0;
        m_keymap[VK_NUMPAD1]    = key_kp1;
        m_keymap[VK_NUMPAD2]    = key_kp2;
        m_keymap[VK_NUMPAD3]    = key_kp3;
        m_keymap[VK_NUMPAD4]    = key_kp4;
        m_keymap[VK_NUMPAD5]    = key_kp5;
        m_keymap[VK_NUMPAD6]    = key_kp6;
        m_keymap[VK_NUMPAD7]    = key_kp7;
        m_keymap[VK_NUMPAD8]    = key_kp8;
        m_keymap[VK_NUMPAD9]    = key_kp9;
        m_keymap[VK_DECIMAL]    = key_kp_period;
        m_keymap[VK_DIVIDE]     = key_kp_divide;
        m_keymap[VK_MULTIPLY]   = key_kp_multiply;
        m_keymap[VK_SUBTRACT]   = key_kp_minus;
        m_keymap[VK_ADD]        = key_kp_plus;

        m_keymap[VK_UP]         = key_up;
        m_keymap[VK_DOWN]       = key_down;
        m_keymap[VK_RIGHT]      = key_right;
        m_keymap[VK_LEFT]       = key_left;
        m_keymap[VK_INSERT]     = key_insert;
        m_keymap[VK_DELETE]     = key_delete;
        m_keymap[VK_HOME]       = key_home;
        m_keymap[VK_END]        = key_end;
        m_keymap[VK_PRIOR]      = key_page_up;
        m_keymap[VK_NEXT]       = key_page_down;

        m_keymap[VK_F1]         = key_f1;
        m_keymap[VK_F2]         = key_f2;
        m_keymap[VK_F3]         = key_f3;
        m_keymap[VK_F4]         = key_f4;
        m_keymap[VK_F5]         = key_f5;
        m_keymap[VK_F6]         = key_f6;
        m_keymap[VK_F7]         = key_f7;
        m_keymap[VK_F8]         = key_f8;
        m_keymap[VK_F9]         = key_f9;
        m_keymap[VK_F10]        = key_f10;
        m_keymap[VK_F11]        = key_f11;
        m_keymap[VK_F12]        = key_f12;
        m_keymap[VK_F13]        = key_f13;
        m_keymap[VK_F14]        = key_f14;
        m_keymap[VK_F15]        = key_f15;

        m_keymap[VK_NUMLOCK]    = key_numlock;
        m_keymap[VK_CAPITAL]    = key_capslock;
        m_keymap[VK_SCROLL]     = key_scrollock;


        switch(m_format)
        {
        case pix_format_bw:
            m_sys_format = pix_format_bw;
            m_bpp = 1;
            m_sys_bpp = 1;
            break;

        case pix_format_gray8:
            m_sys_format = pix_format_gray8;
            m_bpp = 8;
            m_sys_bpp = 8;
            break;

        case pix_format_gray16:
            m_sys_format = pix_format_gray8;
            m_bpp = 16;
            m_sys_bpp = 8;
            break;

        case pix_format_rgb565:
        case pix_format_rgb555:
            m_sys_format = pix_format_rgb555;
            m_bpp = 16;
            m_sys_bpp = 16;
            break;

        case pix_format_rgbAAA:
        case pix_format_bgrAAA:
        case pix_format_rgbBBA:
        case pix_format_bgrABB:
            m_sys_format = pix_format_bgr24;
            m_bpp = 32;
            m_sys_bpp = 24;
            break;

        case pix_format_rgb24:
        case pix_format_bgr24:
            m_sys_format = pix_format_bgr24;
            m_bpp = 24;
            m_sys_bpp = 24;
            break;

        case pix_format_rgb48:
        case pix_format_bgr48:
            m_sys_format = pix_format_bgr24;
            m_bpp = 48;
            m_sys_bpp = 24;
            break;

        case pix_format_bgra32:
        case pix_format_abgr32:
        case pix_format_argb32:
        case pix_format_rgba32:
            m_sys_format = pix_format_bgra32;
            m_bpp = 32;
            m_sys_bpp = 32;
            break;

        case pix_format_bgra64:
        case pix_format_abgr64:
        case pix_format_argb64:
        case pix_format_rgba64:
            m_sys_format = pix_format_bgra32;
            m_bpp = 64;
            m_sys_bpp = 32;
            break;
        }
        ::QueryPerformanceFrequency(&m_sw_freq);
        ::QueryPerformanceCounter(&m_sw_start);
    }


    //------------------------------------------------------------------------
    void platform_specific::create_pmap(unsigned width, 
                                        unsigned height,
                                        rendering_buffer* wnd)
    {
        m_pmap_window.create(width, height, org_e(m_bpp));
        wnd->attach(m_pmap_window.buf(), 
                    m_pmap_window.width(),
                    m_pmap_window.height(),
                      m_flip_y ?
                      m_pmap_window.stride() :
                     -m_pmap_window.stride());
    }


    //------------------------------------------------------------------------
    static void convert_pmap(rendering_buffer* dst, 
                             const rendering_buffer* src, 
                             pix_format_e format)
    {
        switch(format)
        {
        case pix_format_gray8:
            break;

        case pix_format_gray16:
            color_conv(dst, src, color_conv_gray16_to_gray8());
            break;

        case pix_format_rgb565:
            color_conv(dst, src, color_conv_rgb565_to_rgb555());
            break;

        case pix_format_rgbAAA:
            color_conv(dst, src, color_conv_rgbAAA_to_bgr24());
            break;

        case pix_format_bgrAAA:
            color_conv(dst, src, color_conv_bgrAAA_to_bgr24());
            break;

        case pix_format_rgbBBA:
            color_conv(dst, src, color_conv_rgbBBA_to_bgr24());
            break;

        case pix_format_bgrABB:
            color_conv(dst, src, color_conv_bgrABB_to_bgr24());
            break;

        case pix_format_rgb24:
            color_conv(dst, src, color_conv_rgb24_to_bgr24());
            break;

        case pix_format_rgb48:
            color_conv(dst, src, color_conv_rgb48_to_bgr24());
            break;

        case pix_format_bgr48:
            color_conv(dst, src, color_conv_bgr48_to_bgr24());
            break;

        case pix_format_abgr32:
            color_conv(dst, src, color_conv_abgr32_to_bgra32());
            break;

        case pix_format_argb32:
            color_conv(dst, src, color_conv_argb32_to_bgra32());
            break;

        case pix_format_rgba32:
            color_conv(dst, src, color_conv_rgba32_to_bgra32());
            break;

        case pix_format_bgra64:
            color_conv(dst, src, color_conv_bgra64_to_bgra32());
            break;

        case pix_format_abgr64:
            color_conv(dst, src, color_conv_abgr64_to_bgra32());
            break;

        case pix_format_argb64:
            color_conv(dst, src, color_conv_argb64_to_bgra32());
            break;

        case pix_format_rgba64:
            color_conv(dst, src, color_conv_rgba64_to_bgra32());
            break;
        }
    }


    //------------------------------------------------------------------------
    void platform_specific::display_pmap(HDC dc, const rendering_buffer* src)
    {
        if(m_sys_format == m_format)
        {
            m_pmap_window.draw(dc);
        }
        else
        {
            pixel_map pmap_tmp;
            pmap_tmp.create(m_pmap_window.width(), 
                            m_pmap_window.height(),
                            org_e(m_sys_bpp));

            rendering_buffer rbuf_tmp;
            rbuf_tmp.attach(pmap_tmp.buf(),
                            pmap_tmp.width(),
                            pmap_tmp.height(),
                            m_flip_y ?
                              pmap_tmp.stride() :
                             -pmap_tmp.stride());

            convert_pmap(&rbuf_tmp, src, m_format);
            pmap_tmp.draw(dc);
        }
    }



    //------------------------------------------------------------------------
    bool platform_specific::save_pmap(const char* fn, unsigned idx, 
                                      const rendering_buffer* src)
    {
        if(m_sys_format == m_format)
        {
            return m_pmap_img[idx].save_as_bmp(fn);
        }

        pixel_map pmap_tmp;
        pmap_tmp.create(m_pmap_img[idx].width(), 
                          m_pmap_img[idx].height(),
                          org_e(m_sys_bpp));

        rendering_buffer rbuf_tmp;
        rbuf_tmp.attach(pmap_tmp.buf(),
                          pmap_tmp.width(),
                          pmap_tmp.height(),
                          m_flip_y ?
                          pmap_tmp.stride() :
                          -pmap_tmp.stride());

        convert_pmap(&rbuf_tmp, src, m_format);
        return pmap_tmp.save_as_bmp(fn);
    }



    //------------------------------------------------------------------------
    bool platform_specific::load_pmap(const char* fn, unsigned idx, 
                                      rendering_buffer* dst)
    {
        pixel_map pmap_tmp;
        if(!pmap_tmp.load_from_bmp(fn)) return false;

        rendering_buffer rbuf_tmp;
        rbuf_tmp.attach(pmap_tmp.buf(),
                        pmap_tmp.width(),
                        pmap_tmp.height(),
                        m_flip_y ?
                          pmap_tmp.stride() :
                         -pmap_tmp.stride());

        m_pmap_img[idx].create(pmap_tmp.width(), 
                               pmap_tmp.height(), 
                               org_e(m_bpp),
                               0);

        dst->attach(m_pmap_img[idx].buf(),
                    m_pmap_img[idx].width(),
                    m_pmap_img[idx].height(),
                    m_flip_y ?
                       m_pmap_img[idx].stride() :
                      -m_pmap_img[idx].stride());

        switch(m_format)
        {
        case pix_format_gray8:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_gray8()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_gray8()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray8()); break;
            }
            break;

        case pix_format_gray16:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_gray16()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_gray16()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray16()); break;
            }
            break;

        case pix_format_rgb555:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb555()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb555()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb555()); break;
            }
            break;

        case pix_format_rgb565:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb565()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb565()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb565()); break;
            }
            break;

        case pix_format_rgb24:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb24()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb24()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb24()); break;
            }
            break;

        case pix_format_bgr24:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgr24()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgr24()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr24()); break;
            }
            break;

        case pix_format_rgb48:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb48()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgb48()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb48()); break;
            }
            break;

        case pix_format_bgr48:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgr48()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgr48()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr48()); break;
            }
            break;

        case pix_format_abgr32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_abgr32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_abgr32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr32()); break;
            }
            break;

        case pix_format_argb32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_argb32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_argb32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb32()); break;
            }
            break;

        case pix_format_bgra32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgra32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgra32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra32()); break;
            }
            break;

        case pix_format_rgba32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgba32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgba32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba32()); break;
            }
            break;

        case pix_format_abgr64:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_abgr64()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_abgr64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr64()); break;
            }
            break;

        case pix_format_argb64:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_argb64()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_argb64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb64()); break;
            }
            break;

        case pix_format_bgra64:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgra64()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_bgra64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra64()); break;
            }
            break;

        case pix_format_rgba64:
            switch(pmap_tmp.bpp())
            {
            //case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgba64()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_bgr24_to_rgba64()); break;
            //case 32: color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba64()); break;
            }
            break;

        }

        return true;
    }








    //------------------------------------------------------------------------
    unsigned platform_specific::translate(unsigned keycode)
    {
        return m_last_translated_key = (keycode > 255) ? 0 : m_keymap[keycode];
    }



    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(format, flip_y)),
        m_format(format),
        m_bpp(m_specific->m_bpp),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }



    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        if(m_specific->m_hwnd)
        {
            SetWindowText(m_specific->m_hwnd, m_caption);
        }
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        ::QueryPerformanceCounter(&(m_specific->m_sw_start));
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        LARGE_INTEGER stop;
        ::QueryPerformanceCounter(&stop);
        return double(stop.QuadPart - 
                      m_specific->m_sw_start.QuadPart) * 1000.0 / 
                      double(m_specific->m_sw_freq.QuadPart);
    }



    //------------------------------------------------------------------------
    static unsigned get_key_flags(int wflags)
    {
        unsigned flags = 0;
        if(wflags & MK_LBUTTON) flags |= mouse_left;
        if(wflags & MK_RBUTTON) flags |= mouse_right;
        if(wflags & MK_SHIFT)   flags |= kbd_shift;
        if(wflags & MK_CONTROL) flags |= kbd_ctrl;
        return flags;
    }


    void* platform_support::raw_display_handler()
    {
        return m_specific->m_current_dc;
    }


    //------------------------------------------------------------------------
    LRESULT CALLBACK window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC paintDC;


        void* user_data = reinterpret_cast<void*>(::GetWindowLong(hWnd, GWL_USERDATA));
        platform_support* app = 0;

        if(user_data)
        {
            app = reinterpret_cast<platform_support*>(user_data);
        }

        if(app == 0)
        {
            if(msg == WM_DESTROY)
            {
                ::PostQuitMessage(0);
                return 0;
            }
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
        }

        HDC dc = ::GetDC(app->m_specific->m_hwnd);
        app->m_specific->m_current_dc = dc;
        LRESULT ret = 0;

        switch(msg) 
        {
        //--------------------------------------------------------------------
        case WM_CREATE:
            break;
        
        //--------------------------------------------------------------------
        case WM_SIZE:
            app->m_specific->create_pmap(LOWORD(lParam), 
                                         HIWORD(lParam),
                                         &app->rbuf_window());

            app->trans_affine_resizing(LOWORD(lParam), HIWORD(lParam));
            app->on_resize(LOWORD(lParam), HIWORD(lParam));
            app->force_redraw();
            break;
        
        //--------------------------------------------------------------------
        case WM_ERASEBKGND:
            break;
        
        //--------------------------------------------------------------------
        case WM_LBUTTONDOWN:
            ::SetCapture(app->m_specific->m_hwnd);
            app->m_specific->m_cur_x = int16(LOWORD(lParam));
            if(app->flip_y())
            {
                app->m_specific->m_cur_y = app->rbuf_window().height() - int16(HIWORD(lParam));
            }
            else
            {
                app->m_specific->m_cur_y = int16(HIWORD(lParam));
            }
            app->m_specific->m_input_flags = mouse_left | get_key_flags(wParam);
            
            app->m_ctrls.set_cur(app->m_specific->m_cur_x, 
                                 app->m_specific->m_cur_y);
            if(app->m_ctrls.on_mouse_button_down(app->m_specific->m_cur_x, 
                                                 app->m_specific->m_cur_y))
            {
                app->on_ctrl_change();
                app->force_redraw();
            }
            else
            {
                if(app->m_ctrls.in_rect(app->m_specific->m_cur_x, 
                                        app->m_specific->m_cur_y))
                {
                    if(app->m_ctrls.set_cur(app->m_specific->m_cur_x, 
                                            app->m_specific->m_cur_y))
                    {
                        app->on_ctrl_change();
                        app->force_redraw();
                    }
                }
                else
                {
                    app->on_mouse_button_down(app->m_specific->m_cur_x, 
                                              app->m_specific->m_cur_y, 
                                              app->m_specific->m_input_flags);
                }
            }
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;

        //--------------------------------------------------------------------
        case WM_LBUTTONUP:
            ::ReleaseCapture();
            app->m_specific->m_cur_x = int16(LOWORD(lParam));
            if(app->flip_y())
            {
                app->m_specific->m_cur_y = app->rbuf_window().height() - int16(HIWORD(lParam));
            }
            else
            {
                app->m_specific->m_cur_y = int16(HIWORD(lParam));
            }
            app->m_specific->m_input_flags = mouse_left | get_key_flags(wParam);

            if(app->m_ctrls.on_mouse_button_up(app->m_specific->m_cur_x, 
                                               app->m_specific->m_cur_y))
            {
                app->on_ctrl_change();
                app->force_redraw();
            }
            app->on_mouse_button_up(app->m_specific->m_cur_x, 
                                    app->m_specific->m_cur_y, 
                                    app->m_specific->m_input_flags);
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;


        //--------------------------------------------------------------------
        case WM_RBUTTONDOWN:
            ::SetCapture(app->m_specific->m_hwnd);
            app->m_specific->m_cur_x = int16(LOWORD(lParam));
            if(app->flip_y())
            {
                app->m_specific->m_cur_y = app->rbuf_window().height() - int16(HIWORD(lParam));
            }
            else
            {
                app->m_specific->m_cur_y = int16(HIWORD(lParam));
            }
            app->m_specific->m_input_flags = mouse_right | get_key_flags(wParam);
            app->on_mouse_button_down(app->m_specific->m_cur_x, 
                                      app->m_specific->m_cur_y, 
                                      app->m_specific->m_input_flags);
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;

        //--------------------------------------------------------------------
        case WM_RBUTTONUP:
            ::ReleaseCapture();
            app->m_specific->m_cur_x = int16(LOWORD(lParam));
            if(app->flip_y())
            {
                app->m_specific->m_cur_y = app->rbuf_window().height() - int16(HIWORD(lParam));
            }
            else
            {
                app->m_specific->m_cur_y = int16(HIWORD(lParam));
            }
            app->m_specific->m_input_flags = mouse_right | get_key_flags(wParam);
            app->on_mouse_button_up(app->m_specific->m_cur_x, 
                                    app->m_specific->m_cur_y, 
                                    app->m_specific->m_input_flags);
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;

        //--------------------------------------------------------------------
        case WM_MOUSEMOVE:
            app->m_specific->m_cur_x = int16(LOWORD(lParam));
            if(app->flip_y())
            {
                app->m_specific->m_cur_y = app->rbuf_window().height() - int16(HIWORD(lParam));
            }
            else
            {
                app->m_specific->m_cur_y = int16(HIWORD(lParam));
            }
            app->m_specific->m_input_flags = get_key_flags(wParam);


            if(app->m_ctrls.on_mouse_move(
                app->m_specific->m_cur_x, 
                app->m_specific->m_cur_y,
                (app->m_specific->m_input_flags & mouse_left) != 0))
            {
                app->on_ctrl_change();
                app->force_redraw();
            }
            else
            {
                if(!app->m_ctrls.in_rect(app->m_specific->m_cur_x, 
                                         app->m_specific->m_cur_y))
                {
                    app->on_mouse_move(app->m_specific->m_cur_x, 
                                       app->m_specific->m_cur_y, 
                                       app->m_specific->m_input_flags);
                }
            }
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;

        //--------------------------------------------------------------------
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            app->m_specific->m_last_translated_key = 0;
            switch(wParam) 
            {
                case VK_CONTROL:
                    app->m_specific->m_input_flags |= kbd_ctrl;
                    break;

                case VK_SHIFT:
                    app->m_specific->m_input_flags |= kbd_shift;
                    break;

                default:
                    app->m_specific->translate(wParam);
                    break;
            }
        
            if(app->m_specific->m_last_translated_key)
            {
                bool left  = false;
                bool up    = false;
                bool right = false;
                bool down  = false;

                switch(app->m_specific->m_last_translated_key)
                {
                case key_left:
                    left = true;
                    break;

                case key_up:
                    up = true;
                    break;

                case key_right:
                    right = true;
                    break;

                case key_down:
                    down = true;
                    break;

                case key_f2:                        
                    app->copy_window_to_img(agg::platform_support::max_images - 1);
                    app->save_img(agg::platform_support::max_images - 1, "screenshot");
                    break;
                }

                if(app->window_flags() & window_process_all_keys)
                {
                    app->on_key(app->m_specific->m_cur_x,
                                app->m_specific->m_cur_y,
                                app->m_specific->m_last_translated_key,
                                app->m_specific->m_input_flags);
                }
                else
                {
                    if(app->m_ctrls.on_arrow_keys(left, right, down, up))
                    {
                        app->on_ctrl_change();
                        app->force_redraw();
                    }
                    else
                    {
                        app->on_key(app->m_specific->m_cur_x,
                                    app->m_specific->m_cur_y,
                                    app->m_specific->m_last_translated_key,
                                    app->m_specific->m_input_flags);
                    }
                }
            }
/*
            if(!app->wait_mode())
            {
                app->on_idle();
            }
*/
            break;

        //--------------------------------------------------------------------
        case WM_SYSKEYUP:
        case WM_KEYUP:
            app->m_specific->m_last_translated_key = 0;
            switch(wParam) 
            {
                case VK_CONTROL:
                    app->m_specific->m_input_flags &= ~kbd_ctrl;
                    break;

                case VK_SHIFT:
                    app->m_specific->m_input_flags &= ~kbd_shift;
                    break;
            }
            break;

        //--------------------------------------------------------------------
        case WM_CHAR:
        case WM_SYSCHAR:
            if(app->m_specific->m_last_translated_key == 0)
            {
                app->on_key(app->m_specific->m_cur_x,
                            app->m_specific->m_cur_y,
                            wParam,
                            app->m_specific->m_input_flags);
            }
            break;
        
        //--------------------------------------------------------------------
        case WM_PAINT:
            paintDC = ::BeginPaint(hWnd, &ps);
            app->m_specific->m_current_dc = paintDC;
            if(app->m_specific->m_redraw_flag)
            {
                app->on_draw();
                app->m_specific->m_redraw_flag = false;
            }
            app->m_specific->display_pmap(paintDC, &app->rbuf_window());
            app->on_post_draw(paintDC);
            app->m_specific->m_current_dc = 0;
            ::EndPaint(hWnd, &ps);
            break;
        
        //--------------------------------------------------------------------
        case WM_COMMAND:
            break;
        
        //--------------------------------------------------------------------
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        
        //--------------------------------------------------------------------
        default:
            ret = ::DefWindowProc(hWnd, msg, wParam, lParam);
            break;
        }
        app->m_specific->m_current_dc = 0;
        ::ReleaseDC(app->m_specific->m_hwnd, dc);
        return ret;
    }


    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        ::MessageBox(m_specific->m_hwnd, msg, "AGG Message", MB_OK);
    }


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        if(m_specific->m_sys_format == pix_format_undefined)
        {
            return false;
        }

        m_window_flags = flags;

        int wflags = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;

        WNDCLASS wc;
        wc.lpszClassName = "AGGAppClass";
        wc.lpfnWndProc = window_proc;
        wc.style = wflags;
        wc.hInstance = g_windows_instance;
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.lpszMenuName = "AGGAppMenu";
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        ::RegisterClass(&wc);

        wflags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

        if(m_window_flags & window_resize)
        {
            wflags |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        m_specific->m_hwnd = ::CreateWindow("AGGAppClass",
                                            m_caption,
                                            wflags,
                                            100,
                                            100,
                                            width,
                                            height,
                                            0,
                                            0,
                                            g_windows_instance,
                                            0);

        if(m_specific->m_hwnd == 0)
        {
            return false;
        }


        RECT rct;
        ::GetClientRect(m_specific->m_hwnd, &rct);

        ::MoveWindow(m_specific->m_hwnd,   // handle to window
                     100,                  // horizontal position
                     100,                  // vertical position
                     width + (width - (rct.right - rct.left)),
                     height + (height - (rct.bottom - rct.top)),
                     FALSE);
   
        ::SetWindowLong(m_specific->m_hwnd, GWL_USERDATA, (LONG)this);
        m_specific->create_pmap(width, height, &m_rbuf_window);
        m_initial_width = width;
        m_initial_height = height;
        on_init();
        m_specific->m_redraw_flag = true;
        ::ShowWindow(m_specific->m_hwnd, g_windows_cmd_show);
        return true;
    }



    //------------------------------------------------------------------------
    int platform_support::run()
    {
        MSG msg;

        for(;;)
        {
            if(m_wait_mode)
            {
                if(!::GetMessage(&msg, 0, 0, 0))
                {
                    break;
                }
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    ::TranslateMessage(&msg);
                    if(msg.message == WM_QUIT)
                    {
                        break;
                    }
                    ::DispatchMessage(&msg);
                }
                else
                {
                    on_idle();
                }
            }
        }
        return (int)msg.wParam;
    }


    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".bmp"; }


    //------------------------------------------------------------------------
    const char* platform_support::full_file_name(const char* file_name)
    {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
            if(len < 4 || stricmp(fn + len - 4, ".BMP") != 0)
            {
                strcat(fn, ".bmp");
            }
            return m_specific->load_pmap(fn, idx, &m_rbuf_img[idx]);
        }
        return true;
    }



    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
            if(len < 4 || stricmp(fn + len - 4, ".BMP") != 0)
            {
                strcat(fn, ".bmp");
            }
            return m_specific->save_pmap(fn, idx, &m_rbuf_img[idx]);
        }
        return true;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = m_specific->m_pmap_window.width();
            if(height == 0) height = m_specific->m_pmap_window.height();
            m_specific->m_pmap_img[idx].create(width, height, org_e(m_specific->m_bpp));
            m_rbuf_img[idx].attach(m_specific->m_pmap_img[idx].buf(), 
                                   m_specific->m_pmap_img[idx].width(),
                                   m_specific->m_pmap_img[idx].height(),
                                   m_flip_y ?
                                    m_specific->m_pmap_img[idx].stride() :
                                   -m_specific->m_pmap_img[idx].stride());
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->m_redraw_flag = true;
        ::InvalidateRect(m_specific->m_hwnd, 0, FALSE);
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        HDC dc = ::GetDC(m_specific->m_hwnd);
        m_specific->display_pmap(dc, &m_rbuf_window);
        ::ReleaseDC(m_specific->m_hwnd, dc);
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}
};




namespace agg
{
    // That's ridiculous. I have to parse the command line by myself
    // because Windows doesn't provide a method of getting the command
    // line arguments in a form of argc, argv. Of course, there's 
    // CommandLineToArgv() but first, it returns Unicode that I don't
    // need to deal with, but most of all, it's not compatible with Win98.
    //-----------------------------------------------------------------------
    class tokenizer
    {
    public:
        enum sep_flag
        {
            single,
            multiple,
            whole_str
        };

        struct token
        {
            const char* ptr;
            unsigned    len;
        };

    public:
        tokenizer(const char* sep, 
                  const char* trim=0,
                  const char* quote="\"",
                  char mask_chr='\\',
                  sep_flag sf=multiple);

        void  set_str(const char* str);
        token next_token();

    private:
        int  check_chr(const char *str, char chr);

    private:
        const char* m_src_string;
        int         m_start;
        const char* m_sep;
        const char* m_trim;
        const char* m_quote;
        char        m_mask_chr;
        unsigned    m_sep_len;
        sep_flag    m_sep_flag;
    };



    //-----------------------------------------------------------------------
    inline void tokenizer::set_str(const char* str) 
    { 
        m_src_string = str; 
        m_start = 0;
    }


    //-----------------------------------------------------------------------
    inline int tokenizer::check_chr(const char *str, char chr)
    {
        return int(strchr(str, chr));
    }


    //-----------------------------------------------------------------------
    tokenizer::tokenizer(const char* sep, 
                         const char* trim,
                         const char* quote,
                         char mask_chr,
                         sep_flag sf) :
        m_src_string(0),
        m_start(0),
        m_sep(sep),
        m_trim(trim),
        m_quote(quote),
        m_mask_chr(mask_chr),
        m_sep_len(sep ? strlen(sep) : 0),
        m_sep_flag(sep ? sf : single)
    {
    }


    //-----------------------------------------------------------------------
    tokenizer::token tokenizer::next_token()
    {
        unsigned count = 0;
        char quote_chr = 0;
        token tok;

        tok.ptr = 0;
        tok.len = 0;
        if(m_src_string == 0 || m_start == -1) return tok;

        register const char *pstr = m_src_string + m_start;

        if(*pstr == 0) 
        {
            m_start = -1;
            return tok;
        }

        int sep_len = 1;
        if(m_sep_flag == whole_str) sep_len = m_sep_len;

        if(m_sep_flag == multiple)
        {
            //Pass all the separator symbols at the begin of the string
            while(*pstr && check_chr(m_sep, *pstr)) 
            {
                ++pstr;
                ++m_start;
            }
        }

        if(*pstr == 0) 
        {
            m_start = -1;
            return tok;
        }

        for(count = 0;; ++count) 
        {
            char c = *pstr;
            int found = 0;

            //We are outside of qotation: find one of separator symbols
            if(quote_chr == 0)
            {
                if(sep_len == 1)
                {
                    found = check_chr(m_sep, c);
                }
                else
                {
                    found = strncmp(m_sep, pstr, m_sep_len) == 0; 
                }
            }

            ++pstr;

            if(c == 0 || found) 
            {
                if(m_trim)
                {
                    while(count && 
                          check_chr(m_trim, m_src_string[m_start]))
                    {
                        ++m_start;
                        --count;
                    }

                    while(count && 
                          check_chr(m_trim, m_src_string[m_start + count - 1]))
                    {
                        --count;
                    }
                }

                tok.ptr = m_src_string + m_start;
                tok.len = count;

                //Next time it will be the next separator character
                //But we must check, whether it is NOT the end of the string.
                m_start += count;
                if(c) 
                {
                    m_start += sep_len;
                    if(m_sep_flag == multiple)
                    {
                        //Pass all the separator symbols 
                        //after the end of the string
                        while(check_chr(m_sep, m_src_string[m_start])) 
                        {
                            ++m_start;
                        }
                    }
                }
                break;
            }

            //Switch quote. If it is not a quote yet, try to check any of
            //quote symbols. Otherwise quote must be finished with quote_symb
            if(quote_chr == 0)
            {
                if(check_chr(m_quote, c)) 
                {
                    quote_chr = c;
                    continue;
                }
            }
            else
            {
                //We are inside quote: pass all the mask symbols
                if(m_mask_chr && c == m_mask_chr)
                {
                    if(*pstr) 
                    {
                        ++count;
                        ++pstr;
                    }
                    continue; 
                }
                if(c == quote_chr) 
                {
                    quote_chr = 0;
                    continue;
                }
            }
        }
        return tok;
    }


}



//----------------------------------------------------------------------------
int agg_main(int argc, char* argv[]);



//----------------------------------------------------------------------------
/*int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine,
                   int nCmdShow)
{
    agg::g_windows_instance = hInstance;
    agg::g_windows_cmd_show = nCmdShow;

    char* argv_str = new char [strlen(lpszCmdLine) + 3];
    char* argv_ptr = argv_str;

    char* argv[64];
    memset(argv, 0, sizeof(argv));

    agg::tokenizer cmd_line(" ", "\"' ", "\"'", '\\', agg::tokenizer::multiple);
    cmd_line.set_str(lpszCmdLine);

    int argc = 0;
    argv[argc++] = argv_ptr;
    *argv_ptr++ = 0;

    while(argc < 64)
    {
        agg::tokenizer::token tok = cmd_line.next_token();
        if(tok.ptr == 0) break;
        if(tok.len)
        {
            memcpy(argv_ptr, tok.ptr, tok.len);
            argv[argc++] = argv_ptr;
            argv_ptr += tok.len;
            *argv_ptr++ = 0;
        }
    }

    int ret = agg_main(argc, argv);
    delete [] argv_str;

    return ret;
}
*/

#endif


//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// class platform_support
//
//----------------------------------------------------------------------------

#ifdef PLATFORM_AMIGA

#include <agg24/agg_platform_support.h>
#include <agg24/agg_color_conv_rgb8.h>

#include <sys/time.h>
#include <cstring>

#include <classes/requester.h>
#include <classes/window.h>
#include <datatypes/pictureclass.h>
#include <proto/exec.h>
#include <proto/datatypes.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/keymap.h>
#include <proto/Picasso96API.h>
#include <proto/utility.h>

Library* DataTypesBase = 0;
Library* GraphicsBase = 0;
Library* IntuitionBase = 0;
Library* KeymapBase = 0;
Library* P96Base = 0;

DataTypesIFace* IDataTypes = 0;
GraphicsIFace* IGraphics = 0;
IntuitionIFace* IIntuition = 0;
KeymapIFace* IKeymap = 0;
P96IFace* IP96 = 0;

Class* RequesterClass = 0;
Class* WindowClass = 0;


namespace agg
{
	void handle_idcmp(Hook* hook, APTR win, IntuiMessage* msg);

	//------------------------------------------------------------------------
	class platform_specific
	{
	public:
		platform_specific(platform_support& support, pix_format_e format,
			bool flip_y);
		~platform_specific();
		bool handle_input();
		bool load_img(const char* file, unsigned idx, rendering_buffer* rbuf);
		bool create_img(unsigned idx, rendering_buffer* rbuf, unsigned width,
			unsigned height);
		bool make_bitmap();
	public:
		platform_support& m_support;
		RGBFTYPE m_ftype;
		pix_format_e m_format;
		unsigned m_bpp;
		BitMap* m_bitmap;
		bool m_flip_y;
		uint16 m_width;
		uint16 m_height;
		APTR m_window_obj;
		Window* m_window;
		Hook* m_idcmp_hook;
		unsigned m_input_flags;
		bool m_dragging;
		double m_start_time;
		uint16 m_last_key;
		BitMap* m_img_bitmaps[platform_support::max_images];
	};

	//------------------------------------------------------------------------
	platform_specific::platform_specific(platform_support& support,
		pix_format_e format, bool flip_y) :
		m_support(support),
		m_ftype(RGBFB_NONE),
		m_format(format),
		m_bpp(0),
		m_bitmap(0),
		m_flip_y(flip_y),
		m_width(0),
		m_height(0),
		m_window_obj(0),
		m_window(0),
		m_idcmp_hook(0),
		m_input_flags(0),
		m_dragging(false),
		m_start_time(0.0),
		m_last_key(0)
	{
		switch ( format )
		{
		case pix_format_gray8:
			// Not supported.
			break;
		case pix_format_rgb555:
			m_ftype = RGBFB_R5G5B5;
			m_bpp = 15;
			break;
		case pix_format_rgb565:
			m_ftype = RGBFB_R5G6B5;
			m_bpp = 16;
			break;
		case pix_format_rgb24:
			m_ftype = RGBFB_R8G8B8;
			m_bpp = 24;
			break;
		case pix_format_bgr24:
			m_ftype = RGBFB_B8G8R8;
			m_bpp = 24;
			break;
		case pix_format_bgra32:
			m_ftype = RGBFB_B8G8R8A8;
			m_bpp = 32;
			break;
		case pix_format_abgr32:
			m_ftype = RGBFB_A8B8G8R8;
			m_bpp = 32;
			break;
		case pix_format_argb32:
			m_ftype = RGBFB_A8R8G8B8;
			m_bpp = 32;
			break;
        case pix_format_rgba32:
			m_ftype = RGBFB_R8G8B8A8;
			m_bpp = 32;
			break;
		}

		for ( unsigned i = 0; i < platform_support::max_images; ++i )
		{
			m_img_bitmaps[i] = 0;
		}
	}

	//------------------------------------------------------------------------
	platform_specific::~platform_specific()
	{
		IIntuition->DisposeObject(m_window_obj);

		IP96->p96FreeBitMap(m_bitmap);

		for ( unsigned i = 0; i < platform_support::max_images; ++i )
		{
			IP96->p96FreeBitMap(m_img_bitmaps[i]);
		}

		if ( m_idcmp_hook != 0 )
		{
			IExec->FreeSysObject(ASOT_HOOK, m_idcmp_hook);
		}
	}

	//------------------------------------------------------------------------
	bool platform_specific::handle_input()
	{
		int16 code = 0;
		uint32 result = 0;
		Object* obj = reinterpret_cast<Object*>(m_window_obj);

		while ( (result = IIntuition->IDoMethod(obj, WM_HANDLEINPUT,
				&code)) != WMHI_LASTMSG )
		{
			switch ( result & WMHI_CLASSMASK )
			{
			case WMHI_CLOSEWINDOW:
				return true;
				break;
			case WMHI_INTUITICK:
				if ( !m_support.wait_mode() )
				{
					m_support.on_idle();
				}
				break;
			case WMHI_NEWSIZE:
				if ( make_bitmap() )
				{
					m_support.trans_affine_resizing(m_width, m_height);
					m_support.on_resize(m_width, m_height);
					m_support.force_redraw();
				}
				break;
			}
		}

		return false;
	}		

	//------------------------------------------------------------------------
	bool platform_specific::load_img(const char* file, unsigned idx,
		rendering_buffer* rbuf)
	{
		if ( m_img_bitmaps[idx] != 0 )
		{
			IP96->p96FreeBitMap(m_img_bitmaps[idx]);
			m_img_bitmaps[idx] = 0;
		}

		bool result = false;

		Object* picture = IDataTypes->NewDTObject(const_cast<STRPTR>(file),
			DTA_GroupID, GID_PICTURE,
			PDTA_DestMode, PMODE_V43,
			PDTA_Remap, FALSE,
			TAG_END);
		if ( picture != 0 )
		{
			gpLayout layout;
			layout.MethodID = DTM_PROCLAYOUT;
			layout.gpl_GInfo = 0;
			layout.gpl_Initial = 1;
			ULONG loaded = IDataTypes->DoDTMethodA(picture, 0, 0,
				reinterpret_cast<Msg>(&layout));
			if ( loaded != 0 )
			{
				BitMap* src_bitmap = 0;
				IDataTypes->GetDTAttrs(picture,
					PDTA_ClassBitMap, &src_bitmap,
					TAG_END);

				bool supported = false;

				RGBFTYPE ftype = static_cast<RGBFTYPE>(IP96->p96GetBitMapAttr(
					src_bitmap, P96BMA_RGBFORMAT));

				switch ( ftype )
				{
				case RGBFB_R8G8B8:
					supported = true;
					break;
				default:
					m_support.message("File uses unsupported graphics mode.");
					break;
				}

				if ( supported )  {
					uint16 width = IP96->p96GetBitMapAttr(src_bitmap,
						P96BMA_WIDTH);
					uint16 height = IP96->p96GetBitMapAttr(src_bitmap,
						P96BMA_HEIGHT);

					m_img_bitmaps[idx] = IP96->p96AllocBitMap(width, height,
						m_bpp, BMF_USERPRIVATE, 0, m_ftype);
					if ( m_img_bitmaps[idx] != 0 )
					{
						int8u* buf = reinterpret_cast<int8u*>(
							IP96->p96GetBitMapAttr(m_img_bitmaps[idx],
							P96BMA_MEMORY));
						int bpr = IP96->p96GetBitMapAttr(m_img_bitmaps[idx],
							P96BMA_BYTESPERROW);
						int stride = (m_flip_y) ? -bpr : bpr;
						rbuf->attach(buf, width, height, stride);

						// P96 sets the alpha to zero so it can't be used to
						// color convert true color modes.
						if ( m_bpp == 32 )
						{
							RenderInfo ri;
							int32 lock = IP96->p96LockBitMap(src_bitmap,
								reinterpret_cast<uint8*>(&ri),
								sizeof(RenderInfo));

							rendering_buffer rbuf_src;
							rbuf_src.attach(
								reinterpret_cast<int8u*>(ri.Memory),
								width, height, (m_flip_y) ?
									-ri.BytesPerRow : ri.BytesPerRow);

							switch ( m_format )
							{
							case pix_format_bgra32:
								color_conv(rbuf, &rbuf_src,
									color_conv_rgb24_to_bgra32());
								break;
							case pix_format_abgr32:
								color_conv(rbuf, &rbuf_src,
									color_conv_rgb24_to_abgr32());
								break;
							case pix_format_argb32:
								color_conv(rbuf, &rbuf_src,
									color_conv_rgb24_to_argb32());
								break;
							case pix_format_rgba32:
								color_conv(rbuf, &rbuf_src,
									color_conv_rgb24_to_rgba32());
								break;
							}

							IP96->p96UnlockBitMap(src_bitmap, lock);
						}
						else
						{
							IGraphics->BltBitMap(src_bitmap, 0, 0,
								m_img_bitmaps[idx], 0, 0, width, height,
								ABC|ABNC, 0xFF, 0);
						}

						result = true;
					}
				}
			}
		}

		IGraphics->WaitBlit();
		IDataTypes->DisposeDTObject(picture);

		return result;
	}

	//------------------------------------------------------------------------
	bool platform_specific::create_img(unsigned idx, rendering_buffer* rbuf,
		unsigned width, unsigned height)
	{
		if ( m_img_bitmaps[idx] != 0 )
		{
			IP96->p96FreeBitMap(m_img_bitmaps[idx]);
			m_img_bitmaps[idx] = 0;
		}

		m_img_bitmaps[idx] = IP96->p96AllocBitMap(width, height,
			m_bpp, BMF_USERPRIVATE, m_bitmap, m_ftype);
		if ( m_img_bitmaps[idx] != 0 )
		{
			int8u* buf = reinterpret_cast<int8u*>(
				IP96->p96GetBitMapAttr(m_img_bitmaps[idx],
				P96BMA_MEMORY));
			int bpr = IP96->p96GetBitMapAttr(m_img_bitmaps[idx],
				P96BMA_BYTESPERROW);
			int stride = (m_flip_y) ? -bpr : bpr;

			rbuf->attach(buf, width, height, stride);

			return true;
		}

		return false;
	}

	//------------------------------------------------------------------------
	bool platform_specific::make_bitmap()
	{
		uint32 width = 0;
		uint32 height = 0;
		IIntuition->GetWindowAttrs(m_window,
			WA_InnerWidth, &width,
			WA_InnerHeight, &height,
			TAG_END);

		BitMap* bm = IP96->p96AllocBitMap(width, height, m_bpp,
			BMF_USERPRIVATE|BMF_CLEAR, 0, m_ftype);
		if ( bm == 0 )
		{
			return false;
		}

		int8u* buf = reinterpret_cast<int8u*>(
			IP96->p96GetBitMapAttr(bm, P96BMA_MEMORY));
		int bpr = IP96->p96GetBitMapAttr(bm, P96BMA_BYTESPERROW);
		int stride = (m_flip_y) ? -bpr : bpr;

		m_support.rbuf_window().attach(buf, width, height, stride);

		if ( m_bitmap != 0 )
		{
			IP96->p96FreeBitMap(m_bitmap);
			m_bitmap = 0;
		}

		m_bitmap = bm;
		m_width = width;
		m_height = height;

		return true;
	}

	//------------------------------------------------------------------------
	platform_support::platform_support(pix_format_e format, bool flip_y) :
		m_specific(new platform_specific(*this, format, flip_y)),
		m_format(format),
		m_bpp(m_specific->m_bpp),
		m_window_flags(0),
		m_wait_mode(true),
		m_flip_y(flip_y),
		m_initial_width(10),
		m_initial_height(10)
	{
		std::strncpy(m_caption, "Anti-Grain Geometry", 256);
	}

	//------------------------------------------------------------------------
	platform_support::~platform_support()
	{
		delete m_specific;
	}

	//------------------------------------------------------------------------
	void platform_support::caption(const char* cap)
	{
		std::strncpy(m_caption, cap, 256);
		if ( m_specific->m_window != 0 )
		{
			const char* ignore = reinterpret_cast<const char*>(-1);
			IIntuition->SetWindowAttr(m_specific->m_window,
				WA_Title, m_caption, sizeof(char*));
		}
	}

	//------------------------------------------------------------------------
	void platform_support::start_timer()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		m_specific->m_start_time = tv.tv_secs + tv.tv_micro/1e6;
	}

	//------------------------------------------------------------------------
	double platform_support::elapsed_time() const
	{
		timeval tv;
		gettimeofday(&tv, 0);
		double end_time = tv.tv_secs + tv.tv_micro/1e6;

		double elasped_seconds = end_time - m_specific->m_start_time;
		double elasped_millis = elasped_seconds*1e3;

		return elasped_millis;
	}

	//------------------------------------------------------------------------
	void* platform_support::raw_display_handler()
	{
		return 0;	// Not available.
	}

	//------------------------------------------------------------------------
	void platform_support::message(const char* msg)
	{
		APTR req = IIntuition->NewObject(RequesterClass, 0,
			REQ_TitleText, "Anti-Grain Geometry",
			REQ_Image, REQIMAGE_INFO,
			REQ_BodyText, msg,
			REQ_GadgetText, "_Ok",
			TAG_END);
		if ( req == 0 )
		{
			IDOS->Printf("Message: %s\n", msg);
			return;
		}

		orRequest reqmsg;
		reqmsg.MethodID = RM_OPENREQ;
		reqmsg.or_Attrs = 0;
		reqmsg.or_Window = m_specific->m_window;
		reqmsg.or_Screen = 0;
		
		IIntuition->IDoMethodA(reinterpret_cast<Object*>(req),
			reinterpret_cast<Msg>(&reqmsg));
		IIntuition->DisposeObject(req);
	}

	//------------------------------------------------------------------------
	bool platform_support::init(unsigned width, unsigned height,
		unsigned flags)
	{
		if( m_specific->m_ftype == RGBFB_NONE )
		{
			message("Unsupported mode requested.");
			return false;
		}

		m_window_flags = flags;

		m_specific->m_idcmp_hook = reinterpret_cast<Hook*>(
			IExec->AllocSysObjectTags(ASOT_HOOK,
				ASOHOOK_Entry, handle_idcmp,
				ASOHOOK_Data, this,
				TAG_END));
		if ( m_specific->m_idcmp_hook == 0 )
		{
			return false;
		}

		m_specific->m_window_obj = IIntuition->NewObject(WindowClass, 0,
				WA_Title, m_caption,
				WA_AutoAdjustDClip, TRUE,
				WA_InnerWidth, width,
				WA_InnerHeight, height,
				WA_Activate, TRUE,
				WA_SmartRefresh, TRUE,
				WA_NoCareRefresh, TRUE,
				WA_CloseGadget, TRUE,
				WA_DepthGadget, TRUE,
				WA_SizeGadget, (flags & agg::window_resize) ? TRUE : FALSE,
				WA_DragBar, TRUE,
				WA_AutoAdjust, TRUE,
				WA_ReportMouse, TRUE,
				WA_RMBTrap, TRUE,
				WA_MouseQueue, 1,
				WA_IDCMP,
					IDCMP_NEWSIZE |
					IDCMP_MOUSEBUTTONS |
					IDCMP_MOUSEMOVE |
					IDCMP_RAWKEY |
					IDCMP_INTUITICKS,
				WINDOW_IDCMPHook, m_specific->m_idcmp_hook,
				WINDOW_IDCMPHookBits,
					IDCMP_MOUSEBUTTONS |
					IDCMP_MOUSEMOVE |
					IDCMP_RAWKEY,
				TAG_END);
		if ( m_specific->m_window_obj == 0 )
		{
			return false;
		}

		Object* obj = reinterpret_cast<Object*>(m_specific->m_window_obj);
		m_specific->m_window =
			reinterpret_cast<Window*>(IIntuition->IDoMethod(obj, WM_OPEN));
		if ( m_specific->m_window == 0 )
		{
			return false;
		}

		RGBFTYPE ftype = static_cast<RGBFTYPE>(IP96->p96GetBitMapAttr(
			m_specific->m_window->RPort->BitMap, P96BMA_RGBFORMAT));

		switch ( ftype )
		{
		case RGBFB_A8R8G8B8:
		case RGBFB_B8G8R8A8:
		case RGBFB_R5G6B5PC:
			break;
		default:
			message("Unsupported screen mode.\n");
			return false;
		}

		if ( !m_specific->make_bitmap() )
		{
			return false;
		}

		m_initial_width = width;
		m_initial_height = height;

		on_init();
		on_resize(width, height);
		force_redraw();

		return true;
	}

	//------------------------------------------------------------------------
	int platform_support::run()
	{
		uint32 window_mask = 0;
		IIntuition->GetAttr(WINDOW_SigMask, m_specific->m_window_obj,
			&window_mask);
		uint32 wait_mask = window_mask | SIGBREAKF_CTRL_C;

		bool done = false;

		while ( !done )
		{
			uint32 sig_mask = IExec->Wait(wait_mask);
			if ( sig_mask & SIGBREAKF_CTRL_C )
			{
				done = true;
			}
			else
			{
				done = m_specific->handle_input();
			}
		}

		return 0;
	}

	//------------------------------------------------------------------------
	const char* platform_support::img_ext() const
	{
		return ".bmp";
	}

	//------------------------------------------------------------------------
	const char* platform_support::full_file_name(const char* file_name)
	{
		return file_name;
	}

	//------------------------------------------------------------------------
	bool platform_support::load_img(unsigned idx, const char* file)
	{
		if ( idx < max_images )
		{
			static char fn[1024];
			std::strncpy(fn, file, 1024);
			int len = std::strlen(fn);
			if ( len < 4 || std::strcmp(fn + len - 4, ".bmp") != 0 )
			{
				std::strncat(fn, ".bmp", 1024);
			}

			return m_specific->load_img(fn, idx, &m_rbuf_img[idx]);
		}

		return false;
	}

	//------------------------------------------------------------------------
	bool platform_support::save_img(unsigned idx, const char* file)
	{
		message("Not supported");
		return false;
	}

	//------------------------------------------------------------------------
	bool platform_support::create_img(unsigned idx, unsigned width,
		unsigned height)
	{
		if ( idx < max_images )
		{
			if ( width == 0 )
			{
				width = m_specific->m_width;
			}

			if ( height == 0 )
			{
				height = m_specific->m_height;
			}

			return m_specific->create_img(idx, &m_rbuf_img[idx], width,
				height);
		}

		return false;
	}

	//------------------------------------------------------------------------
	void platform_support::force_redraw()
	{
		on_draw();
		update_window();
	}

	//------------------------------------------------------------------------
	void platform_support::update_window()
	{
		// Note this function does automatic color conversion.
		IGraphics->BltBitMapRastPort(m_specific->m_bitmap, 0, 0,
			m_specific->m_window->RPort, m_specific->m_window->BorderLeft,
			m_specific->m_window->BorderTop, m_specific->m_width,
			m_specific->m_height, ABC|ABNC);
	}

	//------------------------------------------------------------------------
	void platform_support::on_init() {}
	void platform_support::on_resize(int sx, int sy) {}
	void platform_support::on_idle() {}
	void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
	void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
	void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
	void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
	void platform_support::on_ctrl_change() {}
	void platform_support::on_draw() {}
	void platform_support::on_post_draw(void* raw_handler) {}

	//------------------------------------------------------------------------
	void handle_idcmp(Hook* hook, APTR obj, IntuiMessage* msg)
	{
		platform_support* app =
			reinterpret_cast<platform_support*>(hook->h_Data);
		Window* window = app->m_specific->m_window;

		int16 x = msg->MouseX - window->BorderLeft;

		int16 y = 0;
		if ( app->flip_y() )
		{
			y = window->Height - window->BorderBottom - msg->MouseY;
		}
		else
		{
			y = msg->MouseY - window->BorderTop;
		}

		switch ( msg->Class )
		{
		case IDCMP_MOUSEBUTTONS:
			if ( msg->Code & IECODE_UP_PREFIX )
			{
				if ( msg->Code == SELECTUP )
				{
					app->m_specific->m_input_flags = mouse_left;
					app->m_specific->m_dragging = false;
				}
				else if ( msg->Code == MENUUP )
				{
					app->m_specific->m_input_flags = mouse_right;
					app->m_specific->m_dragging = false;
				}
				else
				{
					return;
				}


				if ( app->m_ctrls.on_mouse_button_up(x, y) )
				{
					app->on_ctrl_change();
					app->force_redraw();
				}

				app->on_mouse_button_up(x, y, app->m_specific->m_input_flags);
			}
			else
			{
				if ( msg->Code == SELECTDOWN )
				{
					app->m_specific->m_input_flags = mouse_left;
					app->m_specific->m_dragging = true;
				}
				else if ( msg->Code == MENUDOWN )
				{
					app->m_specific->m_input_flags = mouse_right;
					app->m_specific->m_dragging = true;
				}
				else
				{
					return;
				}

				app->m_ctrls.set_cur(x, y);
				if ( app->m_ctrls.on_mouse_button_down(x, y) )
				{
					app->on_ctrl_change();
					app->force_redraw();
				}
				else
				{
					if ( app->m_ctrls.in_rect(x, y) )
					{
						if ( app->m_ctrls.set_cur(x, y) )
						{
							app->on_ctrl_change();
							app->force_redraw();
						}
					}
					else
					{
						app->on_mouse_button_down(x, y,
							app->m_specific->m_input_flags);
					}
				}
			}
			break;
		case IDCMP_MOUSEMOVE:
			if ( app->m_specific->m_dragging )  {
				if ( app->m_ctrls.on_mouse_move(x, y,
					 app->m_specific->m_input_flags & mouse_left) != 0 )
				{
					app->on_ctrl_change();
					app->force_redraw();
				}
				else
				{
					if ( !app->m_ctrls.in_rect(x, y) )
					{
						app->on_mouse_move(x, y,
							app->m_specific->m_input_flags);
					}
				}
			}
			break;
		case IDCMP_RAWKEY:
		{
			static InputEvent ie = { 0 };
			ie.ie_Class = IECLASS_RAWKEY;
			ie.ie_Code = msg->Code;
			ie.ie_Qualifier = msg->Qualifier;

			static const unsigned BUF_SIZE = 16;
			static char key_buf[BUF_SIZE];
			int16 num_chars = IKeymap->MapRawKey(&ie, key_buf, BUF_SIZE, 0);

			uint32 code = 0x00000000;
			switch ( num_chars )
			{
			case 1:
				code = key_buf[0];
				break;
			case 2:
				code = key_buf[0]<<8 | key_buf[1];
				break;
			case 3:
				code = key_buf[0]<<16 | key_buf[1]<<8 | key_buf[2];
				break;
			}

			uint16 key_code = 0;

			if ( num_chars == 1 )
			{
				if ( code >= IECODE_ASCII_FIRST && code <= IECODE_ASCII_LAST )
				{
					key_code = code;
				}
			}

			if ( key_code == 0 )
			{
				switch ( code )
				{
				case 0x00000008: key_code = key_backspace;	break;
				case 0x00000009: key_code = key_tab;		break;
				case 0x0000000D: key_code = key_return;		break;
				case 0x0000001B: key_code = key_escape;		break;
				case 0x0000007F: key_code = key_delete;		break;
				case 0x00009B41:
				case 0x00009B54: key_code = key_up;			break;
				case 0x00009B42:
				case 0x00009B53: key_code = key_down;		break;
				case 0x00009B43:
				case 0x009B2040: key_code = key_right;		break;
				case 0x00009B44:
				case 0x009B2041: key_code = key_left;		break;
				case 0x009B307E: key_code = key_f1;			break;
				case 0x009B317E: key_code = key_f2;			break;
				case 0x009B327E: key_code = key_f3;			break;
				case 0x009B337E: key_code = key_f4;			break;
				case 0x009B347E: key_code = key_f5;			break;
				case 0x009B357E: key_code = key_f6;			break;
				case 0x009B367E: key_code = key_f7;			break;
				case 0x009B377E: key_code = key_f8;			break;
				case 0x009B387E: key_code = key_f9;			break;
				case 0x009B397E: key_code = key_f10;		break;
				case 0x009B3F7E: key_code = key_scrollock;	break;
				}
			}

			if ( ie.ie_Code & IECODE_UP_PREFIX )
			{
				if ( app->m_specific->m_last_key != 0 )
				{
					bool left = (key_code == key_left) ? true : false;
					bool right = (key_code == key_right) ? true : false;
					bool down = (key_code == key_down) ? true : false;
					bool up = (key_code == key_up) ? true : false;

					if ( app->m_ctrls.on_arrow_keys(left, right, down, up) )
					{
						app->on_ctrl_change();
						app->force_redraw();
					}
					else
					{
						app->on_key(x, y, app->m_specific->m_last_key, 0);
					}

					app->m_specific->m_last_key = 0;
				}
			}
			else
			{
				app->m_specific->m_last_key = key_code;
			}
			break;
		}
		default:
			break;
		}
	}
}

//----------------------------------------------------------------------------
int agg_main(int argc, char* argv[]);
bool open_libs();
void close_libs();

//----------------------------------------------------------------------------
bool open_libs()
{
	DataTypesBase = IExec->OpenLibrary("datatypes.library", 51);
	GraphicsBase = IExec->OpenLibrary("graphics.library", 51);
	IntuitionBase = IExec->OpenLibrary("intuition.library", 51);
	KeymapBase = IExec->OpenLibrary("keymap.library", 51);
	P96Base = IExec->OpenLibrary("Picasso96API.library", 2);

	IDataTypes = reinterpret_cast<DataTypesIFace*>(
		IExec->GetInterface(DataTypesBase, "main", 1, 0));
	IGraphics = reinterpret_cast<GraphicsIFace*>(
		IExec->GetInterface(GraphicsBase, "main", 1, 0));
	IIntuition = reinterpret_cast<IntuitionIFace*>(
		IExec->GetInterface(IntuitionBase, "main", 1, 0));
	IKeymap = reinterpret_cast<KeymapIFace*>(
		IExec->GetInterface(KeymapBase, "main", 1, 0));
	IP96 = reinterpret_cast<P96IFace*>(
		IExec->GetInterface(P96Base, "main", 1, 0));

	if ( IDataTypes == 0 ||
		 IGraphics == 0 ||
		 IIntuition == 0 ||
		 IKeymap == 0 ||
		 IP96 == 0 )
	{
		close_libs();
		return false;
	}
	else
	{
		return true;
	}
}

//----------------------------------------------------------------------------
void close_libs()
{
	IExec->DropInterface(reinterpret_cast<Interface*>(IP96));
	IExec->DropInterface(reinterpret_cast<Interface*>(IKeymap));
	IExec->DropInterface(reinterpret_cast<Interface*>(IIntuition));
	IExec->DropInterface(reinterpret_cast<Interface*>(IGraphics));
	IExec->DropInterface(reinterpret_cast<Interface*>(IDataTypes));

	IExec->CloseLibrary(P96Base);
	IExec->CloseLibrary(KeymapBase);
	IExec->CloseLibrary(IntuitionBase);
	IExec->CloseLibrary(GraphicsBase);
	IExec->CloseLibrary(DataTypesBase);
}

//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	if ( !open_libs() )  {
		IDOS->Printf("Can't open libraries.\n");
		return -1;
	}

	ClassLibrary* requester =
		IIntuition->OpenClass("requester.class", 51, &RequesterClass);
	ClassLibrary* window =
		IIntuition->OpenClass("window.class", 51, &WindowClass);
	if ( requester == 0 || window == 0 )
	{
		IDOS->Printf("Can't open classes.\n");
		IIntuition->CloseClass(requester);
		IIntuition->CloseClass(window);
		close_libs();
		return -1;
	}

	int rc = agg_main(argc, argv);

	IIntuition->CloseClass(window);
	IIntuition->CloseClass(requester);
	close_libs();

	return rc;
}

#endif

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: superstippi@gmx.de
//----------------------------------------------------------------------------
//
// class platform_support
//
//----------------------------------------------------------------------------

#ifdef PLATFORM_BEOS

#include <new>
#include <stdio.h>

#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <Path.h>
#include <Roster.h>
#include <TranslationUtils.h>
#include <View.h>
#include <Window.h>

#include <string.h>
#include <agg24/agg_platform_support.h>
#include <agg24/agg_color_conv_rgb8.h>

using std::nothrow;


static void
attach_buffer_to_BBitmap(agg::rendering_buffer& buffer, BBitmap* bitmap, bool flipY)
{
    uint8* bits = (uint8*)bitmap->Bits();
    uint32 width = bitmap->Bounds().IntegerWidth() + 1;
    uint32 height = bitmap->Bounds().IntegerHeight() + 1;
    int32 bpr = bitmap->BytesPerRow();
    if (flipY) {
// XXX: why don't I have to do this?!?
//        bits += bpr * (height - 1);
        bpr = -bpr;
    }
    buffer.attach(bits, width, height, bpr);
}


static color_space
pix_format_to_color_space(agg::pix_format_e format)
{
    color_space bitmapFormat = B_NO_COLOR_SPACE;
    switch (format) {
        case agg::pix_format_rgb555:

            bitmapFormat = B_RGB15;
            break;

        case agg::pix_format_rgb565:

            bitmapFormat = B_RGB16;
            break;

        case agg::pix_format_rgb24:
        case agg::pix_format_bgr24:

            bitmapFormat = B_RGB24;
            break;

        case agg::pix_format_rgba32:
        case agg::pix_format_argb32:
        case agg::pix_format_abgr32:
        case agg::pix_format_bgra32:

            bitmapFormat = B_RGBA32;
            break;
    }
    return bitmapFormat;
}


// #pragma mark -


class AGGView : public BView {
 public:
                            AGGView(BRect frame, agg::platform_support* agg,
                                    agg::pix_format_e format, bool flipY);
    virtual                 ~AGGView();

    virtual void            AttachedToWindow();
    virtual void            DetachedFromWindow();

    virtual void            MessageReceived(BMessage* message);
    virtual void            Draw(BRect updateRect);
    virtual void            FrameResized(float width, float height);

    virtual void            KeyDown(const char* bytes, int32 numBytes);

    virtual void            MouseDown(BPoint where);
    virtual void            MouseMoved(BPoint where, uint32 transit,
                               const BMessage* dragMesage);
    virtual void            MouseUp(BPoint where);

            BBitmap*        Bitmap() const;

            uint8           LastKeyDown() const;
            uint32          MouseButtons();

            void            Update();
            void            ForceRedraw();

            unsigned        GetKeyFlags();

 private:
    BBitmap*                fBitmap;
    agg::pix_format_e       fFormat;
    bool                    fFlipY;

    agg::platform_support*  fAGG;

    uint32                  fMouseButtons;
    int32                   fMouseX;
    int32                   fMouseY;

    uint8                   fLastKeyDown;

    bool                    fRedraw;

    BMessageRunner*         fPulse;
    bigtime_t               fLastPulse;
    bool                    fEnableTicks;
};

AGGView::AGGView(BRect frame,
                 agg::platform_support* agg,
                 agg::pix_format_e format,
                 bool flipY)
    : BView(frame, "AGG View", B_FOLLOW_ALL,
            B_FRAME_EVENTS | B_WILL_DRAW),
      fFormat(format),
      fFlipY(flipY),

      fAGG(agg),

      fMouseButtons(0),
      fMouseX(-1),
      fMouseY(-1),
      
      fLastKeyDown(0),

      fRedraw(true),

      fPulse(NULL),
      fLastPulse(0),
      fEnableTicks(true)
{
    SetViewColor(B_TRANSPARENT_32_BIT);
    
    frame.OffsetTo(0.0, 0.0);
    fBitmap = new BBitmap(frame, 0, pix_format_to_color_space(fFormat));
    if (fBitmap->IsValid()) {
        attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);
    } else {
        delete fBitmap;
        fBitmap = NULL;
    }
}


AGGView::~AGGView()
{
    delete fBitmap;
    delete fPulse;
}


void
AGGView::AttachedToWindow()
{
    BMessage message('tick');
    BMessenger target(this, Looper());
    delete fPulse;
//    BScreen screen;
//    TODO: calc screen retrace
    fPulse = new BMessageRunner(target, &message, 40000);

    // make sure we call this once
    fAGG->on_resize(Bounds().IntegerWidth() + 1,
                    Bounds().IntegerHeight() + 1);
    MakeFocus();
}


void
AGGView::DetachedFromWindow()
{
    delete fPulse;
    fPulse = NULL;
}


void
AGGView::MessageReceived(BMessage* message)
{
    bigtime_t now = system_time();
    switch (message->what) {
        case 'tick':
            // drop messages that have piled up
            if (/*now - fLastPulse > 30000*/fEnableTicks) {
                fLastPulse = now;
                if (!fAGG->wait_mode())
                    fAGG->on_idle();
                Window()->PostMessage('entk', this);
                fEnableTicks = false;
            } else {
//                printf("dropping tick message (%lld)\n", now - fLastPulse);
            }
            break;
        case 'entk':
            fEnableTicks = true;
            if (now - fLastPulse > 30000) {
                fLastPulse = now;
                if (!fAGG->wait_mode())
                    fAGG->on_idle();
            }
            break;
        default:
            BView::MessageReceived(message);
            break;
    }
}


void
AGGView::Draw(BRect updateRect)
{
    if (fBitmap) {
        if (fRedraw) {
            fAGG->on_draw();
            fRedraw = false;
        }
        if (fFormat == agg::pix_format_bgra32) {
            DrawBitmap(fBitmap, updateRect, updateRect);
        } else {
            BBitmap* bitmap = new BBitmap(fBitmap->Bounds(), 0, B_RGBA32);

            agg::rendering_buffer rbufSrc;
            attach_buffer_to_BBitmap(rbufSrc, fBitmap, false);

            agg::rendering_buffer rbufDst;
            attach_buffer_to_BBitmap(rbufDst, bitmap, false);

            switch(fFormat) {
                case agg::pix_format_rgb555:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb555_to_bgra32());
                    break;
                case agg::pix_format_rgb565:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb565_to_bgra32());
                    break;
                case agg::pix_format_rgb24:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb24_to_bgra32());
                    break;
                case agg::pix_format_bgr24:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_bgr24_to_bgra32());
                    break;
                case agg::pix_format_rgba32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgba32_to_bgra32());
                    break;
                case agg::pix_format_argb32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_argb32_to_bgra32());
                    break;
                case agg::pix_format_abgr32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_abgr32_to_bgra32());
                    break;
                case agg::pix_format_bgra32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_bgra32_to_bgra32());
                    break;
            }
            DrawBitmap(bitmap, updateRect, updateRect);
            delete bitmap;
        }
    } else {
        FillRect(updateRect);
    }
}


void
AGGView::FrameResized(float width, float height)
{
    BRect r(0.0, 0.0, width, height);
    BBitmap* bitmap = new BBitmap(r, 0, pix_format_to_color_space(fFormat));
    if (bitmap->IsValid()) {
        delete fBitmap;
        fBitmap = bitmap;
           attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);

        fAGG->trans_affine_resizing((int)width + 1,
                                    (int)height + 1);

        // pass the event on to AGG
        fAGG->on_resize((int)width + 1, (int)height + 1);
        
        fRedraw = true;
        Invalidate();
    } else
        delete bitmap;
}


void
AGGView::KeyDown(const char* bytes, int32 numBytes)
{
    if (bytes && numBytes > 0) {
        fLastKeyDown = bytes[0];

        bool left  = false;
        bool up    = false;
        bool right = false;
        bool down  = false;

        switch (fLastKeyDown) {

            case B_LEFT_ARROW:
                left = true;
                break;

            case B_UP_ARROW:
                up = true;
                break;

            case B_RIGHT_ARROW:
                right = true;
                break;

            case B_DOWN_ARROW:
                down = true;
                break;
        }

/*            case key_f2:                        
fAGG->copy_window_to_img(agg::platform_support::max_images - 1);
fAGG->save_img(agg::platform_support::max_images - 1, "screenshot");
break;
}*/


        if (fAGG->m_ctrls.on_arrow_keys(left, right, down, up)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        } else {
            fAGG->on_key(fMouseX, fMouseY, fLastKeyDown, GetKeyFlags());
        }
//        fAGG->on_key(fMouseX, fMouseY, fLastKeyDown, GetKeyFlags());

    }
}


void
AGGView::MouseDown(BPoint where)
{
    BMessage* currentMessage = Window()->CurrentMessage();
    if (currentMessage) {
        if (currentMessage->FindInt32("buttons", (int32*)&fMouseButtons) < B_OK)
            fMouseButtons = B_PRIMARY_MOUSE_BUTTON;
    } else
        fMouseButtons = B_PRIMARY_MOUSE_BUTTON;

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        // left mouse button -> see if to handle in controls
        fAGG->m_ctrls.set_cur(fMouseX, fMouseY);
        if (fAGG->m_ctrls.on_mouse_button_down(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        } else {
            if (fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
                if (fAGG->m_ctrls.set_cur(fMouseX, fMouseY)) {
                    fAGG->on_ctrl_change();
                    fAGG->force_redraw();
                }
            } else {
                fAGG->on_mouse_button_down(fMouseX, fMouseY, GetKeyFlags());
            }
        }
    } else if (fMouseButtons & B_SECONDARY_MOUSE_BUTTON) {
        // right mouse button -> simple
        fAGG->on_mouse_button_down(fMouseX, fMouseY, GetKeyFlags());
    }
    SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}


void
AGGView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMesage)
{
    // workarround missed mouse up events
    // (if we react too slowly, app_server might have dropped events)
    BMessage* currentMessage = Window()->CurrentMessage();
    int32 buttons = 0;
    if (currentMessage->FindInt32("buttons", &buttons) < B_OK) {
        buttons = 0;
    }
    if (!buttons)
        MouseUp(where);

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fAGG->m_ctrls.on_mouse_move(fMouseX, fMouseY,
                                    (GetKeyFlags() & agg::mouse_left) != 0)) {
        fAGG->on_ctrl_change();
        fAGG->force_redraw();
    } else {
        if (!fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
            fAGG->on_mouse_move(fMouseX, fMouseY, GetKeyFlags());
        }
    }
}


void
AGGView::MouseUp(BPoint where)
{
    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        if (fAGG->m_ctrls.on_mouse_button_up(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        }
        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetKeyFlags());
    } else if (fMouseButtons == B_SECONDARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetKeyFlags());
    }
}


BBitmap*
AGGView::Bitmap() const
{
    return fBitmap;
}


uint8
AGGView::LastKeyDown() const
{
    return fLastKeyDown;
}


uint32
AGGView::MouseButtons()
{
    uint32 buttons = 0;
    if (LockLooper()) {
        buttons = fMouseButtons;
        UnlockLooper();
    }
    return buttons;
}


void
AGGView::Update()
{
    // trigger display update
    if (LockLooper()) {
        Invalidate();
        UnlockLooper();
    }
}


void
AGGView::ForceRedraw()
{
    // force a redraw (fRedraw = true;)
    // and trigger display update
    if (LockLooper()) {
        fRedraw = true;
        Invalidate();
        UnlockLooper();
    }
}


unsigned
AGGView::GetKeyFlags()
{
    uint32 buttons = fMouseButtons;
    uint32 mods = modifiers();
    unsigned flags = 0;
    if (buttons & B_PRIMARY_MOUSE_BUTTON)   flags |= agg::mouse_left;
    if (buttons & B_SECONDARY_MOUSE_BUTTON) flags |= agg::mouse_right;
    if (mods & B_SHIFT_KEY)                 flags |= agg::kbd_shift;
    if (mods & B_COMMAND_KEY)               flags |= agg::kbd_ctrl;
    return flags;
}

// #pragma mark -


class AGGWindow : public BWindow {
 public:
                    AGGWindow()
                    : BWindow(BRect(-50.0, -50.0, -10.0, -10.0),
                              "AGG Application", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
                    {
                    }

    virtual bool    QuitRequested()
                    {
                        be_app->PostMessage(B_QUIT_REQUESTED);
                        return true;
                    }

            bool    Init(BRect frame, agg::platform_support* agg, agg::pix_format_e format,
                              bool flipY, uint32 flags)
                    {
                        MoveTo(frame.LeftTop());
                        ResizeTo(frame.Width(), frame.Height());

                        SetFlags(flags);

                        frame.OffsetTo(0.0, 0.0);
                        fView = new AGGView(frame, agg, format, flipY);
                        AddChild(fView);

                        return fView->Bitmap() != NULL;
                    }


        AGGView*    View() const
                    {
                        return fView;
                    }
 private:
    AGGView*        fView;
};

// #pragma mark -


class AGGApplication : public BApplication {
 public:
                    AGGApplication()
                    : BApplication("application/x-vnd.AGG-AGG")
                    {
                        fWindow = new AGGWindow();
                    }

    virtual void    ReadyToRun()
                    {
                        if (fWindow) {
                            fWindow->Show();
                        }
                    }

    virtual bool    Init(agg::platform_support* agg, int width, int height,
                         agg::pix_format_e format, bool flipY, uint32 flags)
                    {
                        BRect r(50.0, 50.0,
                                50.0 + width - 1.0,
                                50.0 + height - 1.0);
                        uint32 windowFlags = B_ASYNCHRONOUS_CONTROLS;
                        if (!(flags & agg::window_resize))
                            windowFlags |= B_NOT_RESIZABLE;

                        return fWindow->Init(r, agg, format, flipY, windowFlags);;
                    }


        AGGWindow*  Window() const
                    {
                        return fWindow;
                    }

 private:
    AGGWindow*      fWindow;
};


// #pragma mark -


namespace agg
{

class platform_specific {
 public:
                    platform_specific(agg::platform_support* agg,
                                      agg::pix_format_e format, bool flip_y)
                        : fAGG(agg),
                          fApp(NULL),
                          fFormat(format),
                          fFlipY(flip_y),
                          fTimerStart(system_time())
                    {
                        memset(fImages, 0, sizeof(fImages));
                        fApp = new AGGApplication();
                        fAppPath[0] = 0;
                        // figure out where we're running from
                        app_info info;
                        status_t ret = fApp->GetAppInfo(&info);
                        if (ret >= B_OK) {
                            BPath path(&info.ref);
                            ret = path.InitCheck();
                            if (ret >= B_OK) {
                                ret = path.GetParent(&path);
                                if (ret >= B_OK) {
                                    sprintf(fAppPath, "%s", path.Path());
                                } else {
                                    fprintf(stderr, "getting app parent folder failed: %s\n", strerror(ret));
                                }
                            } else {
                                fprintf(stderr, "making app path failed: %s\n", strerror(ret));
                            }
                        } else {
                            fprintf(stderr, "GetAppInfo() failed: %s\n", strerror(ret));
                        }
                    }
                    ~platform_specific()
                    {
                        for (int32 i = 0; i < agg::platform_support::max_images; i++)
                            delete fImages[i];
                        delete fApp;
                    }

    bool            Init(int width, int height, unsigned flags)
                    {
                        return fApp->Init(fAGG, width, height, fFormat, fFlipY, flags);
                    }

    int             Run()
                    {
                        status_t ret = B_NO_INIT;
                        if (fApp) {
                            fApp->Run();
                            ret = B_OK;
                        }
                        return ret;
                    }

    void            SetTitle(const char* title)
                    {
                        if (fApp && fApp->Window() && fApp->Window()->Lock()) {
                            fApp->Window()->SetTitle(title);
                            fApp->Window()->Unlock();
                        }
                    }
    void            StartTimer()
                    {
                        fTimerStart = system_time();
                    }
    double          ElapsedTime() const
                    {
                        return (system_time() - fTimerStart) / 1000.0;
                    }

    void            ForceRedraw()
                    {
                        fApp->Window()->View()->ForceRedraw();
                    }
    void            UpdateWindow()
                    {
                        fApp->Window()->View()->Update();
                    }


    agg::platform_support*  fAGG;
    AGGApplication*     fApp;
    agg::pix_format_e    fFormat;
    bool                fFlipY;
    bigtime_t           fTimerStart;
    BBitmap*            fImages[agg::platform_support::max_images];

    char                fAppPath[B_PATH_NAME_LENGTH];
    char                fFilePath[B_PATH_NAME_LENGTH];
};


    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(this, format, flip_y)),
        m_format(format),
        m_bpp(32/*m_specific->m_bpp*/),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }

    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        m_specific->SetTitle(cap);
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        m_specific->StartTimer();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        return m_specific->ElapsedTime();
    }

    //------------------------------------------------------------------------
    void* platform_support::raw_display_handler()
    {
        // TODO: if we ever support BDirectWindow here, that would
        // be the frame buffer pointer with offset to the window top left
        return NULL;
    }

    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        BAlert* alert = new BAlert("AGG Message", msg, "Ok");
        alert->Go(/*NULL*/);
    }


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        m_initial_width = width;
        m_initial_height = height;
        m_window_flags = flags;

        if (m_specific->Init(width, height, flags)) {
            on_init();
            return true;
        }

        return false;
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
        return m_specific->Run();
    }


    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".ppm"; }


    const char* platform_support::full_file_name(const char* file_name)
    {
        sprintf(m_specific->fFilePath, "%s/%s", m_specific->fAppPath, file_name);
        return m_specific->fFilePath;
    }


    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if (idx < max_images)
        {
            char path[B_PATH_NAME_LENGTH];
            sprintf(path, "%s/%s%s", m_specific->fAppPath, file, img_ext());
            BBitmap* transBitmap = BTranslationUtils::GetBitmap(path);
            if (transBitmap && transBitmap->IsValid()) {
                if(transBitmap->ColorSpace() != B_RGB32 && transBitmap->ColorSpace() != B_RGBA32) {
                    // ups we got a smart ass Translator making our live harder
                    delete transBitmap;
                    return false;
                }

                color_space format = B_RGB24;

                switch (m_format) {
                    case pix_format_gray8:
                        format = B_GRAY8;
                        break;
                    case pix_format_rgb555:
                        format = B_RGB15;
                        break;
                    case pix_format_rgb565:
                        format = B_RGB16;
                        break;
                    case pix_format_rgb24:
                        format = B_RGB24_BIG;
                        break;
                    case pix_format_bgr24:
                        format = B_RGB24;
                        break;
                    case pix_format_abgr32:
                    case pix_format_argb32:
                    case pix_format_bgra32:
                        format = B_RGB32;
                        break;
                    case pix_format_rgba32:
                        format = B_RGB32_BIG;
                        break;
                }
                BBitmap* bitmap = new (nothrow) BBitmap(transBitmap->Bounds(), 0, format);
                if (!bitmap || !bitmap->IsValid()) {
                    fprintf(stderr, "failed to allocate temporary bitmap!\n");
                    delete transBitmap;
                    delete bitmap;
                    return false;
                }

                delete m_specific->fImages[idx];

                rendering_buffer rbuf_tmp;
                attach_buffer_to_BBitmap(rbuf_tmp, transBitmap, m_flip_y);
        
                m_specific->fImages[idx] = bitmap;
        
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
        
                rendering_buffer* dst = &m_rbuf_img[idx];

                switch(m_format)
                {
                case pix_format_gray8:
                    return false;
//                  color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray8()); break;
                    break;
        
                case pix_format_rgb555:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb555()); break;
                    break;
        
                case pix_format_rgb565:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb565()); break;
                    break;
        
                case pix_format_rgb24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb24()); break;
                    break;
        
                case pix_format_bgr24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr24()); break;
                    break;
        
                case pix_format_abgr32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr32()); break;
                    break;
        
                case pix_format_argb32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb32()); break;
                    break;
        
                case pix_format_bgra32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra32()); break;
                    break;
        
                case pix_format_rgba32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba32()); break;
                    break;
                }
                delete transBitmap;
                
                return true;

            } else {
                fprintf(stderr, "failed to load bitmap: '%s'\n", full_file_name(file));
            }
        }
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        // TODO: implement using BTranslatorRoster and friends
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerWidth() + 1;
            if(height == 0) height = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerHeight() + 1;
            BBitmap* bitmap = new BBitmap(BRect(0.0, 0.0, width - 1, height - 1), 0, B_RGBA32);;
            if (bitmap && bitmap->IsValid()) {
                delete m_specific->fImages[idx];
                m_specific->fImages[idx] = bitmap;
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
                return true;
            } else {
                delete bitmap;
            }
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->ForceRedraw();
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        m_specific->UpdateWindow();
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}
}






//----------------------------------------------------------------------------
int agg_main(int argc, char* argv[]);



int
main(int argc, char* argv[])
{
    return agg_main(argc, argv);
}


#endif

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// class platform_support. SDL version.
//
//----------------------------------------------------------------------------

#ifdef USE_SDL

#include <string.h>
#include <agg24/agg_platform_support.h>
#include "SDL.h"
#include "SDL_byteorder.h"


namespace agg
{

    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);
        ~platform_specific();

        pix_format_e  m_format;
        pix_format_e  m_sys_format;
        bool          m_flip_y;
        unsigned      m_bpp;
        unsigned      m_sys_bpp;
        unsigned      m_rmask;
        unsigned      m_gmask;
        unsigned      m_bmask;
        unsigned      m_amask;
        bool          m_update_flag;
        bool          m_resize_flag;
        bool          m_initialized;
        SDL_Surface*  m_surf_screen;
        SDL_Surface*  m_surf_window;
        SDL_Surface*  m_surf_img[platform_support::max_images];
        int           m_cur_x;
        int           m_cur_y;
	int          m_sw_start;
    };



    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
        m_format(format),
        m_sys_format(pix_format_undefined),
        m_flip_y(flip_y),
        m_bpp(0),
        m_sys_bpp(0),
        m_update_flag(true), 
        m_resize_flag(true),
        m_initialized(false),
        m_surf_screen(0),
        m_surf_window(0),
        m_cur_x(0),
        m_cur_y(0)
    {
        memset(m_surf_img, 0, sizeof(m_surf_img));

        switch(m_format)
        {
			case pix_format_gray8:
            m_bpp = 8;
            break;

        case pix_format_rgb565:
            m_rmask = 0xF800;
            m_gmask = 0x7E0;
            m_bmask = 0x1F;
            m_amask = 0;
            m_bpp = 16;
            break;

        case pix_format_rgb555:
            m_rmask = 0x7C00;
            m_gmask = 0x3E0;
            m_bmask = 0x1F;
            m_amask = 0;
            m_bpp = 16;
            break;
			
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        case pix_format_rgb24:
			m_rmask = 0xFF;
            m_gmask = 0xFF00;
            m_bmask = 0xFF0000;
            m_amask = 0;
            m_bpp = 24;
            break;

        case pix_format_bgr24:
            m_rmask = 0xFF0000;
            m_gmask = 0xFF00;
            m_bmask = 0xFF;
            m_amask = 0;
            m_bpp = 24;
            break;

        case pix_format_bgra32:
            m_rmask = 0xFF0000;
            m_gmask = 0xFF00;
            m_bmask = 0xFF;
            m_amask = 0xFF000000;
            m_bpp = 32;
            break;

        case pix_format_abgr32:
            m_rmask = 0xFF000000;
            m_gmask = 0xFF0000;
            m_bmask = 0xFF00;
            m_amask = 0xFF;
            m_bpp = 32;
            break;

        case pix_format_argb32:
            m_rmask = 0xFF00;
            m_gmask = 0xFF0000;
            m_bmask = 0xFF000000;
            m_amask = 0xFF;
            m_bpp = 32;
            break;

        case pix_format_rgba32:
            m_rmask = 0xFF;
            m_gmask = 0xFF00;
            m_bmask = 0xFF0000;
            m_amask = 0xFF000000;
            m_bpp = 32;
            break;
#else //SDL_BIG_ENDIAN (PPC)
        case pix_format_rgb24:
			m_rmask = 0xFF0000;
            m_gmask = 0xFF00;
            m_bmask = 0xFF;
            m_amask = 0;
            m_bpp = 24;
            break;

        case pix_format_bgr24:
            m_rmask = 0xFF;
            m_gmask = 0xFF00;
            m_bmask = 0xFF0000;
            m_amask = 0;
            m_bpp = 24;
            break;

        case pix_format_bgra32:
            m_rmask = 0xFF00;
            m_gmask = 0xFF0000;
            m_bmask = 0xFF000000;
            m_amask = 0xFF;
            m_bpp = 32;
            break;

        case pix_format_abgr32:
            m_rmask = 0xFF;
            m_gmask = 0xFF00;
            m_bmask = 0xFF0000;
            m_amask = 0xFF000000;
            m_bpp = 32;
            break;

        case pix_format_argb32:
            m_rmask = 0xFF0000;
            m_gmask = 0xFF00;
            m_bmask = 0xFF;
            m_amask = 0xFF000000;
            m_bpp = 32;
            break;

        case pix_format_rgba32:
            m_rmask = 0xFF000000;
            m_gmask = 0xFF0000;
            m_bmask = 0xFF00;
            m_amask = 0xFF;
            m_bpp = 32;
            break;
#endif
        }
    }

    //------------------------------------------------------------------------
    platform_specific::~platform_specific()
    {
        int i;
        for(i = platform_support::max_images - 1; i >= 0; --i)
        {
            if(m_surf_img[i]) SDL_FreeSurface(m_surf_img[i]);
        }
        if(m_surf_window) SDL_FreeSurface(m_surf_window);
        if(m_surf_screen) SDL_FreeSurface(m_surf_screen);
    }



    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(format, flip_y)),
        m_format(format),
        m_bpp(m_specific->m_bpp),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y)
    {
        SDL_Init(SDL_INIT_VIDEO);
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }



    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        if(m_specific->m_initialized)
        {
            SDL_WM_SetCaption(cap, 0);
        }
    }
    




    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        m_window_flags = flags;
        unsigned wflags = SDL_SWSURFACE;

        if(m_window_flags & window_hw_buffer)
        {
            wflags = SDL_HWSURFACE;
        }

        if(m_window_flags & window_resize)
        {
            wflags |= SDL_RESIZABLE;
        }

        if(m_specific->m_surf_screen) SDL_FreeSurface(m_specific->m_surf_screen);

        m_specific->m_surf_screen = SDL_SetVideoMode(width, height, m_bpp, wflags);
        if(m_specific->m_surf_screen == 0) 
        {
            fprintf(stderr, 
                    "Unable to set %dx%d %d bpp video: %s\n", 
                    width, 
                    height, 
                    m_bpp, 
                    ::SDL_GetError());
            return false;
        }

        SDL_WM_SetCaption(m_caption, 0);

        if(m_specific->m_surf_window) SDL_FreeSurface(m_specific->m_surf_window);

        m_specific->m_surf_window = 
            SDL_CreateRGBSurface(SDL_HWSURFACE, 
                                 m_specific->m_surf_screen->w, 
                                 m_specific->m_surf_screen->h,
                                 m_specific->m_surf_screen->format->BitsPerPixel,
                                 m_specific->m_rmask, 
                                 m_specific->m_gmask, 
                                 m_specific->m_bmask, 
                                 m_specific->m_amask);

        if(m_specific->m_surf_window == 0) 
        {
            fprintf(stderr, 
                    "Unable to create image buffer %dx%d %d bpp: %s\n", 
                    width, 
                    height, 
                    m_bpp, 
                    SDL_GetError());
            return false;
        }

        m_rbuf_window.attach((unsigned char*)m_specific->m_surf_window->pixels, 
                             m_specific->m_surf_window->w, 
                             m_specific->m_surf_window->h, 
                             m_flip_y ? -m_specific->m_surf_window->pitch : 
                                         m_specific->m_surf_window->pitch);

        if(!m_specific->m_initialized)
        {
            m_initial_width = width;
            m_initial_height = height;
            on_init();
            m_specific->m_initialized = true;
        }
        on_resize(m_rbuf_window.width(), m_rbuf_window.height());
        m_specific->m_update_flag = true;
        return true;
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        SDL_BlitSurface(m_specific->m_surf_window, 0, m_specific->m_surf_screen, 0);
        SDL_UpdateRect(m_specific->m_surf_screen, 0, 0, 0, 0);
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
        SDL_Event event;
        bool ev_flag = false;

        for(;;)
        {
            if(m_specific->m_update_flag)
            {
                on_draw();
                update_window();
                m_specific->m_update_flag = false;
            }

            ev_flag = false;
            if(m_wait_mode)
            {
                SDL_WaitEvent(&event);
                ev_flag = true;
            }
            else
            {
                if(SDL_PollEvent(&event))
                {
                    ev_flag = true;
                }
                else
                {
                    on_idle();
                }
            }

            if(ev_flag)
            {
                if(event.type == SDL_QUIT)
                {
                    break;
                }

                int y;
                unsigned flags = 0;

                switch (event.type) 
                {
                case SDL_VIDEORESIZE:
                    if(!init(event.resize.w, event.resize.h, m_window_flags)) return false;
                    on_resize(m_rbuf_window.width(), m_rbuf_window.height());
                    trans_affine_resizing(event.resize.w, event.resize.h);
                    m_specific->m_update_flag = true;
                    break;

                case SDL_KEYDOWN:
                    {
                        flags = 0;
                        if(event.key.keysym.mod & KMOD_SHIFT) flags |= kbd_shift;
                        if(event.key.keysym.mod & KMOD_CTRL)  flags |= kbd_ctrl;

                        bool left  = false;
                        bool up    = false;
                        bool right = false;
                        bool down  = false;

                        switch(event.key.keysym.sym)
                        {
                        case key_left:
                            left = true;
                            break;

                        case key_up:
                            up = true;
                            break;

                        case key_right:
                            right = true;
                            break;

                        case key_down:
                            down = true;
                            break;
                        }

                        if(m_ctrls.on_arrow_keys(left, right, down, up))
                        {
                            on_ctrl_change();
                            force_redraw();
                        }
                        else
                        {
                            on_key(m_specific->m_cur_x,
                                   m_specific->m_cur_y,
                                   event.key.keysym.sym,
                                   flags);
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    y = m_flip_y ? 
                        m_rbuf_window.height() - event.motion.y : 
                        event.motion.y;

                    m_specific->m_cur_x = event.motion.x;
                    m_specific->m_cur_y = y;
                    flags = 0;
                    if(event.motion.state & SDL_BUTTON_LMASK) flags |= mouse_left;
                    if(event.motion.state & SDL_BUTTON_RMASK) flags |= mouse_right;

                    if(m_ctrls.on_mouse_move(m_specific->m_cur_x, 
                                             m_specific->m_cur_y,
                                             (flags & mouse_left) != 0))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    else
                    {
                        on_mouse_move(m_specific->m_cur_x, 
                                      m_specific->m_cur_y, 
                                      flags);
                    }
		    SDL_Event eventtrash;
		    while (SDL_PeepEvents(&eventtrash, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEMOTION))!=0){;}
                    break;

		case SDL_MOUSEBUTTONDOWN:
                    y = m_flip_y
                        ? m_rbuf_window.height() - event.button.y
                        : event.button.y;

                    m_specific->m_cur_x = event.button.x;
                    m_specific->m_cur_y = y;
                    flags = 0;
                    switch(event.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        {
                            flags = mouse_left;

if(m_ctrls.on_mouse_button_down(m_specific->m_cur_x,
                                m_specific->m_cur_y))
                            {
                                m_ctrls.set_cur(m_specific->m_cur_x, 
                                    m_specific->m_cur_y);
                                on_ctrl_change();
                                force_redraw();
                            }
                            else
                            {
                                if(m_ctrls.in_rect(m_specific->m_cur_x, 
                                    m_specific->m_cur_y))
                                {
                                    if(m_ctrls.set_cur(m_specific->m_cur_x, 
                                        m_specific->m_cur_y))
                                    {
                                        on_ctrl_change();
                                        force_redraw();
                                    }
                                }
                                else
                                {
                                    on_mouse_button_down(m_specific->m_cur_x, 
                                        m_specific->m_cur_y, 
                                        flags);
                                }
                            }
                        }
                        break;
                    case SDL_BUTTON_RIGHT:
                        flags = mouse_right;
                        on_mouse_button_down(m_specific->m_cur_x, 
                            m_specific->m_cur_y, 
                            flags);
                        break;
                    } //switch(event.button.button)
                    break;
		    
                case SDL_MOUSEBUTTONUP:
                    y = m_flip_y
                        ? m_rbuf_window.height() - event.button.y
                        : event.button.y;

                    m_specific->m_cur_x = event.button.x;
                    m_specific->m_cur_y = y;
                    flags = 0;
                    if(m_ctrls.on_mouse_button_up(m_specific->m_cur_x, 
                                                  m_specific->m_cur_y))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    on_mouse_button_up(m_specific->m_cur_x, 
                                       m_specific->m_cur_y, 
                                       flags);
                    break;
                }
            }
        }
        return 0;
    }



    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".bmp"; }

    //------------------------------------------------------------------------
    const char* platform_support::full_file_name(const char* file_name)
    {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            if(m_specific->m_surf_img[idx]) SDL_FreeSurface(m_specific->m_surf_img[idx]);

            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
            if(len < 4 || strcmp(fn + len - 4, ".bmp") != 0)
            {
                strcat(fn, ".bmp");
            }

            SDL_Surface* tmp_surf = SDL_LoadBMP(fn);
            if (tmp_surf == 0) 
            {
                fprintf(stderr, "Couldn't load %s: %s\n", fn, SDL_GetError());
                return false;
            }

            SDL_PixelFormat format;
            format.palette = 0;
            format.BitsPerPixel = m_bpp;
            format.BytesPerPixel = m_bpp >> 8;
            format.Rmask = m_specific->m_rmask;
            format.Gmask = m_specific->m_gmask;
            format.Bmask = m_specific->m_bmask;
            format.Amask = m_specific->m_amask;
            format.Rshift = 0;
            format.Gshift = 0;
            format.Bshift = 0;
            format.Ashift = 0;
            format.Rloss = 0;
            format.Gloss = 0;
            format.Bloss = 0;
            format.Aloss = 0;
            format.colorkey = 0;
            format.alpha = 0;

            m_specific->m_surf_img[idx] = 
                SDL_ConvertSurface(tmp_surf, 
                                   &format, 
                                   SDL_SWSURFACE);

            SDL_FreeSurface(tmp_surf);
            
            if(m_specific->m_surf_img[idx] == 0) return false;

            m_rbuf_img[idx].attach((unsigned char*)m_specific->m_surf_img[idx]->pixels, 
                                   m_specific->m_surf_img[idx]->w, 
                                   m_specific->m_surf_img[idx]->h, 
                                   m_flip_y ? -m_specific->m_surf_img[idx]->pitch : 
                                               m_specific->m_surf_img[idx]->pitch);
            return true;

        }
        return false;
    }




    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        if(idx < max_images && m_specific->m_surf_img[idx])
        {
            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
            if(len < 4 || strcmp(fn + len - 4, ".bmp") != 0)
            {
                strcat(fn, ".bmp");
            }
            return SDL_SaveBMP(m_specific->m_surf_img[idx], fn) == 0;
        }
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {

            if(m_specific->m_surf_img[idx]) SDL_FreeSurface(m_specific->m_surf_img[idx]);

             m_specific->m_surf_img[idx] = 
                 SDL_CreateRGBSurface(SDL_SWSURFACE, 
                                      width, 
                                      height,
                                      m_specific->m_surf_screen->format->BitsPerPixel,
                                      m_specific->m_rmask, 
                                      m_specific->m_gmask, 
                                      m_specific->m_bmask, 
                                      m_specific->m_amask);
            if(m_specific->m_surf_img[idx] == 0) 
            {
                fprintf(stderr, "Couldn't create image: %s\n", SDL_GetError());
                return false;
            }

            m_rbuf_img[idx].attach((unsigned char*)m_specific->m_surf_img[idx]->pixels, 
                                   m_specific->m_surf_img[idx]->w, 
                                   m_specific->m_surf_img[idx]->h, 
                                   m_flip_y ? -m_specific->m_surf_img[idx]->pitch : 
                                               m_specific->m_surf_img[idx]->pitch);

            return true;
        }

        return false;
    }
    
    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        m_specific->m_sw_start = SDL_GetTicks();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        int stop = SDL_GetTicks();
        return double(stop - m_specific->m_sw_start);
    }

    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->m_update_flag = true;
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}


}


int agg_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    return agg_main(argc, argv);
}

#endif



//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: superstippi@gmx.de
//----------------------------------------------------------------------------
//
// class platform_support
//
//----------------------------------------------------------------------------

#ifdef PLATFORM_BEOS

#include <new>
#include <stdio.h>

#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <Path.h>
#include <Roster.h>
#include <TranslationUtils.h>
#include <View.h>
#include <Window.h>

#include <string.h>
#include <agg24/agg_platform_support.h>
#include <agg24/agg_color_conv_rgb8.h>

using std::nothrow;


static void
attach_buffer_to_BBitmap(agg::rendering_buffer& buffer, BBitmap* bitmap, bool flipY)
{
    uint8* bits = (uint8*)bitmap->Bits();
    uint32 width = bitmap->Bounds().IntegerWidth() + 1;
    uint32 height = bitmap->Bounds().IntegerHeight() + 1;
    int32 bpr = bitmap->BytesPerRow();
    if (flipY) {
// XXX: why don't I have to do this?!?
//        bits += bpr * (height - 1);
        bpr = -bpr;
    }
    buffer.attach(bits, width, height, bpr);
}


static color_space
pix_format_to_color_space(agg::pix_format_e format)
{
    color_space bitmapFormat = B_NO_COLOR_SPACE;
    switch (format) {
        case agg::pix_format_rgb555:

            bitmapFormat = B_RGB15;
            break;

        case agg::pix_format_rgb565:

            bitmapFormat = B_RGB16;
            break;

        case agg::pix_format_rgb24:
        case agg::pix_format_bgr24:

            bitmapFormat = B_RGB24;
            break;

        case agg::pix_format_rgba32:
        case agg::pix_format_argb32:
        case agg::pix_format_abgr32:
        case agg::pix_format_bgra32:

            bitmapFormat = B_RGBA32;
            break;
    }
    return bitmapFormat;
}


// #pragma mark -


class AGGView : public BView {
 public:
                            AGGView(BRect frame, agg::platform_support* agg,
                                    agg::pix_format_e format, bool flipY);
    virtual                 ~AGGView();

    virtual void            AttachedToWindow();
    virtual void            DetachedFromWindow();

    virtual void            MessageReceived(BMessage* message);
    virtual void            Draw(BRect updateRect);
    virtual void            FrameResized(float width, float height);

    virtual void            KeyDown(const char* bytes, int32 numBytes);

    virtual void            MouseDown(BPoint where);
    virtual void            MouseMoved(BPoint where, uint32 transit,
                               const BMessage* dragMesage);
    virtual void            MouseUp(BPoint where);

            BBitmap*        Bitmap() const;

            uint8           LastKeyDown() const;
            uint32          MouseButtons();

            void            Update();
            void            ForceRedraw();

            unsigned        GetKeyFlags();

 private:
    BBitmap*                fBitmap;
    agg::pix_format_e       fFormat;
    bool                    fFlipY;

    agg::platform_support*  fAGG;

    uint32                  fMouseButtons;
    int32                   fMouseX;
    int32                   fMouseY;

    uint8                   fLastKeyDown;

    bool                    fRedraw;

    BMessageRunner*         fPulse;
    bigtime_t               fLastPulse;
    bool                    fEnableTicks;
};

AGGView::AGGView(BRect frame,
                 agg::platform_support* agg,
                 agg::pix_format_e format,
                 bool flipY)
    : BView(frame, "AGG View", B_FOLLOW_ALL,
            B_FRAME_EVENTS | B_WILL_DRAW),
      fFormat(format),
      fFlipY(flipY),

      fAGG(agg),

      fMouseButtons(0),
      fMouseX(-1),
      fMouseY(-1),
      
      fLastKeyDown(0),

      fRedraw(true),

      fPulse(NULL),
      fLastPulse(0),
      fEnableTicks(true)
{
    SetViewColor(B_TRANSPARENT_32_BIT);
    
    frame.OffsetTo(0.0, 0.0);
    fBitmap = new BBitmap(frame, 0, pix_format_to_color_space(fFormat));
    if (fBitmap->IsValid()) {
        attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);
    } else {
        delete fBitmap;
        fBitmap = NULL;
    }
}


AGGView::~AGGView()
{
    delete fBitmap;
    delete fPulse;
}


void
AGGView::AttachedToWindow()
{
    BMessage message('tick');
    BMessenger target(this, Looper());
    delete fPulse;
//    BScreen screen;
//    TODO: calc screen retrace
    fPulse = new BMessageRunner(target, &message, 40000);

    // make sure we call this once
    fAGG->on_resize(Bounds().IntegerWidth() + 1,
                    Bounds().IntegerHeight() + 1);
    MakeFocus();
}


void
AGGView::DetachedFromWindow()
{
    delete fPulse;
    fPulse = NULL;
}


void
AGGView::MessageReceived(BMessage* message)
{
    bigtime_t now = system_time();
    switch (message->what) {
        case 'tick':
            // drop messages that have piled up
            if (/*now - fLastPulse > 30000*/fEnableTicks) {
                fLastPulse = now;
                if (!fAGG->wait_mode())
                    fAGG->on_idle();
                Window()->PostMessage('entk', this);
                fEnableTicks = false;
            } else {
//                printf("dropping tick message (%lld)\n", now - fLastPulse);
            }
            break;
        case 'entk':
            fEnableTicks = true;
            if (now - fLastPulse > 30000) {
                fLastPulse = now;
                if (!fAGG->wait_mode())
                    fAGG->on_idle();
            }
            break;
        default:
            BView::MessageReceived(message);
            break;
    }
}


void
AGGView::Draw(BRect updateRect)
{
    if (fBitmap) {
        if (fRedraw) {
            fAGG->on_draw();
            fRedraw = false;
        }
        if (fFormat == agg::pix_format_bgra32) {
            DrawBitmap(fBitmap, updateRect, updateRect);
        } else {
            BBitmap* bitmap = new BBitmap(fBitmap->Bounds(), 0, B_RGBA32);

            agg::rendering_buffer rbufSrc;
            attach_buffer_to_BBitmap(rbufSrc, fBitmap, false);

            agg::rendering_buffer rbufDst;
            attach_buffer_to_BBitmap(rbufDst, bitmap, false);

            switch(fFormat) {
                case agg::pix_format_rgb555:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb555_to_bgra32());
                    break;
                case agg::pix_format_rgb565:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb565_to_bgra32());
                    break;
                case agg::pix_format_rgb24:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgb24_to_bgra32());
                    break;
                case agg::pix_format_bgr24:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_bgr24_to_bgra32());
                    break;
                case agg::pix_format_rgba32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_rgba32_to_bgra32());
                    break;
                case agg::pix_format_argb32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_argb32_to_bgra32());
                    break;
                case agg::pix_format_abgr32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_abgr32_to_bgra32());
                    break;
                case agg::pix_format_bgra32:
                    agg::color_conv(&rbufDst, &rbufSrc,
                                    agg::color_conv_bgra32_to_bgra32());
                    break;
            }
            DrawBitmap(bitmap, updateRect, updateRect);
            delete bitmap;
        }
    } else {
        FillRect(updateRect);
    }
}


void
AGGView::FrameResized(float width, float height)
{
    BRect r(0.0, 0.0, width, height);
    BBitmap* bitmap = new BBitmap(r, 0, pix_format_to_color_space(fFormat));
    if (bitmap->IsValid()) {
        delete fBitmap;
        fBitmap = bitmap;
           attach_buffer_to_BBitmap(fAGG->rbuf_window(), fBitmap, fFlipY);

        fAGG->trans_affine_resizing((int)width + 1,
                                    (int)height + 1);

        // pass the event on to AGG
        fAGG->on_resize((int)width + 1, (int)height + 1);
        
        fRedraw = true;
        Invalidate();
    } else
        delete bitmap;
}


void
AGGView::KeyDown(const char* bytes, int32 numBytes)
{
    if (bytes && numBytes > 0) {
        fLastKeyDown = bytes[0];

        bool left  = false;
        bool up    = false;
        bool right = false;
        bool down  = false;

        switch (fLastKeyDown) {

            case B_LEFT_ARROW:
                left = true;
                break;

            case B_UP_ARROW:
                up = true;
                break;

            case B_RIGHT_ARROW:
                right = true;
                break;

            case B_DOWN_ARROW:
                down = true;
                break;
        }

/*            case key_f2:                        
fAGG->copy_window_to_img(agg::platform_support::max_images - 1);
fAGG->save_img(agg::platform_support::max_images - 1, "screenshot");
break;
}*/


        if (fAGG->m_ctrls.on_arrow_keys(left, right, down, up)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        } else {
            fAGG->on_key(fMouseX, fMouseY, fLastKeyDown, GetKeyFlags());
        }
//        fAGG->on_key(fMouseX, fMouseY, fLastKeyDown, GetKeyFlags());

    }
}


void
AGGView::MouseDown(BPoint where)
{
    BMessage* currentMessage = Window()->CurrentMessage();
    if (currentMessage) {
        if (currentMessage->FindInt32("buttons", (int32*)&fMouseButtons) < B_OK)
            fMouseButtons = B_PRIMARY_MOUSE_BUTTON;
    } else
        fMouseButtons = B_PRIMARY_MOUSE_BUTTON;

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        // left mouse button -> see if to handle in controls
        fAGG->m_ctrls.set_cur(fMouseX, fMouseY);
        if (fAGG->m_ctrls.on_mouse_button_down(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        } else {
            if (fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
                if (fAGG->m_ctrls.set_cur(fMouseX, fMouseY)) {
                    fAGG->on_ctrl_change();
                    fAGG->force_redraw();
                }
            } else {
                fAGG->on_mouse_button_down(fMouseX, fMouseY, GetKeyFlags());
            }
        }
    } else if (fMouseButtons & B_SECONDARY_MOUSE_BUTTON) {
        // right mouse button -> simple
        fAGG->on_mouse_button_down(fMouseX, fMouseY, GetKeyFlags());
    }
    SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}


void
AGGView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMesage)
{
    // workarround missed mouse up events
    // (if we react too slowly, app_server might have dropped events)
    BMessage* currentMessage = Window()->CurrentMessage();
    int32 buttons = 0;
    if (currentMessage->FindInt32("buttons", &buttons) < B_OK) {
        buttons = 0;
    }
    if (!buttons)
        MouseUp(where);

    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fAGG->m_ctrls.on_mouse_move(fMouseX, fMouseY,
                                    (GetKeyFlags() & agg::mouse_left) != 0)) {
        fAGG->on_ctrl_change();
        fAGG->force_redraw();
    } else {
        if (!fAGG->m_ctrls.in_rect(fMouseX, fMouseY)) {
            fAGG->on_mouse_move(fMouseX, fMouseY, GetKeyFlags());
        }
    }
}


void
AGGView::MouseUp(BPoint where)
{
    fMouseX = (int)where.x;
    fMouseY = fFlipY ? (int)(Bounds().Height() - where.y) : (int)where.y;

    // pass the event on to AGG
    if (fMouseButtons == B_PRIMARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        if (fAGG->m_ctrls.on_mouse_button_up(fMouseX, fMouseY)) {
            fAGG->on_ctrl_change();
            fAGG->force_redraw();
        }
        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetKeyFlags());
    } else if (fMouseButtons == B_SECONDARY_MOUSE_BUTTON) {
        fMouseButtons = 0;

        fAGG->on_mouse_button_up(fMouseX, fMouseY, GetKeyFlags());
    }
}


BBitmap*
AGGView::Bitmap() const
{
    return fBitmap;
}


uint8
AGGView::LastKeyDown() const
{
    return fLastKeyDown;
}


uint32
AGGView::MouseButtons()
{
    uint32 buttons = 0;
    if (LockLooper()) {
        buttons = fMouseButtons;
        UnlockLooper();
    }
    return buttons;
}


void
AGGView::Update()
{
    // trigger display update
    if (LockLooper()) {
        Invalidate();
        UnlockLooper();
    }
}


void
AGGView::ForceRedraw()
{
    // force a redraw (fRedraw = true;)
    // and trigger display update
    if (LockLooper()) {
        fRedraw = true;
        Invalidate();
        UnlockLooper();
    }
}


unsigned
AGGView::GetKeyFlags()
{
    uint32 buttons = fMouseButtons;
    uint32 mods = modifiers();
    unsigned flags = 0;
    if (buttons & B_PRIMARY_MOUSE_BUTTON)   flags |= agg::mouse_left;
    if (buttons & B_SECONDARY_MOUSE_BUTTON) flags |= agg::mouse_right;
    if (mods & B_SHIFT_KEY)                 flags |= agg::kbd_shift;
    if (mods & B_COMMAND_KEY)               flags |= agg::kbd_ctrl;
    return flags;
}

// #pragma mark -


class AGGWindow : public BWindow {
 public:
                    AGGWindow()
                    : BWindow(BRect(-50.0, -50.0, -10.0, -10.0),
                              "AGG Application", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
                    {
                    }

    virtual bool    QuitRequested()
                    {
                        be_app->PostMessage(B_QUIT_REQUESTED);
                        return true;
                    }

            bool    Init(BRect frame, agg::platform_support* agg, agg::pix_format_e format,
                              bool flipY, uint32 flags)
                    {
                        MoveTo(frame.LeftTop());
                        ResizeTo(frame.Width(), frame.Height());

                        SetFlags(flags);

                        frame.OffsetTo(0.0, 0.0);
                        fView = new AGGView(frame, agg, format, flipY);
                        AddChild(fView);

                        return fView->Bitmap() != NULL;
                    }


        AGGView*    View() const
                    {
                        return fView;
                    }
 private:
    AGGView*        fView;
};

// #pragma mark -


class AGGApplication : public BApplication {
 public:
                    AGGApplication()
                    : BApplication("application/x-vnd.AGG-AGG")
                    {
                        fWindow = new AGGWindow();
                    }

    virtual void    ReadyToRun()
                    {
                        if (fWindow) {
                            fWindow->Show();
                        }
                    }

    virtual bool    Init(agg::platform_support* agg, int width, int height,
                         agg::pix_format_e format, bool flipY, uint32 flags)
                    {
                        BRect r(50.0, 50.0,
                                50.0 + width - 1.0,
                                50.0 + height - 1.0);
                        uint32 windowFlags = B_ASYNCHRONOUS_CONTROLS;
                        if (!(flags & agg::window_resize))
                            windowFlags |= B_NOT_RESIZABLE;

                        return fWindow->Init(r, agg, format, flipY, windowFlags);;
                    }


        AGGWindow*  Window() const
                    {
                        return fWindow;
                    }

 private:
    AGGWindow*      fWindow;
};


// #pragma mark -


namespace agg
{

class platform_specific {
 public:
                    platform_specific(agg::platform_support* agg,
                                      agg::pix_format_e format, bool flip_y)
                        : fAGG(agg),
                          fApp(NULL),
                          fFormat(format),
                          fFlipY(flip_y),
                          fTimerStart(system_time())
                    {
                        memset(fImages, 0, sizeof(fImages));
                        fApp = new AGGApplication();
                        fAppPath[0] = 0;
                        // figure out where we're running from
                        app_info info;
                        status_t ret = fApp->GetAppInfo(&info);
                        if (ret >= B_OK) {
                            BPath path(&info.ref);
                            ret = path.InitCheck();
                            if (ret >= B_OK) {
                                ret = path.GetParent(&path);
                                if (ret >= B_OK) {
                                    sprintf(fAppPath, "%s", path.Path());
                                } else {
                                    fprintf(stderr, "getting app parent folder failed: %s\n", strerror(ret));
                                }
                            } else {
                                fprintf(stderr, "making app path failed: %s\n", strerror(ret));
                            }
                        } else {
                            fprintf(stderr, "GetAppInfo() failed: %s\n", strerror(ret));
                        }
                    }
                    ~platform_specific()
                    {
                        for (int32 i = 0; i < agg::platform_support::max_images; i++)
                            delete fImages[i];
                        delete fApp;
                    }

    bool            Init(int width, int height, unsigned flags)
                    {
                        return fApp->Init(fAGG, width, height, fFormat, fFlipY, flags);
                    }

    int             Run()
                    {
                        status_t ret = B_NO_INIT;
                        if (fApp) {
                            fApp->Run();
                            ret = B_OK;
                        }
                        return ret;
                    }

    void            SetTitle(const char* title)
                    {
                        if (fApp && fApp->Window() && fApp->Window()->Lock()) {
                            fApp->Window()->SetTitle(title);
                            fApp->Window()->Unlock();
                        }
                    }
    void            StartTimer()
                    {
                        fTimerStart = system_time();
                    }
    double          ElapsedTime() const
                    {
                        return (system_time() - fTimerStart) / 1000.0;
                    }

    void            ForceRedraw()
                    {
                        fApp->Window()->View()->ForceRedraw();
                    }
    void            UpdateWindow()
                    {
                        fApp->Window()->View()->Update();
                    }


    agg::platform_support*  fAGG;
    AGGApplication*     fApp;
    agg::pix_format_e    fFormat;
    bool                fFlipY;
    bigtime_t           fTimerStart;
    BBitmap*            fImages[agg::platform_support::max_images];

    char                fAppPath[B_PATH_NAME_LENGTH];
    char                fFilePath[B_PATH_NAME_LENGTH];
};


    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(this, format, flip_y)),
        m_format(format),
        m_bpp(32/*m_specific->m_bpp*/),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }

    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        m_specific->SetTitle(cap);
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        m_specific->StartTimer();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        return m_specific->ElapsedTime();
    }

    //------------------------------------------------------------------------
    void* platform_support::raw_display_handler()
    {
        // TODO: if we ever support BDirectWindow here, that would
        // be the frame buffer pointer with offset to the window top left
        return NULL;
    }

    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        BAlert* alert = new BAlert("AGG Message", msg, "Ok");
        alert->Go(/*NULL*/);
    }


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        m_initial_width = width;
        m_initial_height = height;
        m_window_flags = flags;

        if (m_specific->Init(width, height, flags)) {
            on_init();
            return true;
        }

        return false;
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
        return m_specific->Run();
    }


    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".ppm"; }


    const char* platform_support::full_file_name(const char* file_name)
    {
        sprintf(m_specific->fFilePath, "%s/%s", m_specific->fAppPath, file_name);
        return m_specific->fFilePath;
    }


    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if (idx < max_images)
        {
            char path[B_PATH_NAME_LENGTH];
            sprintf(path, "%s/%s%s", m_specific->fAppPath, file, img_ext());
            BBitmap* transBitmap = BTranslationUtils::GetBitmap(path);
            if (transBitmap && transBitmap->IsValid()) {
                if(transBitmap->ColorSpace() != B_RGB32 && transBitmap->ColorSpace() != B_RGBA32) {
                    // ups we got a smart ass Translator making our live harder
                    delete transBitmap;
                    return false;
                }

                color_space format = B_RGB24;

                switch (m_format) {
                    case pix_format_gray8:
                        format = B_GRAY8;
                        break;
                    case pix_format_rgb555:
                        format = B_RGB15;
                        break;
                    case pix_format_rgb565:
                        format = B_RGB16;
                        break;
                    case pix_format_rgb24:
                        format = B_RGB24_BIG;
                        break;
                    case pix_format_bgr24:
                        format = B_RGB24;
                        break;
                    case pix_format_abgr32:
                    case pix_format_argb32:
                    case pix_format_bgra32:
                        format = B_RGB32;
                        break;
                    case pix_format_rgba32:
                        format = B_RGB32_BIG;
                        break;
                }
                BBitmap* bitmap = new (nothrow) BBitmap(transBitmap->Bounds(), 0, format);
                if (!bitmap || !bitmap->IsValid()) {
                    fprintf(stderr, "failed to allocate temporary bitmap!\n");
                    delete transBitmap;
                    delete bitmap;
                    return false;
                }

                delete m_specific->fImages[idx];

                rendering_buffer rbuf_tmp;
                attach_buffer_to_BBitmap(rbuf_tmp, transBitmap, m_flip_y);
        
                m_specific->fImages[idx] = bitmap;
        
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
        
                rendering_buffer* dst = &m_rbuf_img[idx];

                switch(m_format)
                {
                case pix_format_gray8:
                    return false;
//                  color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_gray8()); break;
                    break;
        
                case pix_format_rgb555:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb555()); break;
                    break;
        
                case pix_format_rgb565:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb565()); break;
                    break;
        
                case pix_format_rgb24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgb24()); break;
                    break;
        
                case pix_format_bgr24:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgr24()); break;
                    break;
        
                case pix_format_abgr32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_abgr32()); break;
                    break;
        
                case pix_format_argb32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_argb32()); break;
                    break;
        
                case pix_format_bgra32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_bgra32()); break;
                    break;
        
                case pix_format_rgba32:
                    color_conv(dst, &rbuf_tmp, color_conv_bgra32_to_rgba32()); break;
                    break;
                }
                delete transBitmap;
                
                return true;

            } else {
                fprintf(stderr, "failed to load bitmap: '%s'\n", full_file_name(file));
            }
        }
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        // TODO: implement using BTranslatorRoster and friends
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerWidth() + 1;
            if(height == 0) height = m_specific->fApp->Window()->View()->Bitmap()->Bounds().IntegerHeight() + 1;
            BBitmap* bitmap = new BBitmap(BRect(0.0, 0.0, width - 1, height - 1), 0, B_RGBA32);;
            if (bitmap && bitmap->IsValid()) {
                delete m_specific->fImages[idx];
                m_specific->fImages[idx] = bitmap;
                attach_buffer_to_BBitmap(m_rbuf_img[idx], bitmap, m_flip_y);
                return true;
            } else {
                delete bitmap;
            }
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->ForceRedraw();
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        m_specific->UpdateWindow();
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}
}






//----------------------------------------------------------------------------
int agg_main(int argc, char* argv[]);



int
main(int argc, char* argv[])
{
    return agg_main(argc, argv);
}


#endif

//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4 
// Copyright (C) 2002-2005 Maxim Shemanarev (McSeem)
// Copyright (C) 2003 Hansruedi Baer (MacOS support)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//			baer@karto.baug.eth.ch
//----------------------------------------------------------------------------
//
// class platform_support
//
//----------------------------------------------------------------------------
//
// Note:
// I tried to retain the original structure for the Win32 platform as far
// as possible. Currently, not all features are implemented but the examples
// should work properly.
// HB
//----------------------------------------------------------------------------

#ifdef PLATFORM_OSX11

#include <Carbon.h>
#if defined(__MWERKS__)
#include "console.h"
#endif
#include <string.h>
#include <unistd.h>
#include <agg24/agg_platform_support.h>
#include <agg24/agg_mac_pmap.h>
#include <agg24/agg_color_conv_rgb8.h>


namespace agg
{
    
pascal OSStatus DoWindowClose (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoWindowDrawContent (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoAppQuit (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoMouseDown (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoMouseUp (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoMouseDragged (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoKeyDown (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal OSStatus DoKeyUp (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);
pascal void DoPeriodicTask (EventLoopTimerRef theTimer, void* userData);


    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);

        void create_pmap(unsigned width, unsigned height, 
                         rendering_buffer* wnd);

        void display_pmap(WindowRef window, const rendering_buffer* src);
        bool load_pmap(const char* fn, unsigned idx, 
                       rendering_buffer* dst);

        bool save_pmap(const char* fn, unsigned idx, 
                       const rendering_buffer* src);

        unsigned translate(unsigned keycode);

        pix_format_e     m_format;
        pix_format_e     m_sys_format;
        bool             m_flip_y;
        unsigned         m_bpp;
        unsigned         m_sys_bpp;
        WindowRef        m_window;
        pixel_map   	 m_pmap_window;
        pixel_map        m_pmap_img[platform_support::max_images];
        unsigned         m_keymap[256];
        unsigned         m_last_translated_key;
        int              m_cur_x;
        int              m_cur_y;
        unsigned         m_input_flags;
        bool             m_redraw_flag;
        UnsignedWide     m_sw_freq;
        UnsignedWide     m_sw_start;
    };


    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
       	m_format(format),
        m_sys_format(pix_format_undefined),
        m_flip_y(flip_y),
        m_bpp(0),
        m_sys_bpp(0),
        m_window(nil),
        m_last_translated_key(0),
        m_cur_x(0),
        m_cur_y(0),
        m_input_flags(0),
        m_redraw_flag(true)
    {
        memset(m_keymap, 0, sizeof(m_keymap));

        //Keyboard input is not yet fully supported nor tested
        //m_keymap[VK_PAUSE]       = key_pause;
        m_keymap[kClearCharCode]      = key_clear;

        //m_keymap[VK_NUMPAD0]    = key_kp0;
        //m_keymap[VK_NUMPAD1]    = key_kp1;
        //m_keymap[VK_NUMPAD2]    = key_kp2;
        //m_keymap[VK_NUMPAD3]    = key_kp3;
        //m_keymap[VK_NUMPAD4]    = key_kp4;
        //m_keymap[VK_NUMPAD5]    = key_kp5;
        //m_keymap[VK_NUMPAD6]    = key_kp6;
        //m_keymap[VK_NUMPAD7]    = key_kp7;
        //m_keymap[VK_NUMPAD8]    = key_kp8;
        //m_keymap[VK_NUMPAD9]    = key_kp9;
        //m_keymap[VK_DECIMAL]    = key_kp_period;
        //m_keymap[VK_DIVIDE]     = key_kp_divide;
        //m_keymap[VK_MULTIPLY]   = key_kp_multiply;
        //m_keymap[VK_SUBTRACT]   = key_kp_minus;
        //m_keymap[VK_ADD]        = key_kp_plus;

        m_keymap[kUpArrowCharCode]    = key_up;
        m_keymap[kDownArrowCharCode]  = key_down;
        m_keymap[kRightArrowCharCode] = key_right;
        m_keymap[kLeftArrowCharCode]  = key_left;
        //m_keymap[VK_INSERT]     = key_insert;
        m_keymap[kDeleteCharCode]     = key_delete;
        m_keymap[kHomeCharCode]       = key_home;
        m_keymap[kEndCharCode]        = key_end;
        m_keymap[kPageUpCharCode]     = key_page_up;
        m_keymap[kPageDownCharCode]   = key_page_down;

        //m_keymap[VK_F1]         = key_f1;
        //m_keymap[VK_F2]         = key_f2;
        //m_keymap[VK_F3]         = key_f3;
        //m_keymap[VK_F4]         = key_f4;
        //m_keymap[VK_F5]         = key_f5;
        //m_keymap[VK_F6]         = key_f6;
        //m_keymap[VK_F7]         = key_f7;
        //m_keymap[VK_F8]         = key_f8;
        //m_keymap[VK_F9]         = key_f9;
        //m_keymap[VK_F10]        = key_f10;
        //m_keymap[VK_F11]        = key_f11;
        //m_keymap[VK_F12]        = key_f12;
        //m_keymap[VK_F13]        = key_f13;
        //m_keymap[VK_F14]        = key_f14;
        //m_keymap[VK_F15]        = key_f15;

        //m_keymap[VK_NUMLOCK]    = key_numlock;
        //m_keymap[VK_CAPITAL]    = key_capslock;
        //m_keymap[VK_SCROLL]     = key_scrollock;

        switch(m_format)
        {
        case pix_format_gray8:
            m_sys_format = pix_format_gray8;
            m_bpp = 8;
            m_sys_bpp = 8;
            break;

        case pix_format_rgb565:
        case pix_format_rgb555:
            m_sys_format = pix_format_rgb555;
            m_bpp = 16;
            m_sys_bpp = 16;
            break;

        case pix_format_rgb24:
        case pix_format_bgr24:
            m_sys_format = pix_format_rgb24;
            m_bpp = 24;
            m_sys_bpp = 24;
            break;

        case pix_format_bgra32:
        case pix_format_abgr32:
        case pix_format_argb32:
        case pix_format_rgba32:
            m_sys_format = pix_format_argb32;
            m_bpp = 32;
            m_sys_bpp = 32;
            break;
        }
        ::Microseconds(&m_sw_freq);
        ::Microseconds(&m_sw_start);
    }


    //------------------------------------------------------------------------
    void platform_specific::create_pmap(unsigned width, 
                                        unsigned height,
                                        rendering_buffer* wnd)
    {
        m_pmap_window.create(width, height, org_e(m_bpp));
        wnd->attach(m_pmap_window.buf(), 
                    m_pmap_window.width(),
                    m_pmap_window.height(),
                      m_flip_y ?
                     -m_pmap_window.row_bytes() :
                      m_pmap_window.row_bytes());
    }


    //------------------------------------------------------------------------
    void platform_specific::display_pmap(WindowRef window, const rendering_buffer* src)
    {
        if(m_sys_format == m_format)
        {
            m_pmap_window.draw(window);
        }
        else
        {
            pixel_map pmap_tmp;
            pmap_tmp.create(m_pmap_window.width(), 
                            m_pmap_window.height(),
                            org_e(m_sys_bpp));

            rendering_buffer rbuf_tmp;
            rbuf_tmp.attach(pmap_tmp.buf(),
                            pmap_tmp.width(),
                            pmap_tmp.height(),
                            m_flip_y ?
                             -pmap_tmp.row_bytes() :
                              pmap_tmp.row_bytes());

            switch(m_format)
            {
            case pix_format_gray8:
                return;

            case pix_format_rgb565:
                color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb555());
                break;

            case pix_format_bgr24:
                color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgb24());
                break;

            case pix_format_abgr32:
                color_conv(&rbuf_tmp, src, color_conv_abgr32_to_argb32());
                break;

            case pix_format_bgra32:
                color_conv(&rbuf_tmp, src, color_conv_bgra32_to_argb32());
                break;

            case pix_format_rgba32:
                color_conv(&rbuf_tmp, src, color_conv_rgba32_to_argb32());
                break;
            }
            pmap_tmp.draw(window);
        }
    }


    //------------------------------------------------------------------------
    bool platform_specific::save_pmap(const char* fn, unsigned idx, 
                                      const rendering_buffer* src)
    {
        if(m_sys_format == m_format)
        {
            return m_pmap_img[idx].save_as_qt(fn);
        }
        else
        {
            pixel_map pmap_tmp;
            pmap_tmp.create(m_pmap_img[idx].width(), 
                            m_pmap_img[idx].height(),
                            org_e(m_sys_bpp));

            rendering_buffer rbuf_tmp;
            rbuf_tmp.attach(pmap_tmp.buf(),
                            pmap_tmp.width(),
                            pmap_tmp.height(),
                            m_flip_y ?
                             -pmap_tmp.row_bytes() :
                              pmap_tmp.row_bytes());
            switch(m_format)
            {
            case pix_format_gray8:
                return false;

            case pix_format_rgb565:
                color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb555());
                break;

            case pix_format_rgb24:
                color_conv(&rbuf_tmp, src, color_conv_rgb24_to_bgr24());
                break;

            case pix_format_abgr32:
                color_conv(&rbuf_tmp, src, color_conv_abgr32_to_bgra32());
                break;

            case pix_format_argb32:
                color_conv(&rbuf_tmp, src, color_conv_argb32_to_bgra32());
                break;

            case pix_format_rgba32:
                color_conv(&rbuf_tmp, src, color_conv_rgba32_to_bgra32());
                break;
            }
            return pmap_tmp.save_as_qt(fn);
        }
        return true;
    }



    //------------------------------------------------------------------------
    bool platform_specific::load_pmap(const char* fn, unsigned idx, 
                                      rendering_buffer* dst)
    {
        pixel_map pmap_tmp;
        if(!pmap_tmp.load_from_qt(fn)) return false;

        rendering_buffer rbuf_tmp;
        rbuf_tmp.attach(pmap_tmp.buf(),
                        pmap_tmp.width(),
                        pmap_tmp.height(),
                        m_flip_y ?
                         -pmap_tmp.row_bytes() :
                          pmap_tmp.row_bytes());

        m_pmap_img[idx].create(pmap_tmp.width(), 
                               pmap_tmp.height(), 
                               org_e(m_bpp),
                               0);

        dst->attach(m_pmap_img[idx].buf(),
                    m_pmap_img[idx].width(),
                    m_pmap_img[idx].height(),
                    m_flip_y ?
                      -m_pmap_img[idx].row_bytes() :
                       m_pmap_img[idx].row_bytes());

        switch(m_format)
        {
        case pix_format_gray8:
            return false;
            break;

        case pix_format_rgb555:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb555()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_rgb555()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_rgb555()); break;
            }
            break;

        case pix_format_rgb565:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb565()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_rgb565()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_rgb565()); break;
            }
            break;

        case pix_format_rgb24:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgb24()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_rgb24()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_rgb24()); break;
            }
            break;

        case pix_format_bgr24:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgr24()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_bgr24()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_bgr24()); break;
            }
            break;

        case pix_format_abgr32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_abgr32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_abgr32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_abgr32()); break;
            }
            break;

        case pix_format_argb32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_argb32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_argb32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_argb32()); break;
            }
            break;

        case pix_format_bgra32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_bgra32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_bgra32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_bgra32()); break;
            }
            break;

        case pix_format_rgba32:
            switch(pmap_tmp.bpp())
            {
            case 16: color_conv(dst, &rbuf_tmp, color_conv_rgb555_to_rgba32()); break;
            case 24: color_conv(dst, &rbuf_tmp, color_conv_rgb24_to_rgba32()); break;
            case 32: color_conv(dst, &rbuf_tmp, color_conv_argb32_to_rgba32()); break;
            }
            break;
        }
        
        return true;
    }








    //------------------------------------------------------------------------
    unsigned platform_specific::translate(unsigned keycode)
    {
        return m_last_translated_key = (keycode > 255) ? 0 : m_keymap[keycode];
    }



    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(format, flip_y)),
        m_format(format),
        m_bpp(m_specific->m_bpp),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }



    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        if(m_specific->m_window)
        {
        	SetWindowTitleWithCFString (m_specific->m_window, CFStringCreateWithCStringNoCopy (nil, cap, kCFStringEncodingASCII, nil));
        }
    }



    //------------------------------------------------------------------------
    static unsigned get_key_flags(UInt32 wflags)
    {
        unsigned flags = 0;
        
         if(wflags & shiftKey)   flags |= kbd_shift;
         if(wflags & controlKey) flags |= kbd_ctrl;

        return flags;
    }


    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
		SInt16 item;
		Str255 p_msg;
		
		::CopyCStringToPascal (msg, p_msg);
		::StandardAlert (kAlertPlainAlert, (const unsigned char*) "\013AGG Message", p_msg, NULL, &item);
		//::StandardAlert (kAlertPlainAlert, (const unsigned char*) "\pAGG Message", p_msg, NULL, &item);
    }


    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
		::Microseconds (&(m_specific->m_sw_start));
    }


    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        UnsignedWide stop;
        ::Microseconds(&stop);
        return double(stop.lo - 
                      m_specific->m_sw_start.lo) * 1e6 / 
                      double(m_specific->m_sw_freq.lo);
    }


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        if(m_specific->m_sys_format == pix_format_undefined)
        {
            return false;
        }

        m_window_flags = flags;

		// application
		EventTypeSpec		eventType;
		EventHandlerUPP		handlerUPP;

		eventType.eventClass = kEventClassApplication;
		eventType.eventKind = kEventAppQuit;

		handlerUPP = NewEventHandlerUPP(DoAppQuit);

		InstallApplicationEventHandler (handlerUPP, 1, &eventType, nil, nil);

		eventType.eventClass = kEventClassMouse;
		eventType.eventKind = kEventMouseDown;
		handlerUPP = NewEventHandlerUPP(DoMouseDown);
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);

		eventType.eventKind = kEventMouseUp;
		handlerUPP = NewEventHandlerUPP(DoMouseUp);
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);
		
		eventType.eventKind = kEventMouseDragged;
		handlerUPP = NewEventHandlerUPP(DoMouseDragged);
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);

		eventType.eventClass = kEventClassKeyboard;
		eventType.eventKind = kEventRawKeyDown;
		handlerUPP = NewEventHandlerUPP(DoKeyDown);
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);

		eventType.eventKind = kEventRawKeyUp;
		handlerUPP = NewEventHandlerUPP(DoKeyUp);
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);

		eventType.eventKind = kEventRawKeyRepeat;
		handlerUPP = NewEventHandlerUPP(DoKeyDown);		// 'key repeat' is translated to 'key down'
		InstallApplicationEventHandler (handlerUPP, 1, &eventType, this, nil);

		WindowAttributes	windowAttrs;
		Rect				bounds;

		// window
		windowAttrs = kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute | kWindowStandardHandlerAttribute;
		SetRect (&bounds, 0, 0, width, height);
		OffsetRect (&bounds, 100, 100);
		CreateNewWindow (kDocumentWindowClass, windowAttrs, &bounds, &m_specific->m_window);

        if(m_specific->m_window == nil)
        {
            return false;
        }

		// I assume the text is ASCII.
		// Change to kCFStringEncodingMacRoman, kCFStringEncodingISOLatin1, kCFStringEncodingUTF8 or what else you need.
        SetWindowTitleWithCFString (m_specific->m_window, CFStringCreateWithCStringNoCopy (nil, m_caption, kCFStringEncodingASCII, nil));
		
		eventType.eventClass = kEventClassWindow;
		eventType.eventKind = kEventWindowClose;

		handlerUPP = NewEventHandlerUPP(DoWindowClose);
		InstallWindowEventHandler (m_specific->m_window, handlerUPP, 1, &eventType, this, NULL);

		eventType.eventKind = kEventWindowDrawContent;
		handlerUPP = NewEventHandlerUPP(DoWindowDrawContent);
		InstallWindowEventHandler (m_specific->m_window, handlerUPP, 1, &eventType, this, NULL);
		
		// Periodic task
		// Instead of an idle function I use the Carbon event timer.
		// You may decide to change the wait value which is currently 50 milliseconds.
		EventLoopRef		mainLoop;
		EventLoopTimerUPP	timerUPP;
		EventLoopTimerRef	theTimer;

		mainLoop = GetMainEventLoop();
		timerUPP = NewEventLoopTimerUPP (DoPeriodicTask);
		InstallEventLoopTimer (mainLoop, 0, 50 * kEventDurationMillisecond, timerUPP, this, &theTimer);

        m_specific->create_pmap(width, height, &m_rbuf_window);
        m_initial_width = width;
        m_initial_height = height;
        on_init();
        on_resize(width, height);
        m_specific->m_redraw_flag = true;
		
  		ShowWindow (m_specific->m_window);
  		SetPortWindowPort (m_specific->m_window);
		
      return true;
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
		
		RunApplicationEventLoop ();
        return true;
    }


    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".bmp"; }

    //------------------------------------------------------------------------
    const char* platform_support::full_file_name(const char* file_name)
    {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
#if defined(__MWERKS__)
            if(len < 4 || stricmp(fn + len - 4, ".BMP") != 0)
#else
	        if(len < 4 || strncasecmp(fn + len - 4, ".BMP", 4) != 0)
#endif
            {
                strcat(fn, ".bmp");
            }
            return m_specific->load_pmap(fn, idx, &m_rbuf_img[idx]);
        }
        return true;
    }



    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char fn[1024];
            strcpy(fn, file);
            int len = strlen(fn);
#if defined(__MWERKS__)
            if(len < 4 || stricmp(fn + len - 4, ".BMP") != 0)
#else
	        if(len < 4 || strncasecmp(fn + len - 4, ".BMP", 4) != 0)
#endif
            {
                strcat(fn, ".bmp");
            }
            return m_specific->save_pmap(fn, idx, &m_rbuf_img[idx]);
        }
        return true;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = m_specific->m_pmap_window.width();
            if(height == 0) height = m_specific->m_pmap_window.height();
            m_specific->m_pmap_img[idx].create(width, height, org_e(m_specific->m_bpp));
            m_rbuf_img[idx].attach(m_specific->m_pmap_img[idx].buf(), 
                                   m_specific->m_pmap_img[idx].width(),
                                   m_specific->m_pmap_img[idx].height(),
                                   m_flip_y ?
                                   -m_specific->m_pmap_img[idx].row_bytes() :
                                    m_specific->m_pmap_img[idx].row_bytes());
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
    	Rect	bounds;
    	
        m_specific->m_redraw_flag = true;
        // on_ctrl_change ();
		on_draw();

    	SetRect(&bounds, 0, 0, m_rbuf_window.width(), m_rbuf_window.height());
    	InvalWindowRect(m_specific->m_window, &bounds);
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        m_specific->display_pmap(m_specific->m_window, &m_rbuf_window);
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}


//------------------------------------------------------------------------
pascal OSStatus DoWindowClose (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	userData;
	
	QuitApplicationEventLoop ();

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoAppQuit (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	userData;
	
	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoMouseDown (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	Point wheresMyMouse;
	UInt32 modifier;
	
	GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &wheresMyMouse);
	GlobalToLocal (&wheresMyMouse);
	GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifier);

    platform_support * app = reinterpret_cast<platform_support*>(userData);

    app->m_specific->m_cur_x = wheresMyMouse.h;
    if(app->flip_y())
    {
        app->m_specific->m_cur_y = app->rbuf_window().height() - wheresMyMouse.v;
    }
    else
    {
        app->m_specific->m_cur_y = wheresMyMouse.v;
    }
    app->m_specific->m_input_flags = mouse_left | get_key_flags(modifier);
    
    app->m_ctrls.set_cur(app->m_specific->m_cur_x, 
                         app->m_specific->m_cur_y);
    if(app->m_ctrls.on_mouse_button_down(app->m_specific->m_cur_x, 
                                         app->m_specific->m_cur_y))
    {
        app->on_ctrl_change();
        app->force_redraw();
    }
    else
    {
        if(app->m_ctrls.in_rect(app->m_specific->m_cur_x, 
                                app->m_specific->m_cur_y))
        {
            if(app->m_ctrls.set_cur(app->m_specific->m_cur_x, 
                                    app->m_specific->m_cur_y))
            {
                app->on_ctrl_change();
                app->force_redraw();
            }
        }
        else
        {
            app->on_mouse_button_down(app->m_specific->m_cur_x, 
                                      app->m_specific->m_cur_y, 
                                      app->m_specific->m_input_flags);
        }
    }

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoMouseUp (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	Point wheresMyMouse;
	UInt32 modifier;
	
	GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &wheresMyMouse);
	GlobalToLocal (&wheresMyMouse);
	GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifier);

    platform_support * app = reinterpret_cast<platform_support*>(userData);

    app->m_specific->m_cur_x = wheresMyMouse.h;
    if(app->flip_y())
    {
        app->m_specific->m_cur_y = app->rbuf_window().height() - wheresMyMouse.v;
    }
    else
    {
        app->m_specific->m_cur_y = wheresMyMouse.v;
    }
    app->m_specific->m_input_flags = mouse_left | get_key_flags(modifier);

    if(app->m_ctrls.on_mouse_button_up(app->m_specific->m_cur_x, 
                                       app->m_specific->m_cur_y))
    {
        app->on_ctrl_change();
        app->force_redraw();
    }
    app->on_mouse_button_up(app->m_specific->m_cur_x, 
                            app->m_specific->m_cur_y, 
                            app->m_specific->m_input_flags);

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoMouseDragged (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	Point wheresMyMouse;
	UInt32 modifier;
	
	GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &wheresMyMouse);
	GlobalToLocal (&wheresMyMouse);
	GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifier);

    platform_support * app = reinterpret_cast<platform_support*>(userData);

    app->m_specific->m_cur_x = wheresMyMouse.h;
    if(app->flip_y())
    {
        app->m_specific->m_cur_y = app->rbuf_window().height() - wheresMyMouse.v;
    }
    else
    {
        app->m_specific->m_cur_y = wheresMyMouse.v;
    }
    app->m_specific->m_input_flags = mouse_left | get_key_flags(modifier);


    if(app->m_ctrls.on_mouse_move(
        app->m_specific->m_cur_x, 
        app->m_specific->m_cur_y,
        (app->m_specific->m_input_flags & mouse_left) != 0))
    {
        app->on_ctrl_change();
        app->force_redraw();
    }
    else
    {
        app->on_mouse_move(app->m_specific->m_cur_x, 
                           app->m_specific->m_cur_y, 
                           app->m_specific->m_input_flags);
    }

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoKeyDown (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	char key_code;
	UInt32 modifier;
	
	GetEventParameter (theEvent, kEventParamKeyMacCharCodes, typeChar, NULL, sizeof(char), NULL, &key_code);
	GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifier);

	platform_support * app = reinterpret_cast<platform_support*>(userData);

	app->m_specific->m_last_translated_key = 0;
    switch(modifier) 
    {
        case controlKey:
            app->m_specific->m_input_flags |= kbd_ctrl;
            break;

        case shiftKey:
            app->m_specific->m_input_flags |= kbd_shift;
            break;

        default:
            app->m_specific->translate(key_code);
            break;
    }

    if(app->m_specific->m_last_translated_key)
    {
        bool left  = false;
        bool up    = false;
        bool right = false;
        bool down  = false;

        switch(app->m_specific->m_last_translated_key)
        {
        case key_left:
            left = true;
            break;

        case key_up:
            up = true;
            break;

        case key_right:
            right = true;
            break;

        case key_down:
            down = true;
            break;

		//On a Mac, screenshots are handled by the system.
        case key_f2:                        
            app->copy_window_to_img(agg::platform_support::max_images - 1);
            app->save_img(agg::platform_support::max_images - 1, "screenshot");
            break;
        }


        if(app->m_ctrls.on_arrow_keys(left, right, down, up))
        {
            app->on_ctrl_change();
            app->force_redraw();
        }
        else
        {
            app->on_key(app->m_specific->m_cur_x,
                        app->m_specific->m_cur_y,
                        app->m_specific->m_last_translated_key,
                        app->m_specific->m_input_flags);
        }
    }

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoKeyUp (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
	char key_code;
	UInt32 modifier;
	
	GetEventParameter (theEvent, kEventParamKeyMacCharCodes, typeChar, NULL, sizeof(char), NULL, &key_code);
	GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifier);

	platform_support * app = reinterpret_cast<platform_support*>(userData);

    app->m_specific->m_last_translated_key = 0;
    switch(modifier) 
    {
        case controlKey:
            app->m_specific->m_input_flags &= ~kbd_ctrl;
            break;

        case shiftKey:
            app->m_specific->m_input_flags &= ~kbd_shift;
            break;
    }
    
	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal OSStatus DoWindowDrawContent (EventHandlerCallRef nextHandler, EventRef theEvent, void* userData)
{
    platform_support * app = reinterpret_cast<platform_support*>(userData);

    if(app)
    {
        if(app->m_specific->m_redraw_flag)
        {
            app->on_draw();
            app->m_specific->m_redraw_flag = false;
        }
        app->m_specific->display_pmap(app->m_specific->m_window, &app->rbuf_window());
    }

	return CallNextEventHandler (nextHandler, theEvent);
}


//------------------------------------------------------------------------
pascal void DoPeriodicTask (EventLoopTimerRef theTimer, void* userData)
{
    platform_support * app = reinterpret_cast<platform_support*>(userData);
    
    if(!app->wait_mode())
		app->on_idle();
}


}




//----------------------------------------------------------------------------
int agg_main(int argc, char* argv[]);


// Hm. Classic MacOS does not know command line input.
// CodeWarrior provides a way to mimic command line input.
// The function 'ccommand' can be used to get the command
// line arguments.
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
#if defined(__MWERKS__)
	// argc = ccommand (&argv);
#endif
    
    // Check if we are launched by double-clicking under OSX 
	// Get rid of extra argument, this will confuse the standard argument parsing
	// calls used in the examples to get the name of the image file to be used
    if ( argc >= 2 && strncmp (argv[1], "-psn", 4) == 0 ) {
        argc = 1;
    } 

launch:
    return agg_main(argc, argv);
}

#endif

#endif

