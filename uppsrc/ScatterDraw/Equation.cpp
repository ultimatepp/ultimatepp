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
	r2 = 0;
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

int ExplicitEquation::maxFitFunctionEvaluations = 1000;


double PolynomialEquation::f(double x) {
	double y = 0;
	for (int i = 0; i < coeff.GetCount(); ++i) 
		y += coeff[i]*pow(x, i);
	return y;
}

String PolynomialEquation::GetEquation(int numDigits) {
	if (coeff.IsEmpty())
		return String();
	String ret = FormatCoeff(0, numDigits);
	if (coeff.GetCount() == 1)
		return ret;
	ret += Format(" + %s*x", FormatCoeff(1, numDigits));
	for (int i = 2; i < coeff.GetCount(); ++i) 
		ret += Format(" + %s*x^%s", FormatCoeff(i, numDigits), FormatInt(i));
	ret.Replace("+ -", "- ");
	return ret;
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

String FourierEquation::GetEquation(int numDigits) {
	if (coeff.GetCount() < 4)
		return String();
	String ret = FormatCoeff(0, numDigits);
	
	for (int i = 2; i < coeff.GetCount(); i += 2) {
		int n = 1 + (i - 2)/2;
		String nwx = Format("%d*%s*x", n, FormatCoeff(1, numDigits));
		ret += Format(" + %s*cos(%s)", FormatCoeff(i, numDigits), nwx);
		ret += Format(" + %s*sin(%s)", FormatCoeff(i + 1, numDigits), nwx);
	}
	ret.Replace("+ -", "- ");
	return ret;
}


EvalExpr::EvalExpr() {
	noCase = false;
	
	constants.Add("PI", M_PI);
	constants.Add("M_PI", M_PI);
	constants.Add("e", exp(1.0));
	functions.Add("abs", fabs);
	functions.Add("ceil", ceil);
	functions.Add("floor", floor);
	functions.Add("sqrt", sqrt);
	functions.Add("sin", sin);
	functions.Add("cos", cos);
	functions.Add("tan", tan);
	functions.Add("asin", asin);
	functions.Add("acos", acos);
	functions.Add("atan", atan);
	functions.Add("sinh", sinh);
	functions.Add("cosh", cosh);
	functions.Add("tanh", tanh);
}

double EvalExpr::Term(CParser& p) {
	if(p.IsId()) {
		String strId = p.ReadId();
		if(double (*function)(double) = functions.Get(strId, 0)) {
			p.PassChar('(');
			double x = Exp(p);
			p.PassChar(')');
			return function(x);
		}	
		if (noCase)
			strId = ToUpper(strId);
		double ret = constants.Get(strId, Null);
		if (IsNull(ret))
			ret = variables.GetAdd(strId, 0);
		return ret;
	} else if(p.Char('(')) {
		double x = Exp(p);
		p.PassChar(')');
		return x;
	} else
		return p.ReadDouble();
}

double EvalExpr::Pow(CParser& p) {
	double x = Term(p);
	for(;;) {
		if(p.Char('^'))
			x = pow(x, Term(p));
		else
			return x;
	}
}

double EvalExpr::Mul(CParser& p) {
	double x = Pow(p);
	for(;;) {
		if(p.Char('*'))
			x = x * Mul(p);
		else if(p.Char('/')) {
			double y = Pow(p);
			if(y == 0)
				p.ThrowError(t_("Divide by zero"));
			x = x / y;
		} else
			return x;
	}
}

double EvalExpr::Exp(CParser& p) {
	double x = Mul(p);
	for(;;) {
		if(p.Char('+'))
			x = x + Mul(p);
		else if(p.Char('-'))
			x = x - Mul(p);
		else if(p.Char(':'))
			x = x*60 + Mul(p);
		else
			return x;
	}
}

double EvalExpr::Eval(String line) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	CParser p(line);
	try {
		if(p.IsId()) {
			CParser::Pos pos = p.GetPos();
			String var = p.ReadId();
			if(p.Char('=')) {
				double ret = Exp(p);
				variables.GetAdd(var) = ret;
				return ret;
			} else {
				p.SetPos(pos);
				return Exp(p);
			}
		} else
			return Exp(p);
	}
	catch(CParser::Error e) {
		LOG(Format(t_("Error evaluating '%s': %s"), line, e));
		return Null;
	}
}

