#include "Turtle.h"

namespace Upp {

#define WEBKEY(x)	x

const static VectorMap<dword, dword> sKeyCodeMap = {
	{ WEBKEY(3),   K_BREAK           },
	{ WEBKEY(8),   K_BACKSPACE       },
	{ WEBKEY(9),   K_TAB             },
	{ WEBKEY(13),  K_RETURN          },
	{ WEBKEY(16),  K_SHIFT_KEY       },
	{ WEBKEY(17),  K_CTRL_KEY        },
	{ WEBKEY(18),  K_ALT_KEY         },
	{ WEBKEY(20),  K_CAPSLOCK        },
	{ WEBKEY(27),  K_ESCAPE          },
	{ WEBKEY(32),  K_SPACE           },
	{ WEBKEY(33),  K_PAGEUP          },
	{ WEBKEY(34),  K_PAGEDOWN        },
	{ WEBKEY(35),  K_END             },
	{ WEBKEY(36),  K_HOME            },
	{ WEBKEY(37),  K_LEFT            },
	{ WEBKEY(38),  K_UP              },
	{ WEBKEY(39),  K_RIGHT           },
	{ WEBKEY(40),  K_DOWN            },
	{ WEBKEY(45),  K_INSERT          },
	{ WEBKEY(46),  K_DELETE          },
	{ WEBKEY('A'), K_A               },
	{ WEBKEY('B'), K_B               },
	{ WEBKEY('C'), K_C               },
	{ WEBKEY('D'), K_D               },
	{ WEBKEY('E'), K_E               },
	{ WEBKEY('F'), K_F               },
	{ WEBKEY('G'), K_G               },
	{ WEBKEY('H'), K_H               },
	{ WEBKEY('I'), K_I               },
	{ WEBKEY('J'), K_J               },
	{ WEBKEY('K'), K_K               },
	{ WEBKEY('L'), K_L               },
	{ WEBKEY('M'), K_M               },
	{ WEBKEY('N'), K_N               },
	{ WEBKEY('O'), K_O               },
	{ WEBKEY('P'), K_P               },
	{ WEBKEY('Q'), K_Q               },
	{ WEBKEY('R'), K_R               },
	{ WEBKEY('S'), K_S               },
	{ WEBKEY('T'), K_T               },
	{ WEBKEY('U'), K_U               },
	{ WEBKEY('V'), K_V               },
	{ WEBKEY('W'), K_W               },
	{ WEBKEY('X'), K_X               },
	{ WEBKEY('Y'), K_Y               },
	{ WEBKEY('Z'), K_Z               },
	{ WEBKEY('0'), K_0               },
	{ WEBKEY('1'), K_1               },
	{ WEBKEY('2'), K_2               },
	{ WEBKEY('3'), K_3               },
	{ WEBKEY('4'), K_4               },
	{ WEBKEY('5'), K_5               },
	{ WEBKEY('6'), K_6               },
	{ WEBKEY('7'), K_7               },
	{ WEBKEY('8'), K_8               },
	{ WEBKEY('9'), K_9               },
	{ WEBKEY(96),  K_NUMPAD0         },
	{ WEBKEY(97),  K_NUMPAD1         },
	{ WEBKEY(98),  K_NUMPAD2         },
	{ WEBKEY(99),  K_NUMPAD3         },
	{ WEBKEY(100), K_NUMPAD4         },
	{ WEBKEY(101), K_NUMPAD5         },
	{ WEBKEY(102), K_NUMPAD6         },
	{ WEBKEY(103), K_NUMPAD7         },
	{ WEBKEY(104), K_NUMPAD8         },
	{ WEBKEY(105), K_NUMPAD9         },
	{ WEBKEY(106), K_MULTIPLY        },
	{ WEBKEY(107), K_ADD             },
	{ WEBKEY(108), K_SEPARATOR       },
	{ WEBKEY(109), K_SUBTRACT        },
	{ WEBKEY(110), K_DECIMAL         },
	{ WEBKEY(111), K_DIVIDE          },
	{ WEBKEY(145), K_SCROLL          },
	{ WEBKEY(112), K_F1              },
	{ WEBKEY(113), K_F2              },
	{ WEBKEY(114), K_F3              },
	{ WEBKEY(115), K_F4              },
	{ WEBKEY(116), K_F5              },
	{ WEBKEY(117), K_F6              },
	{ WEBKEY(118), K_F7              },
	{ WEBKEY(119), K_F8              },
	{ WEBKEY(120), K_F9              },
	{ WEBKEY(121), K_F10             },
	{ WEBKEY(122), K_F11             },
	{ WEBKEY(123), K_F12             },
	{ WEBKEY(219), K_CTRL_LBRACKET   },
	{ WEBKEY(221), K_CTRL_RBRACKET   },
	{ WEBKEY(173), K_CTRL_MINUS      }, // Firefox specific.
	{ WEBKEY(189), K_CTRL_MINUS      },
	{ WEBKEY(192), K_CTRL_GRAVE      },
	{ WEBKEY(191), K_CTRL_SLASH      },
	{ WEBKEY(220), K_CTRL_BACKSLASH  },
	{ WEBKEY(188), K_CTRL_COMMA      },
	{ WEBKEY(190), K_CTRL_PERIOD     },
	{ WEBKEY(59) , K_CTRL_SEMICOLON  }, // Firefox specific.
	{ WEBKEY(186), K_CTRL_SEMICOLON  },
	{ WEBKEY(61) , K_CTRL_EQUAL      }, // Firefox specific.
	{ WEBKEY(187), K_CTRL_EQUAL      },
	{ WEBKEY(222), K_CTRL_APOSTROPHE },
};

#undef WEBKEY

dword TurtleServer::TranslateWebKeyToK(dword key)
{
	int i = sKeyCodeMap.Find(key);

	if(i < 0)
		return key | K_DELTA;

	key = sKeyCodeMap[i];

	if(key == K_ALT_KEY || key == K_CTRL_KEY || key == K_SHIFT_KEY)
		return key;
	
	if(GetAlt())    key |= K_ALT;
	if(GetCtrl())   key |= K_CTRL;
	if(GetShift())  key |= K_SHIFT;

	return key;
}

void TurtleServer::ReadModifierKeys(CParser& p)
{
	const char *s = p.GetPtr();
	if(*s && *s++ == '1') modifierkeys |= KM_SHIFT; else modifierkeys &= ~KM_SHIFT;
	if(*s && *s++ == '1') modifierkeys |= KM_CTRL;  else modifierkeys &= ~KM_CTRL;
	if(*s && *s++ == '1') modifierkeys |= KM_ALT;   else modifierkeys &= ~KM_ALT;
}
}
