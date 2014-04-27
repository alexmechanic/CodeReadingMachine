#ifndef OPERATIONS
#define OPERATIONS
//ARIPHMETICAL OPERATIONS
//==================================================
#define addi add //потому что как реализация по смыслу одна и та же из за константных ссылок в аргументах
void ADD(const Core (&), DecodedInstrucion);
void ADDU(const Core (&), DecodedInstrucion);
void ADDUI(const Core (&), DecodedInstrucion);

void SUB(const Core (&), DecodedInstrucion);
void SUBU(const Core (&), DecodedInstrucion);

void DIV(const Core (&), DecodedInstrucion);
void DIVU(const Core (&), DecodedInstrucion);

void MULT(const Core (&), DecodedInstrucion);
void MULTU(const Core (&), DecodedInstrucion);
//==================================================

//LOGICAL OPERATIONS
//==================================================
void AND(const Core (&), DecodedInstrucion);
void ANDI(const Core (&), DecodedInstrucion);

void OR(const Core (&), DecodedInstrucion);
void ORI(const Core (&), DecodedInstrucion);
void XOR(const Core (&), DecodedInstrucion);
void XORI(const Core (&), DecodedInstrucion);
void NOR(const Core (&), DecodedInstrucion);

//SHIFT OPERATIONS
//==================================================
void SLL(const Core (&), DecodedInstrucion); // Shift left logical
void SRL(const Core (&), DecodedInstrucion); // Shift right logical

void SLLV(const Core (&), DecodedInstrucion); // Shift left logical variable
void SRLV(const Core (&), DecodedInstrucion); // Shift right logical variable

void SRA(const Core (&), DecodedInstrucion); // Shift right arithmetic
//==================================================

//SET OPERATIONS
//==================================================
void SLT(const Core (&), DecodedInstrucion); // Set on less than (signed)
void SLTI(const Core (&), DecodedInstrucion); // Set on less than immediate (signed)

void SLTU(const Core (&), DecodedInstrucion); // Set on less than (unsigned)
void SLTUI(const Core (&), DecodedInstrucion); // Set on less than immediate (unsigned)
//==================================================

//MOVE OPERATIONS
//==================================================
void MFLO(const Core (&), DecodedInstrucion);
void MFHI(const Core (&), DecodedInstrucion);
//==================================================

//LUI OPERATION
//==================================================
void LUI(const Core (&), DecodedInstrucion, const uint32_t); //  Load upper immediate
//==================================================

//MEMORY OPERATIONS
//==================================================
void SB(const Core (&), DecodedInstrucion); // Store byte (NOT FINISHED YET) 
															// MEM[$s + offset] = (0xff & $t); advance_pc (4); 
															// sb $t, offset($s)
void SW(const Core (&), DecodedInstrucion); // Store word (NOT FINISHED YET) 
															// MEM[$s + offset] = $t; advance_pc (4); 
															// sw $t, offset($s)

void LB(const Core (&), DecodedInstrucion); // Load byte (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lb $t, offset($s)
void LW(const Core (&), DecodedInstrucion); // Load word (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lw $t, offset($s)
//==================================================

//BRANCH OPERATIONS
//==================================================
void BEQ (const Core (&), DecodedInstrucion); // Branch on equal (DOESNOT EXIST YET) 
													   // if $s == $t advance_pc (offset << 2)); else advance_pc (4); 
													   // beq $s, $t, offset
void BNE (const Core (&), DecodedInstrucion); // Branch on not equal (DOESNOT EXIST YET) 
													   // if $s != $t advance_pc (offset << 2)); else advance_pc (4); 
													   // bne $s, $t, offset

void BGEZ (const Core (&), DecodedInstrucion); // Branch on greater than or equal to zero (DOESNOT EXIST YET) 
								     // if $s >= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // bgez $s, offset
void BGEZAL (const Core (&), DecodedInstrucion); // Branch on greater than or equal to zero and link (DOESNOT EXIST YET) 
								   	   // if $s >= 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
								 	   // bgezal $s, offset
void BGTZ (const Core (&), DecodedInstrucion); // Branch on greater than zero (DOESNOT EXIST YET) 
									 // if $s > 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bgtz $s, offset
									
void BLEZ (const Core (&), DecodedInstrucion); // Branch on less than or equal to zero (DOESNOT EXIST YET) 
								     // if $s <= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // blez $s, offset
void BLTZAL (const Core (&), DecodedInstrucion); // Branch on less than zero and link (DOESNOT EXIST YET) 
									   // if $s < 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
									   // bltzal $s, offset
void BLTZ (const Core (&), DecodedInstrucion); // Branch on less than zero (DOESNOT EXIST YET) 
									 // if $s < 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bltz $s, offset
//==================================================

//JUMP OPERATIONS
//==================================================
void J_(const Core (&o_core), DecodedInstrucion m_instruction); // Jump
				  // PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
				  // j target
void JAL (const Core (&o_core), DecodedInstrucion m_instruction); // Jump and link
					 // $31 = PC + 8 (or nPC + 4); PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
					 // jal target
void JR (const Core (&o_core), DecodedInstrucion m_instruction); // Jump register
					// PC = nPC; nPC = $s;
					// jr $s				 
//==================================================

//NOOP OPERATION
//==================================================
void NOOP(); // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
//==================================================

//SYSCALL OPERATION
//==================================================
void SYSCALL();
//==================================================
#endif