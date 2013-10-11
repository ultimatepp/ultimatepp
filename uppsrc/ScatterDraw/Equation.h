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
	void SetCoeff(Array<double> c) {
		coeff.SetCount(c.GetCount());
		for (int i = 0; i < c.GetCount(); ++i)
			coeff[i] = c[i];
	}
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
	PolynomialEquation() 				{}
	PolynomialEquation(Array<double> c) {SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			{return t_("Polynomial");}
	virtual String GetFullName() 		{return t_("Polynomial") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation();
	void SetDegree(int num)				{degree = num;	SetNumCoeff(num + 1);}
};

class FourierEquation : public ExplicitEquation {
public:
	FourierEquation() 					{}
	FourierEquation(Array<double> c) 	{SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			{return t_("Fourier");}
	virtual String GetFullName() 		{return t_("Fourier") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation();
	void SetDegree(int num)				{degree = num;	SetNumCoeff(2*num + 2);}
};

END_UPP_NAMESPACE

#endif
