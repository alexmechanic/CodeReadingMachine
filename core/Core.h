#ifndef CORE
#define CORE

#include "Structures.h"
#include "Instructions.h"
#include "MemoryUnit.h"
#include <cstdint>

#define CACHESIZE 1
#define NUMBER_OF_INSTRUCTIONS 4096 // зависит от максимального размера internalOpcode

class Core {
public:
	Core();
	uint32_t readRegister(Registers);
	int writeRegister(Registers, uint32_t);
	void setDebugOptions(uint32_t);
	void advance_pc (word offset);
	int execute();
	int setPC(uint32_t);
	MemoryUnit *m_memory;
	uint32_t m_pc;
	uint32_t m_nPC;
private:
	int (*instructionsArray[NUMBER_OF_INSTRUCTIONS])(Core &, DecodedInstruction);	// проинициализировать массив
	int fillBasicBlock();
	char isBranch(DecodedInstruction);
	char findInBasicBlockCache(uint32_t);
	DecodedInstruction decode(uint32_t);
	int call(DecodedInstruction);
	uint32_t m_instructionsLeft;
	uint32_t calculateBasicBlockPosition();
	uint32_t m_registers[32 + 2];
	uint32_t m_debugOptions;
	uint32_t m_currentInstr;
	DecodedInstruction m_decodedInstr;
	uint32_t m_nextPC;
	uint32_t m_currentBasicBlock;
	BasicBlock m_basicBlockCache[CACHESIZE];
};

#endif