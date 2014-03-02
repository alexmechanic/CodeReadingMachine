#include <iostream>
#include "MemoryUnit.h"
#include <exception>

using std::cout;
using std::cin;
using std::endl;

int main() {

	//=======================
	//Test drive for MemoryUnit
	//=======================

	MemoryUnit* mu = NULL;
	int buf = 413;
	int foo = 111;

	try {
		cout << extractPageNumber( 16383 )  << " " << extractPageBias( 16383 ) << endl;
		mu = new MemoryUnit();
		mu->write(4, &buf);


		mu->readInt(4, &foo);

		cout << foo << endl;

		mu->loadProgram("program.txt", 8);

		//cout << mu->debug_getLog();
	} catch (std::exception &e) {
		printf("%s\n", e.what());
		delete mu;
		mu = NULL; 
	} catch (...) {
		printf("Unrecognized error on running\n");
		delete mu;
		mu = NULL; 
	}
	

	fflush(stdin);
	getchar();
	return 0; 
}
