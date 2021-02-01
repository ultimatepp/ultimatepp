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
	functor.datasetLen = Eigen::Index(serie.GetCount());
	
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
	if (!IsNum(mean))
		mean = serie.AvgY();
	double sse = 0, sst = 0;
	for (int64 i = 0; i < serie.GetCount(); ++i) {
		double y = serie.y(i);
		if (!!IsNum(y)) {
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

void EvalExpr::EvalThrowError(CParserPP &p, const char *s) {
	CParserPP::Pos pos = p.GetPos();
	CParserPP::Error err(Format("(%d): ", pos.GetColumn()) + String(s));
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

doubleUnit uround(doubleUnit val) {
	val.val = round(val.val);
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
	allowString = false;
	
	constants.Add("pi", doubleUnit(M_PI));
	constants.Add("e", doubleUnit(M_E));
	
	functions.Add("abs", ufabs);
	functions.Add("ceil", uceil);
	functions.Add("floor", ufloor);
	functions.Add("round", uround);
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

doubleUnit EvalExpr::Term(CParserPP& p) {
	bool isneg = p.Char('-');
	if (p.IsId()) {
		String strId = p.ReadIdPP();
		if(doubleUnit (*function)(doubleUnit) = functions.Get(strId, 0)) {
			p.PassChar('(');
			doubleUnit x(Exp(p));
			p.PassChar(')');
			doubleUnit ret(function(x));
			if (IsNull(ret))
				EvalThrowError(p, Format(t_("Error in %s(%f)"), strId, x.val));	
			if (isneg)
				ret.Neg();
			return ret;
		}	
		String strIdSearch;
		if (noCase)
			strIdSearch = ToLower(strId);
		else
			strIdSearch = strId;
		doubleUnit ret(constants.Get(strIdSearch, Null));
		if (IsNull(ret)) {
			int id = FindVariable(strIdSearch);
			if (id >= 0)
				ret = variables[id];
			else {
				if (errorIfUndefined) {
					lastError = Format(t_("Unknown identifier '%s'"), strId);
					return Null;
				}
					//EvalThrowError(p, Format(t_("Unknown identifier '%s'"), strId));	
				lastVariableSetId = variables.FindAdd(strIdSearch, 0);
				ret = variables[lastVariableSetId];
			}
		}
		if (isneg)
			ret.Neg();
		return ret;
	} else if (p.Char('(')) {
		doubleUnit x(Exp(p));
		p.PassChar(')');
		if (isneg)
			x.Neg();
		return x;
	} else {
		if (p.IsChar2('.', '.'))
			p.ThrowError("missing number");
		doubleUnit x(p.ReadDouble());
		if (isneg)
			x.Neg();
		return x;
	}
}

doubleUnit EvalExpr::Pow(CParserPP& p) {
	doubleUnit x(Term(p));
	for(;;) 
		if(p.Char('^')) {
			//if (x.val < 0)
			//	EvalThrowError(p, t_("Complex number"));
			x.Exp(Term(p));
		} else
			return x;
}

doubleUnit EvalExpr::Mul(CParserPP& p) {
	doubleUnit x(Pow(p));
	for(;;) 
		if(p.Char('*'))
			x.Mult(Pow(p));
		else if (p.Char2('|', '|')) 
			x.ResParallel(Pow(p));
		else if(memcmp(p.GetPtr(), "·", strlen("·")) == 0) {
			CParserPP::Pos pos = p.GetPos();
			pos.ptr += strlen("·");
			p.SetPos(pos);
			p.Spaces();
			x.Mult(Pow(p));
		} else if(p.Char('/')) {
			x.Div(Pow(p));
		} else if(memcmp(p.GetPtr(), "º", strlen("º")) == 0) { 
			CParserPP::Pos pos = p.GetPos();
			pos.ptr += strlen("º");
			p.SetPos(pos);
			p.Spaces();
			x.Mult(doubleUnit(M_PI/180.));
		} else
			return x;
}

doubleUnit EvalExpr::Exp(CParserPP& p) {
	doubleUnit x(Mul(p));
	for(;;) 
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

doubleUnit EvalExpr::AssignVariable(String var, String expr) {
	doubleUnit ret;
	if (noCase)
		var = ToLower(var);
	int idalloc = FindAddVariable(var);
	try {
		p.Set(expr);
		
		ret.Set(Exp(p));
		if (!IsNull(ret)) {
			SetVariable(idalloc, ret);
			return ret;
		} else {
			if (allowString) {
				ret.sval = expr;
				SetVariable(idalloc, ret);
				return ret;	
			}
			return Null;
		}
	} catch(CParserPP::Error e) {
		if (allowString) {
			ret.sval = expr;
			SetVariable(idalloc, ret);
			return ret;	
		}
		lastError = e;
		return Null;
	} catch(Exc e) {
		lastError = e;
		return Null;
	} catch(...) {
		lastError = "Unknown error";
		return Null;
	} 
}

void EvalExpr::RenameVariable(String varname, String newvarname) {
	if (noCase) {
		varname = ToLower(varname);
		newvarname = ToLower(newvarname);
	}
	try {
		int id = variables.Find(varname);
		if (id >= 0)
			variables.SetKey(id, newvarname);
	} catch(CParserPP::Error e) {
		lastError = e;
	} catch(Exc e) {
		lastError = e;
	}	
}

doubleUnit EvalExpr::AssignVariable(String var, double d) {
	if (noCase)
		var = ToLower(var);
	try {
		doubleUnit ret(d);
		SetVariable(var, ret);
		return ret;
	} catch(CParserPP::Error e) {
		lastError = e;
		return Null;
	} catch(Exc e) {
		lastError = e;
		return Null;
	} catch(...) {
		lastError = "Unknown error";
		return Null;
	} 
}
		
doubleUnit EvalExpr::Eval(String line) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	p.Set(line);
	try {
		if(p.IsId()) {
			CParserPP::Pos pos = p.GetPos();
			String var = p.ReadIdPP();
			if(p.Char('=')) {
				if (noCase)
					var = ToLower(var);
				doubleUnit ret(Exp(p));
				SetVariable(var, ret);
				return ret;
			} else {
				p.SetPos(pos);
				return Exp(p);
			}
		} else
			return Exp(p);
	} catch(CParserPP::Error e) {
		lastError = e;
		return Null;
	} catch(Exc e) {
		lastError = e;
		return Null;
	}
}

String EvalExpr::TermStr(CParserPP& p, int numDigits) {
	if(p.IsId()) {
		String strId = p.ReadIdPP();
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
				lastVariableSetId = variables.FindAdd(strId, 0);
			return strId;
		} else {
			if (constants.Find(strId) >= 0)
				return strId;
			else {
				lastVariableSetId = variables.FindAdd(strId, 0);
				return FormatDoubleFix(variables[lastVariableSetId].val, numDigits);
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

String EvalExpr::PowStr(CParserPP& p, int numDigits) {
	String x = TermStr(p, numDigits);
	for(;;)
		if(p.Char('^'))
			x = x + "^" + TermStr(p, numDigits);
		else
			return x;
}

String EvalExpr::MulStr(CParserPP& p, int numDigits) {
	String x = PowStr(p, numDigits);
	for(;;)
		if(p.Char('*'))
			x = x + "*" + MulStr(p, numDigits);
		else if(p.Char('/')) 
			x = x + "/" + PowStr(p, numDigits);
		else
			return x;
}

String EvalExpr::ExpStr(CParserPP& p, int numDigits) {
	String x = MulStr(p, numDigits);
	for(;;) 
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

String EvalExpr::EvalStr(String line, int numDigits) {
	line = TrimBoth(line);
	if (line.IsEmpty())
		return Null;
	
	CParserPP p(line);
	try {
		if(p.IsId()) {
			CParserPP::Pos pos = p.GetPos();
			String var = p.ReadIdPP();
			if(p.Char('=')) {
				String ret = ExpStr(p, numDigits);
				lastVariableSetId = variables.FindAdd(var, 0);
				return var + " = " + ret;
			} else {
				p.SetPos(pos);
				return ExpStr(p, numDigits);
			}
		} else
			return ExpStr(p, numDigits);
	} catch(CParserPP::Error e) {
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
		if (!!IsNum(data.x(i)) && !!IsNum(data.y(i)))
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
		
	Init(x, y);
	
	coeff.SetCount(1);
	
	return NoError;
}

void Spline::Init(const double *x, const double *y, int num) {
    nscoeff = num - 1;
    
    Buffer<double> h(nscoeff);
    for(int i = 0; i < nscoeff; ++i)
        h[i] = x[i+1] - x[i];

    Buffer<double> alpha(nscoeff);
    for(int i = 1; i < nscoeff; ++i)
        alpha[i] = 3*(y[i+1] - y[i])/h[i] - 3*(y[i] - y[i-1])/h[i-1];

    Buffer<double> c(nscoeff+1), l(nscoeff+1), mu(nscoeff+1), z(nscoeff+1);
    l[0] = 1;
    mu[0] = 0;
    z[0] = 0;

    for(int i = 1; i < nscoeff; ++i) {
        l[i] = 2*(x[i+1] - x[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }

    l[nscoeff] = 1;
    z[nscoeff] = 0;
    c[nscoeff] = 0;

	scoeff.Alloc(nscoeff);
    for(int i = nscoeff-1; i >= 0; --i) {
        c[i] = z[i] - mu[i] * c[i+1];
        scoeff[i].b = (y[i+1] - y[i])/h[i] - h[i]*(c[i+1] + 2*c[i])/3;
        scoeff[i].d = (c[i+1] - c[i])/3/h[i];
    }

    for(int i = 0; i < nscoeff; ++i) {
        scoeff[i].x = x[i];
        scoeff[i].a = y[i];
        scoeff[i].c = c[i];
    }
    xlast = x[num-1];
}

int Spline::GetPieceIndex(double x) const {
	ASSERT(nscoeff > 0);
    int j;
    for (j = 0; j < nscoeff; j++) {
        if (scoeff[j].x > x) {
            if (j == 0)
                j = 1;
            break;
        }
    }
    return --j;
}

double Spline::f(double x) const {
	int j = GetPieceIndex(x);

    double dx = x - scoeff[j].x;
    double dx2 = dx*dx;
    return scoeff[j].a + scoeff[j].b*dx + scoeff[j].c*dx*dx + scoeff[j].d*dx*dx2;
}

double Spline::df(double x) const {
	int j = GetPieceIndex(x);

    double dx = x - scoeff[j].x;
    return scoeff[j].b + scoeff[j].c*2.*dx + scoeff[j].d*3.*dx*dx;
}

double Spline::d2f(double x) const {
	int j = GetPieceIndex(x);

    double dx = x - scoeff[j].x;
    return scoeff[j].c*2. + scoeff[j].d*6.*dx;
}

double Spline::Integral0(const Coeff &c, double x) {
	double x2 = x*x;
	return c.a*x + c.b*x2/2 + c.c*x*x2/3 + c.d*x2*x2/4;
}

double Spline::Integral(double from, double to) const {
	int ifrom;
	if (!IsNum(from)) {
		ifrom = 0;
		from = scoeff[0].x;
	} else
		ifrom = GetPieceIndex(from);
	int ito;
	if (!IsNum(to)) {
		ito = nscoeff-1;
		to = xlast;
	} else
		ito = GetPieceIndex(to);
	
	ASSERT(ifrom <= ito);
	if (ifrom > ito)
		return 0;
		 
	double res = 0;
	for (int i = ifrom; i < ito; ++i) {
		double val = Integral0(scoeff[i], scoeff[i+1].x - scoeff[i].x) - Integral0(scoeff[i], from - scoeff[i].x);
		res += val;
		from = scoeff[i+1].x;
	}
	double bal =  Integral0(scoeff[ito], to - scoeff[ito].x) - Integral0(scoeff[ito], from - scoeff[ito].x);
	res += bal;
	return res;
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