#ifndef _radial_radial_h
#define _radial_radial_h

#include <CtrlLib/CtrlLib.h>
#include <Geom/Geom.h>

class RadialBasisTransform : DeepCopyOption<RadialBasisTransform>
{
public:
	RadialBasisTransform() {} //: affine(Matrixf3_1()) {}
//	RadialBasisTransform(const RadialBasisTransform& rbt, int deep)
//		: sigma2(rbt.sigma2, 0), radial(rbt.radial, 0), anchor(rbt.anchor, 0), affine(rbt.affine) {}

//	bool           IsAffine() const { return radial.IsEmpty(); }
	Pointf3        Transform(Pointf3 pt) const;
//	double         MinSquared(Pointf3 pt) const;

	Color          Transform(Color c) const;

public:
	Vector<double> sigma2; // Gaussian radial: f(d) = exp(-d^2/sigma2)
	Array<Pointf3> radial; // t(s) = sum(i=0..N-1) of radial[i] * f(||s-anchor[i]||) + affine(s)
	Array<Pointf3> anchor;
//	Matrixf3       affine;
};

RadialBasisTransform CalcTransform(const Vector<Color>& source, const Vector<Color>& target);

class RadialDemo : public TopWindow {
public:
	typedef RadialDemo CLASSNAME;
	RadialDemo();

	virtual void Paint(Draw& draw);

private:
	void PaintSection(Draw& draw, Point src, Point dst, Size sz, Color lt, Color rt, Color rb);
	void Recalc();

private:
	RadialBasisTransform transform;
	ArrayCtrl array;
	Button recalc;
	ColorCtrl source, target;
};

#endif
