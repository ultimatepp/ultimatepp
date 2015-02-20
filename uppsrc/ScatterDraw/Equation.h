#ifndef _ScatterDraw_Equation_h_
#define _ScatterDraw_Equation_h_

NAMESPACE_UPP


#define FormatCoeff(id, numDigits)		(IsNull(numDigits) ? (String("C") + FormatInt(id)) : FormatDoubleFix(coeff[id], numDigits))

class ExplicitEquation : public DataSource {
public:
	ExplicitEquation() {isExplicit = true;}

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
	virtual void GuessCoeff(DataSource &series)	= 0;

	virtual double f(double x1) 			= 0;
	virtual double f(double x1, double x2) 	{NEVER();	return Null;}
	virtual double f(Vector <double> x) 	{NEVER();	return Null;}
	virtual String GetName() = 0;
	virtual String GetFullName()			{return GetName();}
	virtual String GetEquation(int numDigits = 3) = 0;
	virtual inline int64 GetCount()			{return Null;}
	
	void SetNumDigits(int n)				{numDigits = n;}
	int GetNumDigits()						{return numDigits;}
	void SetMaxFitFunctionEvaluations(int n){maxFitFunctionEvaluations = n;}
	int GetMaxFitFunctionEvaluations()		{return maxFitFunctionEvaluations;}
	
	friend struct Equation_functor;
	
	const Array<double> &GetCoeff()			{return coeff;}

	template<class T>	
	static void Register(const String& name) {
		classMap().FindAdd(name, __Create<T>);
	}
	static void Unregister(const String& name) {
		int i = NameIndex(name);
		ASSERT(i >= 0);
		classMap().Remove(i);
	}
	static int            	 NameIndex(const String& name) {return classMap().Find(name);}
	static int            	 GetEquationCount()            {return classMap().GetCount();}
	static ExplicitEquation* Create(int i)                 {return classMap()[i]();}
	
	int GetNumCoeff(int num) {return coeff.GetCount();}
	
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
	
	typedef ExplicitEquation* (*CreateFunc)();
	template<class T>	
	static ExplicitEquation*                      __Create() {return new T;}
	static VectorMap<String, CreateFunc>& classMap() {static VectorMap<String, CreateFunc> cMap; return cMap;}
};

class LinearEquation : public ExplicitEquation {
public:
	LinearEquation() 					{SetCoeff(0, 0);}
	LinearEquation(double c0, double c1){SetCoeff(c0, c1);}
	double f(double x) 					{return coeff[0] + x*coeff[1];}
	virtual String GetName() 			{return t_("Linear");}
	virtual String GetEquation(int numDigits = 3) {	
		String ret = Format("%s + %s*x", FormatCoeff(0, numDigits), FormatCoeff(1, numDigits));
		ret.Replace("+ -", "- ");
		return ret;
	}
	void SetDegree(int num)				{NEVER();}
	virtual void GuessCoeff(DataSource &series)	{coeff[0] = series.AvgY();}
};

