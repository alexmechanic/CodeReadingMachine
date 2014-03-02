#ifndef CORE
#define CORE

#include "Structures.h"
#include "Instructions.h"
#include "MemoryUnit.h"
#include <cstdint>

#define CACHESIZE 1

class Core {
public:
	Core();
	uint32_t readRegister(Registers);
	int writeRegister(Registers, uint32_t);
	void setDebugOptions(uint32_t);
	void setNextAddress(uint32_t);
	int execute();
private:
	DecodedInstruction decode(uint32_t);
	int call(DecodedInstruction);
	uint32_t calculateBasicBlockPosition();
	uint32_t m_pc;
	uint32_t m_registers[32];
	uint32_t m_debugOptions;
	uint32_t m_currentInstr;
	DecodedInstruction m_decodedInstr;
	uint32_t m_basicBlockIterator;
	uint32_t m_currentBasicBlock;
	BasicBlock m_basicBlockCache[CACHESIZE];
	MemoryUnit *m_memory;
};

#endif