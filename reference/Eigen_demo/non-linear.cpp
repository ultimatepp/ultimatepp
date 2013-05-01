#include <Core/Core.h>

using namespace Upp;

#include <plugin/Eigen/Eigen.h>

using namespace Eigen;

#define VerifyIsApprox(a, b)	(fabs(a-(b)) < 0.0001)

struct Eckerle4_functor : NonLinearOptimizationFunctor<double> {
	Eckerle4_functor() : NonLinearOptimizationFunctor(3, 35) {}
	static const double x[35];
	static const double y[35];
	int operator()(const VectorXd &b, VectorXd &fvec) const {
		ASSERT(b.size() == unknowns);
		ASSERT(fvec.size() == datasetLen);
		for(int i = 0; i < 35; i++)
			fvec[i] = b[0]/b[1] * exp(-0.5*(x[i]-b[2])*(x[i]-b[2])/(b[1]*b[1])) - y[i];
		return 0;
	}
};
const double Eckerle4_functor::x[35] = {400.0, 405.0, 410.0, 415.0, 420.0, 425.0, 430.0, 435.0, 436.5, 438.0, 439.5, 441.0, 442.5, 444.0, 445.5, 447.0, 448.5, 450.0, 451.5, 453.0, 454.5, 456.0, 457.5, 459.0, 460.5, 462.0, 463.5, 465.0, 470.0, 475.0, 480.0, 485.0, 490.0, 495.0, 500.0};
const double Eckerle4_functor::y[35] = {0.0001575, 0.0001699, 0.0002350, 0.0003102, 0.0004917, 0.0008710, 0.0017418, 0.0046400, 0.0065895, 0.0097302, 0.0149002, 0.0237310, 0.0401683, 0.0712559, 0.1264458, 0.2073413, 0.2902366, 0.3445623, 0.3698049, 0.3668534, 0.3106727, 0.2078154, 0.1164354, 0.0616764, 0.0337200, 0.0194023, 0.0117831, 0.0074357, 0.0022732, 0.0008800, 0.0004579, 0.0002345, 0.0001586, 0.0001143, 0.0000710 };

struct Thurber_functor : NonLinearOptimizationFunctor<double> {
	static Vector<double> _x, _y;
	
	Thurber_functor() : NonLinearOptimizationFunctor() {
		_x << -3.067E0 << -2.981E0 << -2.921E0 << -2.912E0 << -2.840E0 << -2.797E0 << -2.702E0 << -2.699E0 << -2.633E0 << -2.481E0 << -2.363E0 << -2.322E0 << -1.501E0 << -1.460E0 << -1.274E0 << -1.212E0 << -1.100E0 << -1.046E0 << -0.915E0 << -0.714E0 << -0.566E0 << -0.545E0 << -0.400E0 << -0.309E0 << -0.109E0 << -0.103E0 << 0.010E0 << 0.119E0 << 0.377E0 << 0.790E0 << 0.963E0 << 1.006E0 << 1.115E0 << 1.572E0 << 1.841E0 << 2.047E0 << 2.200E0;
		_y << 80.574E0 << 84.248E0 << 87.264E0 << 87.195E0 << 89.076E0 << 89.608E0 << 89.868E0 << 90.101E0 << 92.405E0 << 95.854E0 << 100.696E0 << 101.060E0 << 401.672E0 << 390.724E0 << 567.534E0 << 635.316E0 << 733.054E0 << 759.087E0 << 894.206E0 << 990.785E0 << 1090.109E0 << 1080.914E0 << 1122.643E0 << 1178.351E0 << 1260.531E0 << 1273.514E0 << 1288.339E0 << 1327.543E0 << 1353.863E0 << 1414.509E0 << 1425.208E0 << 1421.384E0 << 1442.962E0 << 1464.350E0 << 1468.705E0 << 1447.894E0 << 1457.628E0;		
		unknowns = 7;
		datasetLen = _x.GetCount();
	}
	
	int operator()(const VectorXd &b, VectorXd &fvec) const {
		ASSERT(b.size() == unknowns);
		ASSERT(fvec.size() == datasetLen);
		for(int i = 0; i < datasetLen; i++) {
			double x = _x[i], xx = x*x, xxx = xx*x;
			fvec[i] = (b[0] + b[1]*x + b[2]*xx + b[3]*xxx)/(1. + b[4]*x + b[5]*xx + b[6]*xxx) - _y[i];
		}
		return 0;
	}
};

Vector<double> Thurber_functor::_x, Thurber_functor::_y;

