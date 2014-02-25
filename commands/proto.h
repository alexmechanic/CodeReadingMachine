//ARIPHMETICAL OPERATIONS
//==================================================
#define addi add //потому что как реализация по смыслу одна и та же из за константных ссылок в аргументах
void ADD(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void ADDU(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void ADDUI(uint32_t  (&), const uint32_t  (&), const uint32_t);

void SUB(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void SUBU(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));

void DIV(const uint32_t (&), const uint32_t (&));
void DIVU(const uint32_t (&), const uint32_t (&));

void MULT(const uint32_t (&), const uint32_t (&));
void MULTU(const uint32_t (&), const uint32_t (&));
//==================================================

//LOGICAL OPERATIONS
//==================================================
void AND(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void ANDI(uint32_t  (&), const uint32_t  (&), const uint32_t);

void OR(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void ORI(uint32_t  (&), const uint32_t  (&), const uint32_t);
void XOR(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void XORI(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));
void NOR(uint32_t  (&), const uint32_t  (&), const uint32_t  (&));

//SHIFT OPERATIONS
//==================================================
void SLL(uint32_t  (&), const uint32_t  (&), const uint32_t); // Shift left logical
void SRL(uint32_t  (&), const uint32_t  (&), const uint32_t); // Shift right logical

void SLLV(uint32_t  (&), const uint32_t  (&), const uint32_t (&)); // Shift left logical variable
void SRLV(uint32_t  (&), const uint32_t  (&), const uint32_t (&)); // Shift right logical variable

void SRA(uint32_t  (&), const uint32_t  (&), const uint32_t); // Shift right arithmetic
//==================================================

//SET OPERATIONS
//==================================================
void SLT(uint32_t  (&), const uint32_t  (&), const uint32_t  (&)); // Set on less than (signed)
void SLTI(uint32_t  (&), const uint32_t  (&), const uint32_t); // Set on less than immediate (signed)

void SLTU(uint32_t  (&), const uint32_t  (&), const uint32_t  (&)); // Set on less than (unsigned)
void SLTUI(uint32_t  (&), const uint32_t  (&), const uint32_t); // Set on less than immediate (unsigned)
//==================================================

//MOVE OPERATIONS
//==================================================
void MFLO(uint32_t  (&));
void MFHI(uint32_t  (&));
//==================================================

//LUI OPERATION
//==================================================
void LUI(uint32_t  (&), const uint32_t); //  Load upper immediate
//==================================================

//MEMORY OPERATIONS
//==================================================
void SB(uint32_t  (&), const uint32_t (&), const uint32_t); // Store byte (NOT FINISHED YET) 
															// MEM[$s + offset] = (0xff & $t); advance_pc (4); 
															// sb $t, offset($s)
void SW(uint32_t  (&), const uint32_t (&), const uint32_t); // Store word (NOT FINISHED YET) 
															// MEM[$s + offset] = $t; advance_pc (4); 
															// sw $t, offset($s)

void LB(uint32_t  (&), const uint32_t (&), const uint32_t); // Load byte (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lb $t, offset($s)
void LW(uint32_t  (&), const uint32_t (&), const uint32_t); // Load word (NOT FINISHED YET) 
															// $t = MEM[$s + offset]; advance_pc (4); 
															// lw $t, offset($s)
//==================================================

//BRANCH OPERATIONS
//==================================================
void BEQ (uint32_t  (&), const uint32_t (&), uint32_t); // Branch on equal (DOESNOT EXIST YET) 
													   // if $s == $t advance_pc (offset << 2)); else advance_pc (4); 
													   // beq $s, $t, offset
void BNE (uint32_t  (&), const uint32_t (&), uint32_t); // Branch on not equal (DOESNOT EXIST YET) 
													   // if $s != $t advance_pc (offset << 2)); else advance_pc (4); 
													   // bne $s, $t, offset

void BGEZ (uint32_t  (&), uint32_t); // Branch on greater than or equal to zero (DOESNOT EXIST YET) 
								     // if $s >= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // bgez $s, offset
void BGEZAL (uint32_t  (&), uint32_t); // Branch on greater than or equal to zero and link (DOESNOT EXIST YET) 
								   	   // if $s >= 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
								 	   // bgezal $s, offset
void BGTZ (uint32_t  (&), uint32_t); // Branch on greater than zero (DOESNOT EXIST YET) 
									 // if $s > 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bgtz $s, offset
									
void BLEZ (uint32_t  (&), uint32_t); // Branch on less than or equal to zero (DOESNOT EXIST YET) 
								     // if $s >= 0 advance_pc (offset << 2)); else advance_pc (4);
								     // blez $s, offset
void BLTZAL (uint32_t  (&), uint32_t); // Branch on less than zero and link (DOESNOT EXIST YET) 
									   // if $s < 0 $31 = PC + 8 (or nPC + 4); advance_pc (offset << 2)); else advance_pc (4);
									   // bltzal $s, offset
void BLTZ (uint32_t  (&), uint32_t); // Branch on less than zero (DOESNOT EXIST YET) 
									 // if $s < 0 advance_pc (offset << 2)); else advance_pc (4);
									 // bltz $s, offset
//==================================================

//JUMP OPERATIONS
//==================================================
void J(uint32_t); // Jump (DOESNOT EXIST YET) 
				  // PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
				  // j target
void JAL (uint32_t); // Jump and link (DOESNOT EXIST YET) 
					 // $31 = PC + 8 (or nPC + 4); PC = nPC; nPC = (PC & 0xf0000000) | (target << 2);
					 // jal target
void JR (uint32_t); // Jump register (DOESNOT EXIST YET) 
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