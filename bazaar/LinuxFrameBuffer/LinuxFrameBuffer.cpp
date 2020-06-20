#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
//#include <linux/kd.h>
#include <linux/vt.h>

#include "LinuxFrameBuffer.h"

using namespace Upp;


static int open_restricted(const char *path, int flags, void *user_data)
{
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}


static void close_restricted(int fd, void *user_data)
{
    close(fd);
}


static const struct libinput_interface input_interface =
{
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};


LinuxFrameBuffer::~LinuxFrameBuffer()
{
    Destroy();
}


bool LinuxFrameBuffer::Screen::Open(const char * device)
{
    Close();
    
    handle = open(device, O_RDWR);
    if (handle < 0){
        RLOG("Error opening device: '" << device << "': " << GetLastErrorMessage());
        return false;
    }

    struct fb_var_screeninfo info;
    if (ioctl(handle, FBIOGET_VSCREENINFO, &info) == 0){
        resolution.cx = info.xres;
        resolution.cy = info.yres;
        bits_per_pixel = info.bits_per_pixel;
    }

    struct fb_fix_screeninfo finfo;
    if (ioctl(handle, FBIOGET_FSCREENINFO, &finfo) == 0){
        buff_len = finfo.smem_len;
    }
    
    buffer = (uint32 *)mmap(nullptr, buff_len, PROT_READ | PROT_WRITE, MAP_SHARED, handle, 0);
    if (buffer == MAP_FAILED){
        RLOG("Memory mapping error: " << GetLastErrorMessage());
        return false;
    }
    
    image.Create(resolution);
    painter.Create(image, MODE_ANTIALIASED);
    sysdraw.SetTarget(&painter);
    
    return true;
}


void LinuxFrameBuffer::Screen::Close()
{
    if (buffer != nullptr){
        munmap(buffer, buff_len);
        buffer = nullptr;
    }
    
    if (handle >= 0){
        close(handle);
        handle = -1;
    }
}


bool LinuxFrameBuffer::input_t::Open()
{
    Close();
    
    udev = udev_new();
    if (udev == nullptr){
        RLOG("udev_new error");
        return false;
    }
    
    lib_inp = libinput_udev_create_context(&input_interface, nullptr, udev);
    if (lib_inp == nullptr){
        RLOG("libinput create context error");
        return false;
    }
    
    libinput_udev_assign_seat(lib_inp, "seat0");
    libinput_dispatch(lib_inp);
     
    kb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (kb_ctx == nullptr){
        RLOG("xkb context error");
        return false;
    }
    
    kb_map = xkb_keymap_new_from_names(kb_ctx, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (kb_map == nullptr){
        RLOG("xkb map error");
        return false;
    }
    
    kb_state = xkb_state_new(kb_map);
    if (kb_state == nullptr){
        RLOG("xkb state error");
        return false;
    }
    
    return true;
}


void LinuxFrameBuffer::input_t::Close()
{
    if (kb_state != nullptr){
        xkb_state_unref(kb_state);
        kb_state = nullptr;
    }

    if (kb_map != nullptr){
        xkb_keymap_unref(kb_map);
        kb_map = nullptr;
    }

    if (kb_ctx != nullptr){
        xkb_context_unref(kb_ctx);
        kb_ctx = nullptr;
    }
    
    if (lib_inp != nullptr){
        libinput_unref(lib_inp);
        lib_inp = nullptr;
    }
    
    if (udev != nullptr){
        udev_unref(udev);
        udev = nullptr;
    }
}


void LinuxFrameBuffer::Destroy()
{
    input.Close();
    vt.Close();
    screen.Close();
}


bool LinuxFrameBuffer::Create(const char * device)
{
    Destroy();
    
    if (!screen.Open(device)){
        Destroy();
        return false;
    }
    
    //vt.Open("/dev/tty3");
    
    if (!input.Open()){
        Destroy();
        return false;
    }
        
    return true;
}
