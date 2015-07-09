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
	
	float Get(int id) const;
	int GetSize() const;
	
	void Set(int id, float value);
	
	void MultipleByScalar(float scalar);
	
	std::string ToString() const;
	
private:
	float* data;
	int size;
};

}

#endif
