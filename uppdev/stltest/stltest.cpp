#include "stdio.h"
#include <vector>
#include <iostream>

int main(int argc, const char *argv[])
{
	std::vector<int> x;
	x.push_back(1);
	for(int i = 1; i < 20; i++)
		x.push_back(x[0]);
	for(int i = 0; i < (int)x.size(); i++)
		std::cout << x[i] << '\n';
	return 0;
}

