#ifndef CORE
#define CORE

#include "instructions.h"
#include "memory.h"
#include <cstdint>

class Core {
public:
	void Core::setdebugoptions(uint32_t);
	int Core::execute();
};

#endif