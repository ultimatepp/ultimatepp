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

		std::stringstream res1;
  		res1 << a + b;
  		Cout() << "\na + b =\n" << String(res1.str());
  		std::stringstream res2;
  		res2 << a - b;
  		Cout() << "\na - b =\n" << String(res2.str());
  		Cout() << "\nDoing a += b;";
  		
  		a += b;
  		
  		Cout() << "\nNow a =\n" << a;
  		Vector3d v(1,2,3);
  		Vector3d w(1,0,0);
  		std::stringstream res3;
  		res3 << -v + w - v;
  		Cout() << "\n-v + w - v =\n" << String(res3.str());
	}
	Cout() << "\n\nScalar multiplication and division";
	{
		Matrix2d a;
		a << 1, 2,
		     3, 4;
		Vector3d v(1,2,3);
		std::stringstream res1;
		res1 << a * 2.5;
		Cout() << "\na * 2.5 =\n" << String(res1.str());
		std::stringstream res2;
		res2 << 0.1 * v;
		Cout() << "\n0.1 * v =\n" << String(res2.str());
		Cout() << "\nDoing v *= 2;";
		v *= 2;
		Cout() << "\nNow v =\n" << v;
	}
	Cout() << "\n\nTransposition and conjugation";
	{	
		MatrixXcf a = MatrixXcf::Random(2,2);
		Cout() << "\nHere is the matrix a\n" << a;
		std::stringstream res1;
		res1 << a.transpose();
		Cout() << "\nHere is the matrix a^T\n" << String(res1.str());
		std::stringstream res2;
		res2 << a.conjugate();
		Cout() << "\nHere is the conjugate of a\n" << String(res2.str());
		std::stringstream res3;
		res3 << a.adjoint();
		Cout() << "\nHere is the matrix a^*\n" << String(res3.str());
	}
	Cout() << "\n\nMatrix-matrix and matrix-vector multiplication";
	{ 
		Matrix2d mat;
		mat << 1, 2,
		 	   3, 4;
		Vector2d u(-1,1), v(2,0);
		std::stringstream res1;
		res1 << mat*mat;
		Cout() << "\nHere is mat*mat:\n" << String(res1.str());
		std::stringstream res2;
		res1 << mat*u;
		Cout() << "\nHere is mat*u:\n" << String(res2.str());
		std::stringstream res3;
		res3 << u.transpose()*mat;
		Cout() << "\nHere is u^T*mat:\n" << String(res3.str());
		std::stringstream res4;
		res4 << u.transpose()*v;
		Cout() << "\nHere is u^T*v:\n" << String(res4.str());
		std::stringstream res5;
		res5 << u*v.transpose();
		Cout() << "\nHere is u*v^T:\n" << String(res5.str());
		Cout() << "\nLet's multiply mat by itself";
		mat = mat*mat;
		Cout() << "\nNow mat is mat:\n" << mat;		
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
		std::ptrdiff_t i, j;
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
	puts("\nPress enter to continue");
	getchar();
	
	NonLinearTests();
	
	puts("\nPress enter to end");
	getchar();	
}



