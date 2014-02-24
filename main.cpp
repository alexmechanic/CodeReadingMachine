#include <iostream>
#include "MemoryUnit.h"
#include <exception>

using std::cout;
using std::cin;
using std::endl;

int main() {

	//=======================
	//Тест-драйв для MemoryUnit
	//=======================

	MemoryUnit* mu = NULL;
	int buf = 413;
	int foo = 111;

	try {
		cout << extractPageNumber( reinterpret_cast<byte*>(4095) )  << " " << extractPageBias( reinterpret_cast<byte*>(4095) ) << endl;
		mu = new MemoryUnit();
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
