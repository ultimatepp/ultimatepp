#include "VirtualTerminal.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace Upp;

#ifndef KDSKBMUTE
# define KDSKBMUTE 0x4B51
#endif

bool VirtualTerminal::Open(const char * device)
{
    Close();
    
    handle = open(device, O_RDWR);
    
    if (handle < 0){
        RLOG("Error opening device: '" << device << "': " << GetLastErrorMessage());
        return false;
    }
    
    if (ioctl(handle, KDGETMODE, &previous_mode)){
        RLOG("Failed to get current VT mode: '" << device << "': " << GetLastErrorMessage());
    }
    
    if (previous_mode != KD_GRAPHICS && ioctl(handle, KDSETMODE, KD_GRAPHICS)){
        RLOG("Failed to set VT graphics mode: '" << device << "': " << GetLastErrorMessage());
    }
    
    if (tcgetattr(handle, &terminal_attributes)){
        RLOG("Failed to read terminal attributes: '" << device << "': " << GetLastErrorMessage());
    }
    
    struct termios attributes = terminal_attributes;
    cfmakeraw(&attributes);
    attributes.c_oflag |= OPOST;
    if (tcsetattr(handle, TCSANOW, &attributes)){
        RLOG("Failed to set terminal attributes: '" << device << "': " << GetLastErrorMessage());
    }

    if (ioctl(handle, KDGKBMODE, &previous_keyboard_mode)){
        RLOG("Failed to get keyboard mode: '" << device << "': " << GetLastErrorMessage());
    }
    
    if (ioctl(handle, KDSKBMUTE, 1) && ioctl(handle, KDSKBMODE, K_RAW)){
        RLOG("Failed to set keyboard mode: '" << device << "': " << GetLastErrorMessage());
    }
    
    return true;
}


void VirtualTerminal::Close()
{
    if (handle < 0) return;
    
    if (ioctl(handle, KDSETMODE, &previous_mode)){
        RLOG("Failed to restore VT mode: " << GetLastErrorMessage());
    }
    
    if (ioctl(handle, KDSKBMUTE, 0)){
        RLOG("Failed to restore keyboard mute: " << GetLastErrorMessage());
    }
    
    if (ioctl(handle, KDSKBMODE, previous_keyboard_mode)){
        RLOG("Failed to restore keyboard mode: " << GetLastErrorMessage());
    }
        
    if (tcsetattr(handle, TCSANOW, &terminal_attributes)){
        RLOG("Failed to restore terminal attributes: " << GetLastErrorMessage());
    }
    
    close(handle);
    handle = -1;
}
