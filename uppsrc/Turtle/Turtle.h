#ifndef _VirtualGui_Turtle_h
#define _VirtualGui_Turtle_h

#include <CtrlLib/CtrlLib.h>

namespace Upp {

class TurtleServer : public VirtualGui {
public:
    TurtleServer()                                         {}
    TurtleServer(const String& host, int port)             { Host(host).Port(port); }
    TurtleServer(const String& ip, String& host, int port) { Bind(ip).Host(host).Port(port); }

    TurtleServer&       Bind(const String& addr)           { TurtleServer::ip   = addr; return *this; }
    TurtleServer&       Host(const String& host)           { TurtleServer::host = host; return *this; }
    TurtleServer&       Port(int port)                     { TurtleServer::port = port; return *this; }
    TurtleServer&       MaxConnections(int limit)          { TurtleServer::connection_limit = max(1, limit); return *this; }

    static void         DebugMode(bool b = true)           { TurtleServer::debugmode = b; }
    
    static Event<int, String>  WhenConnect;
    static Event<int>          WhenTerminate;
    static Event<>             WhenDisconnect;

private:
    virtual dword       GetOptions()                       { return GUI_SETMOUSECURSOR; }
    virtual Size        GetSize()                          { return desktopsize;  }
    virtual dword       GetMouseButtons()                  { return mousebuttons; }
    virtual dword       GetModKeys()                       { return modifierkeys; }
    virtual bool        IsMouseIn()                        { return mousein; }
    virtual bool        ProcessEvent(bool *quit);
    virtual void        WaitEvent(int ms);
    virtual bool        IsWaitingEvent();
    virtual void        SetMouseCursor(const Image& image);
    virtual void        SetCaret(const Rect& caret)        {}
    virtual SystemDraw& BeginDraw();
    virtual void        CommitDraw();
    virtual void        WakeUpGuiThread()                  {}
    virtual void        Quit()                             { WhenDisconnect(); }
        
private:
    void                MouseButton(dword event, CParser& p);
    void                MouseWheel(CParser& p);
    void                MouseMove(CParser& p);
    void                KeyDown(const String& event, CParser& p);
    void                KeyUp(const String& event, CParser& p);
    void                KeyPress(const String& event, CParser& p);
    void                Resize(CParser& p);

    void                ReadModifierKeys(CParser& p);
    dword               TranslateWebKeyToK(dword key);

    static void         Broadcast(int signal);
    void                SyncClient();
    
public:
    struct Stream : OutStream
    {
        Stream();
        void            Out(const void *data, dword size) final;
        String          FlushStream();
        void            Reset();
        Zlib            zlib;
        bool            hasdata;
    };
    
    struct ImageSysData
    {
        ImageSysData();
       ~ImageSysData();
        void            Init(const Image& img);
        Image           image;
        int             handle;
    };
    
    struct Draw : SDraw
    {
        Draw();
        void            Init(const Size& sz);
        void            PutImage(Point p, const Image& img, const Rect& src) final;
        void            PutRect(const Rect& r, Color color) final;
        Point           pos;
        SystemDraw      sysdraw;
    };

    friend class        TurtleServer::Draw;
    friend class        TurtleServer::ImageSysData;

public:
    enum Commands {
        RECT            = 0,
        IMAGE           = 1,
        SETIMAGE        = 2,
        INVERTRECT      = 3,
        STD_CURSORIMAGE = 4,
        SETCURSORIMAGE  = 5,
        MOUSECURSOR     = 6,
        DISABLESENDING  = 7,
        UPDATESERIAL    = 8,
    
        IMAGEPP         = 9,
        IMAGENP         = 10,
        IMAGEPN         = 11,
        IMAGENN         = 12,
    
        RECTPP          = 13,
        RECTNP          = 14,
        RECTPN          = 15,
        RECTNN          = 16,
    
        SETCARET        = 17,
        
        HORZDRAGLINE    = 18,
        VERTDRAGLINE    = 19,
        
        OPENLINK        = 20,
    };

private:
    static void         Put8(int x);
    static void         Put16(int x);
    static void         Put32(int x);
    static void         Put(Point p);
    static void         Put(Size sz);
    static void         Put(const Rect& r);
    static void         Put(const String& s);
    static void         Flush();

    static void         SetCanvasSize(const Size& sz);
    static void         ResetImageCache();
        
private:
    static WebSocket    websocket;
    static dword        mousebuttons;
    static dword        modifierkeys;
    static Size         desktopsize;
    static int          mainpid;
    static int64        update_serial;
    static int64        recieved_update_serial;
    static int64        serial_0;
    static int          serial_time0;
    static bool         quit;
    static String       host;
    static int          port;
    static String       ip;
    static int          connection_limit;
    static bool         debugmode;
    static bool         mousein;

public:
    // Statistics.
    static Time         stat_started;
    static int64        stat_data_send;
    static int          stat_putrect;
    static int          stat_putimage;
    static int          stat_setimage;
    static int64        stat_setimage_len;
    static int          stat_roundtrip_ms;
    static int          stat_client_ms;
    
private:
    static bool         StartSession();
    friend void         RunTurtleGui(TurtleServer&, Event<>);
};

void RunTurtleGui(TurtleServer& gui, Event<> app_main);

}
#endif
