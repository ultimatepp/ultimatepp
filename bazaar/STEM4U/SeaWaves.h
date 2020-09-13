#ifndef _STEM4U_SeaWaves_h_
#define _STEM4U_SeaWaves_h_


namespace Upp {
	
class SeaWaves {
public:
	static double g; 	//	Gravity (m/s2)
	static double rho; 	//	Water density (kg/m2) 

	SeaWaves() : Tp(-1) {}
	
	// Calculation results	
	double zSurf; 	// Free surface ay x, y
	double dzSurf;
	double vx; 		// Velocities of a particle at x, y, z (z=0 is free surface)
	double vy;
	double vz; 
	double ax; 		// Accelerations of a particle at x, y, z (z=0 is free surface)
	double ay;
	double az;
	double p;		// Pressure (hydrostatic + dynamic)

	bool Init(double _Tp, double _Hs, double _dirM = 0, double _h = 70, int _nd = 15, int _nf = 600, 
				double gamma = 3.3, double disp_ang = M_PI/3., int seed = 123456, double fmin = -1, double fmax = -1);
	void Calc(double x, double y, double z, double t);	
	double ZSurf(double x, double y, double z, double t);
	double Pressure(double x, double y, double z, double t);
	
	static double Power(double Te, double Hs, double h);

	static double WaveNumber(double T, double h, bool exact = true);
	static double WaveLength(double T, double h);
	static double Celerity(double T, double h);		
	static double GroupCelerity(double T, double h);
	
	static double JONSWAP_Spectrum(double Hm0, double Tp, double gamma, double freq);
	bool JONSWAP_Spectrum_test(double Hm0, double Tp, double gamma);
		
	enum SEA_TYPE {SHALLOW, INTERMEDIATE, DEEP};
	static SEA_TYPE GetSeaType(double Tp, double h);
	
private:
	double Tp; 		// Peak period (s)
	double dirM;
	double Hs; 		// Significant wave height (m)
	double h;  		// Sea depth (m)

	double df;

	// Spectral characteristics
	int nd; 	// Directional components number
	int nf; 	// Frequential components number	

	Eigen::VectorXd frecs;
	Eigen::VectorXd k_f; 
	Eigen::VectorXd dirs;
	Eigen::MatrixXd amp_fd;
	Eigen::MatrixXd fi_fd;
};


double Te_fTp(double Tp, double gamma);
double Tp_fTe(double Te, double gamma);
double gamma_fTp_Te(double Tp, double Te);
double Tp_fTm(double Tm, double gamma); 
double Tp_fTz(double Tz, double gamma);
double gamma_fTp_Tz(double Tp, double Tz);
	
}

#endif
