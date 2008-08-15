#include <ctime>
#include <cstdio>
#include <iostream>
#include <fstream>

#include <Core/Core.h>

const int major_iterations = 20;
const int iterations = 50000;

void foutC()
{
	FILE *f = fopen("test_c.txt", "w");
	for(int i = 0; i < iterations; ++i )
	    fputc('a', f);
	fclose(f);
}

void foutCPP ()
{
	std::ofstream f("test_cpp.txt", std::ios_base::out);
	for(int i = 0; i < iterations; ++i )
		f.put('a');
}

void foutUPP()
{
	Upp::FileOut f("test_upp.txt");
	for(int i = 0; i < iterations; ++i )
		f.Put('a');
}

void ffoutC()
{
	FILE *f = fopen("test_c2.txt", "w");
	for(int i = 0; i < iterations; ++i)
		fprintf(f, "%d\n", i);
	fclose(f);
}

void ffoutCPP ()
{
	std::ofstream f("test_cpp2.txt", std::ios_base::out);
	for(int i = 0; i < iterations; ++i )
		f << i << '\n';
}

void ffoutUPP()
{
	Upp::FileOut f("test_upp.txt");
	for(int i = 0; i < iterations; ++i )
		f << i << '\n';
}

void ffoutUPP2()
{
	Upp::FileOut f("test_upp.txt");
	for(int i = 0; i < iterations; ++i )
		f << Upp::Format("%d\n", i);
}

void finC()
{
	FILE *f = fopen("test_c.txt", "r");
	while(fgetc(f) == 'a');
	fclose(f);
}

void finCPP ()
{
	std::ifstream f("test_cpp.txt");
	while(f.get() == 'a');
}

void finUPP()
{
	Upp::FileIn f("test_upp.txt");
	while(f.Get() == 'a');
}

int Bench(void (*main)())
{
	Upp::TimeStop tm;
	for(int i = 0; i < major_iterations; i++)
		(*main)();
	return tm.Elapsed();
}

int main()
{
	int ctime = Bench(foutC);
	int cpptime = Bench(foutCPP);
	int upptime = Bench(foutUPP);

	std::cout << "Output:\n";
	std::cout << "C: " << ctime << " C++: " << cpptime << " U++: " << upptime << "\n";
	std::cout << "C++/C " << static_cast<double>( cpptime ) / ctime << "\n";
	std::cout << "U++/C " << static_cast<double>( upptime ) / ctime << "\n";

	ctime = Bench(ffoutC);
	cpptime = Bench(ffoutCPP);
	upptime = Bench(ffoutUPP);
	int upp2time = Bench(ffoutUPP2);

	std::cout << "Formatted Output:\n";
	std::cout << "C: " << ctime << " C++: " << cpptime << " U++: " << upptime << " U++ Format: " << upp2time << "\n";
	std::cout << "C++/C " << static_cast<double>( cpptime ) / ctime << "\n";
	std::cout << "U++/C " << static_cast<double>( upptime ) / ctime << "\n";
	std::cout << "U++Format/C " << static_cast<double>( upp2time ) / ctime << "\n";

	ctime = Bench(finC);
	cpptime = Bench(finCPP);
	upptime = Bench(finUPP);

	std::cout << "\nInput:\n";
	std::cout << "C: " << ctime << " C++: " << cpptime << " U++: " << upptime << "\n";
	std::cout << "C++/C " << static_cast<double>( cpptime ) / ctime << "\n";
	std::cout << "U++/C " << static_cast<double>( upptime ) / ctime << "\n";
}
