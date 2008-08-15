#ifndef _COMMON_H_
#define _COMMON_H_

/* File dati EERPOM */
								//	0x00	reserved
								//	0x01	reserved
#define		REV				0x02
#define		VER				0x03
#define		I_DOPPLER	0x04
#define		I0  			0x05
#define		I1				0x06
								//	0x07	free
								//	0x08	free
#define		CFG				0x09
#define		LFS				0x0a	// word per (Liters For Step)
#define		NS				0x0b	// word per (Number of Steps)
#define		VREF			0x0c	// word per tensione di riferimento del ADC
								//	0x0d	free
								//	0x0e	free
								//	0x0f	free
								//	0x10	free
								//	0x11	free
								//	0x12	free
								//	0x13	free
								//	0x14	free
								//	0x15	free
								//	0x16	free
								//	0x17	free
#define		MAP_BEG		0X18	// map beginning


#endif //_COMMON_H_
