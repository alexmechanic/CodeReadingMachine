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
	uint32_t a = readRegister(m_instruction.rs), b = readRegister(m_instruction.rt);
	uint32_t sum = a + b;
	uint32_t ssd = a ^ b;
	if ((ssd >= 0) && ((ssd ^ sum) < 0))
		return -2; //overflow exception
	else  
		{ writeRegister(m_instruction.rd, sum);	return 0; }
};
/*overflow check using asm:
(int a, int b) {
  __asm__((
     " add %0,%1\n"
     " jo overflowed"
     : "=0" (a): "r"(a), "r"(b): "flags"
  ));
  return a;
overflowed:
  throw integer_overflow();
}
*/

void ADDU(const Core (&o_core), DecodedInstrucion m_instruction)
{	
	uint32_t sum = readRegister(m_instruction.rs) + readRegister(m_instruction.rt);
	writeRegister(m_instruction.rd, sum);
};

void ADDUI(const Core (&o_core), DecodedInstrucion m_instruction)
{	
	uint32_t sum = readRegister(m_instruction.rs) + m_instruction.immediate;
	writeRegister(m_instruction.rt, sum);
};
//--------------------------------------------------

//commands SUB and its derivatives
//--------------------------------------------------
void SUB(const Core (&o_core), DecodedInstrucion m_instruction)  //sub
{
	uint32_t a = readRegister(m_instruction.rs), b = readRegister(m_instruction.rt);
	uint32_t diff = a - b;
	if (diff <= a)
		writeRegister(m_instruction.rd, diff);	
	else return -2; //overflow exception

};

void SUBU(const Core (&o_core), DecodedInstrucion m_instruction)
{
	uint32_t diff = readRegister(m_instruction.rs) - readRegister(m_instruction.rt);
	writeRegister(m_instruction.rd, diff);
};
//--------------------------------------------------

//commands DIV and its derivatives
//--------------------------------------------------
void DIV(const Core (&o_core), DecodedInstrucion m_instruction)
{
	writeRegister( Registers::LO, readRegister(m_instruction.rs) / readRegister(m_instruction.rt) );
	writeRegister( Registers::HI, readRegister(m_instruction.rs) % readRegister(m_instruction.rt) );
};

void DIVU(const Core (&o_core), DecodedInstrucion m_instruction)
{
	writeRegister( Registers::LO, (readRegister(m_instruction.rs) / readRegister(m_instruction.rt)) );
	writeRegister( Registers::HI, (readRegister(m_instruction.rs) % readRegister(m_instruction.rt)) );
};
//--------------------------------------------------

//commands MULT and its derivatives
//--------------------------------------------------
void MULT(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(readRegister(m_instruction.rs) * readRegister(m_instruction.rt));
	writeRegister( Registers::LO, ((result << 32) >> 32) );
	writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
};

void MULTU(const Core (&o_core), DecodedInstrucion m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(readRegister(m_instruction.rs) * readRegister(m_instruction.rt));
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
{	writeRegister(m_instruction.rd, (readRegister(m_instruction.rs) & readRegister(m_instruction.rt)) );	};

void ANDI(const Core (&o_core), DecodedInstrucion m_instruction)
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) & m_instruction.immediate) );	};
//--------------------------------------------------

//commands OR and its derivatives
//--------------------------------------------------
void OR(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise or
{	writeRegister(m_instruction.rd, readRegister(m_instruction.rs) | readRegister(m_instruction.rt) );		};

void ORI(const Core (&o_core), DecodedInstrucion m_instruction)  // Bitwise or immediate
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) | m_instruction.immediate) );		};

void XOR(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise exclusive or
{	writeRegister(m_instruction.rd, readRegister(m_instruction.rs) ^ readRegister(m_instruction.rt) );		};

void XORI(const Core (&o_core), DecodedInstrucion m_instruction)  //Bitwise exclusive or
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) ^ m_instruction.immediate) );		};

void NOR(const Core (&o_core), DecodedInstrucion m_instruction)  // Bitwise exclusive or immediate
{	writeRegister(m_instruction.rd, (~(readRegister(m_instruction.rs) | readRegister(m_instruction.rt))) );		};
//==================================================

//SHIFT OPERATIONS
//==================================================
void SLL(const Core (&o_core), DecodedInstrucion m_instruction) // Shift left logical
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) << m_instruction.immediate) );		};

void SRL(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right logical
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) >> m_instruction.immediate) );		};

void SLLV(const Core (&o_core), DecodedInstrucion m_instruction) // Shift left logical variable
{	writeRegister(m_instruction.rd, (readRegister(m_instruction.rs) << readRegister(m_instruction.rt)) );	};

