#include "structures.h"

struct Instruction {
}

struct BasicBlock {
	uint32_t pc;
	uint32_t N;
	Instruction inst[8];
}

enum InstructionType {
	r,
	i,
	j
};