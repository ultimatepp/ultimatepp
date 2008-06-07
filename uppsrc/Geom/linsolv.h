class LinearSolver
{
public:
	LinearSolver(int count, double tolerance = 1e-10);

	double        *Row(int r)                        { return &matrix[r * col1]; }
	const double  *Row(int r) const                  { return &matrix[r * col1]; }
	double&        Left(int r, int c)                { return matrix[r * col1 + c]; }
	double         Left(int r, int c) const          { return matrix[r * col1 + c]; }
	double&        Right(int r)                      { return matrix[(r + 1) * col1 - 1]; }
	double         Right(int r) const                { return matrix[(r + 1) * col1 - 1]; }
	double&        operator () (int r, int c)        { return Left(r, c); }
	double         operator () (int r, int c) const  { return Left(r, c); }
	double&        operator () (int r)               { return Right(r); }
	double         operator () (int r) const         { return Right(r); }

	void           AddLSI(const double *bases, double value);

	Vector<double> Solve();

	static void    SelfTest();

private:
	int            rows;
	int            col1;
	Vector<double> matrix;
	Vector<Point>  pivots;
	double         tolerance;
	Vector<int>    left_rows, left_cols;
};
