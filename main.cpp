#include <iostream>
#include "MemoryUnit.h"
#include <exception>

using std::cout;
using std::cin;
using std::endl;

int main() {

	//=======================
	//����-����� ��� MemoryUnit
	//=======================

	MemoryUnit* mu = NULL;
	int buf = 413;
	int foo = 111;

	try {
		mu = new MemoryUnit(9096);
		mu->write(mu->debug_getMemory(), buf);
		mu->readInt(mu->debug_getMemory(), foo);
		cout << foo << endl;

		mu->loadProgram("program.txt");

		cout << mu->debug_getLog();
	} catch (std::exception &e) {
		printf("%s\n", e.what());
		delete mu;
		mu = NULL;
	}
	

	fflush(stdin);
	getchar();
	return 0; 
}