void SRLV(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right logical variable
{	writeRegister(m_instruction.rd, (readRegister(m_instruction.rs) >> readRegister(m_instruction.rt)) );	};

void SRA(const Core (&o_core), DecodedInstrucion m_instruction) // Shift right arithmetic
{	writeRegister(m_instruction.rt, (readRegister(m_instruction.rs) >> m_instruction.immediate) );		}; // в чем разница с Shift right logical?!
//==================================================

//SET OPERATIONS
//==================================================
void SLT(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than (signed)
{	writeRegister(m_instruction.rd, ( \
						(readRegister(m_instruction.rs) < readRegister(m_instruction.rt)) ? 1 : 0 \
						) );	};

void SLTI(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than immediate (signed)
{	writeRegister(m_instruction.rt, ( \
						(readRegister(m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	};

void SLTU(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than (unsigned)
{	writeRegister(m_instruction.rd, ( \
						(readRegister(m_instruction.rs) < readRegister(m_instruction.rt)) ? 1 : 0 \
						) );	};

void SLTUI(const Core (&o_core), DecodedInstrucion m_instruction)  // Set on less than immediate (unsigned)
{	writeRegister(m_instruction.rt, ( \
						(readRegister(m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	};
//==================================================

//MOVE OPERATIONS
//==================================================
void MFLO(const Core (&o_core), DecodedInstrucion m_instruction)  // Move from LO
{	writeRegister(m_instruction.rd, readRegister(Registers::LO) );	};

void MFHI(const Core (&o_core), DecodedInstrucion m_instruction)  // Move from HI
{	writeRegister(m_instruction.rd, readRegister(Registers::HI) );	};
//==================================================

//LUI OPERATION
//==================================================
void LUI(const Core (&o_core), DecodedInstrucion m_instruction)  // load upper immediate
{	writeRegister(m_instruction.rt, (m_instruction.immediate << 16) );	};
//==================================================

//MEMORY OPERATIONS
//==================================================
void LB(const Core (&o_core), DecodedInstrucion m_instruction) //Load byte
{	if (readByte (readRegister(m_instruction.rs)+offset, m_instruction.rt) != 0 )
	{	/*error*/	}
};

void LW(const Core (&o_core), DecodedInstrucion m_instruction) //Load word
{	if (readWord (readRegister(m_instruction.rs)+m_instruction.immediate)!= 0 ); /*(оттуда ли брать Offset?)*/, m_instruction.rt) != 0 )
	{	/*error*/	}
};

void SB(const Core (&o_core), DecodedInstrucion m_instruction); // Store byte
{
	if (writeByte(readRegister(m_instruction.rs)+m_instruction.immediate, (0xff & readRegister(m_instruction.rt))) != 0 )
	{	/*error*/	}
};

void SW(const Core (&o_core), DecodedInstrucion m_instruction); // Store word
{
	if (writeWord(readRegister(m_instruction.rs)+offset, (0xff & readRegister(m_instruction.rt))) != 0 )
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
	o_core->nPC = readRegister(m_instruction.address);
}
//==================================================

//BRANCH OPERATIONS
//==================================================
void BEQ (const Core (&o_core), DecodedInstrucion) // Branch on equal 
{
	if (readRegister(m_instruction.rs) == readRegister(m_instruction.rt)) 
		o_core->advance_pc(m_instruction.immediate << 2);
	else o_core->advance_pc(4);
}

void BNE (const Core (&o_core), DecodedInstrucion); // Branch on not equal
{
	if (readRegister(m_instruction.rs) != readRegister(m_instruction.rt)) 
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

void BGEZ (const Core (&o_core), DecodedInstrucion); // Branch on greater than or equal to zero
{
	if (readRegister(m_instruction.rs) >= 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

void BGEZAL (const Core (&o_core), DecodedInstrucion); // Branch on greater than or equal to zero and link
{
	if (readRegister(m_instruction.rs) >= 0)
	{	
		writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(m_instruction.immediate << 2); 
	}
	else o_core->advance_pc(4); 
}

void BGTZ (const Core (&o_core), DecodedInstrucion); // Branch on greater than zero
{
	if (readRegister(m_instruction.rs) > 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

void BLEZ (const Core (&o_core), DecodedInstrucion); // Branch on less than or equal to zero
{
	if (readRegister(m_instruction.rs) <= 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

void BLTZAL (const Core (&o_core), DecodedInstrucion); // Branch on less than zero and link 
{
	if (readRegister(m_instruction.rs) < 0)
	{	
		writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(m_instruction.immediate << 2); 
	}
	else o_core->advance_pc(4); 
}

void BLTZ (const Core (&o_core), DecodedInstrucion); // Branch on less than zero
{
	if (readRegister(m_instruction.rs) < 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}
//==================================================