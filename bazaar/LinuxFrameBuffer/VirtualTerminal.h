#ifndef _LinuxFrameBuffer_VirtualTerminal_h_
#define _LinuxFrameBuffer_VirtualTerminal_h_

#include <termios.h>
#include <CtrlLib/CtrlLib.h>

namespace Upp{

    struct VirtualTerminal{
    private:
        int handle = -1;
        int previous_mode = 0;
        int previous_keyboard_mode = 0;
        struct termios terminal_attributes;
    public:
        bool Open(const char * device);
        void Close();
    };

}

#endif