String EvalExpr::TermStr(CParser& p, int numDigits) {
	if(p.IsId()) {
		String strId = p.ReadId();
		if(functions.Find(strId) >= 0) {
			p.PassChar('(');
			String x = ExpStr(p, numDigits);
			p.PassChar(')');
			return strId + "(" + x + ")";
		}
		if (noCase)
			strId = ToUpper(strId);
		if (IsNull(numDigits)) {
			if (constants.Find(strId) < 0)
				variables.GetAdd(strId, 0);
			return strId;
		} else {
			if (constants.Find(strId) >= 0)
				return strId;
			else {
				double dat = variables.GetAdd(strId, 0);
				return FormatDoubleFix(dat, numDigits);
			}
		}
	}
	if(p.Char('(')) {
		String x = ExpStr(p, numDigits);
		p.PassChar(')');
		return "(" + x + ")";
	}
	return FormatDoubleFix(p.ReadDouble(), IsNull(numDigits) ? 3 : numDigits);
}

String EvalExpr::PowStr(CParser& p, int numDigits) {
	String x = TermStr(p, numDigits);
	for(;;)
		if(p.Char('^'))
			x = x + "^" + TermStr(p, numDigits);
		else
			return x;
}

String EvalExpr::MulStr(CParser& p, int numDigits) {
	String x = PowStr(p, numDigits);
	for(;;)
		if(p.Char('*'))
			x = x + "*" + MulStr(p, numDigits);
		else if(p.Char('/')) 
			x = x + "/" + PowStr(p, numDigits);
		else
			return x;
}

String EvalExpr::ExpStr(CParser& p, int numDigits) {
	String x = MulStr(p, numDigits);
	for(;;) {
		if(p.Char('+'))
			x = x + " + " + MulStr(p, numDigits);
		else if(p.Char('-'))
			x = x + " - " + MulStr(p, numDigits);
		else if(p.Char(':'))
			x = x + ":" + MulStr(p, numDigits);
		else {
			x.Replace("+ -", "- ");
			return x;
		}
	}
}

String EvalExpr::EvalStr(String line, int numDigits) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	CParser p(line);
	try {
		if(p.IsId()) {
			CParser::Pos pos = p.GetPos();
			String var = p.ReadId();
			if(p.Char('=')) {
				String ret = ExpStr(p, numDigits);
				variables.GetAdd(var, 0);
				return var + " = " + ret;
			} else {
				p.SetPos(pos);
				return ExpStr(p, numDigits);
			}
		} else
			return ExpStr(p, numDigits);
	}
	catch(CParser::Error e) {
		LOG(Format(t_("Error evaluating '%s': %s"), line, e));
		return Null;
	}
}

INITBLOCK {
	ExplicitEquation::Register<LinearEquation>("LinearEquation");
	ExplicitEquation::Register<PolynomialEquation2>("PolynomialEquation2");
	ExplicitEquation::Register<PolynomialEquation3>("PolynomialEquation3");
	ExplicitEquation::Register<PolynomialEquation4>("PolynomialEquation4");
	ExplicitEquation::Register<PolynomialEquation5>("PolynomialEquation5");
	ExplicitEquation::Register<SinEquation>("SinEquation");
	ExplicitEquation::Register<ExponentialEquation>("ExponentialEquation");
	ExplicitEquation::Register<Rational1Equation>("Rational1Equation");
	ExplicitEquation::Register<FourierEquation1>("FourierEquation1");
	ExplicitEquation::Register<FourierEquation2>("FourierEquation2");
	ExplicitEquation::Register<FourierEquation3>("FourierEquation3");
	ExplicitEquation::Register<FourierEquation4>("FourierEquation4");
}

END_UPP_NAMESPACE