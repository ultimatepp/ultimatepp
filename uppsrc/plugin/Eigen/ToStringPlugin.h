#ifndef _Eigen_UppMatrixPlugin_h_
#define _Eigen_UppMatrixPlugin_h_

UPP::String ToString() const{
	std::stringstream s;
	s << *this;
	return s.str();
}

#endif
