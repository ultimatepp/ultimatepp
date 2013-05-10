#include "ScatterDraw.h"

#include <plugin/Eigen/Eigen.h>

NAMESPACE_UPP

using namespace Eigen;

struct Equation_functor : NonLinearOptimizationFunctor<double> {
	DataSource *series;
	ExplicitEquation *fSource;
	Equation_functor() {}
	
	int operator()(const VectorXd &b, VectorXd &fvec) const {
		ASSERT(b.size() == unknowns);
		ASSERT(fvec.size() == datasetLen);
		for (int i = 0; i < unknowns; ++i)
			(*fSource).SetCoeffVal(i, b(i));
		for(int64 i = 0; i < datasetLen; i++) 
			fvec(ptrdiff_t(i)) = (*fSource).f((*series).x(i)) - (*series).y(i);
		return 0;
	}
};

void ExplicitEquation::SetNumCoeff(int num) {
	coeff.SetCount(num); 
	for (int i = 0; i < num; ++i)
		coeff[i] = 0;
}

ExplicitEquation::FitError ExplicitEquation::Fit(DataSource &series, double &r2) {
	if (series.IsExplicit() || series.IsParam())
		return InadequateDataSource;
	
	if (series.GetCount() < coeff.GetCount())
		return SmallDataSource;
		
	ptrdiff_t numUnknowns = coeff.GetCount();
	
	VectorXd x(numUnknowns);
	for (int i = 0; i < numUnknowns; ++i)
		x(i) = coeff[i];
	
	Equation_functor functor;	
	functor.series = &series;
	functor.fSource = this;
	functor.unknowns = numUnknowns;
	functor.datasetLen = series.GetCount();
	
	NumericalDiff<Equation_functor> numDiff(functor);
	LevenbergMarquardt<NumericalDiff<Equation_functor> > lm(numDiff);
// 	ftol is a nonnegative input variable that measures the relative error desired in the sum of squares 
	lm.parameters.ftol = 1.E4*NumTraits<double>::epsilon();
//  xtol is a nonnegative input variable that measures the relative error desired in the approximate solution
	lm.parameters.xtol = 1.E4*NumTraits<double>::epsilon();
	lm.parameters.maxfev = maxFitFunctionEvaluations;
	int ret = lm.minimize(x);
	if (ret == LevenbergMarquardtSpace::ImproperInputParameters)
		return ExplicitEquation::ImproperInputParameters;
	if (ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation)
		return TooManyFunctionEvaluation;
	
	double mean = series.AvgY();
	double sse = 0, sst = 0;
	for (int64 i = 0; i < series.GetCount(); ++i) {
		double res = series.y(i) - f(series.x(i));
		sse += res*res;
		double d = series.y(i) - mean;
		sst += d*d;
	}
	r2 = 1 - sse/sst;

	return NoError;
}

int ExplicitEquation::numDigits = 3;
int ExplicitEquation::maxFitFunctionEvaluations = 1000;


double PolynomialEquation::f(double x) {
	double y = 0;
	for (int i = 0; i < coeff.GetCount(); ++i) 
		y += coeff[i]*pow(x, i);
	return y;
}

String PolynomialEquation::GetEquation() {
	if (coeff.IsEmpty())
		return String();
	String y;
	y = FormatDoubleFix(coeff[0], numDigits);
	if (coeff.GetCount() == 1)
		return y;
	y += " " + String(coeff[1] >= 0 ? "+" : "-") + " " + FormatDoubleFix(fabs(coeff[1]), numDigits) + "*x";
	for (int i = 2; i < coeff.GetCount(); ++i) 
		y += " " + String(coeff[i] >= 0 ? "+" : "-") + " " + FormatDoubleFix(fabs(coeff[i]), numDigits) + "*x^" + FormatInt(i);
	return y;
}
	
double FourierEquation::f(double x) {
	double y = coeff[0];
	double w = coeff[1];
	for (int i = 2; i < coeff.GetCount(); i += 2) {
		int n = 1 + (i - 2)/2;
		y += coeff[i]*cos(n*w*x) + coeff[i+1]*sin(n*w*x);
	}
	return y;
}

String FourierEquation::GetEquation() {
	if (coeff.GetCount() < 4)
		return String();
	String y = FormatDoubleFix(coeff[0], numDigits);
	
	for (int i = 2; i < coeff.GetCount(); i += 2) {
		int n = 1 + (i - 2)/2;
		String nwx = Format("%d*%s*x", n, FormatDoubleFix(fabs(coeff[1]), numDigits));
		String signA = coeff[i] >= 0 ? "+" : "-";
		String signB = coeff[i+1] >= 0 ? "+" : "-";
		y += " " + signA + " " + FormatDoubleFix(fabs(coeff[i]), numDigits) + "*cos(" + nwx + ")";
		y += " " + signB + " " + FormatDoubleFix(fabs(coeff[i+1]), numDigits) + "*sin(" + nwx + ")";
	}
	return y;
}
	
END_UPP_NAMESPACE