#include <Core/Core.h>

using namespace Upp;

#include <Eigen/Eigen.h>

using namespace Eigen;

void NonLinearTests();

CONSOLE_APP_MAIN
{
	Cout() << "\n\nCoefficient accessors";
	{
		MatrixXd m(2,2);
		m(0,0) = 3;
		m(1,0) = 2.5;
		m(0,1) = -1;
		m(1,1) = m(1,0) + m(0,1);
		Cout() << "\nHere is the matrix m:\n" << m;
		
		VectorXd v(2);
		v(0) = 4;
		v(1) = v(0) - 1;
		Cout() << "\nHere is the vector v:\n" << v;
	}
	Cout() << "\n\nResizing";
	{
		MatrixXd m(2,5);
		m.resize(4,3);
		Cout() << "\nThe matrix m is of size " << m.rows() << "x" << m.cols();
		Cout() << "\nIt has " << m.size() << " coefficients";

		VectorXd v(2);
		v.resize(5);
		Cout() << "\nThe vector v is of size " << v.size();
		Cout() << "\nAs a matrix, v is of size " << v.rows() << "x" << v.cols();
	}
	Cout() << "\n\nAssignment and resizing";
	{
		MatrixXf a(2,2);
		Cout() << "\na is of size " << a.rows() << "x" << a.cols();
		MatrixXf b(3,3);
		a = b;
		Cout() << "\na is now of size " << a.rows() << "x" << a.cols();
	}
	Cout() << "\n\nAddition and subtraction";
	{
		Matrix2d a;
		a << 1, 2,
		     3, 4;
		MatrixXd b(2,2);
		b << 2, 3,
		     1, 4;

		Cout() << "\na + b =\n" << a + b;
		Cout() << "\na - b =\n" << a - b;
		Cout() << "\nDoing a += b;";
		a += b;
		Cout() << "\nNow a =\n" << a;

		Vector3d v(1,2,3);
		Vector3d w(1,0,0);
		Cout() << "\n-v + w - v =\n" << -v + w - v;
	}
	Cout() << "\n\nScalar multiplication and division";
	{
		Matrix2d a;
		a << 1, 2,
		     3, 4;
		Vector3d v(1,2,3);
		Cout() << "\na * 2.5 =\n" << a * 2.5;
		Cout() << "\n0.1 * v =\n" << 0.1 * v;
		Cout() << "\nDoing v *= 2;";
		v *= 2;
		Cout() << "\nNow v =\n" << v;
	}
	Cout() << "\n\nTransposition and conjugation";
	{
		MatrixXcf a = MatrixXcf::Random(2,2);
		Cout() << "\nHere is the matrix a\n" << a;
		Cout() << "\nHere is the matrix a^T\n" << a.transpose();
		Cout() << "\nHere is the conjugate of a\n" << a.conjugate();
		Cout() << "\nHere is the matrix a^*\n" << a.adjoint();
	}
	Cout() << "\n\nMatrix-matrix and matrix-vector multiplication";
	{
		Matrix2d mat;
		mat << 1, 2,
		       3, 4;
		Vector2d u(-1,1), v(2,0);
		Cout() << "\nHere is mat*mat:\n" << mat*mat;
		Cout() << "\nHere is mat*u:\n" << mat*u;
		Cout() << "\nHere is u^T*mat:\n" << u.transpose()*mat;
		Cout() << "\nHere is u^T*v:\n" << u.transpose()*v;
		Cout() << "\nHere is u*v^T:\n" << u*v.transpose();
		Cout() << "\nLet's multiply mat by itself";
		mat *= mat;
		Cout() << "\nNow mat is mat:\n" << mat;
	}
	Cout() << "\n\nDot product and cross product";
	{
		Vector3d v(1,2,3);
	  	Vector3d w(0,1,2);
	
	  	Cout() << "\nDot product: " << v.dot(w);
	  	double dp = v.adjoint()*w; // automatic conversion of the inner product to a scalar
	  	Cout() << "\nDot product via a matrix product: " << dp;
	  	Cout() << "\nCross product:\n" << v.cross(w);
	}
	Cout() << "\n\nBasic arithmetic reduction operations";
	{
		Eigen::Matrix2d mat;
		mat << 1, 2,
		       3, 4;
		Cout() << "\nHere is mat.sum():       " << mat.sum();
		Cout() << "\nHere is mat.prod():      " << mat.prod();
		Cout() << "\nHere is mat.mean():      " << mat.mean();
		Cout() << "\nHere is mat.minCoeff():  " << mat.minCoeff();
		Cout() << "\nHere is mat.maxCoeff():  " << mat.maxCoeff();
		Cout() << "\nHere is mat.trace():     " << mat.trace();
		
		Matrix3f m = Matrix3f::Random();
		int i, j;
		float minOfM = m.minCoeff(&i,&j);
		Cout() << "\nHere is the matrix m:\n" << m;
		Cout() << "\nIts minimum coefficient (" << minOfM 
		       << ") is at position (" << i << "," << j << ")\n";
		
		RowVector4i v = RowVector4i::Random();
		int maxOfV = v.maxCoeff(&i);
		Cout() << "\nHere is the vector v: " << v;
		Cout() << "\nIts maximum coefficient (" << maxOfV 
			   << ") is at position " << i;
	}
	Cout() << "\n\nAccessing values inside an Array";
	{
		ArrayXXf  m(2,2);
		
		// assign some values coefficient by coefficient
		m(0,0) = 1.0; m(0,1) = 2.0;
		m(1,0) = 3.0; m(1,1) = m(0,1) + m(1,0);
		
		// print values to standard output
		Cout() << "\n" << m;
		
		// using the comma-initializer is also allowed
		m << 1.0,2.0,
		   	 3.0,4.0;
		 
		// print values to standard output
		Cout() << "\n" << m;
	}
	Cout() << "\n\nAddition and subtraction";
	{
		ArrayXXf a(3,3);
		ArrayXXf b(3,3);
		a << 1,2,3,
		   	 4,5,6,
		   	 7,8,9;
		b << 1,2,3,
		   	 1,2,3,
		   	 1,2,3;
		   
		// Adding two arrays
		Cout() << "\na + b = " << "\n" << a + b;
		
		// Subtracting a scalar from an array
		Cout() << "\na - 2 = " << "\n" << a - 2;
	}
	Cout() << "\n\nArray multiplication";
	{
		ArrayXXf a(2,2);
		ArrayXXf b(2,2);
		a << 1,2,
		   	 3,4;
		b << 5,6,
		   	 7,8;
		Cout() << "\na * b = " << "\n" << a * b;
	}
	Cout() << "\n\nOther coefficient-wise operations";
	{
		ArrayXf a = ArrayXf::Random(5);
		a *= 2;
		Cout() << "\na =" << "\n" << a;
		Cout() << "\na.abs() =" << "\n" << a.abs();
		Cout() << "\na.abs().sqrt() =" << "\n" << a.abs().sqrt();
		Cout() << "\na.min(a.abs().sqrt()) =" << "\n" << a.min(a.abs().sqrt());
	}
	Cout() << "\n\nConverting between array and matrix expressions";
	{
		MatrixXf m(2,2);
		MatrixXf n(2,2);
		MatrixXf result(2,2);
		
		m << 1,2,
		   	 3,4;
		n << 5,6,
		   	 7,8;
		
		result = m * n;
		Cout() << "\n-- Matrix m*n: --" << "\n" << result;
		result = m.array() * n.array();
		Cout() << "\n-- Array m*n: --" << "\n" << result;
		result = m.cwiseProduct(n);
		Cout() << "\n-- With cwiseProduct: --" << "\n" << result;
		result = m.array() + 4;
		Cout() << "\n-- Array m + 4: --" << "\n" << result;
	}
	{
		MatrixXf m(2,2);
		MatrixXf n(2,2);
		MatrixXf result(2,2);
		
		m << 1,2,
		     3,4;
		n << 5,6,
		     7,8;
		
		result = (m.array() + 4).matrix() * m;
		Cout() << "\n-- Combination 1: --" << "\n" << result;
		result = (m.array() * n.array()).matrix() * m;
		Cout() << "\n-- Combination 2: --" << "\n" << result;
	}

	// Next to add: http://eigen.tuxfamily.org/dox/TutorialBlockOperations.html
		
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	NonLinearTests();
	
	Cout() << "\nPress enter to end";
	ReadStdIn();
}



