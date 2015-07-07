#include "AndroidMathUtility.h"

namespace AndroidMathUtility {

int Power(int number, int n) {
	int result = number;
	for(int i = 0; i < n - 1; i++) {
		result *= number;
	}
	return result;
}

}
