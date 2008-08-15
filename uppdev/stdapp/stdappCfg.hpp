#ifndef STDAPP_CFG_HPP
#define STDAPP_CFG_HPP

#include <stdapp/stdappDef.hpp>

class Cfg
{
public:
    bool restore_cfg;
    bool show_splash;
    bool maximized;
    bool minimized;
    int main_window_left;
    int main_window_top;
    int main_window_width;
    int main_window_height;
    int language;

    Cfg(
        const int init_language = default_language,
        const bool init_restore_cfg = default_restore_cfg,
        const bool init_show_splash = default_show_splash,
        const bool init_maximized = default_maximized,
        const bool init_minimized = default_minimized,
        const int init_main_window_left = default_main_window_left,
        const int init_main_window_top = default_main_window_top,
        const int init_main_window_width = default_main_window_width,
        const int init_main_window_height = default_main_window_height
        );

    /* text configuration file selected
    void Serialize(Stream& stream);
    text configuration file selected */
};

#endif
