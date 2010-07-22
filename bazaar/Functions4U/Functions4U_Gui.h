#ifndef _Functions4U_Functions4U_Gui_h_
#define _Functions4U_Functions4U_Gui_h_

#ifdef CTRLLIB_H

inline const RGBA *GetPixel(Image &img, int x, int y) {
	return img + x + y*img.GetWidth();
}

inline RGBA *GetPixel(ImageBuffer &img, int x, int y) {
	return img + x + y*img.GetWidth();
}

Image Rotate180(const Image& img);
Image GetRect(const Image& orig, const Rect &r);

Drawing DrawEquation(String str);
QtfRichObject QtfEquation(String str);

Image NativePathIconX(const char *path, bool folder, int flags = 0);

class EquationDraw {
public:
	EquationDraw();
	Drawing Exp(CParser& p);
	
	static Drawing Text(String text, bool italic = true, int offsetX=0, int offsetY=0, double betw = 1);
	
private:
	String ReplaceSymbols(String var);
	Drawing Term(CParser& p, bool noBracket = false);	
	Drawing Mul(CParser& p);
	
private:
	static Drawing Bracket(Drawing &data);
	static Drawing Sqrt(Drawing &right);
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

#ifdef _GridCtrl_GridCtrl_h_
Vector<Value> ReadCol(GridCtrl& grid, int col, int begin, int end);
Vector<Vector<Value> > GetGridData(GridCtrl& grid);
void SetGridData(GridCtrl& grid, Vector<Vector<Value> > &data);

NAMESPACE_UPP
template <> void Xmlize(XmlIO xml, GridCtrl& r);
END_UPP_NAMESPACE

#endif

#endif
#endif
