gcc -O3 -shared -Wall -Wl,--export-all-symbols -mpreferred-stack-boundary=2 -march=i386 -falign-functions=0 -fno-strict-aliasing -DTCC_TARGET_PE -DLIBTCC -o libtcc.dll tcc.c
pause