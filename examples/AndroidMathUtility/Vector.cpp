#include <sstream>

#include "AndroidMathUtility.h"

namespace AndroidMathUtility {

Vector::Vector()
{
	this->size = 0;
}

Vector::Vector(int size)
{
	data = new float[size];
	for(int i = 0; i < size; i++) {
		data[i] = 0.0f;
	}
	this->size = size;
}

Vector::Vector(const Vector& vec)
{
	if(vec.GetSize() > 0) {
		size = vec.GetSize();
		data = new float[size];
		for(int i = 0; i < size; i++) {
			data[i] = vec.data[i];
		}
		
	}
}

Vector::~Vector()
{
	delete[] data;
}

std::string Vector::ToString() const
{
	std::stringstream ss;
	ss << "[";
	for(int i = 0; i < size; i++) {
		ss << data[i];
		if(i + 1 < size)
			ss << ", ";
	}
	ss << "]";
	return ss.str();
}

}
