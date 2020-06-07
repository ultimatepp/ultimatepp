#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include "sundials.h"

namespace Upp {

using namespace Eigen;

#include <kinsol/kinsol.h>           		  /* access to KINSOL func., consts. */
#include <ida/ida.h>                          /* prototypes for IDA fcts., consts.    */
#include <nvector/nvector_serial.h>           /* access to serial N_Vector            */
#include <sunmatrix/sunmatrix_dense.h>        /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h>        /* access to dense SUNLinearSolver      */
#include <sunnonlinsol/sunnonlinsol_newton.h> /* access to Newton SUNNonlinearSolver  */
#include <sundials/sundials_types.h>          /* defs. of realtype, sunindextype      */
#include <sundials/sundials_math.h>           /* defs. of SUNRabs, SUNRexp, etc.      */


static void CheckMem(void *returnvalue, const char *funcname) {
	if (returnvalue == NULL) 
		throw Exc(Format(t_("SUNDIALS_ERROR: %s() failed - returned NULL pointer"), funcname));
}

static void CheckRet(int returnvalue, const char *funcname) {
	if (returnvalue < 0) 
	  	throw Exc(Format(t_("SUNDIALS_ERROR: %s() failed with retval = %d"), funcname, returnvalue));
}

static void PrintFinalStats(void *mem) {
	int retval;
	long int nst, nni, nje, nre, nreLS, netf, ncfn, nge;
	
	retval = IDAGetNumSteps(mem, &nst);
	CheckRet(retval, "IDAGetNumSteps");
	retval = IDAGetNumResEvals(mem, &nre);
	CheckRet(retval, "IDAGetNumResEvals");
	retval = IDAGetNumJacEvals(mem, &nje);
	CheckRet(retval, "IDAGetNumJacEvals");
	retval = IDAGetNumNonlinSolvIters(mem, &nni);
	CheckRet(retval, "IDAGetNumNonlinSolvIters");
	retval = IDAGetNumErrTestFails(mem, &netf);
	CheckRet(retval, "IDAGetNumErrTestFails");
	retval = IDAGetNumNonlinSolvConvFails(mem, &ncfn);
	CheckRet(retval, "IDAGetNumNonlinSolvConvFails");
	retval = IDAGetNumLinResEvals(mem, &nreLS);
	CheckRet(retval, "IDAGetNumLinResEvals");
	retval = IDAGetNumGEvals(mem, &nge);
	CheckRet(retval, "IDAGetNumGEvals");
	
	printf("\nFinal Run Statistics: \n\n");
	printf("Number of steps                    = %ld\n", nst);
	printf("Number of residual evaluations     = %ld\n", nre+nreLS);
	printf("Number of Jacobian evaluations     = %ld\n", nje);
	printf("Number of nonlinear iterations     = %ld\n", nni);
	printf("Number of error test failures      = %ld\n", netf);
	printf("Number of nonlinear conv. failures = %ld\n", ncfn);
	printf("Number of root fn. evaluations     = %ld\n", nge);
}

static void PrintIteration(realtype t, realtype *y, int neq) {
	Cout() << "\n" << Format("%7.3f ", t);
	for (int i = 0; i < neq; ++i)
		Cout() << Format("%12.4e ", y[i]);
}

static void PrintRootInfo(int *roots, int nroots) {
	Cout() << "\nRoots found[] = ";
	for (int i = 0; i < nroots; ++i)
		Cout() << roots[i] << " ";
}

static String GetIdaErrorMsg(int ret) {
	switch (ret) {
	case IDA_TOO_MUCH_WORK:	return "IDASolve: IDA_TOO_MUCH_WORK";
	case IDA_TOO_MUCH_ACC:  return "IDASolve: IDA_TOO_MUCH_ACC";
	case IDA_ERR_FAIL:      return "IDASolve: IDA_ERR_FAIL";
	case IDA_MEM_NULL:		return "ida_mem is NULL";
	case IDA_NO_MALLOC:		return "ida_mem was not allocated";	
	case IDA_ILL_INPUT:		return "bad value for icopt, tout1, or id";
	case IDA_LINIT_FAIL:	return "the linear solver linit routine failed";
 	case IDA_BAD_EWT:		return "zero value of some component of ewt";	
 	case IDA_RES_FAIL:		return "res had a non-recoverable error";	
 	case IDA_FIRST_RES_FAIL:return "res failed recoverably on the first call";	
 	case IDA_LSETUP_FAIL:	return "lsetup had a non-recoverable error";	
 	case IDA_LSOLVE_FAIL:	return "lsolve had a non-recoverable error";	
 	case IDA_NO_RECOVERY:	return "res, lsetup, or lsolve had a recoverable error, but IDACalcIC could not recover";	
 	case IDA_CONSTR_FAIL:	return "the inequality constraints could not be met";	
 	case IDA_LINESEARCH_FAIL:return"the linesearch failed (either on steptol test or on the maxbacks test)";
 	case IDA_CONV_FAIL:		return "the Newton iterations failed to converge";
	default:				return Format("Unknown SUndoasl eerror %d", ret);
	}
}
                	
void SolveDAE(const VectorXd &y, const VectorXd &dy, double dt, double maxt, Upp::Array<VectorXd> &res,  
		Function <bool(double t, const double y[], const double dy[], double residual[])>Residual, int numZero,
		Function <bool(double t, const double y[], const double dy[], double residual[])>ResidualZero) {
	ASSERT(y.size() == dy.size());
	Eigen::Index numEq = y.size();
	res.SetCount(int(numEq));
	for (int i = 0; i < numEq; ++i) {
		res[i].resize(int(maxt/dt)+1);
		res[i](0) = y[i];
	}
	SolveDAE(y.data(), dy.data(), int(numEq), dt, maxt, Residual, numZero, ResidualZero, [&](double t, const double y[], const double dy[], bool isZero, int *whichZero)->int {
		int iter = int(round(t/dt));
		for (int i = 0; i < int(numEq); ++i)
			res[i](iter) = y[i]; 
		return true;
	});		
}

void SolveDAE(const VectorXd &y, const VectorXd &dy, double dt, double maxt, 
		Upp::Array<VectorXd> &res, Upp::Array<VectorXd> &dres, 
		Function <bool(double t, const double y[], const double dy[], double residual[])>Residual, int numZero,
		Function <bool(double t, const double y[], const double dy[], double residual[])>ResidualZero) {
	ASSERT(y.size() == dy.size());
	Eigen::Index numEq = y.size();
	res.SetCount(int(numEq));
	dres.SetCount(int(numEq));
	for (int i = 0; i < numEq; ++i) {
		res[i].resize(int(maxt/dt)+1);
		res[i](0) = y[i];
		dres[i].resize(int(maxt/dt)+1);
		dres[i](0) = dy[i];
	}
	SolveDAE(y.data(), dy.data(), int(numEq), dt, maxt, Residual, numZero, ResidualZero, [&](double t, const double y[], const double dy[], bool isZero, int *whichZero)->int {
		int iter = int(round(t/dt));
		for (int i = 0; i < int(numEq); ++i) {
			res[i](iter) = y[i]; 
			dres[i](iter) = dy[i];
		}
		return true;
	});		
}

void SolveDAE(const double y[], const double dy[], int numEq, double dt, double maxt, 
		Function <bool(double t, const double y[], const double dy[], double residual[])> Residual, int numZero,  
		Function <bool(double t, const double y[], const double dy[], double residual[])> ResidualZero,
		Function <bool(double t, const double y[], const double dy[], bool isZero, int *whichZero)>OnIteration) {
	int retval, retvalr;
	Buffer<int> rootsfound(numEq);
	SUNNonlinearSolver NLS = NULL;
	SUNLinearSolver LS = NULL;
	SUNMatrix A = NULL;
	void *mem = NULL;
	N_Vector avtol = NULL, yy = NULL, yp = NULL;
	
	Exc error;
	try {
		yy = N_VNew_Serial(numEq);
		CheckMem((void *)yy, "N_VNew_Serial");
		yp = N_VNew_Serial(numEq);
		CheckMem((void *)yp, "N_VNew_Serial");
		avtol = N_VNew_Serial(numEq);
		CheckMem((void *)avtol, "N_VNew_Serial");
	
	  	/* Create and initialize  y, y', and absolute tolerance vectors. */
		realtype *yval = N_VGetArrayPointer(yy);
		memcpy(yval, y, numEq*sizeof(double));
		
		realtype *ypval = N_VGetArrayPointer(yp);
		memcpy(ypval, dy, numEq*sizeof(double));
		
		realtype rtol = 1.0e-4;
		
		realtype *atval = N_VGetArrayPointer(avtol);
		for (int i = 0; i < numEq; ++i)
			atval[i] = 1.0e-8;
	
	  	/* Integration limits */
	  	double t0 = 0;
	
		struct UserData {
			Function <bool(double t, const double y[], const double dy[], double residual[])> Residual;
			Function <bool(double t, const double y[], const double dy[], double residual[])> ResidualZero;
		} userData;
		
		userData.Residual = Residual;
		userData.ResidualZero = ResidualZero;
		
	    auto ResFun = [](realtype t, N_Vector _y, N_Vector _dy, N_Vector _res, void *user_data) { 
			realtype *y = N_VGetArrayPointer(_y);
			realtype *dy = N_VGetArrayPointer(_dy);
			realtype *res = N_VGetArrayPointer(_res);
			
			UserData &userData = *(UserData *)user_data;
			
			return userData.Residual(t, y, dy, res) ? 0 : -1;
		};
	    auto ResZeroFun = [](realtype t, N_Vector _y, N_Vector _dy, realtype *res, void *user_data) { 
			realtype *y = N_VGetArrayPointer(_y);
			realtype *dy = N_VGetArrayPointer(_dy);
			
			UserData &userData = *(UserData *)user_data;
			
			return userData.ResidualZero(t, y, dy, res) ? 0 : -1;
		};	
		
		/* Call IDACreate and IDAInit to initialize IDA memory */
		mem = IDACreate();
		CheckMem((void *)mem, "IDACreate");	
		
		retval = IDASetUserData(mem, (void *)&userData);
		CheckRet(retval, "IDASetUserData");
	
		retval = IDAInit(mem, ResFun, t0, yy, yp);
		CheckRet(retval, "IDAInit");
		
		/* Call IDASVtolerances to set tolerances */
		retval = IDASVtolerances(mem, rtol, avtol);
		CheckRet(retval, "IDASVtolerances");
		
		/* Call IDARootInit to specify the root function grob with 2 components */
		if (ResidualZero && numZero > 0) {
			retval = IDARootInit(mem, numZero, ResZeroFun);
			CheckRet(retval, "IDARootInit");
		}
		
		/* Create dense SUNMatrix for use in linear solver */
		A = SUNDenseMatrix(numEq, numEq);
		CheckMem((void *)A, "SUNDenseMatrix");
		
		/* Create dense SUNLinearSolver object */
		LS = SUNLinSol_Dense(yy, A);
		CheckMem((void *)LS, "SUNLinSol_Dense");
		
		/* Attach the matrix and linear solver */
		retval = IDASetLinearSolver(mem, LS, A);
		CheckRet(retval, "IDASetLinearSolver");
		
		retval = IDASetMaxNumSteps(mem, 20000);
		CheckRet(retval, "IDASetLinearSolver");
		
		/* Create Newton SUNNonlinearSolver object. IDA uses a
		* Newton SUNNonlinearSolver by default, so it is unecessary
		* to create it and attach it. It is done in this example code
		* solely for demonstration purposes. */
		NLS = SUNNonlinSol_Newton(yy);
		CheckMem((void *)NLS, "SUNNonlinSol_Newton");
		
		/* Attach the nonlinear solver */
		retval = IDASetNonlinearSolver(mem, NLS);
		CheckRet(retval, "IDASetNonlinearSolver");
		
		/* In loop, call IDASolve, print results, and test for error.
		Break out of loop when NOUT preset output times have been reached. */
	
		int iiter = 1; 
		double tnext = dt;
		double titer;
		while(true) {
			retval = IDASolve(mem, tnext, &titer, yy, yp, IDA_NORMAL);
			CheckRet(retval, "IDASolve");
		
			realtype *y  = N_VGetArrayPointer(yy);
			realtype *dy = N_VGetArrayPointer(yp);
			
			//PrintIteration(titer, y, numEq);
			
			if (retval == IDA_ROOT_RETURN) {
				retvalr = IDAGetRootInfo(mem, rootsfound);
				CheckRet(retvalr, "IDAGetRootInfo");
				//PrintRootInfo(rootsfound, numEq);
			} else if (retval == IDA_SUCCESS) {
				iiter++;
				tnext = iiter*dt;
			} else		
				throw Exc(Format(t_("Sundials IDA error: %s"), GetIdaErrorMsg(retval)));
			
			if(OnIteration && !OnIteration(titer, y, dy, retval == IDA_ROOT_RETURN, rootsfound))
				break;
			else if (tnext > maxt) 
				break;
	  	}
	  	//PrintFinalStats(mem);
	} catch(Exc err) {
		error = err;
	}
	IDAFree(&mem);
	SUNNonlinSolFree(NLS);
	SUNLinSolFree(LS);
	SUNMatDestroy(A);
	N_VDestroy(avtol);
	N_VDestroy(yy);
	N_VDestroy(yp);
	
	if (!error.IsEmpty())
		throw error;
}

/*
0 	then no constraint is imposed on ui.
1 	then ui will be constrained to be ui 0:0.
-1 	then ui will be constrained to be ui 0:0.
2 	then ui will be constrained to be ui > 0:0.
-2 	then ui will be constrained to be ui < 0:0.
*/
void SolveNonLinearEquationsSun(double y[], int numEq, 
			Function <bool(const double b[], double residuals[])> Residual, int constraints[]) {
	N_Vector s = NULL;
	void *kmem = NULL;
	
  	Exc error;
  	try {
  		N_Vector u = N_VNew_Serial(numEq);	
		CheckMem((void *)u, "N_VNew_Serial");
  		realtype *udata = NV_DATA_S(u);
 		memcpy(udata, y, numEq*sizeof(double));		
  		
		s = N_VNew_Serial(numEq);
	  	CheckMem((void *)s, "N_VNew_Serial");
	
		N_VConst_Serial(1. ,s); /* no scaling */
		
		double fnormtol = 1.e-6;
		double scsteptol = 1.e-6;
		int numIteraciones = 500;
	  	bool ret = true;

	  	kmem = KINCreate();
	  	CheckMem((void *)kmem, "KINCreate");
	  	
	  	auto ErrorFun = [](int error_code, const char *module, const char *function, char *msg, void *data) {
		  	String serror = Format("%d, module %s, function %s", error_code, module, function);
		  	if (error_code == KIN_WARNING)
		    	Cout() << Format(t_("Kinsol Warning (%s): %s"), serror, msg);
		  	else {
		  		char *str = KINGetReturnFlagName(error_code);
		  		String cerror(str);
		  		free(str);
		    	throw Exc(Format(t_("Kinsol Error %s (%s): %s"), cerror, serror, msg));
		  	}
		};
	  	
	  	int flag;
	  	
		flag = KINSetErrHandlerFn(kmem, ErrorFun, NULL); 
		CheckRet(flag, "KINSetErrHandlerFn");
	  	
	  	struct UserData {
			Function <bool(const double y[], double residual[])> Residual;
		} userData;
		
		userData.Residual = Residual;	  	
	  	
	  	flag = KINSetUserData(kmem, &userData);
	  	CheckRet(flag, "KINSetUserData");
		flag = KINSetFuncNormTol(kmem, fnormtol);
		CheckRet(flag, "KINSetFuncNormTol");
		flag = KINSetScaledStepTol(kmem, scsteptol);
		CheckRet(flag, "KINSetScaledStepTol");
		flag = KINSetNumMaxIters(kmem, numIteraciones);
	  	CheckRet(flag, "KINSetNumMaxIters");
		
	  	auto ResFun = [](N_Vector u, N_Vector f, void *user_data) { 
			realtype *y = NV_DATA_S(u);
			realtype *res = NV_DATA_S(f);
			
			UserData &userData = *(UserData *)user_data;
			
			return userData.Residual(y, res) ? 0 : 1;
		};
		
		flag = KINInit(kmem, ResFun, u);
		CheckRet(flag, "KINInit");
	
		if (constraints != nullptr) {
			N_Vector constr = N_VNew_Serial(numEq);	
			CheckMem((void *)constr, "N_VNew_Serial");
			realtype *constrdata = NV_DATA_S(constr);
			for (int i = 0; i < numEq; ++i)
			constrdata[i] = constraints[i];
  		
		 	flag = KINSetConstraints(kmem, constr);
		  	CheckRet(flag, "KINSetConstraints");
		  	
			N_VDestroy_Serial(constr);
		}
		
		/* Create dense SUNMatrix */
		SUNMatrix J = SUNDenseMatrix(numEq, numEq);
		CheckMem((void *)J, "SUNDenseMatrix");
		
		/* Create dense SUNLinearSolver object */
		SUNLinearSolver LS = SUNLinSol_Dense(u, J);
		CheckMem((void *)LS, "SUNLinSol_Dense");
		
		/* Attach the matrix and linear solver to KINSOL */
		flag = KINSetLinearSolver(kmem, LS, J);
		CheckRet(flag, "KINSetLinearSolver");
	
		int glstr = KIN_NONE;	// KIN_NONE 		KIN_LINESEARCH	Using line search
		int mset = 1;			// 1 Exact Newton	0 Modified Newton
	
		flag = KINSetMaxSetupCalls(kmem, mset);
		CheckRet(flag, "KINSetMaxSetupCalls");
		
		flag = KINSol(kmem, u, glstr, s, s);
		CheckRet(flag, "KINSol");
		
		for (int i = 0; i < numEq; ++i) 
			if (udata[i] != udata[i]) 
				throw Exc(Format(t_("Obtained NaN in value %d"), i));
		 memcpy(y, udata, numEq*sizeof(double));		
		
  	} catch(Exc err) {
  		error = err;
  	}
 	N_VDestroy_Serial(s);
  	KINFree(&kmem);
  	if (!error.IsEmpty())
  		throw error;
}

}
