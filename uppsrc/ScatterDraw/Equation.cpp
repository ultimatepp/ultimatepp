#include "ScatterDraw.h"
#include <plugin/Eigen/Eigen.h>

namespace Upp {
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

ExplicitEquation::FitError ExplicitEquation::Fit(DataSource &serie, double &r2) {
	r2 = Null;
	
	if (serie.IsExplicit() || serie.IsParam())
		return InadequateDataSource;
	
	if (serie.GetCount() < coeff.GetCount())
		return SmallDataSource;
	
	ptrdiff_t numUnknowns = coeff.GetCount();
	
	VectorXd x(numUnknowns);
	for (int i = 0; i < numUnknowns; ++i)
		x(i) = coeff[i];
	
	Equation_functor functor;	
	functor.series = &serie;
	functor.fSource = this;
	functor.unknowns = numUnknowns;
	functor.datasetLen = serie.GetCount();
	
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
	else if (ret == LevenbergMarquardtSpace::TooManyFunctionEvaluation)
		return TooManyFunctionEvaluation;

	r2 = R2Y(serie);

	return NoError;
}

double ExplicitEquation::R2Y(DataSource &serie, double mean) {
	if (IsNull(mean))
		mean = serie.AvgY();
	double sse = 0, sst = 0;
	for (int64 i = 0; i < serie.GetCount(); ++i) {
		double y = serie.y(i);
		if (!IsNull(y)) {
			double err = y - f(serie.x(i));
			sse += err*err;
			double d = y - mean;
			sst += d*d;
		}
	}
	if (sst < 1E-50 || sse > sst)
		return 0;
	return 1 - sse/sst;
}

int ExplicitEquation::maxFitFunctionEvaluations = 1000;


double PolynomialEquation::f(double x) {
	if (x < 0)
		return Null;
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

static inline double DegToRad(double deg) {return deg*M_PI/180.;}
static inline double RadToDeg(double rad) {return rad*180./M_PI;}

void EvalExpr::EvalThrowError(CParser &p, const char *s) {
	CParser::Pos pos = p.GetPos();
	CParser::Error err(Format("(%d): ", pos.GetColumn()) + s);
	throw err;
}

doubleUnit usqrt(doubleUnit val) {
	val.Sqrt();
	return val;
}

doubleUnit ufabs(doubleUnit val) {
	val.val = fabs(val.val);
	return val;
}

doubleUnit uceil(doubleUnit val) {
	val.val = ceil(val.val);
	return val;
}

doubleUnit ufloor(doubleUnit val) {
	val.val = floor(val.val);
	return val;
}

doubleUnit usin(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = sin(val.val);
	return val;
}

doubleUnit ucos(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = cos(val.val);
	return val;
}

doubleUnit utan(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = tan(val.val);
	return val;
}

doubleUnit uasin(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = asin(val.val);
	return val;
}

doubleUnit uacos(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = acos(val.val);
	return val;
}

doubleUnit uatan(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = atan(val.val);
	return val;
}

doubleUnit usinh(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = sinh(val.val);
	return val;
}

doubleUnit ucosh(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = cosh(val.val);
	return val;
}

doubleUnit utanh(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = tanh(val.val);
	return val;
}

doubleUnit uexp(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = exp(val.val);
	return val;
}

doubleUnit uDegToRad(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = DegToRad(val.val);
	return val;
}

doubleUnit uRadToDeg(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = RadToDeg(val.val);
	return val;
}

doubleUnit ulog(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = log(val.val);
	return val;
}

doubleUnit ulog10(doubleUnit val) {
	if (!val.unit.IsAdim())
		return Null;
	val.val = log10(val.val);
	return val;
}

EvalExpr::EvalExpr() {
	noCase = false;
	errorIfUndefined = false;
	
	constants.Add("pi", doubleUnit(M_PI));
	constants.Add("e", doubleUnit(M_E));
	
	functions.Add("abs", ufabs);
	functions.Add("ceil", uceil);
	functions.Add("floor", ufloor);
	functions.Add("sqrt", usqrt);
	functions.Add("sin", usin);
	functions.Add("cos", ucos);
	functions.Add("tan", utan);
	functions.Add("asin", uasin);
	functions.Add("acos", uacos);
	functions.Add("atan", uatan);
	functions.Add("sinh", usinh);
	functions.Add("cosh", ucosh);
	functions.Add("tanh", utanh);
	functions.Add("log", ulog);
	functions.Add("log10", ulog10);
	functions.Add("exp", uexp);
	functions.Add("degToRad", uDegToRad);
	functions.Add("radToDeg", uRadToDeg);
}

doubleUnit EvalExpr::Term(CParser& p) {
	bool isneg = p.Char('-');
	if (p.IsId()) {
		String strId = p.ReadId();
		if(doubleUnit (*function)(doubleUnit) = functions.Get(strId, 0)) {
			p.PassChar('(');
			doubleUnit x = Exp(p);
			p.PassChar(')');
			doubleUnit ret = function(x);
			if (IsNull(ret))
				EvalThrowError(p, Format(t_("Error in %s(%f)"), strId, x.val));	
			if (isneg)
				ret.Neg();
			return ret;
		}	
		String strsearch;
		if (noCase)
			strsearch = ToLower(strId);
		else
			strsearch = strId;
		doubleUnit ret = constants.Get(strsearch, Null);
		if (IsNull(ret)) {
			ret = variables.Get(strsearch, Null);
			if (IsNull(ret)) {
				if (errorIfUndefined)
					EvalThrowError(p, Format(t_("Unknown identifier '%s'"), strId));	
				ret = variables.GetAdd(strsearch, 0);
			}
		}
		if (isneg)
			ret.Neg();
		return ret;
	} else if (p.Char('(')) {
		doubleUnit x = Exp(p);
		p.PassChar(')');
		if (isneg)
			x.Neg();
		return x;
	} else {
		doubleUnit x = doubleUnit(p.ReadDouble());
		if (isneg)
			x.Neg();
		return x;
	}
}

doubleUnit EvalExpr::Pow(CParser& p) {
	doubleUnit x = Term(p);
	for(;;) {
		if(p.Char('^')) {
			if (x.val < 0)
				EvalThrowError(p, t_("Complex number"));
			x.Exp(Term(p));
		} else
			return x;
	}
}

doubleUnit EvalExpr::Mul(CParser& p) {
	doubleUnit x = Pow(p);
	for(;;) {
		if(p.Char('*'))
			x.Mult(Pow(p));
		else if (p.Char2('|', '|')) 
			x.ResParallel(Pow(p));
		else if(memcmp(p.GetPtr(), "·", strlen("·")) == 0) {
			CParser::Pos pos = p.GetPos();
			pos.ptr += strlen("·");
			p.SetPos(pos);
			p.Spaces();
			x.Mult(Pow(p));
		} else if(p.Char('/')) {
			x.Div(Pow(p));
		} else if(memcmp(p.GetPtr(), "º", strlen("º")) == 0) { 
			CParser::Pos pos = p.GetPos();
			pos.ptr += strlen("º");
			p.SetPos(pos);
			p.Spaces();
			x.Mult(doubleUnit(M_PI/180.));
		} else
			return x;
	}
}

doubleUnit EvalExpr::Exp(CParser& p) {
	doubleUnit x = Mul(p);
	for(;;) {
		if(p.Char('+'))
			x.Sum(Mul(p));
		else if(p.Char('-'))
			x.Sub(Mul(p));
		else if(p.Char(':')) {
			x.Mult(doubleUnit(60));
			x.Sum(Mul(p));
		} else
			return x;
	}
}

doubleUnit EvalExpr::Eval(String line) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	p.Set(line);
	try {
		if(p.IsId()) {
			CParser::Pos pos = p.GetPos();
			String var = p.ReadId();
			if(p.Char('=')) {
				doubleUnit ret = Exp(p);
				if (noCase)
					var = ToLower(var);
				SetVariable(var, ret);
				return ret;
			} else {
				p.SetPos(pos);
				return Exp(p);
			}
		} else
			return Exp(p);
	} catch(CParser::Error e) {
		lastError = e;
		return Null;
	} catch(Exc e) {
		lastError = e;
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
			strId = ToLower(strId);
		if (IsNull(numDigits)) {
			if (constants.Find(strId) < 0)
				variables.GetAdd(strId, 0);
			return strId;
		} else {
			if (constants.Find(strId) >= 0)
				return strId;
			else {
				double dat = variables.GetAdd(strId, 0).val;
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
	} catch(CParser::Error e) {
		lastError = Format(t_("Error evaluating '%s': %s"), line, e);
		return Null;
	} catch(Exc e) {
		lastError = Format(t_("Error: %s"), e);
		return Null;
	} catch(String e) {
		lastError = Format(t_("Error: %s"), e);
		return Null;
	} catch(...) {
		lastError = t_("Unknown error");
		return Null;
	}
}

void EvalExpr::ClearVariables() {
	variables.Clear();
}


ExplicitEquation::FitError SplineEquation::Fit(DataSource &data, double &r2) {	
	Vector<Pointf> seriesRaw;
	for (int64 i = 0; i < data.GetCount(); ++i) {		// Remove Nulls	
		if (!IsNull(data.x(i)) && !IsNull(data.y(i)))
			seriesRaw << Pointf(data.x(i), data.y(i));
	}

	if(seriesRaw.IsEmpty())
        return SmallDataSource;
      
    r2 = 1;
    
	PointfLess less;
	Sort(seriesRaw, less);								// Sort

	Vector<double> x, y;
	x.Reserve(seriesRaw.GetCount());
	y.Reserve(seriesRaw.GetCount());
	x << seriesRaw[0].x;
	y << seriesRaw[0].y;
	for (int i = 1; i < seriesRaw.GetCount(); ++i) {	// Remove points with duplicate x
		if (seriesRaw[i].x != seriesRaw[i - 1].x) {
			x << seriesRaw[i].x;
			y << seriesRaw[i].y;
		}
	}
	
	if (x.GetCount() < 2)
		return SmallDataSource;
		
	Spline::Fit(x, y);
	
	return NoError;
}

void Spline::Fit(const double *x, const double *y, int num) {
    ncoeff = num - 1;
    
    Buffer<double> h(ncoeff);
    for(int i = 0; i < ncoeff; ++i)
        h[i] = x[i+1] - x[i];

    Buffer<double> alpha(ncoeff);
    for(int i = 1; i < ncoeff; ++i)
        alpha[i] = 3*(y[i+1] - y[i])/h[i] - 3*(y[i] - y[i-1])/h[i-1];

    Buffer<double> c(ncoeff+1), l(ncoeff+1), mu(ncoeff+1), z(ncoeff+1);
    l[0] = 1;
    mu[0] = 0;
    z[0] = 0;

    for(int i = 1; i < ncoeff; ++i) {
        l[i] = 2*(x[i+1] - x[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }

    l[ncoeff] = 1;
    z[ncoeff] = 0;
    c[ncoeff] = 0;

	coeff.Alloc(ncoeff);
    for(int i = ncoeff-1; i >= 0; --i) {
        c[i] = z[i] - mu[i] * c[i+1];
        coeff[i].b = (y[i+1] - y[i])/h[i] - h[i]*(c[i+1] + 2*c[i])/3;
        coeff[i].d = (c[i+1] - c[i])/3/h[i];
    }

    for(int i = 0; i < ncoeff; ++i) {
        coeff[i].x = x[i];
        coeff[i].a = y[i];
        coeff[i].c = c[i];
    }
}

double Spline::f(double x) {
    int j;
    for (j = 0; j < ncoeff; j++) {
        if (coeff[j].x > x) {
            if (j == 0)
                j = 1;
            break;
        }
    }
    j--;

    double dx = x - coeff[j].x;
    return coeff[j].a + coeff[j].b*dx + coeff[j].c*dx*dx + coeff[j].d*dx*dx*dx;
}

INITBLOCK {
	ExplicitEquation::Register<LinearEquation>("LinearEquation");
	ExplicitEquation::Register<PolynomialEquation2>("PolynomialEquation2");
	ExplicitEquation::Register<PolynomialEquation3>("PolynomialEquation3");
	ExplicitEquation::Register<PolynomialEquation4>("PolynomialEquation4");
	ExplicitEquation::Register<PolynomialEquation5>("PolynomialEquation5");
	ExplicitEquation::Register<SinEquation>("SinEquation");
	ExplicitEquation::Register<DampedSinEquation>("DampedSinusoidal");
	ExplicitEquation::Register<Sin_DampedSinEquation>("Sin_DampedSinusoidal");
	ExplicitEquation::Register<ExponentialEquation>("ExponentialEquation");
	ExplicitEquation::Register<RealExponentEquation>("RealExponentEquation");
	ExplicitEquation::Register<Rational1Equation>("Rational1Equation");
	ExplicitEquation::Register<FourierEquation1>("FourierEquation1");
	ExplicitEquation::Register<FourierEquation2>("FourierEquation2");
	ExplicitEquation::Register<FourierEquation3>("FourierEquation3");
	ExplicitEquation::Register<FourierEquation4>("FourierEquation4");
	ExplicitEquation::Register<WeibullEquation>("WeibullEquation");
	ExplicitEquation::Register<WeibullCumulativeEquation>("WeibullCumulativeEquation");
	ExplicitEquation::Register<NormalEquation>("NormalEquation");
}

}