void NonLinearOptimization() {
	Cout() << "\n\nNon linear equations optimization using Levenberg Marquardt based on Minpack\n"
	     "(Given a set of non linear equations and a set of data longer than the equations, "
	     "the program finds the equation coefficients that better fit with the equations)";
	
	{
		Cout() << "\n\nEckerle4 equation\nSee: http://www.itl.nist.gov/div898/strd/nls/data/eckerle4.shtml";
		
		VectorXd x(3);
		
		x << 1., 10., 500.;		// Initial values
		
		Eckerle4_functor functor;
		NumericalDiff<Eckerle4_functor> numDiff(functor);
		LevenbergMarquardt<NumericalDiff<Eckerle4_functor> > lm(numDiff);
		int ret = lm.minimize(x);
		if (ret == LevenbergMarquardtSpace::ImproperInputParameters || 
			ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation)
			Cout() << "\nNo convergence!: " << ret;
		else {
			if (VerifyIsApprox(lm.fvec.squaredNorm(), 1.4635887487E-03))
				Cout() << "\nNorm^2 is right";
			if (VerifyIsApprox(x[0], 1.5543827178) &&
				VerifyIsApprox(x[1], 4.0888321754) &&
				VerifyIsApprox(x[2], 4.5154121844E+02))
				Cout() << "\nNon-linear function optimization is right!";
		}
	}
	{
		Cout() << "\n\nThurber equation\nSee: http://www.itl.nist.gov/div898/strd/nls/data/thurber.shtml\n";
		
		VectorXd x(7);
		x << 1000, 1000, 400, 40, 0.7, 0.3, 0.0;	// Initial values
		
		Thurber_functor functor;	// Vars and equations known at run time
		NumericalDiff<Thurber_functor> numDiff(functor);
		LevenbergMarquardt<NumericalDiff<Thurber_functor> > lm(numDiff);
		lm.parameters.ftol = 1.E4*NumTraits<double>::epsilon();
		lm.parameters.xtol = 1.E4*NumTraits<double>::epsilon();
		int ret = lm.minimize(x);
		if (ret == LevenbergMarquardtSpace::ImproperInputParameters || 
			ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation)
			Cout() << "\nNo convergence!: " << ret;
		else {
			if (VerifyIsApprox(lm.fvec.squaredNorm(), 5.6427082397E+03))
				Cout() << "\nNorm^2 is right";
			if (VerifyIsApprox(x[0], 1.2881396800E+03) &&
			    VerifyIsApprox(x[1], 1.4910792535E+03) &&
			    VerifyIsApprox(x[2], 5.8323836877E+02) &&
			    VerifyIsApprox(x[3], 7.5416644291E+01) &&
			    VerifyIsApprox(x[4], 9.6629502864E-01) &&
			    VerifyIsApprox(x[5], 3.9797285797E-01) &&
			    VerifyIsApprox(x[6], 4.9727297349E-02))
				Cout() << "\nNon-linear function optimization is right!";
		}
	}
}

struct Hybrd_functor : NonLinearOptimizationFunctor<double>
{
	Hybrd_functor() : NonLinearOptimizationFunctor<double>(9, 9) {}
	int operator()(const VectorXd &x, VectorXd &fvec) const {
		double temp, temp1, temp2;
		const ptrdiff_t n = x.size();
		
		ASSERT(fvec.size() == n);
		for (ptrdiff_t k = 0; k < n; k++) {
			temp = (3. - 2.*x[k])*x[k];
			temp1 = 0.;
			if (k) 
				temp1 = x[k-1];
			temp2 = 0.;
			if (k != n-1) 
				temp2 = x[k+1];
			fvec[k] = temp - temp1 - 2.*temp2 + 1.;
		}
		return 0;
	}
};

void NonLinearSolving() {
	Cout() << "\n\nNon linear equation solving using the Powell hybrid method (\"dogleg\") based on Minpack. "
	       << "(Finds a zero of a system of n nonlinear equations in n variables)";
	
	const int n = 9;
	VectorXd x;
	
	x.setConstant(n, -1.);		// Initial values
	
	Hybrd_functor functor;
	HybridNonLinearSolver<Hybrd_functor> solver(functor);
	int ret = solver.solveNumericalDiff(x);
	if (ret == HybridNonLinearSolverSpace::ImproperInputParameters ||
	    ret == HybridNonLinearSolverSpace::TooManyFunctionEvaluation ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressJacobian ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressIterations)
		Cout() << "\nNo convergence!: " << ret;
	else {
		if (solver.nfev != 14)
			Cout() << "\nError with nfev!";
		if (VerifyIsApprox(solver.fvec.blueNorm(), 1.192636e-08))
			Cout() << "\nNorm is right";
		if (VerifyIsApprox(x[0], -0.5706545) && 
		    VerifyIsApprox(x[1], -0.6816283) &&     
		    VerifyIsApprox(x[2], -0.7017325) && 
		    VerifyIsApprox(x[3], -0.7042129) &&  
		    VerifyIsApprox(x[4], -0.701369) && 
		    VerifyIsApprox(x[5], -0.6918656) && 
		    VerifyIsApprox(x[6], -0.665792) && 
		    VerifyIsApprox(x[7], -0.5960342) &&  
		    VerifyIsApprox(x[8], -0.4164121))
			Cout() << "\nEquation solving is right!";
	}
}

void NonLinearTests() {
	NonLinearSolving();
	NonLinearOptimization();
}



