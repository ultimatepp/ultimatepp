#include <Core/Core.h>
#include <plugin/Eigen/Eigen.h>

using namespace Upp;

using namespace Eigen;

void NonLinearTests();
void FFTTests();

struct SerialTest {
	MatrixXd m;
	VectorXd v;
	SerialTest() : m(2, 2), v(3){}
	void Print() {
		Cout() << "\nHere is the matrix:\n" << m;
		Cout() << "\nHere is the vector:\n" << v;
	}
	void Serialize(Stream& stream) {
		::Serialize(stream, m);
		::Serialize(stream, v);
	}
	void Jsonize(JsonIO &jio) {
		jio("matrix", m)("vector", v);
	}
	void Xmlize(XmlIO &xml) {
		xml("matrix", m)("vector", v);
	}
};
		
CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	UppLog() << "Eigen library demo";
	
	// https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html
	UppLog() << "\n\nTutorial page 1 - The Matrix class";
	
	UppLog() << "\n\nCoefficient accessors";
	{
		MatrixXd m(2,2);
		m(0,0) = 3;
		m(1,0) = 2.5;
		m(0,1) = -1;
		m(1,1) = m(1,0) + m(0,1);
		UppLog() << "\nHere is the matrix m:\n" << m;
		
		VectorXd v(2);
		v(0) = 4;
		v(1) = v(0) - 1;
		UppLog() << "\nHere is the vector v:\n" << v;
	}
	UppLog() << "\n\nResizing";
	{
		MatrixXd m(2,5);
		m.resize(4,3);
		UppLog() << "\nThe matrix m is of size " << m.rows() << "x" << m.cols();
		UppLog() << "\nIt has " << m.size() << " coefficients";

		VectorXd v(2);
		v.resize(5);
		UppLog() << "\nThe vector v is of size " << v.size();
		UppLog() << "\nAs a matrix, v is of size " << v.rows() << "x" << v.cols();
	}
	UppLog() << "\n\nAssignment and resizing";
	{
		double _dat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};// Assignment from C vector
		VectorXd dat = Map<VectorXd>(_dat, sizeof(_dat)/sizeof(double));  
		UppLog() << "\nC array data is " << dat.transpose();
		
		const int dec = 5;
    	VectorXd decimated = Map<VectorXd, 0, InnerStride<dec>>(dat.data(), 1+((dat.size()-1)/dec));
    	UppLog() << "\nDecimated " << decimated.transpose();
		
		VectorXd even = Map<VectorXd, 0, InnerStride<2>>(dat.data(), (dat.size()+1)/2);
		UppLog() << "\nEven " << even.transpose();
		
		VectorXd odd = Map<VectorXd, 0, InnerStride<2>>(dat.data()+1, dat.size()/2);
		UppLog() << "\nOdd  " << odd.transpose();
	
		MatrixXf a(2,2);
		UppLog() << "\na is of size " << a.rows() << "x" << a.cols();
		MatrixXf b(3,3);
		a = b;
		UppLog() << "\na is now of size " << a.rows() << "x" << a.cols();
	}
	
	// https://eigen.tuxfamily.org/dox-devel/group__TutorialMatrixArithmetic.html
	UppLog() << "\n\nTutorial page 2 - Matrix and vector arithmetic";
	
	UppLog() << "\n\nAddition and subtraction";
	{
		Matrix2d a;
		a << 1, 2,
		     3, 4;
		MatrixXd b(2,2);
		b << 2, 3,
		     1, 4;

		UppLog() << "\na + b =\n" << a + b;
		UppLog() << "\na - b =\n" << a - b;
		UppLog() << "\nDoing a += b;";
		a += b;
		UppLog() << "\nNow a =\n" << a;

		Vector3d v(1,2,3);
		Vector3d w(1,0,0);
		UppLog() << "\n-v + w - v =\n" << -v + w - v;
	}
	UppLog() << "\n\nScalar multiplication and division";
	{
		Matrix2d a;
		a << 1, 2,
		     3, 4;
		Vector3d v(1,2,3);
		UppLog() << "\na * 2.5 =\n" << a * 2.5;
		UppLog() << "\n0.1 * v =\n" << 0.1 * v;
		UppLog() << "\nDoing v *= 2;";
		v *= 2;
		UppLog() << "\nNow v =\n" << v;
	}
	UppLog() << "\n\nTransposition and conjugation";
	{
		MatrixXcf a = MatrixXcf::Random(2,2);
		UppLog() << "\nHere is the matrix a\n" << a;
		UppLog() << "\nHere is the matrix a^T\n" << a.transpose();
		UppLog() << "\nHere is the conjugate of a\n" << a.conjugate();
		UppLog() << "\nHere is the matrix a^*\n" << a.adjoint();
		
		VectorXd v(5);
		v << 1, 2, 3, 4, 5;
		UppLog() << "\n\nInitial vector  " << v.transpose();
		UppLog() <<   "\nReversed vector " << v.reverse().transpose();
	}
	UppLog() << "\n\nMatrix-matrix and matrix-vector multiplication";
	{
		Matrix2d mat;
		mat << 1, 2,
		       3, 4;
		Vector2d u(-1,1), v(2,0);
		UppLog() << "\nHere is mat*mat:\n" << mat*mat;
		UppLog() << "\nHere is mat*u:\n" << mat*u;
		UppLog() << "\nHere is u^T*mat:\n" << u.transpose()*mat;
		UppLog() << "\nHere is u^T*v:\n" << u.transpose()*v;
		UppLog() << "\nHere is u*v^T:\n" << u*v.transpose();
		UppLog() << "\nLet's multiply mat by itself";
		mat *= mat;
		UppLog() << "\nNow mat is mat:\n" << mat;
	}
	UppLog() << "\n\nDot product and cross product";
	{
		Vector3d v(1,2,3);
	  	Vector3d w(0,1,2);
	
	  	UppLog() << "\nDot product: " << v.dot(w);
	  	double dp = v.adjoint()*w; // automatic conversion of the inner product to a scalar
	  	UppLog() << "\nDot product via a matrix product: " << dp;
	  	UppLog() << "\nCross product:\n" << v.cross(w);
	}
	UppLog() << "\n\nBasic arithmetic reduction operations";
	{
		Eigen::Matrix2d mat;
		mat << 1, 2,
		       3, 4;
		UppLog() << "\nHere is mat.sum():       " << mat.sum();
		UppLog() << "\nHere is mat.prod():      " << mat.prod();
		UppLog() << "\nHere is mat.mean():      " << mat.mean();
		UppLog() << "\nHere is mat.minCoeff():  " << mat.minCoeff();
		UppLog() << "\nHere is mat.maxCoeff():  " << mat.maxCoeff();
		UppLog() << "\nHere is mat.trace():     " << mat.trace();
		
		Matrix3f m = Matrix3f::Random();
		ptrdiff_t i, j;
		float minOfM = m.minCoeff(&i, &j);
		UppLog() << "\nHere is the matrix m:\n" << m;
		UppLog() << "\nIts minimum coefficient (" << minOfM 
		       << ") is at position (" << i << "," << j << ")\n";
		
		RowVector4i v = RowVector4i::Random();
		ptrdiff_t maxOfV = v.maxCoeff(&i);
		UppLog() << "\nHere is the vector v: " << v;
		UppLog() << "\nIts maximum coefficient (" << maxOfV 
			   << ") is at position " << i;
	}
	
	// https://eigen.tuxfamily.org/dox/group__TutorialArrayClass.html
	UppLog() << "\n\nTutorial page 3 - The Array class and coefficient-wise operations ";
	
	UppLog() << "\n\nAccessing values inside an Array";
	{
		ArrayXXf  m(2,2);
		
		// assign some values coefficient by coefficient
		m(0,0) = 1.0; m(0,1) = 2.0;
		m(1,0) = 3.0; m(1,1) = m(0,1) + m(1,0);
		
		// print values to standard output
		UppLog() << "\n" << m;
		
		// using the comma-initializer is also allowed
		m << 1.0,2.0,
		   	 3.0,4.0;
		 
		// print values to standard output
		UppLog() << "\n" << m;
	}
	UppLog() << "\n\nAddition and subtraction";
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
		UppLog() << "\na + b = " << "\n" << a + b;
		
		// Subtracting a scalar from an array
		UppLog() << "\na - 2 = " << "\n" << a - 2;
	}
	UppLog() << "\n\nArray multiplication";
	{
		ArrayXXf a(2,2);
		ArrayXXf b(2,2);
		a << 1,2,
		   	 3,4;
		b << 5,6,
		   	 7,8;
		UppLog() << "\na * b = " << "\n" << a * b;
	}
	UppLog() << "\n\nOther coefficient-wise operations";
	{
		ArrayXf a = ArrayXf::Random(5);
		a *= 2;
		UppLog() << "\na =" << "\n" << a;
		UppLog() << "\na.abs() =" << "\n" << a.abs();
		UppLog() << "\na.abs().sqrt() =" << "\n" << a.abs().sqrt();
		UppLog() << "\na.min(a.abs().sqrt()) =" << "\n" << a.min(a.abs().sqrt());
	}
	UppLog() << "\n\nConverting between array and matrix expressions";
	{
		MatrixXf m(2,2);
		MatrixXf n(2,2);
		m << 1,2,
		   	 3,4;
		n << 5,6,
		   	 7,8;
		
		UppLog() << "\n-- Matrix m*n: --" << "\n" << m * n;
		UppLog() << "\n-- Array m*n: --" << "\n" << m.array() * n.array();
		UppLog() << "\n-- With cwiseProduct: --" << "\n" << m.cwiseProduct(n);
		UppLog() << "\n-- Array m + 4: --" << "\n" << m.array() + 4;
	}
	{
		MatrixXf m(2,2);
		MatrixXf n(2,2);
		m << 1,2,
		     3,4;
		n << 5,6,
		     7,8;
		
		UppLog() << "\n-- Combination 1: --" << "\n" << (m.array() + 4).matrix() * m;
		UppLog() << "\n-- Combination 2: --" << "\n" << (m.array() * n.array()).matrix() * m;
	}
	
	// https://eigen.tuxfamily.org/dox/group__TutorialBlockOperations.html
	UppLog() << "\n\nTutorial page 4 - Block operations";
	
	UppLog() << "\n\nUsing block operations";
	{
		Eigen::MatrixXf m(4,4);
		m << 1, 2, 3, 4,
		     5, 6, 7, 8,
		     9,10,11,12,
		   	 13,14,15,16;
		UppLog() << "\nBlock in the middle\n";
		UppLog() << m.block<2,2>(1,1);
		for (ptrdiff_t i = 1; i <= 3; ++i) {
			UppLog() << "\nBlock of size " << i << "x" << i << "\n";
			UppLog() << m.block(0, 0, i, i);
		}
	}
	{
		Array22d m;
		m << 1,2,
		   	 3,4;
		Array44d a = Array44d::Constant(0.6);
		UppLog() << "\nHere is the array a:\n" << a;
		a.block<2,2>(1,1) = m;
		UppLog() << "\nHere is now a with m copied into its central 2x2 block:\n" << a;
		a.block(0,0,2,3) = a.block(2,1,2,3);
		UppLog() << "\nHere is now a with bottom-right 2x3 block copied into top-left 2x2 block:\n" << a;
	}
	UppLog() << "\n\nColumns and rows";
	{
		Eigen::MatrixXf m(3,3);
		m << 1,2,3,
			 4,5,6,
		   	 7,8,9;
		UppLog() << "\nHere is the matrix m:\n" << m;
		UppLog() << "\n2nd Row: " << m.row(1);
		m.col(2) += 3 * m.col(0);
		UppLog() << "\nAfter adding 3 times the first column into the third column, the matrix m is:\n";
		UppLog() << m;
	}
	UppLog() << "\n\nCorner-related operations";
	{
		Eigen::Matrix4f m;
		m << 1, 2, 3, 4,
		   	 5, 6, 7, 8,
		   	 9, 10,11,12,
		   	 13,14,15,16;
		UppLog() << "\nm.leftCols(2) =\n" << m.leftCols(2);
		UppLog() << "\nm.bottomRows<2>() =\n" << m.bottomRows<2>();
		m.topLeftCorner(1,3) = m.bottomRightCorner(3,1).transpose();
		UppLog() << "\nAfter assignment, m = \n" << m;
	}
	UppLog() << "\n\nBlock operations for vectors";
	{
		Eigen::ArrayXf v(6);
		v << 1, 2, 3, 4, 5, 6;
		UppLog() << "\nv.head(3) =\n" << v.head(3);
		UppLog() << "\nv.tail<3>() = \n" << v.tail<3>();
		v.segment(1,4) *= 2;
		UppLog() << "\nafter 'v.segment(1,4) *= 2', v =\n" << v;
	}
	
	// https://eigen.tuxfamily.org/dox/group__TutorialAdvancedInitialization.html
	UppLog() << "\n\nTutorial page 5 - Advanced initialization";

	UppLog() << "\n\nThe comma initializer";
	{
		RowVectorXd vec1(3);
		vec1 << 1, 2, 3;
		UppLog() << "\nvec1 = " << vec1;
		
		RowVectorXd vec2(4);
		vec2 << 1, 4, 9, 16;
		UppLog() << "\nvec2 = " << vec2;
		
		RowVectorXd joined(7);
		joined << vec1, vec2;
		UppLog() << "\njoined = " << joined;
		
		MatrixXf matA(2, 2);
		matA << 1, 2, 3, 4;
		MatrixXf matB(4, 4);
		matB << matA, matA/10, matA/10, matA;
		UppLog() << matB;
		
		Matrix3f m;
		m.row(0) << 1, 2, 3;
		m.block(1,0,2,2) << 4, 5, 7, 8;
		m.col(2).tail(2) << 6, 9;                   
		UppLog() << m;
	}
	UppLog() << "\n\nSpecial matrices and arrays";
	{
		UppLog() << "\nA fixed-size array:\n";
		Array33f a1 = Array33f::Zero();
		UppLog() << a1 << "\n\n";
		
		UppLog() << "\nA one-dimensional dynamic-size array:\n";
		ArrayXf a2 = ArrayXf::Zero(3);
		UppLog() << a2 << "\n\n";
		
		UppLog() << "\nA two-dimensional dynamic-size array:\n";
		ArrayXXf a3 = ArrayXXf::Zero(3, 4);
		UppLog() << a3 << "\n";
		
		UppLog() << "\nA two-dimensional dynamic-size array set to 1.23:\n";
		MatrixXd a4 = MatrixXd::Constant(3, 4, 1.23);
		UppLog() << a4 << "\n";
		
		ArrayXXd table(10, 4);
		table.col(0) = ArrayXd::LinSpaced(10, 0, 90);
		table.col(1) = M_PI / 180 * table.col(0);
		table.col(2) = table.col(1).sin();
		table.col(3) = table.col(1).cos();
		UppLog() << "\n  Degrees   Radians      Sine    Cosine\n";
		UppLog() << table;
		
		const ptrdiff_t size = 6;
		MatrixXd mat1(size, size);
		mat1.topLeftCorner(size/2, size/2)     = MatrixXd::Zero(size/2, size/2);
		mat1.topRightCorner(size/2, size/2)    = MatrixXd::Identity(size/2, size/2);
		mat1.bottomLeftCorner(size/2, size/2)  = MatrixXd::Identity(size/2, size/2);
		mat1.bottomRightCorner(size/2, size/2) = MatrixXd::Zero(size/2, size/2);
		UppLog() << "\n" << mat1;
		
		MatrixXd mat2(size, size);
		mat2.topLeftCorner(size/2, size/2).setZero();
		mat2.topRightCorner(size/2, size/2).setIdentity();
		mat2.bottomLeftCorner(size/2, size/2).setIdentity();
		mat2.bottomRightCorner(size/2, size/2).setZero();
		UppLog() << "\n" << mat2;
		
		MatrixXd mat3(size, size);
		mat3 << MatrixXd::Zero(size/2, size/2), MatrixXd::Identity(size/2, size/2),
		        MatrixXd::Identity(size/2, size/2), MatrixXd::Zero(size/2, size/2);
		UppLog() << "\n" << mat3;
	}
	UppLog() << "\n\nUsage as temporary objects";
	{
		MatrixXd m = MatrixXd::Random(3,3);
		m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
		UppLog() << "\nm =\n" << m;
		VectorXd v(3);
		v << 1, 2, 3;
		UppLog() << "\nm * v =\n" << m * v;
	}
	{
		MatrixXf mat = MatrixXf::Random(2, 3);
		UppLog() << mat;
		mat = (MatrixXf(2,2) << 0, 1, 1, 0).finished() * mat;
		UppLog() << mat;
	}
	
	// https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
	UppLog() << "\n\nTutorial page 6 - Linear algebra and decompositions";

	UppLog() << "\n\nBasic linear solving	Ax = b";
	{
		Matrix3f A;
		Vector3f b;
		A << 1, 2, 3,  
			 4, 5, 6,  
			 7, 8,10;
		b << 3, 3, 4;
		UppLog() << "\nHere is the matrix A:\n" << A;
		UppLog() << "\nHere is the vector b:\n" << b;
		Vector3f x = A.colPivHouseholderQr().solve(b);
		UppLog() << "\nThe solution is:\n" << x;
	}
	{
		Matrix2f A, b;
		A << 2, -1, -1, 3;
		b << 1, 2, 3, 1;
		UppLog() << "\nHere is the matrix A:\n" << A;
		UppLog() << "\nHere is the right hand side b:\n" << b;
		Matrix2f x = A.ldlt().solve(b);
		UppLog() << "\nThe solution is:\n" << x;
	}
	UppLog() << "\n\nChecking if a solution really exists";
	{
		MatrixXd A = MatrixXd::Random(100,100);
		MatrixXd b = MatrixXd::Random(100,50);
		MatrixXd x = A.fullPivLu().solve(b);
		double relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
		UppLog() << "\nThe relative error is:\n" << relative_error;
	}
	UppLog() << "\n\nComputing eigenvalues and eigenvectors";
	{
		Matrix2f A;
		A << 1, 2, 2, 3;
		UppLog() << "\nHere is the matrix A:\n" << A;
		SelfAdjointEigenSolver<Matrix2f> eigensolver(A);
		UppLog() << "\nThe eigenvalues of A are:\n" << eigensolver.eigenvalues();
		UppLog() << "\nHere's a matrix whose columns are eigenvectors of A "
		    << "corresponding to these eigenvalues:\n"
		    << eigensolver.eigenvectors();
	}
	UppLog() << "\n\nComputing inverse and determinant";
	{
		Matrix3f A;
		A << 1, 2, 1,
		     2, 1, 0,
		    -1, 1, 2;
		UppLog() << "\nHere is the matrix A:\n" << A;
		UppLog() << "\nThe determinant of A is " << A.determinant();
		UppLog() << "\nThe inverse of A is:\n" << A.inverse();
	}
	UppLog() << "\n\nLeast squares solving";
	{
		MatrixXf A = MatrixXf::Random(5, 2);
		UppLog() << "\nHere is the matrix A:\n" << A;
		VectorXf b = VectorXf::Random(5);
		UppLog() << "\nHere is the right hand side b:\n" << b;
		UppLog() << "\nThe least-squares solution is:\n"
		       << A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
	}
	UppLog() << "\n\nSeparating the computation from the construction";
	{
		Matrix2f A, b;
		LLT<Matrix2f> llt;
		A << 2, -1, -1, 3;
		b << 1, 2, 3, 1;
		UppLog() << "\nHere is the matrix A:\n" << A;
		UppLog() << "\nHere is the right hand side b:\n" << b;
		UppLog() << "\nComputing LLT decomposition...";
		llt.compute(A);
		UppLog() << "\nThe solution is:\n" << llt.solve(b);
		A(1,1)++;
		UppLog() << "\nThe matrix A is now:\n" << A;
		UppLog() << "\nComputing LLT decomposition...";
		llt.compute(A);
		UppLog() << "\nThe solution is now:\n" << llt.solve(b);
	}
	UppLog() << "\n\nRank-revealing decompositions";
	{
		Matrix3f A;
		A << 1, 2, 5,
		     2, 1, 4,
		     3, 0, 3;
		UppLog() << "\nHere is the matrix A:\n" << A;
		FullPivLU<Matrix3f> lu_decomp(A);
		UppLog() << "\nThe rank of A is " << lu_decomp.rank();
		UppLog() << "\nHere is a matrix whose columns form a basis of the null-space of A:\n"
		    << lu_decomp.kernel();
		UppLog() << "\nHere is a matrix whose columns form a basis of the column-space of A:\n"
		    << lu_decomp.image(A); // yes, have to pass the original A
	}
	{
		Matrix2d A;
		A << 2, 1,
		     2, 0.9999999999;
		FullPivLU<Matrix2d> lu(A);
		UppLog() << "\nBy default, the rank of A is found to be " << lu.rank();
		lu.setThreshold(1e-5);
		UppLog() << "\nWith threshold 1e-5, the rank of A is found to be " << lu.rank();
	}
	
	// https://eigen.tuxfamily.org/dox/group__TutorialReductionsVisitorsBroadcasting.html
	UppLog() << "\n\nTutorial page 7 - Reductions, visitors and broadcasting";
		
	UppLog() << "\n\nReductions";
	{
		Eigen::Matrix2d mat;
		mat << 1, 2,
			   3, 4;
		UppLog() << "\nHere is mat.sum():       " << mat.sum();
		UppLog() << "\nHere is mat.prod():      " << mat.prod();
		UppLog() << "\nHere is mat.mean():      " << mat.mean();
		UppLog() << "\nHere is mat.minCoeff():  " << mat.minCoeff();
		UppLog() << "\nHere is mat.maxCoeff():  " << mat.maxCoeff();
		UppLog() << "\nHere is mat.trace():     " << mat.trace();
	}	
	UppLog() << "\n\nNorm computations";
	{
		VectorXf v(2);
		MatrixXf m(2,2), n(2,2);
		
		v << -1,
		   	  2;
		
		m << 1,-2,
			-3, 4;
		
		UppLog() << "\nv.squaredNorm() = " << v.squaredNorm();
		UppLog() << "\nv.norm() = " << v.norm();
		UppLog() << "\nv.lpNorm<1>() = " << v.lpNorm<1>();
		UppLog() << "\nv.lpNorm<Infinity>() = " << v.lpNorm<Infinity>();
		
		UppLog() << "\n";
		UppLog() << "\nm.squaredNorm() = " << m.squaredNorm();
		UppLog() << "\nm.norm() = " << m.norm();
		UppLog() << "\nm.lpNorm<1>() = " << m.lpNorm<1>();
		UppLog() << "\nm.lpNorm<Infinity>() = " << m.lpNorm<Infinity>();
	}
	UppLog() << "\n\nBoolean reductions";
	{
		ArrayXXf a(2,2);
		
		a << 1,2,
		   	 3,4;
		
		UppLog() << "\n(a > 0).all()   = " << (a > 0).all();
		UppLog() << "\n(a > 0).any()   = " << (a > 0).any();
		UppLog() << "\n(a > 0).count() = " << (a > 0).count();
		UppLog() << "\n";
		UppLog() << "\n(a > 2).all()   = " << (a > 2).all();
		UppLog() << "\n(a > 2).any()   = " << (a > 2).any();
		UppLog() << "\n(a > 2).count() = " << (a > 2).count();
	}
	UppLog() << "\n\nVisitors";
	{
		Eigen::MatrixXf m(2,2);
		
		m << 1, 2,
		   	 3, 4;
		
		//get location of maximum
		MatrixXf::Index maxRow, maxCol;
		float max = m.maxCoeff(&maxRow, &maxCol);
		
		//get location of minimum
		MatrixXf::Index minRow, minCol;
		float min = m.minCoeff(&minRow, &minCol);
		
		UppLog() << "\nMax: " << max <<  ", at: " << maxRow << "," << maxCol;
		UppLog() << "\nMin: " << min << ", at: " << minRow << "," << minCol;
	}
	UppLog() << "\n\nPartial reductions";
	{
		Eigen::MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		UppLog() << "\nColumn's maximum: \n" << mat.colwise().maxCoeff();
	}
	{
		Eigen::MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		UppLog() << "\nRow's maximum: \n" << mat.rowwise().maxCoeff();
	}
	UppLog() << "\n\nCombining partial reductions with other operations";
	{
		MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		MatrixXf::Index   maxIndex;
		float maxNorm = mat.colwise().sum().maxCoeff(&maxIndex);
		
		UppLog() << "\nMaximum sum at position " << maxIndex;
		
		UppLog() << "\nThe corresponding vector is: ";
		UppLog() << "\n" << mat.col( maxIndex );
		UppLog() << "\nAnd its sum is is: " << maxNorm;
	}
	UppLog() << "\n\nBroadcasting";
	{
		Eigen::MatrixXf mat(2,4);
		Eigen::VectorXf v(2);
		
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		     
		v << 0,
		     1;
		   
		//add v to each column of m
		mat.colwise() += v;
		
		UppLog() << "\nBroadcasting result: ";
		UppLog() << "\n" << mat;
	}
	{
		Eigen::MatrixXf mat(2,4);
		Eigen::VectorXf v(4);
		
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		     
		v << 0,1,2,3;
		   
		//add v to each row of m
		mat.rowwise() += v.transpose();
		
		UppLog() << "\nBroadcasting result: ";
		UppLog() << "\n" << mat;
	}
	UppLog() << "\n\nCombining broadcasting with other operations";
	{
		Eigen::MatrixXf m(2,4);
		Eigen::VectorXf v(2);
		
		m << 1, 23, 6, 9,
		   	 3, 11, 7, 2;
		   
		v << 2,
		     3;
		
		MatrixXf::Index index;
		// find nearest neighbour
		(m.colwise() - v).colwise().squaredNorm().minCoeff(&index);
		
		UppLog() << "\nNearest neighbour is column " << index << ":";
		UppLog() << "\n" << m.col(index);
	}
		
	UppLog() << "\n\nSerializing tests";
	{
		SerialTest serialTest, serialTest_j, serialTest_x, serialTest_s;
		serialTest.m << 1, 2,
		     			4, 8;
		serialTest.v << 1, 2, 4;

		StoreAsJsonFile(serialTest, GetExeDirFile("Json.txt"));
		LoadFromJsonFile(serialTest_j, GetExeDirFile("Json.txt"));
		UppLog() << "\nJSON demo";
		serialTest_j.Print();
		
		StoreAsXMLFile(serialTest, "XMLdata", GetExeDirFile("Xml.txt"));
		LoadFromXMLFile(serialTest_x, GetExeDirFile("Xml.txt"));
		UppLog() << "\nXML demo";
		serialTest_x.Print();
		
		StoreToFile(serialTest, GetExeDirFile("Serial.dat"));
		LoadFromFile(serialTest_s, GetExeDirFile("Serial.dat"));
		UppLog() << "\nSerialization demo";
		serialTest_s.Print();
	}
	
	NonLinearTests();
	
	FFTTests();
	
	#ifdef flagDEBUG
	Cout() << "\nPress enter key to end";
	ReadStdIn();
	#endif   
}



