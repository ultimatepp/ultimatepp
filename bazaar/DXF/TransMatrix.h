#ifndef _DXF_TransMatrix_h_
#define _DXF_TransMatrix_h_

#include <Core/Core.h>

using namespace Upp;

// a transformation matrix class
class TransMatrix : Moveable<TransMatrix>
{
	private:
		// values of transformation matrix
		// we store here just 4 values, no need to store the whole matrix
		double c, s, tx, ty;
		
	protected:
	
	public:
	
		// constructors
		TransMatrix(void);
		TransMatrix(TransMatrix const &m);
	
		// sets to null transformation
		TransMatrix &SetIdentity(void);
		
		// multiply by another matrix
		TransMatrix &Multiply(TransMatrix const &m);
		
		// invert
		TransMatrix &Invert(TransMatrix const &m);
		
		// translate by given distance
		TransMatrix &Translate(double dx, double dy);
		TransMatrix &Translate(Pointf const &p);
		
		// rotate
		TransMatrix &Rotate(double a);
		TransMatrix &Rotate(Pointf const &center, double a);
		
		// scale
		TransMatrix &Scale(double s);
		
		// transform a point
		Pointf operator()(Pointf const &p) const;
		
		// rotate an angle by rotation part of matrix
		double R(double a) const;
		
		// scale a distance with scale part of matrix
		double S(double d) const;
		
		// assignement
		TransMatrix &operator=(TransMatrix const &m);
};
#endif
