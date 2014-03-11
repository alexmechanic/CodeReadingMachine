#include <string>
using namespace std;
//ARIPHMETICAL OPERATIONS
//==================================================
//commands ADD and its derivatives
//--------------------------------------------------
void ADD (const Core (&o_core), DecodedInstrucion m_instruction)  //ADD + ADD immediate
// константные ссылки. необходима для последнего аргумента чтобы работало сложение с константыой, вторая - до кучи, и потому как не нужно измеять значение второго регистра
//при случае вызова ADDI имя функции тупо заменяется на ADD, ибо нечего 2 одинаковых функции заводить
{	
	uint32_t sum = readRegister(r1) + readRegister(r2);
	writeRegister(r_to, sum);	
};

void ADDU(const Core (&o_core), DecodedInstrucion m_instruction)
{	
	uint32_t sum = readRegister(r1) + readRegister(r2);
	writeRegister(r_to, sum);
};

void ADDUI(const Core (&o_core), DecodedInstrucion m_instruction)
{	
	uint32_t sum = readRegister(r1) + C;
	writeRegister(r_to, sum);
};
//--------------------------------------------------

//commands SUB and its derivatives
//--------------------------------------------------
void SUB(const Core (&o_core), DecodedInstrucion m_instruction)  //sub
// константные ссылки. необходима для последнего аргумента потому как не нужно измеять значение ни второго, ни первого регистров
{
	uint32_t diff = readRegister(r1) - readRegister(r2); // потом обработать случаи переполнения (?)
	writeRegister(r_to, diff);

};

