#include <Core/Core.h>
#include <plugin/Eigen/Eigen.h>

using namespace Upp;

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
	static VectorXd _x, _y;
	
	Thurber_functor() : NonLinearOptimizationFunctor() {
		double x[] = {-3.067, -2.981, -2.921, -2.912, -2.840, -2.797, -2.702, -2.699, -2.633, -2.481, -2.363, -2.322, -1.501, -1.460, -1.274, -1.212, -1.100, -1.046, -0.915, -0.714, -0.566, -0.545, -0.400, -0.309, -0.109, -0.103, 0.010, 0.119, 0.377, 0.790, 0.963, 1.006, 1.115, 1.572, 1.841, 2.047, 2.200};
		double y[] = {80.574, 84.248, 87.264, 87.195, 89.076, 89.608, 89.868, 90.101, 92.405, 95.854, 100.696, 101.060, 401.672, 390.724, 567.534, 635.316, 733.054, 759.087, 894.206, 990.785, 1090.109, 1080.914, 1122.643, 1178.351, 1260.531, 1273.514, 1288.339, 1327.543, 1353.863, 1414.509, 1425.208, 1421.384, 1442.962, 1464.350, 1468.705, 1447.894, 1457.628};		
		_x = Map<VectorXd>(x, sizeof(x)/sizeof(double)); 
		_y = Map<VectorXd>(y, sizeof(y)/sizeof(double)); 
		unknowns = 7;
		datasetLen = _x.size();
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

VectorXd Thurber_functor::_x, Thurber_functor::_y;

void NonLinearOptimization() {
	UppLog() << "\n\nNon linear equations optimization using Levenberg Marquardt based on Minpack\n"
	     "(Given a set of non linear equations and a set of data longer than the equations, "
	     "the program finds the equation coefficients that better fit with the equations)";
	
	{
		UppLog() << "\n\nEckerle4 equation\nSee: http://www.itl.nist.gov/div898/strd/nls/data/eckerle4.shtml";
		
		VectorXd x(3);
		
		x << 1., 10., 500.;		// Initial values
		
		Eckerle4_functor functor;
		NumericalDiff<Eckerle4_functor> numDiff(functor);
		LevenbergMarquardt<NumericalDiff<Eckerle4_functor> > lm(numDiff);
		int ret = lm.minimize(x);
		if (ret == LevenbergMarquardtSpace::ImproperInputParameters || 
			ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation)
			UppLog() << "\nNo convergence!: " << ret;
		else {
			if (VerifyIsApprox(lm.fvec.squaredNorm(), 1.4635887487E-03))
				UppLog() << "\nNorm^2 is right";
			else
				UppLog() << "\nNorm^2 is NOT right";
			if (VerifyIsApprox(x[0], 1.5543827178) &&
				VerifyIsApprox(x[1], 4.0888321754) &&
				VerifyIsApprox(x[2], 4.5154121844E+02))
				UppLog() << "\nNon-linear function optimization is right!";
			else
				UppLog() << "\nNon-linear function optimization is NOT right!";
		}
	}
	{
		UppLog() << "\n\nThis is a simpler way, using NonLinearOptimization()";
		double x[] = {400.0, 405.0, 410.0, 415.0, 420.0, 425.0, 430.0, 435.0, 436.5, 438.0, 439.5, 441.0, 442.5, 444.0, 445.5, 447.0, 448.5, 450.0, 451.5, 453.0, 454.5, 456.0, 457.5, 459.0, 460.5, 462.0, 463.5, 465.0, 470.0, 475.0, 480.0, 485.0, 490.0, 495.0, 500.0};
		double f[] = {0.0001575, 0.0001699, 0.0002350, 0.0003102, 0.0004917, 0.0008710, 0.0017418, 0.0046400, 0.0065895, 0.0097302, 0.0149002, 0.0237310, 0.0401683, 0.0712559, 0.1264458, 0.2073413, 0.2902366, 0.3445623, 0.3698049, 0.3668534, 0.3106727, 0.2078154, 0.1164354, 0.0616764, 0.0337200, 0.0194023, 0.0117831, 0.0074357, 0.0022732, 0.0008800, 0.0004579, 0.0002345, 0.0001586, 0.0001143, 0.0000710};	
		int num = sizeof(x)/sizeof(double);
		VectorXd y(3);
		y << 1., 10., 500.;
		VERIFY(NonLinearOptimization(y, num, [&](const VectorXd &y, VectorXd &residual)->int {
			for(int i = 0; i < num; i++)
				residual[i] = y[0]/y[1] * exp(-0.5*(x[i]-y[2])*(x[i]-y[2])/(y[1]*y[1])) - f[i];
			return 0;	
		}));
		
		VERIFY(VerifyIsApprox(y[0], 1.5543827178) &&
				VerifyIsApprox(y[1], 4.0888321754) &&
				VerifyIsApprox(y[2], 4.5154121844E+02));
	}
	{
		UppLog() << "\n\nThurber equation\nSee: http://www.itl.nist.gov/div898/strd/nls/data/thurber.shtml\n";
		
		VectorXd x(7);
		x << 1000, 1000, 400, 40, 0.7, 0.3, 0.0;	// Initial values
		
		Thurber_functor functor;	// Vars and equations known at run time
		NumericalDiff<Thurber_functor> numDiff(functor);
		LevenbergMarquardt<NumericalDiff<Thurber_functor> > lm(numDiff);
		lm.parameters.ftol = 1.E4*NumTraits<double>::epsilon();
		lm.parameters.xtol = 1.E4*NumTraits<double>::epsilon();
		int ret = lm.minimize(x);
		
		VERIFY(!(ret == LevenbergMarquardtSpace::ImproperInputParameters || 
			ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation));
		
		VERIFY(VerifyIsApprox(lm.fvec.squaredNorm(), 5.6427082397E+03));
		
		VERIFY(VerifyIsApprox(x[0], 1.2881396800E+03) &&
			    VerifyIsApprox(x[1], 1.4910792535E+03) &&
			    VerifyIsApprox(x[2], 5.8323836877E+02) &&
			    VerifyIsApprox(x[3], 7.5416644291E+01) &&
			    VerifyIsApprox(x[4], 9.6629502864E-01) &&
			    VerifyIsApprox(x[5], 3.9797285797E-01) &&
			    VerifyIsApprox(x[6], 4.9727297349E-02));
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
	UppLog() << "\n\nNon linear equation solving using the Powell hybrid method (\"dogleg\") based on Minpack. "
	       << "(Finds a zero of a system of n nonlinear equations in n variables)";
	
	const int n = 9;
	VectorXd x;
	
	x.setConstant(n, -1.);		// Initial values
	
	Hybrd_functor functor;
	HybridNonLinearSolver<Hybrd_functor> solver(functor);
	int ret = solver.solveNumericalDiff(x);
	
	VERIFY(!(ret == HybridNonLinearSolverSpace::ImproperInputParameters ||
	    ret == HybridNonLinearSolverSpace::TooManyFunctionEvaluation ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressJacobian ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressIterations));

	VERIFY(VerifyIsApprox(solver.fvec.blueNorm(), 1.192636e-08));
			
	VERIFY(VerifyIsApprox(x[0], -0.5706545) && 
		    VerifyIsApprox(x[1], -0.6816283) &&     
		    VerifyIsApprox(x[2], -0.7017325) && 
		    VerifyIsApprox(x[3], -0.7042129) &&  
		    VerifyIsApprox(x[4], -0.701369) && 
		    VerifyIsApprox(x[5], -0.6918656) && 
		    VerifyIsApprox(x[6], -0.665792) && 
		    VerifyIsApprox(x[7], -0.5960342) &&  
		    VerifyIsApprox(x[8], -0.4164121));
	
	UppLog() << "\n\nThis is a simpler way, using SolveNonLinearEquations()";
	
	x.setConstant(n, -1.);		// Initial values
	
	VERIFY(SolveNonLinearEquations(x, [&](const VectorXd &x, VectorXd &residual)->int {
		const ptrdiff_t n = x.size();
		
		ASSERT(residual.size() == n);
		for (ptrdiff_t k = 0; k < n; k++) {
			double temp1 = 0.;
			if (k) 
				temp1 = x[k-1];
			double temp2 = 0.;
			if (k != n-1) 
				temp2 = x[k+1];
			residual[k] = (3. - 2.*x[k])*x[k] - temp1 - 2.*temp2 + 1.;
		}
		return 0;
	}));	// No convergence!
	VERIFY(VerifyIsApprox(x[0], -0.5706545) && 
		    VerifyIsApprox(x[1], -0.6816283) &&     
		    VerifyIsApprox(x[2], -0.7017325) && 
		    VerifyIsApprox(x[3], -0.7042129) &&  
		    VerifyIsApprox(x[4], -0.701369) && 
		    VerifyIsApprox(x[5], -0.6918656) && 
		    VerifyIsApprox(x[6], -0.665792) && 
		    VerifyIsApprox(x[7], -0.5960342) &&  
		    VerifyIsApprox(x[8], -0.4164121));
}

void NonLinearTests() {
	NonLinearSolving();
	NonLinearOptimization();
}



