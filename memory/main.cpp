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
		// 1
		mu = new MemoryUnit();
		mu->write(4, &buf);
		mu->readInt(4, &foo);
		cout << foo << endl;

		// 2
		//mu->loadProgram("program.txt", 8);

		//3
		for(int i = 1000; i < config::SIZE_OF_RAM_MEMORY + 4000; i += 2000)
			mu->write(i, &i);

		mu->readInt(1000, &foo);
		cout << foo << endl;
		mu->readInt(19000, &foo);
		cout << foo << endl;

	} catch (std::exception &e) {
		printf("%s\n", e.what());
		delete mu;
		mu = NULL; 
	} catch (...) {
		printf("Unrecognized error at running\n");
		delete mu;
		mu = NULL; 
	}
	
	delete mu;
	fflush(stdin);
	getchar();
	return 0; 
}