void SUBU(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходима для последнего аргумента потому как не нужно измеять значение ни второго, ни первого регистров
{
	uint32_t diff = readRegister(r1) - readRegister(r2); // потом обработать случаи переполнения (?)
	writeRegister(r_to, diff);
};
//--------------------------------------------------

//commands DIV and its derivatives
//--------------------------------------------------
void DIV(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	writeRegister( Registers::LO, readRegister(r1) / readRegister(r2) );
	writeRegister( Registers::HI, readRegister(r1) % readRegister(r2) );
};

void DIVU(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	writeRegister( Registers::LO, (readRegister(r1) / readRegister(r2)) );
	writeRegister( Registers::HI, (readRegister(r1) % readRegister(r2)) );
};
//--------------------------------------------------

//commands MULT and its derivatives
//--------------------------------------------------
void MULT(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(readRegister(r1) * readRegister(r2));
	writeRegister( Registers::LO, ((result << 32) >> 32) );
	writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
};

void MULTU(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(readRegister(r1) * readRegister(r2));
	writeRegister( Registers::LO, ((result << 32) >> 32) );
	writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
};
//--------------------------------------------------
//==================================================

//LOGICAL OPERATIONS
//==================================================

//commands AND and its derivatives
//--------------------------------------------------
void AND(const Core (&o_core), DecodedInstrucion m_instruction)
{	writeRegister(r_to, (readRegister(r1) & readRegister(r2)) );	};

void ANDI(const Core (&o_core), DecodedInstrucion m_instruction)
{	writeRegister(r_to, (readRegister(r1) & C) );	};
//--------------------------------------------------

//commands OR and its derivatives
//--------------------------------------------------
void OR(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise or
{	writeRegister(r_to, readRegister(r1) | readRegister(r2) );		};

void ORI(const Core (&o_core), DecodedInstrucion m_instruction)  // Bitwise or immediate
{	writeRegister(r_to, (readRegister(r1) | C) );		};

void XOR(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise exclusive or
{	writeRegister(r_to, readRegister(r1) ^ readRegister(r2) );		};

void XORI(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise exclusive or
{	writeRegister(r_to, (readRegister(r1) ^ C) );		};

void NOR(const Core (&o_core), DecodedInstrucion m_instruction)  // Bitwise exclusive or immediate
{	writeRegister(r_to, (~(r1 | r2)) );		};
//==================================================

//SHIFT OPERATIONS
//==================================================
void SLL(const Core (&o_core), DecodedInstrucion m_instruction) // Shift left logical
{	writeRegister(r_to, (readRegister(r1) << C) );		};

void SRL(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right logical
{	writeRegister(r_to, (readRegister(r1) >> C) );		};

void SLLV(const Core (&o_core), DecodedInstrucion m_instruction) // Shift left logical variable
{	writeRegister(r_to, (readRegister(r1) << readRegister(r2)) );	};

void SRLV(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right logical variable
{	writeRegister(r_to, (readRegister(r1) >> readRegister(r2)) );	};

void SRA(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right arithmetic
{	writeRegister(r_to, (readRegister(r1) >> C) );		}; // в чем разница с Shift right logical?!
//==================================================

//SET OPERATIONS
//==================================================
void SLT(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than (signed)
{	writeRegister(r_to, ( \
						(readRegister(r1) < readRegister(r2)) ? 1 : 0 \
						) );	};

void SLTI(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than immediate (signed)
{	writeRegister(r_to, ( \
						(readRegister(r1) < C) ? 1 : 0 \
						) );	};

void SLTU(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than (unsigned)
{	writeRegister(r_to, ( \
						(readRegister(r1) < readRegister(r2)) ? 1 : 0 \
						) );	};

void SLTUI(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than immediate (unsigned)
{	writeRegister(r_to, ( \
						(readRegister(r1) < C) ? 1 : 0 \
						) );	};
//==================================================

//MOVE OPERATIONS
//==================================================
void MFLO(const Core (&o_core), DecodedInstrucion m_instruction)  // Move from LO
{	writeRegister(r_to, readRegister(Registers::LO) );	};

void MFHI(const Core (&o_core), DecodedInstrucion m_instruction)  // Move from HI
{	writeRegister(r_to, readRegister(Registers::HI) );	};
//==================================================

//LUI OPERATION
//==================================================
void LUI(const Core (&o_core), DecodedInstrucion m_instruction)  // load upper immediate
{	writeRegister(r_to, (C << 16) );	};
//==================================================

//MEMORY OPERATIONS
//==================================================
void LB(const Core (&o_core), DecodedInstrucion m_instruction) //Load byte
{	if (readByte (r1+offset, r_to) != 0 )
	{	/*error*/	}
};

void LW(const Core (&o_core), DecodedInstrucion m_instruction) //Load byte
{	if (readWord (r1+offset, r_to) != 0 )
	{	/*error*/	}
};

void SB(const Core (&o_core), DecodedInstrucion m_instruction); // Store byte
{
	if (writeByte(r1+offset, r_from) != 0 )
	{	/*error*/	}
};

void SW(const Core (&o_core), DecodedInstrucion m_instruction); // Store word
{
	if (writeWord(r1+offset, r_from) != 0 )
	{	/*error*/	}
};
//==================================================

//NOOP OPERATION
//==================================================
void NOOP() // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
{	o_core->advance_pc(4);	};
//==================================================

//SYSCALL OPERATION
//==================================================
void SYSCALL()
{	
	int tmp_reg = readRegister($v0);
	switch(tmp_reg)
	{
		case 1: 
		{	cout << readRegister(Regsters::a0) << endl;	break;	} // куда выводить - решим, переопределить поток cout придется
		case 2: 
		{	cout << readRegister(Regsters::f12) << endl;	break;	}
		case 3: 
		{	cout << readRegister(Regsters::f12) << endl;	break;	}
		case 4: 
		{	cout << readRegister(Regsters::a0) << ends << endl;	break;	}
		case 5:
		{	int num; cin >> num; writeRegister(Regsters::v0, num)); break;	} // вопрос: откуда считывать, из памяти или с... экрана?
		case 6:
		{	float num; cin >> num; writeRegister(Regsters::f0, num)); break;	}
		case 7:
		{	double num; cin >> num; writeRegister(Regsters::f0, num)); break;	}
		case 8:
		{	 char[readRegister(Regsters::a1)] str; cin >> str; writeRegister(Regsters::a0, str); break;	}
		case 9:
		{	 /* ??? */		}
		case 10:
		{	 /*как выходить из программы будем?! */		}
		case 11:
		{	 string num; cin >> num; writeRegister(Regsters::f0, num)); break;	}
		case 12:
		{	 char symb; cin >> symb; writeRegister(Regsters::a0, symb)); break;	}
		case 13, 14, 15, 16:
		{	 /* хз как делать, надо ли? */	}
	}
	o_core->advance_pc(4);	
};
//==================================================

//JUMP OPERATIONS
//==================================================
void J(const Core (&o_core), DecodedInstrucion m_instruction) // Jump
{
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = (o_core->m_pc & 0xf0000000) | (target << 2);
}
void JAL (const Core (&o_core), DecodedInstrucion m_instruction) // Jump and link
{
	writeRegister(Registers::r31, /* что за регистр 31?! */ o_core->m_pc + 8) /*(or nPC + 4)*/; 
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = (o_core->m_pc & 0xf0000000) | (target << 2);
}
void JR (const Core (&o_core), DecodedInstrucion m_instruction) // Jump register
{
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = readRegister(m_instruction.RegisterToJump);
}
//==================================================

//BRANCH OPERATIONS
//==================================================
void BEQ (const Core (&o_core), DecodedInstrucion) // Branch on equal 
{
	if (readRegister(r1) == readRegister(r2)) 
		o_core->advance_pc(offset << 2);
	else o_core->advance_pc(4);
}

void BNE (const Core (&o_core), DecodedInstrucion); // Branch on not equal
{
	if (readRegister(r1) != readRegister(r2)) 
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

void BGEZ (const Core (&o_core), DecodedInstrucion); // Branch on greater than or equal to zero
{
	if (readRegister(r1) >= 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

void BGEZAL (const Core (&o_core), DecodedInstrucion); // Branch on greater than or equal to zero and link
{
	if (readRegister(r1) >= 0)
	{	
		writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(offset << 2); 
	}
	else o_core->advance_pc(4); 
}

void BGTZ (const Core (&o_core), DecodedInstrucion); // Branch on greater than zero
{
	if (readRegister(r1) > 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

void BLEZ (const Core (&o_core), DecodedInstrucion); // Branch on less than or equal to zero
{
	if (readRegister(r1) <= 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

void BLTZAL (const Core (&o_core), DecodedInstrucion); // Branch on less than zero and link 
{
	if (readRegister(r1) < 0)
	{	
		writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(offset << 2); 
	}
	else o_core->advance_pc(4); 
}

void BLTZ (const Core (&o_core), DecodedInstrucion); // Branch on less than zero
{
	if (readRegister(r1) < 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}
//==================================================