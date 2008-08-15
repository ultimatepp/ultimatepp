#include "radial.h"

Pointf3 RadialBasisTransform::Transform(Pointf3 pt) const
{
	Pointf3 out(0, 0, 0); // = pt * affine;
	for(int i = 0; i < anchor.GetCount(); i++)
		out += radial[i] * exp(-Squared(pt - anchor[i]) / sigma2[i]);
	return out;
}

Color RadialBasisTransform::Transform(Color c) const
{
	if(IsNull(c)) return c;
	Pointf3 out = Transform(Pointf3(c.GetR(), c.GetG(), c.GetB()));
	return Color(fround(minmax(out.x, 0.0, 255.0)), fround(minmax(out.y, 0.0, 255.0)), fround(minmax(out.z, 0.0, 255.0)));
}

/*
double RadialBasisTransform::MinSquared(Pointf3 pt) const
{
//	if(IsAffine())
//		return Null;
	double d = Squared(pt - anchor[0]);
	for(int i = 1; i < anchor.GetCount(); i++)
		d = min(d, Squared(pt - anchor[i]));
	return d;
}
*/

RadialBasisTransform CalcTransform(const Array<Pointf3>& source, const Array<Pointf3>& target /*, const Vector<double>& rad*/)
{
	int n = source.GetCount();
	ASSERT(n == target.GetCount());

	RadialBasisTransform rbt;

/*
	switch(n) {
	case 0: return rbt;
	case 1: rbt.affine.a = target[0] - source[0]; return rbt;
	case 2: rbt.affine = Matrixf3Affine(source[0], target[0], source[1], target[1]); return rbt;
	case 3: rbt.affine = Matrixf3Affine(source[0], target[0], source[1], target[1], source[2], target[2]); return rbt;
	case 4: rbt.affine = Matrixf3Affine(source[0], target[0], source[1], target[1], source[2], target[2], source[3], target[3]); return rbt;
	}
*/

	enum {
//		AXX, AXY, AXZ, AYX, AYY, AYZ, AZX, AZY, AZZ, BX, BY, BZ,
		RX, RY, RZ
	};
	LinearSolver solver(/*12 +*/ 3 * n);
	double xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = ymin = zmin = +1e100;
	xmax = ymax = zmax = -1e100;
	int i;
	for(i = 0; i < n; i++) {
		Pointf3 src = source[i];
		if(src.x < xmin) xmin = src.x;
		if(src.x > xmax) xmax = src.x;
		if(src.y < ymin) ymin = src.y;
		if(src.y > ymax) ymax = src.y;
		if(src.z < zmin) zmin = src.z;
		if(src.z > zmax) zmax = src.z;
	}
	double sigma2 = pow((xmax - xmin) * (ymax - ymin) * (zmax - zmin), 2.0 / 3.0) / n;

	for(i = 0; i < n; i++) {
		ASSERT(!IsNull(source[i]) && !IsNull(target[i]));
		Pointf3 si = source[i], ti = target[i];
/*
		solver(AXX, AXX)  += si.x * si.x;
		solver(AXX, AYX)  += si.y * si.x;
		solver(AXX, AZX)  += si.z * si.x;
		solver(AXX, BX)   += 1.0  * si.x;
		solver.Right(AXX) += ti.x * si.x;

		solver(AYX, AXX)  += si.x * si.y;
		solver(AYX, AYX)  += si.y * si.y;
		solver(AYX, AZX)  += si.z * si.y;
		solver(AYX, BX)   += 1.0  * si.y;
		solver.Right(AYX) += ti.x * si.y;

		solver(AZX, AXX)  += si.x * si.z;
		solver(AZX, AYX)  += si.y * si.z;
		solver(AZX, AZX)  += si.z * si.z;
		solver(AZX, BX)   += 1.0  * si.z;
		solver.Right(AZX) += ti.x * si.z;

		solver(BX, AXX)   += si.x;
		solver(BX, AYX)   += si.y;
		solver(BX, AZX)   += si.z;
		solver(BX, BX)    += 1;
		solver.Right(BX)  += ti.x;

		solver(AXY, AXY)  += si.x * si.x;
		solver(AXY, AYY)  += si.y * si.x;
		solver(AXY, AZY)  += si.z * si.x;
		solver(AXY, BY)   += 1    * si.x;
		solver.Right(AXY) += ti.y * si.x;

		solver(AYY, AXY)  += si.x * si.y;
		solver(AYY, AYY)  += si.y * si.y;
		solver(AYY, AZY)  += si.z * si.y;
		solver(AYY, BY)   += 1    * si.y;
		solver.Right(AYY) += ti.y * si.y;

		solver(AZY, AXY)  += si.x * si.z;
		solver(AZY, AYY)  += si.y * si.z;
		solver(AZY, AZY)  += si.z * si.z;
		solver(AZY, BY)   += 1    * si.z;
		solver.Right(AZY) += ti.y * si.z;

		solver(BY, AXY)   += si.x;
		solver(BY, AYY)   += si.y;
		solver(BY, AZY)   += si.z;
		solver(BY, BY)    += 1;
		solver.Right(BY)  += ti.y;

		solver(AXZ, AXZ)  += si.x * si.x;
		solver(AXZ, AYZ)  += si.y * si.x;
		solver(AXZ, AZZ)  += si.z * si.x;
		solver(AXZ, BZ)   += 1    * si.x;
		solver.Right(AXZ) += ti.z * si.x;

		solver(AYZ, AXZ)  += si.x * si.y;
		solver(AYZ, AYZ)  += si.y * si.y;
		solver(AYZ, AZZ)  += si.z * si.y;
		solver(AYZ, BZ)   += 1    * si.y;
		solver.Right(AYZ) += ti.z * si.y;

		solver(AZZ, AXZ)  += si.x * si.z;
		solver(AZZ, AYZ)  += si.y * si.z;
		solver(AZZ, AZZ)  += si.z * si.z;
		solver(AZZ, BZ)   += 1    * si.z;
		solver.Right(AZZ) += ti.z * si.z;

		solver(BZ, AXZ)   += si.x;
		solver(BZ, AYZ)   += si.y;
		solver(BZ, AZZ)   += si.z;
		solver(BZ, BZ)    += 1;
		solver.Right(BZ)  += ti.z;
*/
		int rx = RX + 3 * i, ry = RY + 3 * i, rz = RZ + 3 * i;
//		solver(rx, AXX)    = solver(ry, AXY) = solver(rz, AXZ) = si.x;
//		solver(rx, AYX)    = solver(ry, AYY) = solver(rz, AYZ) = si.y;
//		solver(rx, AZX)    = solver(ry, AZY) = solver(rz, AZZ) = si.z;
//		solver(rx, BX)     = solver(ry, BY)  = solver(rz, BZ) = 1;
		solver.Right(rx)   = ti.x;
		solver.Right(ry)   = ti.y;
		solver.Right(rz)   = ti.z;
		for(int r = 0; r < n; r++) {
//			double s2 = rad[r];
//			s2 = (s2 ? s2 * s2 : sigma2);
			solver(rx, RX + 3 * r) = solver(ry, RY + 3 * r) = solver(rz, RZ + 3 * r)
			= exp(-Squared(source[r] - si) / sigma2);
		}
	}

	Vector<double> out = solver.Solve();
/*
	rbt.affine.x.x = Nvl(out[AXX], 1.0);
	rbt.affine.x.y = Nvl(out[AXY], 0.0);
	rbt.affine.x.z = Nvl(out[AXZ], 0.0);
	rbt.affine.y.x = Nvl(out[AYX], 0.0);
	rbt.affine.y.y = Nvl(out[AYY], 1.0);
	rbt.affine.y.z = Nvl(out[AYZ], 0.0);
	rbt.affine.z.x = Nvl(out[AZX], 0.0);
	rbt.affine.z.y = Nvl(out[AZY], 0.0);
	rbt.affine.z.z = Nvl(out[AZZ], 1.0);
	rbt.affine.a.x = Nvl(out[BX], 0.0);
	rbt.affine.a.y = Nvl(out[BY], 0.0);
	rbt.affine.a.z = Nvl(out[BZ], 0.0);
*/
	for(i = 0; i < n; i++) {
		Pointf3 a(out[RX + 3 * i], out[RY + 3 * i], out[RZ + 3 * i]);
		if(!IsNull(a.x) && !IsNull(a.y) && !IsNull(a.z)
		&& (fabs(a.x) >= 1 || fabs(a.y) >= 1 || fabs(a.z) >= 1)) {
			rbt.radial.Add(a);
//			double s2 = rad[i];
			rbt.sigma2.Add(sigma2); //s2 ? s2 * s2 : sigma2);
			rbt.anchor.Add(source[i]);
		}
	}

	return rbt;
}

RadialBasisTransform CalcTransform(const Vector<Color>& source, const Vector<Color>& target)
{
	int n = source.GetCount();
	ASSERT(n == target.GetCount());
	Array<Pointf3> spt, dpt;
	spt.Reserve(n);
	dpt.Reserve(n);
	for(int i = 0; i < n; i++)
		if(!IsNull(source[i]) && !IsNull(target[i])) {
			spt.Add(Pointf3(source[i].GetR(), source[i].GetG(), source[i].GetB()));
			dpt.Add(Pointf3(target[i].GetR(), target[i].GetG(), target[i].GetB()));
		}
	return CalcTransform(spt, dpt);
}
