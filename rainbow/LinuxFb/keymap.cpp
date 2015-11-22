#include "LinuxFbLocal.h"

#define LLOG(x)       //LOG(x)

NAMESPACE_UPP

dword modkeys = 0;

enum KMOD {
	KMOD_NONE  = 0x00,

	KMOD_LSHIFT= 0x01,
	KMOD_RSHIFT= 0x02,
	KMOD_LCTRL = 0x04,
	KMOD_RCTRL = 0x08,
	KMOD_LALT  = 0x10,
	KMOD_RALT  = 0x20,

	KMOD_CAPS  = 0x40,
	KMOD_NUM   = 0x80,
	
	KMOD_CTRL = KMOD_LCTRL | KMOD_RCTRL,
	KMOD_SHIFT = KMOD_LSHIFT | KMOD_RSHIFT,
	KMOD_ALT = KMOD_LALT | KMOD_RALT,
};

#define CHFLAG(w, m, b) if(b) w |= m; else w &= ~m;
void SaveModKeys(dword keycode, dword pressed)
{
	switch(keycode)
	{
		case SCANCODE_LEFTSHIFT: CHFLAG(modkeys, KMOD_LSHIFT, pressed) break;
		case SCANCODE_RIGHTSHIFT: CHFLAG(modkeys, KMOD_RSHIFT, pressed) break;
		case SCANCODE_LEFTCONTROL: CHFLAG(modkeys, KMOD_LCTRL, pressed) break;
		case SCANCODE_RIGHTCONTROL: CHFLAG(modkeys, KMOD_RCTRL, pressed) break;
		case SCANCODE_LEFTALT: CHFLAG(modkeys, KMOD_LALT, pressed) break;
		case SCANCODE_RIGHTALT: CHFLAG(modkeys, KMOD_RALT, pressed) break;
		case SCANCODE_CAPSLOCK: CHFLAG(modkeys, KMOD_CAPS, pressed) break;
		case SCANCODE_NUMLOCK: CHFLAG(modkeys, KMOD_NUM, pressed) break;
	}
}

bool GetShift()       { return modkeys & KMOD_SHIFT; }
bool GetCtrl()        { return modkeys & KMOD_CTRL; }
bool GetAlt()         { return modkeys & KMOD_ALT; }
bool GetCapsLock()    { return modkeys & KMOD_CAPS; }

dword fbKEYtoK(dword chr) {

	if(chr == SCANCODE_TAB)
		chr = K_TAB;
	else
	if(chr == SCANCODE_SPACE)
		chr = K_SPACE;
	else
	if(chr == SCANCODE_ENTER)
		chr = K_RETURN;
	else
		chr = chr + K_DELTA;

	//if the mod keys themselves, no need to have CTRL+ xxx behaviour indicator
	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY)
		return chr;

	if(GetCtrl()) chr |= K_CTRL;
	if(GetAlt()) chr |= K_ALT;
	if(GetShift()) chr |= K_SHIFT;

	return chr;
}

END_UPP_NAMESPACE

//kernel defines conflict with some upp K_ enums, like K_SHIFT, K_ALT, K_CTRL
//so we separate as much as possible
#include <linux/keyboard.h>

NAMESPACE_UPP

//The kernel copy of translation tables
//from a console keycode in MEDIUMRAW to unicode
static uint16 kmap[MAX_NR_KEYMAPS][NR_KEYS];

void dupkmap(int fd)
{
	struct kbentry entry;

	if(fd < 0) return;

	for(int m=0; m<MAX_NR_KEYMAPS; ++m) {
		memset(kmap[m], 0, NR_KEYS*sizeof(uint16));
		for(int i=0; i<NR_KEYS; ++i) {
			entry.kb_table = m; entry.kb_index = i;
			if(ioctl(fd, KDGKBENT, &entry) < 0)
			{
				fprintf(stderr, "Error: reading keymap\n");
				return;
			}

			if(entry.kb_value == K_ENTER ) entry.kb_value = K(KT_ASCII,13);

			//correct numpad
			if(KTYP(entry.kb_value) == KT_PAD) {
				switch(entry.kb_value) {
					case K_P0:
					case K_P1:
					case K_P2:
					case K_P3:
					case K_P4:
					case K_P5:
					case K_P6:
					case K_P7:
					case K_P8:
					case K_P9:
						kmap[m][i]=entry.kb_value;
						kmap[m][i]+= '0';
						break;
					case K_PPLUS:  kmap[m][i]=K(KT_ASCII,'+'); break;
					case K_PMINUS: kmap[m][i]=K(KT_ASCII,'-'); break;
					case K_PSTAR:  kmap[m][i]=K(KT_ASCII,'*'); break;
					case K_PSLASH: kmap[m][i]=K(KT_ASCII,'/'); break;
					case K_PENTER: kmap[m][i]=K(KT_ASCII,'\r'); break;
					case K_PCOMMA: kmap[m][i]=K(KT_ASCII,','); break;
					case K_PDOT:   kmap[m][i]=K(KT_ASCII,'.'); break;
					default: break;
				}
			}

			if((KTYP(entry.kb_value) == KT_LATIN)
				|| (KTYP(entry.kb_value) == KT_ASCII)
				|| (KTYP(entry.kb_value) == KT_LETTER)
			)
				kmap[m][i] = entry.kb_value;
		}
	}
}

dword TranslateUnicode(dword keycode)
{
	int m = 0;

	if(modkeys & KMOD_SHIFT) m |= (1<<KG_SHIFT);
	if(modkeys & KMOD_CTRL) m |= (1<<KG_CTRL);
	if(modkeys & KMOD_LALT) m |= (1<<KG_ALT);
	if(modkeys & KMOD_RALT) m |= (1<<KG_ALTGR);

	//CAPS changes shift meaning in both directions
	if((modkeys & KMOD_CAPS) 
		&& KTYP(kmap[m][keycode]) == KT_LETTER
	)
		m ^= (1<<KG_SHIFT);

	//num pad handling stays same so far
	if((modkeys & KMOD_NUM)
		&& KTYP(kmap[m][keycode]) == KT_PAD
	)
		return KVAL(kmap[m][keycode]);
	else
		return KVAL(kmap[m][keycode]);
}

END_UPP_NAMESPACE
