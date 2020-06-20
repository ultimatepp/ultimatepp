#ifndef _VirtualGuiLinuxFrameBuffer_LinuxFrameBuffer_h_
#define _VirtualGuiLinuxFrameBuffer_LinuxFrameBuffer_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <libinput.h>
#include <xkbcommon/xkbcommon.h>
#include "VirtualTerminal.h"

namespace Upp {
    
class LinuxFrameBuffer: public VirtualGui {
    virtual Size        GetSize()                           { return screen.GetSize(); }
    virtual dword       GetMouseButtons()                   { return input.mouse.buttons; }
    virtual dword       GetModKeys()                        { return input.keyboard.modkeys; }
    virtual bool        IsMouseIn()                         { return true; }
    virtual bool        ProcessEvent(bool *quit);
    virtual void        WaitEvent(int ms);
    virtual bool        IsWaitingEvent();
    virtual void        WakeUpGuiThread()                   {}
    virtual void        SetMouseCursor(const Image& image)  {}
    virtual SystemDraw& BeginDraw();
    virtual void        CommitDraw();

    virtual void        Quit()                              {}

private:
    struct Screen{

    private:
        int             handle = -1;
        uint32          *buffer = nullptr;
        size_t          buff_len = 0;
        Size            resolution;
        int             bits_per_pixel = 0;
        SystemDraw      sysdraw;
        ImageBuffer     image;
        BufferPainter   painter;

    public:
        bool Open(const char * device);
        void Close();
        Size GetSize()                  { return resolution; }
        void * GetBuffer()              { return buffer; }
        SystemDraw & GetSystemDraw()    { return sysdraw; }
        ImageBuffer & GetImageBuffer()  { return image; }
    }screen;
    
    struct input_t{
        struct udev         *udev = nullptr;
        struct libinput     *lib_inp = nullptr;
        
        struct xkb_context  *kb_ctx = nullptr;
        struct xkb_keymap   *kb_map = nullptr;
        struct xkb_state    *kb_state = nullptr;
        
        struct mouse_t{
            uint32      buttons = 0;
            Point       pos;
        }mouse;
        
        struct keyboard_t{
            uint32      modkeys = 0;
        }keyboard;
        
        bool Open();
        void Close();
    }input;
    
    VirtualTerminal vt;
    
    uint32 TranslateScanCode(uint32 chr);
    String ScanCodeToUtf8(uint32 chr);
    
public:
    bool Create(const char * device);
    void Destroy();

    ~LinuxFrameBuffer();
};

}


#endif
