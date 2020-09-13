#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include "SeaWaves.h"


namespace Upp {

using namespace Eigen;

double SeaWaves::rho = 1024;
double SeaWaves::g = 9.81;

double SeaWaves::WaveNumber(double T, double h, bool exact) {
	double y = 4*M_PI*M_PI*h/g/(T*T);
	double k1 = 1 + 0.6666666666*y + 0.3555555555*pow(y,2) + 0.1608465608*pow(y,3) 
				+ 0.0632098765*pow(y,4) + 0.0217540484*pow(y,5) + 0.0065407983*pow(y,6);
	double k = sqrt((y*y + y/k1))/h;
	
	if (!exact)
		return k;
	
	double w = 2*M_PI/T;
	
	VectorXd x(1);
	x[0] = k;
	if (SolveNonLinearEquations(x, [&](const VectorXd &x, VectorXd &residual)->int {
		residual[0] = w*w - g*k*tanh(x[0]*h);
		return 0;
	}))
		return x[0];

	return -1;
}

SeaWaves::SEA_TYPE SeaWaves::GetSeaType(double T, double h) {
    double h_L = h/WaveLength(T, h);
	if (h_L < 1./20)
		return SHALLOW;
	else if (h_L < 1./2)
		return INTERMEDIATE;
	return DEEP;
}

double SeaWaves::WaveLength(double T, double h) 	{return 2*M_PI/WaveNumber(T, h);}

double SeaWaves::Celerity(double T, double h) 	  	{return WaveLength(T, h)/T;}

double SeaWaves::GroupCelerity(double T, double h) {
    double k = WaveNumber(T, h);
    double L = 2*M_PI/k;			
    double c = L/T;               	
    double n = 0.5*(1 + 2*k*h/sinh(2*k*h));   
    return c*n;                
}

double SeaWaves::Power(double Te, double Hs, double h) {
	double k = WaveNumber(Te, h);
    double L = 2*M_PI/k;			
    double c = L/Te;               	
    double n = 0.5*(1 + 2*k*h/sinh(2*k*h));   
    double Cg = c*n;                
    double E = 1/16.*rho*g*Hs*Hs;	
	return E*Cg/1000.;       	
}

double SeaWaves::JONSWAP_Spectrum(double Hm0, double Tp, double gamma, double freq) {
	double sigma_f = freq <= 1/Tp ? 0.07 : 0.09;	     
	double beta = 0.0624/(0.230 + 0.0336*gamma - 0.185/(1.9 + gamma))*(1.094 - 0.01915*log(gamma));
		
	return beta*Hm0*Hm0*pow(Tp,-4)*pow(freq, -5)*exp(-5./4.*pow(Tp*freq, -4)) * pow(gamma, exp(-pow((Tp*freq - 1), 2)/(2*sigma_f*sigma_f)));	
}

bool SeaWaves::JONSWAP_Spectrum_test(double Hm0, double Tp, double gamma) {
	if ((Tp > 5*sqrt(Hm0)) || (Tp < 3.6*sqrt(Hm0)))
		return false;
	if (gamma > 7 || gamma < 1) 
		return false;
	return true;
}

bool SeaWaves::Init(double Tp, double Hs, double dirM, double h, int nd, int nf, double gamma, 
					double disp_ang, int seed, double fmin, double fmax) {
	if(!(Tp > 0 && nd > 0 && nf > 0 && gamma >= 1))
		return false;
	
	this->Tp = Tp; 				
	this->dirM = dirM; 			
	this->Hs = Hs;             	
	this->h = h;                
	this->nd = nd; 				
	this->nf = nf = nf/nd;	

	frecs.resize(nf);
	k_f.resize(nf);
	dirs.resize(nd);
	amp_fd.resize(nd, nf);
	fi_fd.resize(nd, nf);

	double fp = 1/Tp;      
	if (nf > 1) {
		if (fmin < 0)
			fmin = 0.25/Tp;
		else if (fmin > fp)
			return false;
		if (fmax < 0)
			fmax = 4/Tp;   
		else if (fmax < fp)
			return false;
	} else
		fmin = fmax = fp;
	
	df = 1;
	if (nf > 1)
		df = (fmax-fmin)/(nf - 1);
	
	for(int f = 0; f < nf; f++) {
		int n = f + 1;
	    frecs[f] = fmin + (n - 1)*df;
	    k_f[f] = WaveNumber(1/frecs[f], h); 
	}
        
	Buffer<double> Sf_f(nf);
	
	if (nf > 1) {
		double beta = 0.0624/(0.230 + 0.0336*gamma - 0.185*(pow(1.9 + gamma, -1)))*(1.094 - 0.01915*log(gamma));
		for(int f = 0; f < nf; f++) {
			double sigma_f;
		    if(frecs[f] <= fp)
		        sigma_f = 0.07;
		    else
		        sigma_f = 0.09;	     
	
			Sf_f[f] = beta*pow(Hs, 2)*pow(Tp,-4)*pow(frecs[f], -5)*exp(-1.25*pow(Tp*frecs[f], -4)) 
						  *pow(gamma, exp(-pow((Tp*frecs[f]-1), 2)/2/pow(sigma_f, 2)));
		}
	} else 
		Sf_f[0] = 1/2.*pow(Hs/2., 2);
	
	if (nd > 1) {
	    double dirmin = dirM - disp_ang; 	
	    double dirmax = dirM + disp_ang; 	
	    double dd = (dirmax - dirmin)/(nd-1);
	    
	    for (int d = 0; d < nd; ++d)
	        dirs[d] = dirmin + d*dd;    

	    double L0 = g*Tp*Tp/2./M_PI; 
	    double per = Hs/L0;  								
	    double Smax = pow(10, -1.2195*log10(per) - 0.5573); 
	    
	    Buffer<double> incdir_d(nd);
	
	    for (int d = 0; d < nd; ++d)
	        incdir_d[d] = dirs[d] - dirM;
				
		for (int f = 0; f < nf; f++) { 
		    double s_f;				
		    if (frecs[f] <= fp)
		        s_f = pow((frecs[f]/fp), 5)*Smax;
		    else
		        s_f = pow((frecs[f]/fp), -2.5)*Smax;   
		    
		    double G0_f = 0;  
		    for (int d = 0; d < nd; d++) 
		        G0_f += pow(cos(incdir_d[d]/2), 2*s_f)*dd;
		    G0_f = 1/G0_f;
	
	      	for (int d = 0; d < nd; d++) {
		        double G_fd = G0_f*pow(cos(incdir_d[d]/2.), 2*s_f);
		        double Sfd_fd = Sf_f[f]*G_fd;
		        amp_fd(d, f) = sqrt(2*Sfd_fd*df*dd);
		    }
		}
	} else {
	    dirs[0] = dirM;
	    for (int f = 0; f < nf; ++f)
	        amp_fd(0, f) = sqrt(2*Sf_f[f]*df);
	}
		
	if (nf > 1) {
		if (IsNull(seed)) {
			std::random_device rd;
			seed = rd();
		} 
		std::mt19937 random_engine(seed);
		std::uniform_real_distribution<double> random_distribution(-M_PI, M_PI);
		for (int f = 0; f < nf; f++) 
		    for (int d = 0; d < nd; d++) 
				fi_fd(d, f) = random_distribution(random_engine);		
	} else {
		for (int d = 0; d < nd; d++) 
			fi_fd(d, 0) = 0;
	}
	
	return true;
}

void SeaWaves::Calc(double x, double y, double z, double t) {	
	ASSERT(frecs.size() > 0);
	
    zSurf = dzSurf = vx = vy = vz = ax = ay = az = 0;
	
	p = -rho*g*z;    
	
	for (int f = 0; f < nf; f++) {
    	for (int d = 0; d < nd; d++) {
    		double frec = frecs[f];
    		
	        double AUXsl = amp_fd(d, f)*cos(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));         
            zSurf += AUXsl;
            dzSurf += 2*M_PI*frec*amp_fd(d, f)*sin(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));         
	        
	        double KP     = cosh(k_f[f]*(h+z))/cosh(k_f[f]*h);
	        double KP_sin = sinh(k_f[f]*(h+z))/cosh(k_f[f]*h);
	
	        double AUXvh = g*amp_fd(d, f)*k_f[f]/(2*M_PI*frec)*KP*cos(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));         
	        vx += AUXvh*cos(dirs[d]);
	        vy += AUXvh*sin(dirs[d]);
	        