class PolynomialEquation : public ExplicitEquation {
public:
	PolynomialEquation() 				       	{}
	PolynomialEquation(const Array<double>& c) 	{SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			       	{return t_("Polynomial");}
	virtual String GetFullName() 		       	{return t_("Polynomial") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation(int numDigits = 3);
	void SetDegree(int num)				       	{degree = num;	SetNumCoeff(num + 1);}
	virtual void GuessCoeff(DataSource &series) {
		coeff[0] = series.AvgY();
		int realDegree = degree;
		for (degree = 2; degree < realDegree; degree++) 
			Fit(series);
	}
};	

class PolynomialEquation2 : public PolynomialEquation {
public:
	PolynomialEquation2() {SetDegree(2);}
};

class PolynomialEquation3 : public PolynomialEquation {
public:
	PolynomialEquation3() {SetDegree(3);}
};

class PolynomialEquation4 : public PolynomialEquation {
public:
	PolynomialEquation4() {SetDegree(4);}
};

class PolynomialEquation5 : public PolynomialEquation {
public:
	PolynomialEquation5() {SetDegree(5);}
};

class SinEquation : public ExplicitEquation {
public:
	SinEquation() 					{coeff.Clear();	coeff << 0. << 0.1 << 0.1 << 0.1;}
	SinEquation(double offset, double A, double w, double phi) 	{Init(offset, A, w, phi);}
	void Init(double offset, double A, double w, double phi) 	{coeff.Clear();	coeff << offset << A << w << phi;}
	double f(double x)				{return coeff[0] + coeff[1]*sin(coeff[2]*x + coeff[3]);}
	virtual String GetName() 		{return t_("Sine");}
	virtual String GetEquation(int numDigits = 3) {
		String ret = Format("%s + %s*sin(%s*t + %s)", FormatCoeff(0, numDigits), FormatCoeff(1, numDigits)
													, FormatCoeff(2, numDigits), FormatCoeff(3, numDigits));
		ret.Replace("+ -", "- ");
		return ret;
	}													
	void SetDegree(int num)				{NEVER();}
	virtual void GuessCoeff(DataSource &series)	{
		coeff[0] = series.AvgY();	
		coeff[1] = series.SinEstim_Amplitude(coeff[0]);
		double frequency, phase;
		if (series.SinEstim_FreqPhase(frequency, phase, coeff[0])) {
			coeff[2] = frequency;
			coeff[3] = phase;
		}
	}
};
	
class FourierEquation : public ExplicitEquation {
public:
	FourierEquation() 					        {}
	FourierEquation(const Array<double>& c) 	{SetCoeff(c);}
	double f(double x);
	virtual String GetName() 			        {return t_("Fourier");}
	virtual String GetFullName() 		        {return t_("Fourier") + String(" n = ") + FormatInt(degree);}
	virtual String GetEquation(int numDigits = 3);
	virtual void GuessCoeff(DataSource &series) {coeff[0] = series.AvgY();}
	void SetDegree(int num)				        {degree = num;	SetNumCoeff(2*num + 2);}
};

class FourierEquation1 : public FourierEquation {
public:
	FourierEquation1() {SetDegree(1);}
};

class FourierEquation2 : public FourierEquation {
public:
	FourierEquation2() {SetDegree(2);}
};

class FourierEquation3 : public FourierEquation {
public:
	FourierEquation3() {SetDegree(3);}
};

class FourierEquation4 : public FourierEquation {
public:
	FourierEquation4() {SetDegree(4);}
};

class ExponentialEquation : public ExplicitEquation {
public:
	ExponentialEquation() 						{SetCoeff(1, 0);}
	ExponentialEquation(double c0, double c1)	{SetCoeff(c0, c1);}
	double f(double x) 					{return coeff[0]*exp(-x) + coeff[1];}
	virtual String GetName() 			{return t_("Exponential");}
	virtual String GetEquation(int numDigits = 3) {
		String ret = Format("%s*e^-x + %s", FormatCoeff(0, numDigits), FormatCoeff(1, numDigits));
		ret.Replace("+ -", "- ");
		return ret;
	}	
	virtual void GuessCoeff(DataSource &series) {}
	void SetDegree(int num)				{NEVER();}
};

class Rational1Equation : public ExplicitEquation {
public:
	Rational1Equation() 				{SetCoeff(1, 0, 0);}
	Rational1Equation(double c0, double c1, double c2)	{SetCoeff(c0, c1, c2);}
	double f(double x) 					{return coeff[0]/(x + coeff[1]) + coeff[2];}
	virtual String GetName() 			{return t_("Rational_1");}
	virtual String GetEquation(int numDigits = 3) {
		String ret = Format("%s/(x + %s) + %s", FormatCoeff(0, numDigits), FormatCoeff(1, numDigits), FormatCoeff(2, numDigits));
		ret.Replace("+ -", "- ");
		return ret;
	}			
	virtual void GuessCoeff(DataSource &series) {}
	void SetDegree(int num)				{NEVER();}
};


class EvalExpr {
public:
	EvalExpr();
	double Eval(String line);
	String EvalStr(String line, int numDigits = 3);
	void SetCaseSensitivity(bool val = true) {noCase = !val;}
	
	const String &GetFunction(int id) 						{return functions.GetKey(id);}
	int GetFunctionsCount() 								{return functions.GetCount();}
	
	void SetConstant(String name, double value = 0)			{constants.GetAdd(name, value);}
	void SetConstant(int id, double value = 0)				{constants[id] = value;}
	double GetConstant(String name)							{return constants.Get(name, Null);}
	void GetConstant(int id, String &name, double &value)	{name = constants.GetKey(id); value = constants[id];}
	int GetConstantsCount() 								{return constants.GetCount();}

	void SetVariable(String name, double value = 0)			{variables.GetAdd(name, value);}
	void SetVariable(int id, double value = 0)				{variables[id] = value;}
	double GetVariable(String name)							{return variables.Get(name, Null);}	
	void GetVariable(int id, String &name, double &value)	{name = variables.GetKey(id); value = variables[id];}
	int GetVariablesCount() 								{return variables.GetCount();}

private:
	void *Functions_Get(CParser& p);
	double Term(CParser& p);
	double Pow(CParser& p);
	double Mul(CParser& p);
	double Exp(CParser& p);
	String TermStr(CParser& p, int numDigits);
	String PowStr(CParser& p, int numDigits);
	String MulStr(CParser& p, int numDigits);
	String ExpStr(CParser& p, int numDigits);
	bool noCase;
	
	VectorMap<String, double> constants;
	VectorMap<String, double> variables;
	VectorMap<String, double (*)(double)> functions;
};

class UserEquation : public ExplicitEquation {
public:
	UserEquation() {}
	UserEquation(String _name, String _strEquation, String varHoriz = "x")	{Init(_name, _strEquation, varHoriz);}
	void Init(String _name, String _strEquation, String varHoriz = "x") {
		name = _name;
		strEquation = _strEquation;
		eval.SetConstant(varHoriz);
		idx = eval.GetConstantsCount() - 1;
		eval.EvalStr(strEquation);
		coeff.SetCount(eval.GetVariablesCount());
	}
	double f(double x) {
		eval.SetConstant(idx, x);
		for (int i = 0; i < coeff.GetCount(); ++i)
			eval.SetVariable(i, coeff[i]);
		return eval.Eval(strEquation);
	}
	String SetName(String _name) 					{name = _name;}
	virtual String GetName() 						{return name;}
	virtual String GetEquation(int numDigits = 3)	{return eval.EvalStr(strEquation, numDigits);}
	virtual void GuessCoeff(DataSource &series) 	{}
	void SetDegree(int num)							{NEVER();}

private:
	String name;
	String strEquation;
	EvalExpr eval;
	int idx;
};


END_UPP_NAMESPACE

#endif
