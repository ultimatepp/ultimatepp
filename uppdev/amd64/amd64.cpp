#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	for(int i = 0; i < 100; i++) {
		printf("0x%llX\n", (long long)malloc(1000));
		printf("0x%llX\n", (long long)malloc(100000000));
	}
}
