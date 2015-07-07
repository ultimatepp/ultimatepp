#ifndef _AndroidMathUtility_Vector_h_
#define _AndroidMathUtility_Vector_h_

#include <string>

namespace AndroidMathUtility {

class Vector {
public:
	Vector();
	Vector(int size);
	Vector(const Vector& vec);
	virtual ~Vector();
	
	int GetSize() const { return this->size; }
	
	std::string ToString() const;
	
private:
	float* data;
	int size;
};

}

#endif
