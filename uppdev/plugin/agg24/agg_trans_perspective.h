//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// Perspective 2D transformations
//
//----------------------------------------------------------------------------
//#ifndef AGG_TRANS_PERSPECTIVE_INCLUDED
//#define AGG_TRANS_PERSPECTIVE_INCLUDED

//#include "agg_trans_affine.h"

namespace agg
{
	//=======================================================trans_perspective
	struct trans_perspective
	{
		double sx, shy, w0, shx, sy, w1, tx, ty, w2;

		//------------------------------------------------------- Construction
		// Identity matrix
		trans_perspective() : 
			sx (1), shy(0), w0(0), 
			shx(0), sy (1), w1(0), 
			tx (0), ty (0), w2(1) {}

		// Custom matrix
		trans_perspective(double v0, double v1, double v2, 
						double v3, double v4, double v5,
						double v6, double v7, double v8) :
		sx (v0), shy(v1), w0(v2), 
		shx(v3), sy (v4), w1(v5), 
		tx (v6), ty (v7), w2(v8) {}

		// Custom matrix from m[9]
		explicit trans_perspective(const double* m) :
		sx (m[0]), shy(m[1]), w0(m[2]), 
		shx(m[3]), sy (m[4]), w1(m[5]), 
		tx (m[6]), ty (m[7]), w2(m[8]) {}

		// From affine
		explicit trans_perspective(const trans_affine& a) : 
		sx (a.sx ), shy(a.shy), w0(0), 
		shx(a.shx), sy (a.sy ), w1(0), 
		tx (a.tx ), ty (a.ty ), w2(1) {}

		// Rectangle to quadrilateral
		trans_perspective(double x1, double y1, double x2, double y2, 
						const double* quad);

		// Quadrilateral to rectangle
		trans_perspective(const double* quad, 
						double x1, double y1, double x2, double y2);

		// Arbitrary quadrilateral transformations
		trans_perspective(const double* src, const double* dst);

		//-------------------------------------- Quadrilateral transformations
		// The arguments are double[8] that are mapped to quadrilaterals:
		// x1,y1, x2,y2, x3,y3, x4,y4
		bool quad_to_quad(const double* qs, const double* qd);

		bool rect_to_quad(double x1, double y1, 
						double x2, double y2,
						const double* q);

		bool quad_to_rect(const double* q,
						double x1, double y1, 
						double x2, double y2);

		// Map square (0,0,1,1) to the quadrilateral and vice versa
		bool square_to_quad(const double* q);
		bool quad_to_square(const double* q);


		//--------------------------------------------------------- Operations
		// Reset - load an identity matrix
		const trans_perspective& reset();

		// Invert matrix. Returns false in degenerate case
		bool invert();

		// Direct transformations operations
		const trans_perspective& translate(double x, double y);
		const trans_perspective& rotate(double a);
		const trans_perspective& scale(double s);
		const trans_perspective& scale(double x, double y);

		// Multiply the matrix by another one
		const trans_perspective& multiply(const trans_perspective& m);

		// Multiply "m" by "this" and assign the result to "this"
		const trans_perspective& premultiply(const trans_perspective& m);

		// Multiply matrix to inverse of another one
		const trans_perspective& multiply_inv(const trans_perspective& m);

		// Multiply inverse of "m" by "this" and assign the result to "this"
		const trans_perspective& premultiply_inv(const trans_perspective& m);

		// Multiply the matrix by another one
		const trans_perspective& multiply(const trans_affine& m);

		// Multiply "m" by "this" and assign the result to "this"
		const trans_perspective& premultiply(const trans_affine& m);

		// Multiply the matrix by inverse of another one
		const trans_perspective& multiply_inv(const trans_affine& m);

		// Multiply inverse of "m" by "this" and assign the result to "this"
		const trans_perspective& premultiply_inv(const trans_affine& m);

		//--------------------------------------------------------- Load/Store
		void store_to(double* m) const;
		const trans_perspective& load_from(const double* m);

