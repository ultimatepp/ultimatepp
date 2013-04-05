#include <Core/Core.h>

using namespace Upp;

#include <plugin/Eigen/Eigen.h>

using namespace Eigen;

void NonLinearTests();

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
	// http://eigen.tuxfamily.org/dox/TutorialMatrixClass.html
	Cout() << "\n\nTutorial page 1 - The Matrix class";
	
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
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	// http://eigen.tuxfamily.org/dox/TutorialMatrixArithmetic.html
	Cout() << "\n\nTutorial page 2 - Matrix and vector arithmetic";
	
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
		ptrdiff_t i, j;
		float minOfM = m.minCoeff(&i, &j);
		Cout() << "\nHere is the matrix m:\n" << m;
		Cout() << "\nIts minimum coefficient (" << minOfM 
		       << ") is at position (" << i << "," << j << ")\n";
		
		RowVector4i v = RowVector4i::Random();
		ptrdiff_t maxOfV = v.maxCoeff(&i);
		Cout() << "\nHere is the vector v: " << v;
		Cout() << "\nIts maximum coefficient (" << maxOfV 
			   << ") is at position " << i;
	}
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	// http://eigen.tuxfamily.org/dox/TutorialArrayClass.html
	Cout() << "\n\nTutorial page 3 - The Array class and coefficient-wise operations ";
	
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
		m << 1,2,
		   	 3,4;
		n << 5,6,
		   	 7,8;
		
		Cout() << "\n-- Matrix m*n: --" << "\n" << m * n;
		Cout() << "\n-- Array m*n: --" << "\n" << m.array() * n.array();
		Cout() << "\n-- With cwiseProduct: --" << "\n" << m.cwiseProduct(n);
		Cout() << "\n-- Array m + 4: --" << "\n" << m.array() + 4;
	}
	{
		MatrixXf m(2,2);
		MatrixXf n(2,2);
		m << 1,2,
		     3,4;
		n << 5,6,
		     7,8;
		
		Cout() << "\n-- Combination 1: --" << "\n" << (m.array() + 4).matrix() * m;
		Cout() << "\n-- Combination 2: --" << "\n" << (m.array() * n.array()).matrix() * m;
	}
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	// http://eigen.tuxfamily.org/dox/TutorialBlockOperations.html
	Cout() << "\n\nTutorial page 4 - Block operations";
	
	Cout() << "\n\nUsing block operations";
	{
		Eigen::MatrixXf m(4,4);
		m << 1, 2, 3, 4,
		     5, 6, 7, 8,
		     9,10,11,12,
		   	 13,14,15,16;
		Cout() << "\nBlock in the middle\n";
		Cout() << m.block<2,2>(1,1);
		for (ptrdiff_t i = 1; i <= 3; ++i) {
			Cout() << "\nBlock of size " << i << "x" << i << "\n";
			Cout() << m.block(0, 0, i, i);
		}
	}
	{
		Array22d m;
		m << 1,2,
		   	 3,4;
		Array44d a = Array44d::Constant(0.6);
		Cout() << "\nHere is the array a:\n" << a;
		a.block<2,2>(1,1) = m;
		Cout() << "\nHere is now a with m copied into its central 2x2 block:\n" << a;
		a.block(0,0,2,3) = a.block(2,1,2,3);
		Cout() << "\nHere is now a with bottom-right 2x3 block copied into top-left 2x2 block:\n" << a;
	}
	Cout() << "\n\nColumns and rows";
	{
		Eigen::MatrixXf m(3,3);
		m << 1,2,3,
			 4,5,6,
		   	 7,8,9;
		Cout() << "\nHere is the matrix m:\n" << m;
		Cout() << "\n2nd Row: " << m.row(1);
		m.col(2) += 3 * m.col(0);
		Cout() << "\nAfter adding 3 times the first column into the third column, the matrix m is:\n";
		Cout() << m;
	}
	Cout() << "\n\nCorner-related operations";
	{
		Eigen::Matrix4f m;
		m << 1, 2, 3, 4,
		   	 5, 6, 7, 8,
		   	 9, 10,11,12,
		   	 13,14,15,16;
		Cout() << "\nm.leftCols(2) =\n" << m.leftCols(2);
		Cout() << "\nm.bottomRows<2>() =\n" << m.bottomRows<2>();
		m.topLeftCorner(1,3) = m.bottomRightCorner(3,1).transpose();
		Cout() << "\nAfter assignment, m = \n" << m;
	}
	Cout() << "\n\nBlock operations for vectors";
	{
		Eigen::ArrayXf v(6);
		v << 1, 2, 3, 4, 5, 6;
		Cout() << "\nv.head(3) =\n" << v.head(3);
		Cout() << "\nv.tail<3>() = \n" << v.tail<3>();
		v.segment(1,4) *= 2;
		Cout() << "\nafter 'v.segment(1,4) *= 2', v =\n" << v;
	}
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	//http://eigen.tuxfamily.org/dox/TutorialAdvancedInitialization.html
	Cout() << "\n\nTutorial page 5 - Advanced initialization";

	Cout() << "\n\nThe comma initializer";
	{
		RowVectorXd vec1(3);
		vec1 << 1, 2, 3;
		Cout() << "\nvec1 = " << vec1;
		
		RowVectorXd vec2(4);
		vec2 << 1, 4, 9, 16;
		Cout() << "\nvec2 = " << vec2;
		
		RowVectorXd joined(7);
		joined << vec1, vec2;
		Cout() << "\njoined = " << joined;
		
		MatrixXf matA(2, 2);
		matA << 1, 2, 3, 4;
		MatrixXf matB(4, 4);
		matB << matA, matA/10, matA/10, matA;
		Cout() << matB;
		
		Matrix3f m;
		m.row(0) << 1, 2, 3;
		m.block(1,0,2,2) << 4, 5, 7, 8;
		m.col(2).tail(2) << 6, 9;                   
		Cout() << m;
	}
	Cout() << "\n\nSpecial matrices and arrays";
	{
		Cout() << "\nA fixed-size array:\n";
		Array33f a1 = Array33f::Zero();
		Cout() << a1 << "\n\n";
		
		Cout() << "\nA one-dimensional dynamic-size array:\n";
		ArrayXf a2 = ArrayXf::Zero(3);
		Cout() << a2 << "\n\n";
		
		Cout() << "\nA two-dimensional dynamic-size array:\n";
		ArrayXXf a3 = ArrayXXf::Zero(3, 4);
		Cout() << a3 << "\n";
		
		Cout() << "\nA two-dimensional dynamic-size array set to 1.23:\n";
		MatrixXd a4 = MatrixXd::Constant(3, 4, 1.23);
		Cout() << a4 << "\n";
		
		ArrayXXd table(10, 4);
		table.col(0) = ArrayXd::LinSpaced(10, 0, 90);
		table.col(1) = M_PI / 180 * table.col(0);
		table.col(2) = table.col(1).sin();
		table.col(3) = table.col(1).cos();
		Cout() << "\n  Degrees   Radians      Sine    Cosine\n";
		Cout() << table;
		
		const ptrdiff_t size = 6;
		MatrixXd mat1(size, size);
		mat1.topLeftCorner(size/2, size/2)     = MatrixXd::Zero(size/2, size/2);
		mat1.topRightCorner(size/2, size/2)    = MatrixXd::Identity(size/2, size/2);
		mat1.bottomLeftCorner(size/2, size/2)  = MatrixXd::Identity(size/2, size/2);
		mat1.bottomRightCorner(size/2, size/2) = MatrixXd::Zero(size/2, size/2);
		Cout() << "\n" << mat1;
		
		MatrixXd mat2(size, size);
		mat2.topLeftCorner(size/2, size/2).setZero();
		mat2.topRightCorner(size/2, size/2).setIdentity();
		mat2.bottomLeftCorner(size/2, size/2).setIdentity();
		mat2.bottomRightCorner(size/2, size/2).setZero();
		Cout() << "\n" << mat2;
		
		MatrixXd mat3(size, size);
		mat3 << MatrixXd::Zero(size/2, size/2), MatrixXd::Identity(size/2, size/2),
		        MatrixXd::Identity(size/2, size/2), MatrixXd::Zero(size/2, size/2);
		Cout() << "\n" << mat3;
	}
	Cout() << "\n\nUsage as temporary objects";
	{
		MatrixXd m = MatrixXd::Random(3,3);
		m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
		Cout() << "\nm =\n" << m;
		VectorXd v(3);
		v << 1, 2, 3;
		Cout() << "\nm * v =\n" << m * v;
	}
	{
		MatrixXf mat = MatrixXf::Random(2, 3);
		Cout() << mat;
		mat = (MatrixXf(2,2) << 0, 1, 1, 0).finished() * mat;
		Cout() << mat;
	}
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	//http://eigen.tuxfamily.org/dox/TutorialLinearAlgebra.html
	Cout() << "\n\nTutorial page 6 - Linear algebra and decompositions";

	Cout() << "\n\nBasic linear solving	Ax = b";
	{
		Matrix3f A;
		Vector3f b;
		A << 1, 2, 3,  
			 4, 5, 6,  
			 7, 8,10;
		b << 3, 3, 4;
		Cout() << "\nHere is the matrix A:\n" << A;
		Cout() << "\nHere is the vector b:\n" << b;
		Vector3f x = A.colPivHouseholderQr().solve(b);
		Cout() << "\nThe solution is:\n" << x;
	}
	{
		Matrix2f A, b;
		A << 2, -1, -1, 3;
		b << 1, 2, 3, 1;
		Cout() << "\nHere is the matrix A:\n" << A;
		Cout() << "\nHere is the right hand side b:\n" << b;
		Matrix2f x = A.ldlt().solve(b);
		Cout() << "\nThe solution is:\n" << x;
	}
	Cout() << "\n\nChecking if a solution really exists";
	{
		MatrixXd A = MatrixXd::Random(100,100);
		MatrixXd b = MatrixXd::Random(100,50);
		MatrixXd x = A.fullPivLu().solve(b);
		double relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
		Cout() << "\nThe relative error is:\n" << relative_error;
	}
	Cout() << "\n\nComputing eigenvalues and eigenvectors";
	{
		Matrix2f A;
		A << 1, 2, 2, 3;
		Cout() << "\nHere is the matrix A:\n" << A;
		SelfAdjointEigenSolver<Matrix2f> eigensolver(A);
		Cout() << "\nThe eigenvalues of A are:\n" << eigensolver.eigenvalues();
		Cout() << "\nHere's a matrix whose columns are eigenvectors of A "
		    << "corresponding to these eigenvalues:\n"
		    << eigensolver.eigenvectors();
	}
	Cout() << "\n\nComputing inverse and determinant";
	{
		Matrix3f A;
		A << 1, 2, 1,
		     2, 1, 0,
		    -1, 1, 2;
		Cout() << "\nHere is the matrix A:\n" << A;
		Cout() << "\nThe determinant of A is " << A.determinant();
		Cout() << "\nThe inverse of A is:\n" << A.inverse();
	}
	Cout() << "\n\nLeast squares solving";
	{
		MatrixXf A = MatrixXf::Random(5, 2);
		Cout() << "\nHere is the matrix A:\n" << A;
		VectorXf b = VectorXf::Random(5);
		Cout() << "\nHere is the right hand side b:\n" << b;
		Cout() << "\nThe least-squares solution is:\n"
		       << A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
	}
	Cout() << "\n\nSeparating the computation from the construction";
	{
		Matrix2f A, b;
		LLT<Matrix2f> llt;
		A << 2, -1, -1, 3;
		b << 1, 2, 3, 1;
		Cout() << "\nHere is the matrix A:\n" << A;
		Cout() << "\nHere is the right hand side b:\n" << b;
		Cout() << "\nComputing LLT decomposition...";
		llt.compute(A);
		Cout() << "\nThe solution is:\n" << llt.solve(b);
		A(1,1)++;
		Cout() << "\nThe matrix A is now:\n" << A;
		Cout() << "\nComputing LLT decomposition...";
		llt.compute(A);
		Cout() << "\nThe solution is now:\n" << llt.solve(b);
	}
	Cout() << "\n\nRank-revealing decompositions";
	{
		Matrix3f A;
		A << 1, 2, 5,
		     2, 1, 4,
		     3, 0, 3;
		Cout() << "\nHere is the matrix A:\n" << A;
		FullPivLU<Matrix3f> lu_decomp(A);
		Cout() << "\nThe rank of A is " << lu_decomp.rank();
		Cout() << "\nHere is a matrix whose columns form a basis of the null-space of A:\n"
		    << lu_decomp.kernel();
		Cout() << "\nHere is a matrix whose columns form a basis of the column-space of A:\n"
		    << lu_decomp.image(A); // yes, have to pass the original A
	}
	{
		Matrix2d A;
		A << 2, 1,
		     2, 0.9999999999;
		FullPivLU<Matrix2d> lu(A);
		Cout() << "\nBy default, the rank of A is found to be " << lu.rank();
		lu.setThreshold(1e-5);
		Cout() << "\nWith threshold 1e-5, the rank of A is found to be " << lu.rank();
	}
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	// http://eigen.tuxfamily.org/dox/TutorialReductionsVisitorsBroadcasting.html
	Cout() << "\n\nTutorial page 7 - Reductions, visitors and broadcasting";
		
	Cout() << "\n\nReductions";
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
	}	
	Cout() << "\n\nNorm computations";
	{
		VectorXf v(2);
		MatrixXf m(2,2), n(2,2);
		
		v << -1,
		   	  2;
		
		m << 1,-2,
			-3, 4;
		
		Cout() << "\nv.squaredNorm() = " << v.squaredNorm();
		Cout() << "\nv.norm() = " << v.norm();
		Cout() << "\nv.lpNorm<1>() = " << v.lpNorm<1>();
		Cout() << "\nv.lpNorm<Infinity>() = " << v.lpNorm<Infinity>();
		
		Cout() << "\n";
		Cout() << "\nm.squaredNorm() = " << m.squaredNorm();
		Cout() << "\nm.norm() = " << m.norm();
		Cout() << "\nm.lpNorm<1>() = " << m.lpNorm<1>();
		Cout() << "\nm.lpNorm<Infinity>() = " << m.lpNorm<Infinity>();
	}
	Cout() << "\n\nBoolean reductions";
	{
		ArrayXXf a(2,2);
		
		a << 1,2,
		   	 3,4;
		
		Cout() << "\n(a > 0).all()   = " << (a > 0).all();
		Cout() << "\n(a > 0).any()   = " << (a > 0).any();
		Cout() << "\n(a > 0).count() = " << (a > 0).count();
		Cout() << "\n";
		Cout() << "\n(a > 2).all()   = " << (a > 2).all();
		Cout() << "\n(a > 2).any()   = " << (a > 2).any();
		Cout() << "\n(a > 2).count() = " << (a > 2).count();
	}
	Cout() << "\n\nVisitors";
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
		
		Cout() << "\nMax: " << max <<  ", at: " << maxRow << "," << maxCol;
		Cout() << "\nMin: " << min << ", at: " << minRow << "," << minCol;
	}
	Cout() << "\n\nPartial reductions";
	{
		Eigen::MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		Cout() << "\nColumn's maximum: \n" << mat.colwise().maxCoeff();
	}
	{
		Eigen::MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		Cout() << "\nRow's maximum: \n" << mat.rowwise().maxCoeff();
	}
	Cout() << "\n\nCombining partial reductions with other operations";
	{
		MatrixXf mat(2,4);
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		
		MatrixXf::Index   maxIndex;
		float maxNorm = mat.colwise().sum().maxCoeff(&maxIndex);
		
		Cout() << "\nMaximum sum at position " << maxIndex;
		
		Cout() << "\nThe corresponding vector is: ";
		Cout() << "\n" << mat.col( maxIndex );
		Cout() << "\nAnd its sum is is: " << maxNorm;
	}
	Cout() << "\n\nBroadcasting";
	{
		Eigen::MatrixXf mat(2,4);
		Eigen::VectorXf v(2);
		
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		     
		v << 0,
		     1;
		   
		//add v to each column of m
		mat.colwise() += v;
		
		Cout() << "\nBroadcasting result: ";
		Cout() << "\n" << mat;
	}
	{
		Eigen::MatrixXf mat(2,4);
		Eigen::VectorXf v(4);
		
		mat << 1, 2, 6, 9,
		       3, 1, 7, 2;
		     
		v << 0,1,2,3;
		   
		//add v to each row of m
		mat.rowwise() += v.transpose();
		
		Cout() << "\nBroadcasting result: ";
		Cout() << "\n" << mat;
	}
	Cout() << "\n\nCombining broadcasting with other operations";
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
		
		Cout() << "\nNearest neighbour is column " << index << ":";
		Cout() << "\n" << m.col(index);
	}
		
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
	
	Cout() << "\n\nSerializing tests";
	{
		SerialTest serialTest, serialTest_j, serialTest_x, serialTest_s;
		serialTest.m << 1, 2,
		     			4, 8;
		serialTest.v << 1, 2, 4;

		StoreAsJsonFile(serialTest, AppendFileName(GetDesktopFolder(), "Json.txt"));
		LoadFromJsonFile(serialTest_j, AppendFileName(GetDesktopFolder(), "Json.txt"));
		Cout() << "\nJSON demo";
		serialTest_j.Print();
		
		StoreAsXMLFile(serialTest, "XMLdata", AppendFileName(GetDesktopFolder(), "Xml.txt"));
		LoadFromXMLFile(serialTest_x, AppendFileName(GetDesktopFolder(), "Xml.txt"));
		Cout() << "\nXML demo";
		serialTest_x.Print();
		
		StoreToFile(serialTest, AppendFileName(GetDesktopFolder(), "Serial.dat"));
		LoadFromFile(serialTest_s, AppendFileName(GetDesktopFolder(), "Serial.dat"));
		Cout() << "\nSerialization demo";
		serialTest_s.Print();
	}
	
	Cout() << "\nPress enter to continue\n";
	ReadStdIn();
		
	NonLinearTests();
	
	Cout() << "\nPress enter to end";
	ReadStdIn();
}



