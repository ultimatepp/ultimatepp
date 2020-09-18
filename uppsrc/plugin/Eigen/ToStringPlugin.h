	
UPP::String ToString() const {
	std::stringstream s;
	s << *this;
	return s.str();
}

