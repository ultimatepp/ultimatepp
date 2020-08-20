#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include "Permutations.h"

namespace Upp {

using namespace Eigen;

Buffer<Buffer<int>> PermutationsWithRepetition(int nVals, int nOptionsVal) {
	Buffer<Buffer<int>> list; 

	int num = int(pow(nOptionsVal, nVals));
	list.Alloc(nVals);
	for (int ip = 0; ip < nVals; ++ip) {
		list[ip].Alloc(num);
		int nrep = int(pow(nOptionsVal, ip));
		int val = 0, irep = 0;
		for (int i = 0; i < num; ++i) {
			list[ip][i] = val;
			irep++;
			if (irep >= nrep) {
				irep = 0;
				val++;
				if (val >= nOptionsVal)
					val = 0;
			}
		}
	}
	return list;
}

}