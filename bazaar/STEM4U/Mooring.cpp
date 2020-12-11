#include <Core/Core.h>
#include <plugin/Eigen/Eigen.h>
#include "Sundials.h"
#include "Mooring.h"


namespace Upp {


MooringStatus Catenary(double rho_m, double rho_m3, double rho_water, double moorlen, double BL,
			  double xanchorvessel, double zanchor, double zvessel, 
			  double &Fhanchorvessel, double &Fvanchor, double &Fvvessel, 
			  double &xonfloor, Vector<double> &x, Vector<double> &z, int num) {
	if (zanchor < 0 && zvessel < 0)
		throw Exc("Catenary. Anchor and vessel z positions have to be positive");
	if (xanchorvessel < 0)
		throw Exc("Catenary. x distance between anchor and vessel has to be positive");
	
	const double g = 9.81;       		

	double lmb = rho_m*(rho_m3 - rho_water)/rho_m3;
	
  	Buffer<double> udata(1, 1);	
	SolveNonLinearEquationsSun(udata, 1, [&](const double y[], double residuals[])->bool {
		double Blimit = y[0];
		residuals[0] =  sqrt(zanchor*(zanchor + 2*Blimit)) + sqrt(zvessel*(zvessel + 2*Blimit)) - moorlen; 
		return true;
	});
	double Blimit = udata[0];

    double xanchorvessel_limit = Blimit*(acosh(zanchor/Blimit + 1) + acosh(zvessel/Blimit + 1));
	
	double delta = xanchorvessel/(num - 1);
	x.SetCount(num);
	z.SetCount(num);
	
	MooringStatus status;
	
	if (xanchorvessel < moorlen - zanchor - zvessel) {				
		status = LOOSE_ON_FLOOR;
		
		if (!IsNull(rho_m)) {
	        Fhanchorvessel = 0;
	        Fvanchor = lmb*g*zanchor;
	        Fvvessel = lmb*g*zvessel;
		} else
			Fhanchorvessel = Fvanchor = Fvvessel = Null;
		
     	xonfloor = xanchorvessel;
     	
		x.SetCount(4);
		z.SetCount(4);
		x[0] = 0;				z[0] = zanchor;
		x[1] = 0;				z[1] = 0;
		x[2] = xanchorvessel;	z[2] = 0;
		x[3] = xanchorvessel;	z[3] = zvessel;
	} else if (xanchorvessel < xanchorvessel_limit) {
        status = CATENARY_ON_FLOOR;
	
	  	Buffer<int> consdata(1, 2);		// B > 0
	  	
	  	Buffer<double> udata(1, Blimit/2.);	
		SolveNonLinearEquationsSun(udata, 1, [&](const double y[], double residuals[])->bool {
			double B = y[0];
			residuals[0] = xanchorvessel - B*acosh(zanchor/B + 1) - B*acosh(zvessel/B + 1) + sqrt(zanchor*(zanchor + 2*B)) + sqrt(zvessel*(zvessel + 2*B)) - moorlen; 
			return true;
		}, consdata);		
		double B = udata[0];

        double xcatanchor = B*acosh(zanchor/B + 1);
        double xcatvessel = B*acosh(zvessel/B + 1);
		xonfloor = xanchorvessel - xcatanchor - xcatvessel;

		if (!IsNull(rho_m)) {
	        Fhanchorvessel = lmb*g*B;        
	        Fvanchor = Fhanchorvessel*sinh(xcatanchor/B); 
	        Fvvessel = Fhanchorvessel*sinh(xcatvessel/B); 
		} else
			Fhanchorvessel = Fvanchor = Fvvessel = Null;
        
        for (int i = 0; i < x.size(); ++i) {
            x[i] = i*delta;
            if (x[i] < xcatanchor)
        		z[i] = B*(cosh((xcatanchor - x[i])/B) - 1);
            else if (x[i] > (xanchorvessel - xcatvessel))
                z[i] = B*(cosh((max(0., x[i] - (xanchorvessel - xcatvessel)))/B) - 1);
            else
                z[i] = 0;
        }
    } else if (xanchorvessel < sqrt(sqr(moorlen) - sqr(zvessel - zanchor))) {
        status = CATENARY;
        
		int neq = 2;
        double deltaz = zvessel - zanchor;

	  	Buffer<double> udata(neq);
	  	Buffer<int> consdata(2);
	  	consdata[0] = 2;			// B > 0
	  	consdata[1] = 0;
	  	
	  	double x1_0, B_0, x1, B;
	  	bool done = false;
	  	for (x1_0 = 0; x1_0 < abs(xanchorvessel) && !done; x1_0 += abs(xanchorvessel)/4) {
	  		for (B_0 = abs(Blimit); B_0 < 10*B_0 && !done; B_0 += 5*abs(Blimit)/4) {		
		  		udata[0] = B_0;	
		  		udata[1] = x1_0;
				SolveNonLinearEquationsSun(udata, neq, [&](const double y[], double residuals[])->bool {
					double B = y[0];
					double x1 = y[1];
					
					residuals[0] = B*(sinh((x1 + xanchorvessel)/B) - sinh(x1/B)) - moorlen;
					residuals[1] = B*(cosh((x1 + xanchorvessel)/B) - cosh(x1/B)) - deltaz;
					
					return true;
				}, consdata);
				B = udata[0];
				x1 = udata[1];		
				if (abs((x1 + xanchorvessel)/B) < 3)
					done = true;
	  		}
	  	}
	  	if (!done) 
			throw Exc("Catenary. Solving problem");

		if (!IsNull(rho_m)) {
	        Fhanchorvessel = lmb*g*abs(B);    
	        Fvanchor = -Fhanchorvessel*sinh(x1/B);  			
	        Fvvessel = Fhanchorvessel*sinh((x1 + xanchorvessel)/B);  	
		} else
			Fhanchorvessel = Fvanchor = Fvvessel = Null;
		
        xonfloor = 0;
        
        for (int i = 0; i < x.size(); ++i) {
            x[i] = i*delta;
        	z[i] = B*(cosh((x[i] + x1)/B) - 1) - B*(cosh(x1/B) - 1) + zanchor;
        }
    } else {
		status = BROKEN;
		if (!IsNull(rho_m)) 
			Fhanchorvessel = Fvanchor = Fvvessel = xonfloor = 0;
		else
			Fhanchorvessel = Fvanchor = Fvvessel = xonfloor = Null;
		
		x.Clear();
		z.Clear();
    }
    
	if (!IsNull(BL))    
    	if (max(sqrt(sqr(Fhanchorvessel) + sqr(Fvanchor)), sqrt(sqr(Fhanchorvessel) + sqr(Fvvessel))) >= BL)
        	status = BL_EXCEDEED;
    
	return status;
}

MooringStatus Catenary(double rho_m, double rho_m3, double rho_water, double moorlen, double BL,
			  double xanchorvessel, double zanchor, double zvessel, 
			  double &Fhanchorvessel, double &Fvanchor, double &Fvvessel, double &xonfloor) {
	Vector<double> x, z;
				      
	return Catenary(rho_m, rho_m3, rho_water, moorlen, BL, xanchorvessel, 
			zanchor, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, xonfloor, x, z, 0);				      
}

MooringStatus Catenary(double moorlen, double xanchorvessel, double zanchor, double zvessel, double &xonfloor,
				Vector<double> &x, Vector<double> &z, int num) {
	double rho_m = Null,  rho_m3 = Null, rho_water = Null, BL = Null, Fhanchorvessel, Fvanchor, Fvvessel;		      

	return Catenary(rho_m, rho_m3, rho_water, moorlen, BL, xanchorvessel, 
			zanchor, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, xonfloor, x, z, num);
}

MooringStatus Catenary(double moorlen, double xanchorvessel, double zanchor, double zvessel, double &xonfloor) {
	double rho_m = Null,  rho_m3 = Null, rho_water = Null, BL = Null, Fhanchorvessel, Fvanchor, Fvvessel;		      
	Vector<double> x, z;
	return Catenary(rho_m, rho_m3, rho_water, moorlen, BL, xanchorvessel, 
			zanchor, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, xonfloor, x, z, 0);
}

MooringStatus CatenaryGetLen(double xonfloor, double xanchorvessel, double zanchor, double zvessel, double &moorlen) {
	if (xonfloor >= xanchorvessel) {
		moorlen = xanchorvessel + 2*(xonfloor - xanchorvessel) + zanchor + zvessel;
		return LOOSE_ON_FLOOR;
	} 
	
	Buffer<int> consdata(2, 2);		// B > 0 && moorlen > 0
  	Buffer<double> udata(2);	
  	udata[0] = 1;
  	udata[1] = sqrt(sqr(xanchorvessel) + sqr(zanchor - zvessel));
	SolveNonLinearEquationsSun(udata, 2, [&](const double y[], double residuals[])->bool {
		double B = y[0];
		double moorlen = y[1];
		residuals[0] = xanchorvessel - B*acosh(zanchor/B + 1) - B*acosh(zvessel/B + 1) + sqrt(zanchor*(zanchor + 2*B)) + sqrt(zvessel*(zvessel + 2*B)) - moorlen; 
    	double xcatanchor = B*acosh(zanchor/B + 1);
    	double xcatvessel = B*acosh(zvessel/B + 1);
		residuals[1] = xanchorvessel - xcatanchor - xcatvessel - xonfloor;
		return true;
	}, consdata);		
	double B = udata[0];
	moorlen = udata[1];
	
	return Catenary(moorlen, xanchorvessel, zanchor, zvessel, xonfloor);
}


const char *MooringStatusStr(MooringStatus status) {
	const char *str[5] = {"loose on floor", "catenary on floor", "catenary", 
						 "line length exceeded", "break load exdeeded"};
	ASSERT(int(status) < 5);
	return str[int(status)];
}

}