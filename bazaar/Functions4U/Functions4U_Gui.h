#ifndef _Functions4U_Functions4U_Gui_h_
#define _Functions4U_Functions4U_Gui_h_

#ifdef CTRLLIB_H

inline const RGBA *GetPixel(const Image &img, int x, int y) {
	return &img[y][x];
}

inline RGBA *GetPixel(ImageBuffer &img, int x, int y) {
	return &img[y][x];
}

inline bool IsValid(const Image &img, int x, int y) {
	return x >= 0 && y >= 0 && x < img.GetWidth() && y < img.GetHeight();
}

inline bool IsValid(ImageBuffer &img, int x, int y) {
	return x >= 0 && y >= 0 && x < img.GetWidth() && y < img.GetHeight();
}

template <class T> 
inline bool IsValid(const Image &img, T &t) {
	return t.x >= 0 && t.y >= 0 && t.x < img.GetWidth() && t.y < img.GetHeight();
}

template <class T> 
inline bool IsValid(ImageBuffer &img, T &t) {
	return t.x >= 0 && t.y >= 0 && t.x < img.GetWidth() && t.y < img.GetHeight();
}

Drawing DrawEquation(const String &str);
QtfRichObject QtfEquation(const String &str);

Image NativePathIconX(const char *path, bool folder, int flags = 0);

class CParserPlus : public CParser {
public:
	CParserPlus(const char *ptr) : CParser(ptr) {}
	String ReadIdPlus() throw(Error);
};

class EquationDraw {
public:
	EquationDraw();
	Drawing Exp(CParserPlus& p);
	
	static Drawing Text(String text, bool italic = true, int offsetX=0, int offsetY=0, double betw = 1);
	
private:
	String ReplaceSymbols(String var);
	Drawing Term(CParserPlus& p, bool noBracket = false);	
	String TermTrig(CParserPlus& p);
	Drawing Mul(CParserPlus& p);
	
private:
	static Drawing Bracket(Drawing &data);
	static Drawing Sqrt(Drawing &right);
	static Drawing Exponent(Drawing &right);
	static Drawing Der(Drawing &data);
	static Drawing Abs(Drawing &data);
	static Drawing Integral(Drawing &data, Drawing &sub, Drawing &sup);
	static Drawing Summat(Drawing &data, Drawing &sub, Drawing &sup);
	static Drawing Exp(Drawing &data, Drawing &exp);
	static Drawing Function(String function, Drawing &content);
	static Drawing Equal(Drawing &left, Drawing &right);
	static Drawing NumDenom(Drawing &num, Drawing &denom);
	
	static Drawing JoinCenter(Drawing &left, Drawing &right);
	
	static Drawing Expression(String str);

private:
	static Drawing SubSup(Drawing &drwText, Drawing &drwSub, Drawing &drwSup);
	static Drawing SubSup(String text, String sub, String sup);
	static Drawing SubSup(Drawing &drwText, String sub, String sup);
	static Drawing SubSupInv(Drawing &drwText, Drawing &drwSub, Drawing &drwSup);
	static Drawing SubSupInv(String text, String sub, String sup);
	static Drawing SubSupInv(Drawing &drwText, String sub, String sup);
	static Drawing JoinFlex(Drawing &left, double betw1, Drawing &right, double betw2);
	
private:
	VectorMap<String, String> symbols;
};

bool SaveImage(const Image &img, int qualityBpp, const String &fileName, String ext = "");
bool PrintImage(const Image &img, int x = 0, int y = 0, int width = Null, int height = Null);
void DrawRectLine(Draw& w, int x, int y, int width, int height, int lineWidth, const Color &color);
void DrawRectLine(Draw& w, Point &pos, Size &s, int lineWidth, const Color &color);
void DrawRectLine(Draw& w, Rect &r, int lineWidth, const Color &color);


#endif
#endif
