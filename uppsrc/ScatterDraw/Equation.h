#ifndef _ScatterDraw_Equation_h_
#define _ScatterDraw_Equation_h_

NAMESPACE_UPP

class ExplicitEquation : public DataSource {
public:
	ExplicitEquation() 				{isExplicit = true;}

	virtual void SetDegree(int num) = 0;
	enum FitError {
		NoError = 1, 
		InadequateDataSource = -1,
		SmallDataSource = -2,
		ImproperInputParameters = -3,
		TooManyFunctionEvaluation = -4
	};
	FitError Fit(DataSource &series, double &r2);
	FitError Fit(DataSource &series)		{double dummy; return Fit(series, dummy);}

	virtual double f(double x1) 			= 0;
	virtual double f(double x1, double x2) 	{NEVER();	return Null;}
	virtual double f(Vector <double> x) 	{NEVER();	return Null;}
	virtual String GetName() = 0;
	virtual String GetFullName()			{return GetName();}
	virtual String GetEquation() = 0;
	
	void SetNumDigits(int n)				{numDigits = n;}
	int GetNumDigits()						{return numDigits;}
	void SetMaxFitFunctionEvaluations(int n){maxFitFunctionEvaluations = n;}
	int GetMaxFitFunctionEvaluations()		{return maxFitFunctionEvaluations;}
	
	friend struct Equation_functor;
	
	const Array<double> &GetCoeff()			{return coeff;}
	
protected:
	Array<double> coeff;
	int degree;
	static int numDigits, maxFitFunctionEvaluations;
	
	void SetNumCoeff(int num);
	void SetCoeff(const Array<double>& c)           {coeff = clone(c);}
	void SetCoeff(double c0, double c1, double c2)	{coeff.Clear();	coeff << c0 << c1 << c2;}
	void SetCoeff(double c0, double c1) 			{coeff.Clear();	coeff << c0 << c1;}
	void SetCoeff(double c0) 						{coeff.Clear();	coeff << c0;}
	void SetCoeffVal(int id, double c) 				{coeff[id] = c;}
};

class LinearEquation : public ExplicitEquation {
public:
	LinearEquation() 					{SetCoeff(0, 0);}
	LinearEquation(double c0, double c1){SetCoeff(c0, c1);}
	double f(double x) 					{return coeff[0] + x*coeff[1];}
	virtual String GetName() 			{return t_("Linear");}
	virtual String GetEquation() 		{return FormatDoubleFix(coeff[0], numDigits) + " " + 
												(coeff[1] >= 0 ? "+" : "-") + " " +
												FormatDoubleFix(fabs(coeff[1]), numDigits) + "*x";}
	void SetDegree(int num)				{NEVER();}
};

class PolynomialEquation : public ExplicitEquation {
public:
	PolynomialEquation() 				       	{}
	PolynomialEquation(const Array<double>& c) 	{SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			       	{return t_("Polynomial");}
	virtual String GetFullName() 		       	{return t_("Polynomial") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation();
	void SetDegree(int num)				       	{degree = num;	SetNumCoeff(num + 1);}
};	

class SinEquation : public ExplicitEquation {
public:
	SinEquation() 					{coeff.Clear();	coeff << 0. << 0.1 << 0.1 << 0.1;}
	SinEquation(double offset, double A, double w, double phi) 	{coeff.Clear();	coeff << offset << A << w << phi;}
	double f(double x)				{return coeff[0] + coeff[1]*sin(coeff[2]*x + coeff[3]);}
	virtual String GetName() 		{return t_("Sinusoidal");}
	virtual String GetEquation()	{return FormatDoubleFix(coeff[0], numDigits) + " + " + 
											FormatDoubleFix(coeff[1], numDigits) + "*sin(" +
											FormatDoubleFix(coeff[2], numDigits) + "*t + " +
											FormatDoubleFix(coeff[3], numDigits) + ")";}
	void SetDegree(int num)			{NEVER();}
};

class FourierEquation : public ExplicitEquation {
public:
	FourierEquation() 					        {}
	FourierEquation(const Array<double>& c) 	{SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			        {return t_("Fourier");}
	virtual String GetFullName() 		        {return t_("Fourier") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation();
	void SetDegree(int num)				        {degree = num;	SetNumCoeff(2*num + 2);}
};

class ExponentialEquation : public ExplicitEquation {
public:
	ExponentialEquation() 						{SetCoeff(1, 0);}
	ExponentialEquation(double c0, double c1)	{SetCoeff(c0, c1);}
	double f(double x) 					{return coeff[0]*exp(-x) + coeff[1];}
	virtual String GetName() 			{return t_("Exponential");}
	virtual String GetEquation() 		{return FormatDoubleFix(fabs(coeff[0]), numDigits) + "*e^-x " +
												(coeff[1] >= 0 ? "+" : "-") + " " + FormatDoubleFix(coeff[1], numDigits);}
	void SetDegree(int num)				{NEVER();}
};

class Rational1Equation : public ExplicitEquation {
public:
	Rational1Equation() 				{SetCoeff(1, 0, 0);}
	Rational1Equation(double c0, double c1, double c2)	{SetCoeff(c0, c1, c2);}
	double f(double x) 					{return coeff[0]/(x + coeff[1]) + coeff[2];}
	virtual String GetName() 			{return t_("Rational_1");}
	virtual String GetEquation() 		{return FormatDoubleFix(fabs(coeff[0]), numDigits) + "/(x " + (coeff[1] >= 0 ? "+" : "-") + " " + 
												FormatDoubleFix(fabs(coeff[1]), numDigits) + ") " + (coeff[2] >= 0 ? "+" : "-") + " " + 
												FormatDoubleFix(fabs(coeff[2]), numDigits);}
	void SetDegree(int num)				{NEVER();}
};


END_UPP_NAMESPACE

#endif
