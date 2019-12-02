#ifndef _Functions4U_SvgColors_h_
#define _Functions4U_SvgColors_h_


namespace Upp {

struct ColorDef {
	const char *name;
	Color color;
};

Color GetSvgColor(const char *color);

inline Color Aliceblue() 		{return Color(240,248,255);}
inline Color Antiquewhite()		{return Color(250,235,215);}
inline Color Aqua() 			{return Color(0,255,255);}
inline Color Aquamarine() 		{return Color(127,255,212);}
inline Color Azure() 			{return Color(240,255,255);}
inline Color Beige() 			{return Color(245,245,220);}
inline Color Bisque() 			{return Color(255,228,196);}
//inline Color Black() 			{return Color(0,0,0);}
inline Color Blanchedalmond() 	{return Color(255,235,205);}
//inline Color Blue() 			{return Color(0,0,255);}
inline Color Blueviolet() 		{return Color(138,43,226);}
//inline Color Brown() 			{return Color(165,42,42);}
inline Color Burlywood() 		{return Color(222,184,135);}
inline Color Cadetblue() 		{return Color(95,158,160);}
inline Color Chartreuse() 		{return Color(127,255,0);}
inline Color Chocolate() 		{return Color(210,105,30);}
inline Color Coral() 			{return Color(255,127,80);}
inline Color Cornflowerblue()	{return Color(100,149,237);}
inline Color Cornsilk() 		{return Color(255,248,220);}
inline Color Crimson()			{return Color(220,20,60);}
//inline Color Cyan()			{return Color(0,255,255);}
inline Color Darkblue()			{return Color(0,0,139);}
inline Color Darkcyan()			{return Color(0,139,139);}
inline Color Darkgoldenrod()	{return Color(184,134,11);}
inline Color Darkgray()			{return Color(169,169,169);}
inline Color Darkgreen()		{return Color(0,100,0);}
inline Color Darkgrey()			{return Color(169,169,169);}
inline Color Darkkhaki()		{return Color(189,183,107);}
inline Color Darkmagenta()		{return Color(139,0,139);}
inline Color Darkolivegreen()	{return Color(85,107,47);}
inline Color Darkorange()		{return Color(255,140,0);}
inline Color Darkorchid()		{return Color(153,50,204);}
inline Color Darkred()			{return Color(139,0,0);}
inline Color Darksalmon()		{return Color(233,150,122);}
inline Color Darkseagreen()		{return Color(143,188,143);}
inline Color Darkslateblue()	{return Color(72,61,139);}
inline Color Darkslategray()	{return Color(47,79,79);}
inline Color Darkslategrey()	{return Color(47,79,79);}
inline Color Darkturquoise()	{return Color(0,206,209);}
inline Color Darkviolet()		{return Color(148,0,211);}
inline Color Deeppink()			{return Color(255,20,147);}
inline Color Deepskyblue()		{return Color(0,191,255);}
inline Color Dimgray()			{return Color(105,105,105);}
inline Color Dimgrey()			{return Color(105,105,105);}
inline Color Dodgerblue()		{return Color(30,144,255);}
inline Color Firebrick()		{return Color(178,34,34);}
inline Color Floralwhite()		{return Color(255,250,240);}
inline Color Forestgreen()		{return Color(34,139,34);}
inline Color Fuchsia()			{return Color(255,0,255);}
inline Color Gainsboro()		{return Color(220,220,220);}
inline Color Ghostwhite()		{return Color(248,248,255);}
inline Color Gold()				{return Color(255,215,0);}
inline Color Goldenrod()		{return Color(218,165,32);}
//inline Color Gray()			{return Color(128,128,128);}
//inline Color Green()			{return Color(0,128,0);}
inline Color Greenyellow()		{return Color(173,255,47);}
inline Color Grey()				{return Color(128,128,128);}
inline Color Honeydew()			{return Color(240,255,240);}
inline Color Hotpink()			{return Color(255,105,180);}
inline Color Indianred()		{return Color(205,92,92);}
inline Color Indigo()			{return Color(75,0,130);}
inline Color Ivory()			{return Color(255,255,240);}
inline Color Khaki()			{return Color(240,230,140);}
inline Color Lavender()			{return Color(230,230,250);}
inline Color Lavenderblush()	{return Color(255,240,245);}
inline Color Lawngreen()		{return Color(124,252,0);}
inline Color Lemonchiffon()		{return Color(255,250,205);}
inline Color Lightblue()		{return Color(173,216,230);}
inline Color Lightcoral()		{return Color(240,128,128);}
inline Color Lightcyan()		{return Color(224,255,255);}
inline Color Lightgoldenrodyellow()		{return Color(250,250,210);}
inline Color Lightgray()		{return Color(211,211,211);}
inline Color Lightgreen()		{return Color(144,238,144);}
inline Color Lightgrey()		{return Color(211,211,211);}
inline Color Lightpink()		{return Color(255,182,193);}
inline Color Lightsalmon()		{return Color(255,160,122);}
inline Color Lightseagreen()	{return Color(32,178,170);}
inline Color Lightskyblue()		{return Color(135,206,250);}
inline Color Lightslategray()	{return Color(119,136,153);}
inline Color Lightslategrey()	{return Color(119,136,153);}
inline Color Lightsteelblue()	{return Color(176,196,222);}
inline Color Lightyellow()		{return Color(255,255,224);}
inline Color Lime()				{return Color(0,255,0);}
inline Color Limegreen()		{return Color(50,205,50);}
inline Color Linen()			{return Color(250,240,230);}
//inline Color Magenta()		{return Color(255,0,255);}
inline Color Maroon()			{return Color(128,0,0);}
inline Color Mediumaquamarine()	{return Color(102,205,170);}
inline Color Mediumblue()		{return Color(0,0,205);}
inline Color Mediumorchid()		{return Color(186,85,211);}
inline Color Mediumpurple()		{return Color(147,112,219);}
inline Color Mediumseagreen()	{return Color(60,179,113);}
inline Color Mediumslateblue()	{return Color(123,104,238);}
inline Color Mediumspringgreen(){return Color(0,250,154);}
inline Color Mediumturquoise()	{return Color(72,209,204);}
inline Color Mediumvioletred()	{return Color(199,21,133);}
inline Color Midnightblue()		{return Color(25,25,112);}
inline Color Mintcream()		{return Color(245,255,250);}
inline Color Mistyrose()		{return Color(255,228,225);}
inline Color Moccasin()			{return Color(255,228,181);}
inline Color Navajowhite()		{return Color(255,222,173);}
inline Color Navy()				{return Color(0,0,128);}
inline Color Oldlace()			{return Color(253,245,230);}
inline Color Olive()			{return Color(128,128,0);}
inline Color Olivedrab()		{return Color(107,142,35);}
inline Color Orange()			{return Color(255,165,0);}
inline Color Orangered()		{return Color(255,69,0);}
inline Color Orchid()			{return Color(218,112,214);}
inline Color Palegoldenrod()	{return Color(238,232,170);}
inline Color Palegreen()		{return Color(152,251,152);}
inline Color Paleturquoise()	{return Color(175,238,238);}
inline Color Palevioletred()	{return Color(219,112,147);}
inline Color Papayawhip()		{return Color(255,239,213);}
inline Color Peachpuff()		{return Color(255,218,185);}
inline Color Peru()				{return Color(205,133,63);}
inline Color Pink()				{return Color(255,192,203);}
inline Color Plum()				{return Color(221,160,221);}
inline Color Powderblue()		{return Color(176,224,230);}
inline Color Purple()			{return Color(128,0,128);}
//inline Color Red()			{return Color(255,0,0);}
inline Color Rosybrown()		{return Color(188,143,143);}
inline Color Royalblue()		{return Color(65,105,225);}
inline Color Saddlebrown()		{return Color(139,69,19);}
inline Color Salmon()			{return Color(250,128,114);}
inline Color Sandybrown()		{return Color(244,164,96);}
inline Color Seagreen()			{return Color(46,139,87);}
inline Color Seashell()			{return Color(255,245,238);}
inline Color Sienna()			{return Color(160,82,45);}
inline Color Silver()			{return Color(192,192,192);}
inline Color Skyblue()			{return Color(135,206,235);}
inline Color Slateblue()		{return Color(106,90,205);}
inline Color Slategray()		{return Color(112,128,144);}
inline Color Slategrey()		{return Color(112,128,144);}
inline Color Snow()				{return Color(255,250,250);}
inline Color Springgreen()		{return Color(0,255,127);}
inline Color Steelblue()		{return Color(70,130,180);}
inline Color Tan()				{return Color(210,180,140);}
inline Color Teal()				{return Color(0,128,128);}
inline Color Thistle()			{return Color(216,191,216);}
inline Color Tomato()			{return Color(255,99,71);}
inline Color Turquoise()		{return Color(64,224,208);}
inline Color Violet()			{return Color(238,130,238);}
inline Color Wheat()			{return Color(245,222,179);}
//inline Color White()			{return Color(255,255,255);}
inline Color Whitesmoke()		{return Color(245,245,245);}
//inline Color Yellow()			{return Color(255,255,0);}
inline Color Yellowgreen()		{return Color(154,205,50);}

}

#endif