		//---------------------------------------------------------- Operators
		// Multiply the matrix by another one
		const trans_perspective& operator *= (const trans_perspective& m)
		{
			return multiply(m);
		}
		const trans_perspective& operator *= (const trans_affine& m)
		{
			return multiply(m);
		}

		// Multiply the matrix by inverse of another one
		const trans_perspective& operator /= (const trans_perspective& m)
		{
			return multiply_inv(m);
		}
		const trans_perspective& operator /= (const trans_affine& m)
		{
			return multiply_inv(m);
		}

		// Multiply the matrix by another one and return
		// the result in a separete matrix.
		trans_perspective operator * (const trans_perspective& m)
		{
			return trans_perspective(*this).multiply(m);
		}
		trans_perspective operator * (const trans_affine& m)
		{
			return trans_perspective(*this).multiply(m);
		}

		// Multiply the matrix by inverse of another one 
		// and return the result in a separete matrix.
		trans_perspective operator / (const trans_perspective& m)
		{
			return trans_perspective(*this).multiply_inv(m);
		}
		trans_perspective operator / (const trans_affine& m)
		{
			return trans_perspective(*this).multiply_inv(m);
		}

		// Calculate and return the inverse matrix
		trans_perspective operator ~ () const
		{
			trans_perspective ret = *this;
			ret.invert();
			return ret;
		}

		// Equal operator with default epsilon
		bool operator == (const trans_perspective& m) const
		{
			return is_equal(m, affine_epsilon);
		}

		// Not Equal operator with default epsilon
		bool operator != (const trans_perspective& m) const
		{
			return !is_equal(m, affine_epsilon);
		}

		//---------------------------------------------------- Transformations
		// Direct transformation of x and y
		void transform(double* x, double* y) const;

		// Direct transformation of x and y, affine part only
		void transform_affine(double* x, double* y) const;

		// Direct transformation of x and y, 2x2 matrix only, no translation
		void transform_2x2(double* x, double* y) const;

		// Inverse transformation of x and y. It works slow because
		// it explicitly inverts the matrix on every call. For massive 
		// operations it's better to invert() the matrix and then use 
		// direct transformations. 
		void inverse_transform(double* x, double* y) const;


		//---------------------------------------------------------- Auxiliary
		const trans_perspective& from_affine(const trans_affine& a);
		double determinant() const;
		double determinant_reciprocal() const;

		bool is_valid(double epsilon = affine_epsilon) const;
		bool is_identity(double epsilon = affine_epsilon) const;
		bool is_equal(const trans_perspective& m, 
					double epsilon = affine_epsilon) const;

		// Determine the major affine parameters. Use with caution 
		// considering possible degenerate cases.
		double scale() const;
		double rotation() const;
		void   translation(double* dx, double* dy) const;
		void   scaling(double* x, double* y) const;
		void   scaling_abs(double* x, double* y) const;



		//--------------------------------------------------------------------
		class iterator_x
		{
			double den;
			double den_step;
			double nom_x;
			double nom_x_step;
			double nom_y;
			double nom_y_step;

		public:
			double x;
			double y;

			iterator_x() {}
			iterator_x(double px, double py, double step, const trans_perspective& m) :
				den(px * m.w0 + py * m.w1 + m.w2),
				den_step(m.w0 * step),
				nom_x(px * m.sx + py * m.shx + m.tx),
				nom_x_step(step * m.sx),
				nom_y(px * m.shy + py * m.sy + m.ty),
				nom_y_step(step * m.shy),
				x(nom_x / den),
				y(nom_y / den)
			{}

			void operator ++ ()
			{
				den   += den_step;
				nom_x += nom_x_step;
				nom_y += nom_y_step;
				double d = 1.0 / den;
				x = nom_x * d;
				y = nom_y * d;
			}
		};

		//--------------------------------------------------------------------
		iterator_x begin(double x, double y, double step) const
		{
			return iterator_x(x, y, step, *this);
		}
	};


}

//#endif

