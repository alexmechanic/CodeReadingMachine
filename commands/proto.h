#ifndef OPERATIONS
#define OPERATIONS
//ARIPHMETICAL OPERATIONS
//==================================================
#define addi add //потому что как реализация по смыслу одна и та же из за константных ссылок в аргументах
int ADD(const Core (&), DecodedInstruction);
int ADDU(const Core (&), DecodedInstruction);
int ADDUI(const Core (&), DecodedInstruction);

int SUB(const Core (&), DecodedInstruction);
int SUBU(const Core (&), DecodedInstruction);

int DIV(const Core (&), DecodedInstruction);
int DIVU(const Core (&), DecodedInstruction);

int MULT(const Core (&), DecodedInstruction);
int MULTU(const Core (&), DecodedInstruction);
//==================================================

//LOGICAL OPERATIONS
//==================================================
int AND(const Core (&), DecodedInstruction);
int ANDI(const Core (&), DecodedInstruction);

int OR(const Core (&), DecodedInstruction);
int ORI(const Core (&), DecodedInstruction);
int XOR(const Core (&), DecodedInstruction);
int XORI(const Core (&), DecodedInstruction);
int NOR(const Core (&), DecodedInstruction);

//SHIFT OPERATIONS
//==================================================
int SLL(const Core (&), DecodedInstruction); // Shift left logical
int SRL(const Core (&), DecodedInstruction); // Shift right logical

int SLLV(const Core (&), DecodedInstruction); // Shift left logical variable
int SRLV(const Core (&), DecodedInstruction); // Shift right logical variable

int SRA(const Core (&), DecodedInstruction); // Shift right arithmetic
//==================================================

//SET OPERATIONS
//==================================================
int SLT(const Core (&), DecodedInstruction); // Set on less than (signed)
int SLTI(const Core (&), DecodedInstruction); // Set on less than immediate (signed)

int SLTU(const Core (&), DecodedInstruction); // Set on less than (unsigned)
int SLTUI(const Core (&), DecodedInstruction); // Set on less than immediate (unsigned)
//==================================================

//MOVE OPERATIONS
//==================================================
int MFLO(const Core (&), DecodedInstruction);
int MFHI(const Core (&), DecodedInstruction);
//==================================================

//LUI OPERATION
//==================================================
int LUI(const Core (&), DecodedInstruction, const uint32_t); //  Load upper immediate
//==================================================

//MEMORY OPERATIONS
//==================================================
int SB(const Core (&), DecodedInstruction); // Store byte (NOT FINISHED YET) 
															// MEM[$s + offset] = (0xff & $t); advance_pc (4); 
															// sb $t, offset($s)
int SW(const Core (&), DecodedInstruction); // Store word (NOT FINISHED YET) 
															// MEM[$s + offset] = $t; advance_pc (4); 
															// sw $t, offset($s)

int LB(const Core (&), DecodedInstruction); // Load byte (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lb $t, offset($s)
int LW(const Core (&), DecodedInstruction); // Load word (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lw $t, offset($s)
//==================================================

//BRANCH OPERATIONS
//==================================================
int BEQ (const Core (&), DecodedInstruction); // Branch on equal (DOESNOT EXIST YET) 
													   // if $s == $t advance_pc (offset << 2)); else advance_pc (4); 
													   // beq $s, $t, offset
int BNE (const Core (&), DecodedInstruction); // Branch on not equal (DOESNOT EXIST YET) 
													   // if $s != $t advance_pc (offset << 2)); else advance_pc (4); 
													   // bne $s, $t, offset

int BGEZ (const Core (&), DecodedInstruction); // Branch on greater than or equal to zero (DOESNOT EXIST YET) 
								     // if $s >= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // bgez $s, offset
int BGEZAL (const Core (&), DecodedInstruction); // Branch on greater than or equal to zero and link (DOESNOT EXIST YET) 
								   	   // if $s >= 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
								 	   // bgezal $s, offset
int BGTZ (const Core (&), DecodedInstruction); // Branch on greater than zero (DOESNOT EXIST YET) 
									 // if $s > 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bgtz $s, offset
									
int BLEZ (const Core (&), DecodedInstruction); // Branch on less than or equal to zero (DOESNOT EXIST YET) 
								     // if $s <= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // blez $s, offset
int BLTZAL (const Core (&), DecodedInstruction); // Branch on less than zero and link (DOESNOT EXIST YET) 
									   // if $s < 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
									   // bltzal $s, offset
int BLTZ (const Core (&), DecodedInstruction); // Branch on less than zero (DOESNOT EXIST YET) 
									 // if $s < 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bltz $s, offset
//==================================================

//JUMP OPERATIONS
//==================================================
int J_(const Core (&o_core), DecodedInstruction m_instruction); // Jump
				  // PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
				  // j target
int JAL (const Core (&o_core), DecodedInstruction m_instruction); // Jump and link
					 // $31 = PC + 8 (or nPC + 4); PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
					 // jal target
int JR (const Core (&o_core), DecodedInstruction m_instruction); // Jump register
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