#include <Core/Core.h>
#include "Eigen.h"

namespace Upp {

using namespace Eigen;
	
bool NonLinearOptimization(VectorXd &y, Eigen::Index numData, 
			Function <int(const VectorXd &b, VectorXd &residual)> Residual,
			double xtol, double ftol, int maxfev) {
	Basic_functor functor(Residual);
	functor.unknowns = y.size();
	functor.datasetLen = numData;
	Eigen::NumericalDiff<Basic_functor> numDiff(functor);
	Eigen::LevenbergMarquardt<Eigen::NumericalDiff<Basic_functor> > lm(numDiff);
	if (!IsNull(xtol))
		lm.parameters.xtol *= xtol;
	if (!IsNull(ftol))
		lm.parameters.ftol *= ftol;
	if (!IsNull(maxfev))
		lm.parameters.maxfev = maxfev;
	int ret = lm.minimize(y);
	if (ret == Eigen::LevenbergMarquardtSpace::ImproperInputParameters || 
		ret == Eigen::LevenbergMarquardtSpace::TooManyFunctionEvaluation ||
		ret == Eigen::LevenbergMarquardtSpace::CosinusTooSmall) 
		return false;
	return true;
}

bool SolveNonLinearEquations(VectorXd &y, Function <int(const VectorXd &b, VectorXd &residual)> Residual,
			double xtol, int maxfev) {
	Basic_functor functor(Residual);
	HybridNonLinearSolver<Basic_functor> solver(functor);
	if (!IsNull(xtol))
		solver.parameters.xtol *= xtol;
	if (!IsNull(maxfev))
		solver.parameters.maxfev = maxfev;
	int ret = solver.solveNumericalDiff(y);
	if (ret == HybridNonLinearSolverSpace::ImproperInputParameters ||
	    ret == HybridNonLinearSolverSpace::TooManyFunctionEvaluation ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressJacobian ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressIterations)
		return false;
	return true;
}

}