#ifndef OPERATIONS
#define OPERATIONS
#include "Structures.h"
#include "Core.h"
//ARIPHMETICAL OPERATIONS
//==================================================
#define addi add //потому что как реализация по смыслу одна и та же из за константных ссылок в аргументах
int ADD(Core &, DecodedInstruction);
int ADDU(Core &, DecodedInstruction);
int ADDUI(Core &, DecodedInstruction);

int SUB(Core &, DecodedInstruction);
int SUBU(Core &, DecodedInstruction);

int DIV(Core &, DecodedInstruction);
int DIVU(Core &, DecodedInstruction);

int MULT(Core &, DecodedInstruction);
int MULTU(Core &, DecodedInstruction);
//==================================================

//LOGICAL OPERATIONS
//==================================================
int AND(Core &, DecodedInstruction);
int ANDI(Core &, DecodedInstruction);

int OR(Core &, DecodedInstruction);
int ORI(Core &, DecodedInstruction);
int XOR(Core &, DecodedInstruction);
int XORI(Core &, DecodedInstruction);
int NOR(Core &, DecodedInstruction);

//SHIFT OPERATIONS
//==================================================
int SLL(Core &, DecodedInstruction); // Shift left logical
int SRL(Core &, DecodedInstruction); // Shift right logical

int SLLV(Core &, DecodedInstruction); // Shift left logical variable
int SRLV(Core &, DecodedInstruction); // Shift right logical variable

int SRA(Core &, DecodedInstruction); // Shift right arithmetic
//==================================================

//SET OPERATIONS
//==================================================
int SLT(Core &h, DecodedInstruction); // Set on less than (signed)
int SLTI(Core &, DecodedInstruction); // Set on less than immediate (signed)

int SLTU(Core &, DecodedInstruction); // Set on less than (unsigned)
int SLTUI(Core &, DecodedInstruction); // Set on less than immediate (unsigned)
//==================================================

//MOVE OPERATIONS
//==================================================
int MFLO(Core &, DecodedInstruction);
int MFHI(Core &, DecodedInstruction);
//==================================================

//LUI OPERATION
//==================================================
int LUI(Core &, DecodedInstruction); //  Load upper immediate
//==================================================

//MEMORY OPERATIONS
//==================================================
int SB(Core &, DecodedInstruction); // Store byte (NOT FINISHED YET) 
															// MEM[$s + offset] = (0xff & $t); advance_pc (4); 
															// sb $t, offset($s)
int SW(Core &, DecodedInstruction); // Store word (NOT FINISHED YET) 
															// MEM[$s + offset] = $t; advance_pc (4); 
															// sw $t, offset($s)

int LB(Core &, DecodedInstruction); // Load byte (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lb $t, offset($s)
int LW(Core &, DecodedInstruction); // Load word (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lw $t, offset($s)
//==================================================

//BRANCH OPERATIONS
//==================================================
int BEQ (Core &, DecodedInstruction); // Branch on equal (DOESNOT EXIST YET) 
													   // if $s == $t advance_pc (offset << 2)); else advance_pc (4); 
													   // beq $s, $t, offset
int BNE (Core &, DecodedInstruction); // Branch on not equal (DOESNOT EXIST YET) 
													   // if $s != $t advance_pc (offset << 2)); else advance_pc (4); 
													   // bne $s, $t, offset

int BGEZ (Core &, DecodedInstruction); // Branch on greater than or equal to zero (DOESNOT EXIST YET) 
								     // if $s >= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // bgez $s, offset
int BGEZAL (Core &, DecodedInstruction); // Branch on greater than or equal to zero and link (DOESNOT EXIST YET) 
								   	   // if $s >= 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
								 	   // bgezal $s, offset
int BGTZ (Core &, DecodedInstruction); // Branch on greater than zero (DOESNOT EXIST YET) 
									 // if $s > 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bgtz $s, offset
									
int BLEZ (Core &, DecodedInstruction); // Branch on less than or equal to zero (DOESNOT EXIST YET) 
								     // if $s <= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // blez $s, offset
int BLTZAL (Core &, DecodedInstruction); // Branch on less than zero and link (DOESNOT EXIST YET) 
									   // if $s < 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
									   // bltzal $s, offset
int BLTZ (Core &, DecodedInstruction); // Branch on less than zero (DOESNOT EXIST YET) 
									 // if $s < 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bltz $s, offset
//==================================================

//JUMP OPERATIONS
//==================================================
int J_(Core (&o_core), DecodedInstruction m_instruction); // Jump
				  // PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
				  // j target
int JAL (Core (&o_core), DecodedInstruction m_instruction); // Jump and link
					 // $31 = PC + 8 (or nPC + 4); PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
					 // jal target
int JR (Core (&o_core), DecodedInstruction m_instruction); // Jump register
					// PC = nPC; nPC = $s;
					// jr $s				 
//==================================================

//NOOP OPERATION
//==================================================
int NOOP(); // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
//==================================================

//SYSCALL OPERATION
//==================================================
int SYSCALL();
//==================================================
#endif