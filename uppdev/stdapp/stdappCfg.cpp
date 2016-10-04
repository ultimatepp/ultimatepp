#include <stdapp/stdappCfg.hpp>

Cfg::Cfg(
    const int init_language,
    const bool init_restore_cfg,
    const bool init_show_splash,
    const bool init_maximized,
    const bool init_minimized,
    const int init_main_window_left,
    const int init_main_window_top,
    const int init_main_window_width,
    const int init_main_window_height
    )
    :
    restore_cfg(init_restore_cfg),
    show_splash(init_show_splash),
    maximized(init_maximized),
    minimized(init_minimized),
    main_window_left(init_main_window_left),
    main_window_top(init_main_window_top),
    main_window_width(init_main_window_width),
    main_window_height(init_main_window_height),
    language(init_language)
{
}

/* text configuration file selected
void Cfg::Serialize(Stream& stream)
{
    stream
        % restore_cfg
        % show_splash
        % maximized
        % minimized
        % main_window_left
        % main_window_top
        % main_window_width
        % main_window_height
        % language
        ;
}
text configuration file selected */
