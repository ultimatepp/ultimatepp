#include "agg24.h"  //aris002

namespace agg
{


	//------------------------------------------------------------------------
	inline bool trans_perspective::square_to_quad(const double* q)
	{
		double dx = q[0] - q[2] + q[4] - q[6];
		double dy = q[1] - q[3] + q[5] - q[7];
		if(dx == 0.0 && dy == 0.0)
		{   
			// Affine case (parallelogram)
			//---------------
			sx  = q[2] - q[0];
			shy = q[3] - q[1];
			w0  = 0.0;
			shx = q[4] - q[2];
			sy  = q[5] - q[3];
			w1  = 0.0;
			tx  = q[0];
			ty  = q[1];
			w2  = 1.0;
		}
		else
		{
			double dx1 = q[2] - q[4];
			double dy1 = q[3] - q[5];
			double dx2 = q[6] - q[4];
			double dy2 = q[7] - q[5];
			double den = dx1 * dy2 - dx2 * dy1;
			if(den == 0.0)
			{
				// Singular case
				//---------------
				sx = shy = w0 = shx = sy = w1 = tx = ty = w2 = 0.0;
				return false;
			}
			// General case
			//---------------
			double u = (dx * dy2 - dy * dx2) / den;
			double v = (dy * dx1 - dx * dy1) / den;
			sx  = q[2] - q[0] + u * q[2];
			shy = q[3] - q[1] + u * q[3];
			w0  = u;
			shx = q[6] - q[0] + v * q[6];
			sy  = q[7] - q[1] + v * q[7];
			w1  = v;
			tx  = q[0];
			ty  = q[1];
			w2  = 1.0;
		}
		return true;
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::invert()
	{
		double d0 = sy  * w2 - w1  * ty;
		double d1 = w0  * ty - shy * w2;
		double d2 = shy * w1 - w0  * sy;
		double d  = sx  * d0 + shx * d1 + tx * d2;
		if(d == 0.0) 
		{
			sx = shy = w0 = shx = sy = w1 = tx = ty = w2 = 0.0;
			return false;
		}
		d = 1.0 / d;
		trans_perspective a = *this;
		sx  = d * d0;
		shy = d * d1;
		w0  = d * d2;
		shx = d * (a.w1 *a.tx  - a.shx*a.w2);
		sy  = d * (a.sx *a.w2  - a.w0 *a.tx);
		w1  = d * (a.w0 *a.shx - a.sx *a.w1);
		tx  = d * (a.shx*a.ty  - a.sy *a.tx);
		ty  = d * (a.shy*a.tx  - a.sx *a.ty);
		w2  = d * (a.sx *a.sy  - a.shy*a.shx);
		return true;
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::quad_to_square(const double* q)
	{
		if(!square_to_quad(q)) return false;
		invert();
		return true;
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::quad_to_quad(const double* qs, 
												const double* qd)
	{
		trans_perspective p;
		if(!  quad_to_square(qs)) return false;
		if(!p.square_to_quad(qd)) return false;
		multiply(p);
		return true;
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::rect_to_quad(double x1, double y1, 
												double x2, double y2,
												const double* q)
	{
		double r[8];
		r[0] = r[6] = x1;
		r[2] = r[4] = x2;
		r[1] = r[3] = y1;
		r[5] = r[7] = y2;
		return quad_to_quad(r, q);
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::quad_to_rect(const double* q,
												double x1, double y1, 
												double x2, double y2)
	{
		double r[8];
		r[0] = r[6] = x1;
		r[2] = r[4] = x2;
		r[1] = r[3] = y1;
		r[5] = r[7] = y2;
		return quad_to_quad(q, r);
	}

	//------------------------------------------------------------------------
	inline trans_perspective::trans_perspective(double x1, double y1, 
												double x2, double y2, 
												const double* quad)
	{
		rect_to_quad(x1, y1, x2, y2, quad);
	}

	//------------------------------------------------------------------------
	inline trans_perspective::trans_perspective(const double* quad, 
												double x1, double y1, 
												double x2, double y2)
	{
		quad_to_rect(quad, x1, y1, x2, y2);
	}

	//------------------------------------------------------------------------
	inline trans_perspective::trans_perspective(const double* src, 
												const double* dst) 
	{
		quad_to_quad(src, dst);
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& trans_perspective::reset()
	{
		sx  = 1; shy = 0; w0 = 0; 
		shx = 0; sy  = 1; w1 = 0;
		tx  = 0; ty  = 0; w2 = 1;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::multiply(const trans_perspective& a)
	{
		trans_perspective b = *this;
		sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
		shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
		tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
		shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
		sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
		ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
		w0  = a.w0 *b.sx  + a.w1 *b.shy + a.w2*b.w0;
		w1  = a.w0 *b.shx + a.w1 *b.sy  + a.w2*b.w1;
		w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2*b.w2;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::multiply(const trans_affine& a)
	{
		trans_perspective b = *this;
		sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
		shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
		tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
		shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
		sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
		ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::premultiply(const trans_perspective& b)
	{
		trans_perspective a = *this;
		sx  = a.sx *b.sx  + a.shx*b.shy + a.tx*b.w0;
		shx = a.sx *b.shx + a.shx*b.sy  + a.tx*b.w1;
		tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx*b.w2;
		shy = a.shy*b.sx  + a.sy *b.shy + a.ty*b.w0;
		sy  = a.shy*b.shx + a.sy *b.sy  + a.ty*b.w1;
		ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty*b.w2;
		w0  = a.w0 *b.sx  + a.w1 *b.shy + a.w2*b.w0;
		w1  = a.w0 *b.shx + a.w1 *b.sy  + a.w2*b.w1;
		w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2*b.w2;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::premultiply(const trans_affine& b)
	{
		trans_perspective a = *this;
		sx  = a.sx *b.sx  + a.shx*b.shy;
		shx = a.sx *b.shx + a.shx*b.sy;
		tx  = a.sx *b.tx  + a.shx*b.ty  + a.tx;
		shy = a.shy*b.sx  + a.sy *b.shy;
		sy  = a.shy*b.shx + a.sy *b.sy;
		ty  = a.shy*b.tx  + a.sy *b.ty  + a.ty;
		w0  = a.w0 *b.sx  + a.w1 *b.shy;
		w1  = a.w0 *b.shx + a.w1 *b.sy;
		w2  = a.w0 *b.tx  + a.w1 *b.ty  + a.w2;
		return *this;
	}

	//------------------------------------------------------------------------
	const trans_perspective& 
	trans_perspective::multiply_inv(const trans_perspective& m)
	{
		trans_perspective t = m;
		t.invert();
		return multiply(t);
	}

	//------------------------------------------------------------------------
	const trans_perspective&
	trans_perspective::multiply_inv(const trans_affine& m)
	{
		trans_affine t = m;
		t.invert();
		return multiply(t);
	}

	//------------------------------------------------------------------------
	const trans_perspective&
	trans_perspective::premultiply_inv(const trans_perspective& m)
	{
		trans_perspective t = m;
		t.invert();
		return *this = t.multiply(*this);
	}

	//------------------------------------------------------------------------
	const trans_perspective&
	trans_perspective::premultiply_inv(const trans_affine& m)
	{
		trans_perspective t(m);
		t.invert();
		return *this = t.multiply(*this);
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::translate(double x, double y)
	{
		tx += x;
		ty += y;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& trans_perspective::rotate(double a)
	{
		multiply(trans_affine_rotation(a));
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& trans_perspective::scale(double s)
	{
		multiply(trans_affine_scaling(s));
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& trans_perspective::scale(double x, double y)
	{
		multiply(trans_affine_scaling(x, y));
		return *this;
	}

	//------------------------------------------------------------------------
	inline void trans_perspective::transform(double* px, double* py) const
	{
		double x = *px;
		double y = *py;
		double m = 1.0 / (x*w0 + y*w1 + w2);
		*px = m * (x*sx  + y*shx + tx);
		*py = m * (x*shy + y*sy  + ty);
	}

	//------------------------------------------------------------------------
	inline void trans_perspective::transform_affine(double* x, double* y) const
	{
		double tmp = *x;
		*x = tmp * sx  + *y * shx + tx;
		*y = tmp * shy + *y * sy  + ty;
	}

	//------------------------------------------------------------------------
	inline void trans_perspective::transform_2x2(double* x, double* y) const
	{
		double tmp = *x;
		*x = tmp * sx  + *y * shx;
		*y = tmp * shy + *y * sy;
	}

	//------------------------------------------------------------------------
	inline void trans_perspective::inverse_transform(double* x, double* y) const
	{
		trans_perspective t(*this);
		if(t.invert()) t.transform(x, y);
	}

	//------------------------------------------------------------------------
	inline void trans_perspective::store_to(double* m) const
	{
		*m++ = sx;  *m++ = shy; *m++ = w0; 
		*m++ = shx; *m++ = sy;  *m++ = w1;
		*m++ = tx;  *m++ = ty;  *m++ = w2;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& trans_perspective::load_from(const double* m)
	{
		sx  = *m++; shy = *m++; w0 = *m++; 
		shx = *m++; sy  = *m++; w1 = *m++;
		tx  = *m++; ty  = *m++; w2 = *m++;
		return *this;
	}

	//------------------------------------------------------------------------
	inline const trans_perspective& 
	trans_perspective::from_affine(const trans_affine& a)
	{
		sx  = a.sx;  shy = a.shy; w0 = 0; 
		shx = a.shx; sy  = a.sy;  w1 = 0;
		tx  = a.tx;  ty  = a.ty;  w2 = 1;
		return *this;
	}

	//------------------------------------------------------------------------
	inline double trans_perspective::determinant() const
	{
		return sx  * (sy  * w2 - ty  * w1) +
			shx * (ty  * w0 - shy * w2) +
			tx  * (shy * w1 - sy  * w0);
	}

	//------------------------------------------------------------------------
	inline double trans_perspective::determinant_reciprocal() const
	{
		return 1.0 / determinant();
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::is_valid(double epsilon) const
	{
		return fabs(sx) > epsilon && fabs(sy) > epsilon && fabs(w2) > epsilon;
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::is_identity(double epsilon) const
	{
		return is_equal_eps(sx,  1.0, epsilon) &&
			is_equal_eps(shy, 0.0, epsilon) &&
			is_equal_eps(w0,  0.0, epsilon) &&
			is_equal_eps(shx, 0.0, epsilon) && 
			is_equal_eps(sy,  1.0, epsilon) &&
			is_equal_eps(w1,  0.0, epsilon) &&
			is_equal_eps(tx,  0.0, epsilon) &&
			is_equal_eps(ty,  0.0, epsilon) &&
			is_equal_eps(w2,  1.0, epsilon);
	}

	//------------------------------------------------------------------------
	inline bool trans_perspective::is_equal(const trans_perspective& m, 
											double epsilon) const
	{
		return is_equal_eps(sx,  m.sx,  epsilon) &&
			is_equal_eps(shy, m.shy, epsilon) &&
			is_equal_eps(w0,  m.w0,  epsilon) &&
			is_equal_eps(shx, m.shx, epsilon) && 
			is_equal_eps(sy,  m.sy,  epsilon) &&
			is_equal_eps(w1,  m.w1,  epsilon) &&
			is_equal_eps(tx,  m.tx,  epsilon) &&
			is_equal_eps(ty,  m.ty,  epsilon) &&
			is_equal_eps(w2,  m.w2,  epsilon);
	}

	//------------------------------------------------------------------------
	inline double trans_perspective::scale() const
	{
		double x = 0.707106781 * sx  + 0.707106781 * shx;
		double y = 0.707106781 * shy + 0.707106781 * sy;
		return sqrt(x*x + y*y);
	}

	//------------------------------------------------------------------------
	inline double trans_perspective::rotation() const
	{
		double x1 = 0.0;
		double y1 = 0.0;
		double x2 = 1.0;
		double y2 = 0.0;
		transform(&x1, &y1);
		transform(&x2, &y2);
		return atan2(y2-y1, x2-x1);
	}

	//------------------------------------------------------------------------
	void trans_perspective::translation(double* dx, double* dy) const
	{
		*dx = tx;
		*dy = ty;
	}

	//------------------------------------------------------------------------
	void trans_perspective::scaling(double* x, double* y) const
	{
		double x1 = 0.0;
		double y1 = 0.0;
		double x2 = 1.0;
		double y2 = 1.0;
		trans_perspective t(*this);
		t *= trans_affine_rotation(-rotation());
		t.transform(&x1, &y1);
		t.transform(&x2, &y2);
		*x = x2 - x1;
		*y = y2 - y1;
	}

	//------------------------------------------------------------------------
	void trans_perspective::scaling_abs(double* x, double* y) const
	{
		*x = sqrt(sx  * sx  + shx * shx);
		*y = sqrt(shy * shy + sy  * sy);
	}

}
