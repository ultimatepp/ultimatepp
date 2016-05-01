#include <sstream>

#include "AndroidMathUtility.h"

namespace AndroidMathUtility {

Vector::Vector()
{
	this->size = 0;
	this->data = NULL;
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
		this->size = vec.GetSize();
		this->data = new float[size];
		for(int i = 0; i < size; i++) {
			this->data[i] = vec.data[i];
		}
	}
	else {
		this->size = 0;
		this->data = NULL;
	}
}

Vector::~Vector()
{
	delete[] data;
}

float Vector::Get(int id) const
{
	return this->data[id];
}

int Vector::GetSize() const
{
	return this->size;
}

void Vector::Set(int id, float value)
{
	this->data[id] = value;
}

void Vector::MultipleByScalar(float scalar)
{
	for(int i = 0; i < size; i++) {
		this->data[i] *= scalar;
	}
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
