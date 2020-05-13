#include <Core/Core.h>
#include <Eigen.h>


namespace Upp {

using namespace Eigen;

bool NonLinearOptimization(VectorXd &y, int numData, Function <int(const VectorXd &b, VectorXd &residual)> Residual) {
	Basic_functor functor(Residual);
	functor.unknowns = y.size();
	functor.datasetLen = numData;
	Eigen::NumericalDiff<Basic_functor> numDiff(functor);
	Eigen::LevenbergMarquardt<Eigen::NumericalDiff<Basic_functor> > lm(numDiff);
	int ret = lm.minimize(y);
	if (ret == Eigen::LevenbergMarquardtSpace::ImproperInputParameters || 
		ret == Eigen::LevenbergMarquardtSpace::TooManyFunctionEvaluation)
		return false;
	return true;
}

bool NonLinearSolver(VectorXd &y, Function <int(const VectorXd &b, VectorXd &residual)> Residual) {
	Basic_functor functor(Residual);
	HybridNonLinearSolver<Basic_functor> solver(functor);
	int ret = solver.solveNumericalDiff(y);
	if (ret == HybridNonLinearSolverSpace::ImproperInputParameters ||
	    ret == HybridNonLinearSolverSpace::TooManyFunctionEvaluation ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressJacobian ||
	    ret == HybridNonLinearSolverSpace::NotMakingProgressIterations)
		return false;
	return true;
}

}