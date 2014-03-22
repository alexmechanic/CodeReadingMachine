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

	LogKeeper *logKeeper = new LogKeeper();
	MemoryUnit* mu = NULL;
	int buf = 413;
	int bar = 612;
	int bas = 1025;
	int foo = 111;

	try {
		// 1
		mu = new MemoryUnit(logKeeper);
		mu->write(4, &buf);
		mu->readInt(4, &foo);
		cout << "---> " << foo << endl; //ok
		mu->write(15000, &bar); //this operation pops 0vpn entry out of TLB and causes error later
		mu->readInt(15000, &foo);
		cout << "---> " << foo << endl; //ok
		mu->write(10000, &bas); //second page added to previous entry, so we have no same error here
		mu->readInt(10000, &foo); 
		cout << "---> " << foo << endl; //still ok 
		mu->readInt(4, &foo); // !!! since we lost information where is 0'th page is located it reads junk instead of 413 !!!
		cout << "---> " << foo << endl;

		// 2
		//mu->loadProgram("program.txt", 8);

		//3
		/*
		for(int i = 1000; i < config::SIZE_OF_RAM_MEMORY + 4000; i += 2000)
			mu->write(i, &i);

		mu->readInt(1000, &foo);
		cout << foo << endl;
		mu->readInt(19000, &foo);
		cout << foo << endl;*/

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
	delete logKeeper;
	fflush(stdin);
	getchar();
	return 0; 
}