	        vz += g*amp_fd(d, f)*k_f[f]/(2*M_PI*frec)*KP_sin*sin(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));
	        
	        double AUXah = g*amp_fd(d, f)*pow((k_f[f]),2)/(2*M_PI*frec)*KP*sin(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));
	        ax += AUXah*cos(dirs[d]);
	        ay += AUXah*sin(dirs[d]);
	    
	        az += -g*amp_fd(d, f)*pow((k_f[f]),2)/(2*M_PI*frec)*KP_sin*cos(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frec*t + fi_fd(d, f));         
	        
	   		p += rho*g*KP*AUXsl;	
    	}
	}
}

double SeaWaves::ZSurf(double x, double y, double z, double t) {
	ASSERT(frecs.size() > 0);
	
    double zSurf = 0;
	for (int f = 0; f < nf; f++) 
    	for (int d = 0; d < nd; d++) 
	        zSurf += amp_fd(d, f)*cos(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y 
	        						  - 2*M_PI*frecs[f]*t + fi_fd(d, f));  
    return zSurf;
}

double SeaWaves::Pressure(double x, double y, double z, double t) {
	ASSERT(frecs.size() > 0);
	
	double p = -rho*g*z;  
	for (int f = 0; f < nf; f++) 
    	for (int d = 0 ; d < nd; d++) {
	        double KP    = cosh(k_f[f]*(h+z))/cosh(k_f[f]*h);	
    		double AUXsl = amp_fd(d, f)*cos(k_f[f]*cos(dirs[d])*x + k_f[f]*sin(dirs[d])*y - 2*M_PI*frecs[f]*t + fi_fd(d, f));         
    		p += rho*g*KP*AUXsl;	
    	}
    return p;
}


// DNV-RP-C205, page 34 
double Te_fTp(double Tp, double gamma) 	  {return Tp*(4.2 + gamma)/(5 + gamma);}
double Tp_fTe(double Te, double gamma) 	  {return Te*(5 + gamma)/(4.2 + gamma);}
double gamma_fTp_Te(double Tp, double Te) {return (Te*5 - Tp*4.2)/(Tp - Te);}
double Tp_fTm(double Tm, double gamma) 	  {return Tm/(0.7303 + 0.04936*gamma - 0.006556*pow(gamma, 2) + 0.0003610*pow(gamma, 3));}
double Tp_fTz(double Tz, double gamma) 	  {return Tz/(0.6673 + 0.05037*gamma - 0.006230*pow(gamma, 2) + 0.0003341*pow(gamma, 3));}
	
double gamma_fTp_Tz(double Tp, double Tz) {
	VectorXd x(1);
	x[0] = 5;
	if (SolveNonLinearEquations(x, [&](const VectorXd &x, VectorXd &residual)->int {
		double gamma = x[0];
		double gamma2 = gamma*gamma;
		residual[0] = - Tz/Tp + 0.6673 + 0.05037*gamma - 0.006230*gamma2 + 0.0003341*gamma*gamma2;
		return 0;
	}))
		return x[0];
	return -1;
}
	
}	