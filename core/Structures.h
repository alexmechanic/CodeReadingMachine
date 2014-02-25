#ifndef STRUCTURES
#define STRUCTURES

#include <cstdint>

#define BASICBLOCKSIZE 8

union Instruction {
	uint32_t code;
	struct {
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned rd : 5;
		unsigned shamt : 5;
		unsigned function : 6;
	};
	struct {
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned immediate : 16;
	};
	struct {
		unsigned opcode : 6;
		unsigned address : 26;
	};
};

enum InstructionFormat {R, I, J};

enum Registers	{zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7,
				t8, t9, s0, s1, s2, s3, s4, s5, s6, s7, s8, k0, k1, gp, sp, ra};

struct DecodedInstruction {
	InstructionFormat format;
	union {
		struct {
			unsigned internalOpcode : 16;
			unsigned additionalData : 16;
			unsigned rs : 8;
			unsigned rt : 8;
			unsigned rd : 8;
			unsigned shamt : 8;
		};
		struct {
			unsigned internalOpcode : 16;
			unsigned additionalData : 16;
			unsigned rs : 8;
			unsigned rt : 8;
			unsigned immediate : 16;
		};
		struct {
			unsigned internalOpcode : 16;
			unsigned additionalData : 16;
			unsigned address : 32;
		};
	};
};

struct BasicBlock {
	uint32_t pc;
	uint32_t N;
	DecodedInstruction inst[BASICBLOCKSIZE];
};

#endif