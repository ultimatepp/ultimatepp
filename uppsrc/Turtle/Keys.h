#define WEBKEY(x) x

K_BACK       = WEBKEY(8) + K_DELTA,
K_BACKSPACE  = WEBKEY(8) + K_DELTA,

//handled extra in fbKEYtoK
K_TAB        = 9, //SDLK_TAB,

K_SPACE      = 32, //SDLK_SPACE,

K_RETURN     = 13, //SDLK_RETURN,
K_ENTER      = K_RETURN,

K_SHIFT_KEY  = WEBKEY(16) + K_DELTA,
K_CTRL_KEY   = WEBKEY(17) + K_DELTA,
K_ALT_KEY    = WEBKEY(18) + K_DELTA,

K_CAPSLOCK   = WEBKEY(20) + K_DELTA,
K_ESCAPE     = WEBKEY(27) + K_DELTA,
K_PRIOR      = WEBKEY(33) + K_DELTA,
K_PAGEUP     = WEBKEY(33) + K_DELTA,
K_NEXT       = WEBKEY(34) + K_DELTA,
K_PAGEDOWN   = WEBKEY(34) + K_DELTA,
K_END        = WEBKEY(35) + K_DELTA,
K_HOME       = WEBKEY(36) + K_DELTA,
K_LEFT       = WEBKEY(37) + K_DELTA,
K_UP         = WEBKEY(38) + K_DELTA,
K_RIGHT      = WEBKEY(39) + K_DELTA,
K_DOWN       = WEBKEY(40) + K_DELTA,
K_INSERT     = WEBKEY(45) + K_DELTA,
K_DELETE     = WEBKEY(46) + K_DELTA,

K_NUMPAD0    = WEBKEY(96) + K_DELTA,
K_NUMPAD1    = WEBKEY(97) + K_DELTA,
K_NUMPAD2    = WEBKEY(98) + K_DELTA,
K_NUMPAD3    = WEBKEY(99) + K_DELTA,
K_NUMPAD4    = WEBKEY(100) + K_DELTA,
K_NUMPAD5    = WEBKEY(101) + K_DELTA,
K_NUMPAD6    = WEBKEY(102) + K_DELTA,
K_NUMPAD7    = WEBKEY(103) + K_DELTA,
K_NUMPAD8    = WEBKEY(104) + K_DELTA,
K_NUMPAD9    = WEBKEY(105) + K_DELTA,
K_MULTIPLY   = WEBKEY(106) + K_DELTA,
K_ADD        = WEBKEY(107) + K_DELTA,
K_SEPARATOR  = WEBKEY(108) + K_DELTA,
K_SUBTRACT   = WEBKEY(109) + K_DELTA,
K_DECIMAL    = WEBKEY(110) + K_DELTA,
K_DIVIDE     = WEBKEY(111) + K_DELTA,
K_SCROLL     = WEBKEY(145) + K_DELTA,

K_F1         = WEBKEY(112) + K_DELTA,
K_F2         = WEBKEY(113) + K_DELTA,
K_F3         = WEBKEY(114) + K_DELTA,
K_F4         = WEBKEY(115) + K_DELTA,
K_F5         = WEBKEY(116) + K_DELTA,
K_F6         = WEBKEY(117) + K_DELTA,
K_F7         = WEBKEY(118) + K_DELTA,
K_F8         = WEBKEY(119) + K_DELTA,
K_F9         = WEBKEY(120) + K_DELTA,
K_F10        = WEBKEY(121) + K_DELTA,
K_F11        = WEBKEY(122) + K_DELTA,
K_F12        = WEBKEY(123) + K_DELTA,

K_A          = WEBKEY('A') + K_DELTA,
K_B          = WEBKEY('B') + K_DELTA,
K_C          = WEBKEY('C') + K_DELTA,
K_D          = WEBKEY('D') + K_DELTA,
K_E          = WEBKEY('E') + K_DELTA,
K_F          = WEBKEY('F') + K_DELTA,
K_G          = WEBKEY('G') + K_DELTA,
K_H          = WEBKEY('H') + K_DELTA,
K_I          = WEBKEY('I') + K_DELTA,
K_J          = WEBKEY('J') + K_DELTA,
K_K          = WEBKEY('K') + K_DELTA,
K_L          = WEBKEY('L') + K_DELTA,
K_M          = WEBKEY('M') + K_DELTA,
K_N          = WEBKEY('N') + K_DELTA,
K_O          = WEBKEY('O') + K_DELTA,
K_P          = WEBKEY('P') + K_DELTA,
K_Q          = WEBKEY('Q') + K_DELTA,
K_R          = WEBKEY('R') + K_DELTA,
K_S          = WEBKEY('S') + K_DELTA,
K_T          = WEBKEY('T') + K_DELTA,
K_U          = WEBKEY('U') + K_DELTA,
K_V          = WEBKEY('V') + K_DELTA,
K_W          = WEBKEY('W') + K_DELTA,
K_X          = WEBKEY('X') + K_DELTA,
K_Y          = WEBKEY('Y') + K_DELTA,
K_Z          = WEBKEY('Z') + K_DELTA,
K_0          = WEBKEY('0') + K_DELTA,
K_1          = WEBKEY('1') + K_DELTA,
K_2          = WEBKEY('2') + K_DELTA,
K_3          = WEBKEY('3') + K_DELTA,
K_4          = WEBKEY('4') + K_DELTA,
K_5          = WEBKEY('5') + K_DELTA,
K_6          = WEBKEY('6') + K_DELTA,
K_7          = WEBKEY('7') + K_DELTA,
K_8          = WEBKEY('8') + K_DELTA,
K_9          = WEBKEY('9') + K_DELTA,

K_CTRL_LBRACKET  = K_CTRL|219|K_DELTA,
K_CTRL_RBRACKET  = K_CTRL|221|K_DELTA,
K_CTRL_MINUS     = K_CTRL|189|K_DELTA,
K_CTRL_GRAVE     = K_CTRL|192|K_DELTA,
K_CTRL_SLASH     = K_CTRL|191|K_DELTA,
K_CTRL_BACKSLASH = K_CTRL|220|K_DELTA,
K_CTRL_COMMA     = K_CTRL|188|K_DELTA,
K_CTRL_PERIOD    = K_CTRL|190|K_DELTA,
K_CTRL_SEMICOLON = K_CTRL|59|K_DELTA,
K_CTRL_EQUAL     = K_CTRL|61|K_DELTA,
K_CTRL_APOSTROPHE= K_CTRL|222|K_DELTA,

K_BREAK      = WEBKEY(3) + K_DELTA, // Is it really?

K_PLUS       = 0, // not yet defined
K_MINUS      = 0, // not yet defined
K_COMMA      = 0, // not yet defined
K_PERIOD     = 0, // not yet defined
K_SEMICOLON  = 0, // not yet defined
K_SLASH      = 0, // not yet defined
K_GRAVE      = 0, // not yet defined
K_LBRACKET   = 0, // not yet defined
K_BACKSLASH  = 0, // not yet defined
K_RBRACKET   = 0, // not yet defined
K_QUOTEDBL   = 0, // not yet defined
