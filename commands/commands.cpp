#include <string>
using namespace std;
//ARIPHMETICAL OPERATIONS
//==================================================
//commands ADD and its derivatives
//--------------------------------------------------
int ADD (Core (&o_core), DecodedInstruction m_instruction)  //ADD + ADD immediate
// константные ссылки. необходима для последнего аргумента чтобы работало сложение с константыой, вторая - до кучи, и потому как не нужно измеять значение второго регистра
//при случае вызова ADDI имя функции тупо заменяется на ADD, ибо нечего 2 одинаковых функции заводить
{
	uint32_t a = o_core.readRegister(m_instruction.rs), b = o_core.readRegister(m_instruction.rt);
	uint32_t sum = a + b;
	uint32_t ssd = a ^ b;
	if ((ssd >= 0) && ((ssd ^ sum) < 0))
		return -2; //overflow exception
	else  
		{ o_core.writeRegister(m_instruction.rd, sum);	return 0; }
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

int ADDU(Core (&o_core), DecodedInstruction m_instruction)
{	
	uint32_t sum = o_core.readRegister(m_instruction.rs) + o_core.readRegister(m_instruction.rt);
	o_core.writeRegister(m_instruction.rd, sum);
};

int ADDUI(Core (&o_core), DecodedInstruction m_instruction)
{	
	uint32_t sum = o_core.readRegister(m_instruction.rs) + m_instruction.immediate;
	o_core.writeRegister(m_instruction.rt, sum);
};
//--------------------------------------------------

//commands SUB and its derivatives
//--------------------------------------------------
int SUB(Core (&o_core), DecodedInstruction m_instruction)  //sub
{
	uint32_t a = o_core.readRegister(m_instruction.rs), b = o_core.readRegister(m_instruction.rt);
	uint32_t diff = a - b;
	if (diff <= a)
		o_core.writeRegister(m_instruction.rd, diff);	
	else return -2; //overflow exception

};

int SUBU(Core (&o_core), DecodedInstruction m_instruction)
{
	uint32_t diff = o_core.readRegister(m_instruction.rs) - o_core.readRegister(m_instruction.rt);
	o_core.writeRegister(m_instruction.rd, diff);
};
//--------------------------------------------------

//commands DIV and its derivatives
//--------------------------------------------------
int DIV(Core (&o_core), DecodedInstruction m_instruction)
{
	o_core.writeRegister( Registers::LO, o_core.readRegister(m_instruction.rs) / o_core.readRegister(m_instruction.rt) );
	o_core.writeRegister( Registers::HI, o_core.readRegister(m_instruction.rs) % o_core.readRegister(m_instruction.rt) );
};
int DIVU(Core (&o_core), DecodedInstruction m_instruction)
{
	o_core.writeRegister( Registers::LO, (o_core.readRegister(m_instruction.rs) / o_core.readRegister(m_instruction.rt)) );
	o_core.writeRegister( Registers::HI, (o_core.readRegister(m_instruction.rs) % o_core.readRegister(m_instruction.rt)) );
};
//--------------------------------------------------

//commands MULT and its derivatives
//--------------------------------------------------
int MULT(Core (&o_core), DecodedInstruction m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(o_core.readRegister(m_instruction.rs) * o_core.readRegister(m_instruction.rt));
	o_core.writeRegister( Registers::LO, ((result << 32) >> 32) );
	o_core.writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
};

int MULTU(Core (&o_core), DecodedInstruction m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(o_core.readRegister(m_instruction.rs) * o_core.readRegister(m_instruction.rt));
	o_core.writeRegister( Registers::LO, ((result << 32) >> 32) );
	o_core.writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
};
//--------------------------------------------------
//==================================================

//LOGICAL OPERATIONS
//==================================================

//commands AND and its derivatives
//--------------------------------------------------
int AND(Core (&o_core), DecodedInstruction m_instruction)
{	o_core.writeRegister(m_instruction.rd, (o_core.readRegister(m_instruction.rs) & o_core.readRegister(m_instruction.rt)) );	};

int ANDI(Core (&o_core), DecodedInstruction m_instruction)
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) & m_instruction.immediate) );	};
//--------------------------------------------------

//commands OR and its derivatives
//--------------------------------------------------
int OR(Core (&o_core), DecodedInstruction m_instruction)  //Bitwise or
{	o_core.writeRegister(m_instruction.rd, o_core.readRegister(m_instruction.rs) | o_core.readRegister(m_instruction.rt) );		};

int ORI(Core (&o_core), DecodedInstruction m_instruction)  // Bitwise or immediate
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) | m_instruction.immediate) );		};

int XOR(Core (&o_core), DecodedInstruction m_instruction)  //Bitwise exclusive or
{	o_core.writeRegister(m_instruction.rd, o_core.readRegister(m_instruction.rs) ^ o_core.readRegister(m_instruction.rt) );		};

int XORI(Core (&o_core), DecodedInstruction m_instruction)  //Bitwise exclusive or
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) ^ m_instruction.immediate) );		};

int NOR(Core (&o_core), DecodedInstruction m_instruction)  // Bitwise exclusive or immediate
{	o_core.writeRegister(m_instruction.rd, (~(o_core.readRegister(m_instruction.rs) | o_core.readRegister(m_instruction.rt))) );		};
//==================================================

//SHIFT OPERATIONS
//==================================================
int SLL(Core (&o_core), DecodedInstruction m_instruction) // Shift left logical
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) << m_instruction.immediate) );		};

int SRL(Core (&o_core), DecodedInstruction m_instruction) // Shift right logical
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) >> m_instruction.immediate) );		};

int SLLV(Core (&o_core), DecodedInstruction m_instruction) // Shift left logical variable
{	o_core.writeRegister(m_instruction.rd, (o_core.readRegister(m_instruction.rs) << o_core.readRegister(m_instruction.rt)) );	};

int SRLV(Core (&o_core), DecodedInstruction m_instruction) // Shift right logical variable
{	o_core.writeRegister(m_instruction.rd, (o_core.readRegister(m_instruction.rs) >> o_core.readRegister(m_instruction.rt)) );	};

int SRA(Core (&o_core), DecodedInstruction m_instruction) // Shift right arithmetic
{	o_core.writeRegister(m_instruction.rt, (o_core.readRegister(m_instruction.rs) >> m_instruction.immediate) );		}; // в чем разница с Shift right logical?!
//==================================================

//SET OPERATIONS
//==================================================
int SLT(Core (&o_core), DecodedInstruction m_instruction)  // Set on less than (signed)
{	o_core.writeRegister(m_instruction.rd, ( \
						(o_core.readRegister(m_instruction.rs) < o_core.readRegister(m_instruction.rt)) ? 1 : 0 \
						) );	};

int SLTI(Core (&o_core), DecodedInstruction m_instruction)  // Set on less than immediate (signed)
{	o_core.writeRegister(m_instruction.rt, ( \
						(o_core.readRegister(m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	};

int SLTU(Core (&o_core), DecodedInstruction m_instruction)  // Set on less than (unsigned)
{	o_core.writeRegister(m_instruction.rd, ( \
						(o_core.readRegister(m_instruction.rs) < o_core.readRegister(m_instruction.rt)) ? 1 : 0 \
						) );	};

int SLTUI(Core (&o_core), DecodedInstruction m_instruction)  // Set on less than immediate (unsigned)
{	o_core.writeRegister(m_instruction.rt, ( \
						(o_core.readRegister(m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	};
//==================================================

//MOVE OPERATIONS
//==================================================
int MFLO(Core (&o_core), DecodedInstruction m_instruction)  // Move from LO
{	o_core.writeRegister(m_instruction.rd, o_core.readRegister(Registers::LO) );	};

int MFHI(Core (&o_core), DecodedInstruction m_instruction)  // Move from HI
{	o_core.writeRegister(m_instruction.rd, o_core.readRegister(Registers::HI) );	};
//==================================================

//LUI OPERATION
//==================================================
int LUI(Core (&o_core), DecodedInstruction m_instruction)  // load upper immediate
{	o_core.writeRegister(m_instruction.rt, (m_instruction.immediate << 16) );	};
//==================================================

//MEMORY OPERATIONS
//==================================================
int LB(Core (&o_core), DecodedInstruction m_instruction) //Load byte
{	if (readByte (o_core.readRegister(m_instruction.rs)+offset, m_instruction.rt) != 0 )
	{	/*error*/	}
};

int LW(Core (&o_core), DecodedInstruction m_instruction) //Load word
{	if (readWord (o_core.readRegister(m_instruction.rs)+m_instruction.immediate)!= 0 ); /*(оттуда ли брать Offset?)*/, m_instruction.rt) != 0 )
	{	/*error*/	}
};

int SB(Core (&o_core), DecodedInstruction m_instruction); // Store byte
{
	if (writeByte(o_core.readRegister(m_instruction.rs)+m_instruction.immediate, (0xff & o_core.readRegister(m_instruction.rt))) != 0 )
	{	/*error*/	}
};

int SW(Core (&o_core), DecodedInstruction m_instruction); // Store word
{
	if (writeWord(o_core.readRegister(m_instruction.rs)+offset, (0xff & o_core.readRegister(m_instruction.rt))) != 0 )
	{	/*error*/	}
};
//==================================================

//NOOP OPERATION
//==================================================
int NOOP() // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
{	o_core->advance_pc(4);	};
//==================================================

//SYSCALL OPERATION
//==================================================
int SYSCALL()
{	
	int tmp_reg = o_core.readRegister($v0);
	switch(tmp_reg)
	{
		case 1: 
		{	cout << o_core.readRegister(Regsters::a0) << endl;	break;	} // куда выводить - решим, переопределить поток cout придется
		case 2: 
		{	cout << o_core.readRegister(Regsters::f12) << endl;	break;	}
		case 3: 
		{	cout << o_core.readRegister(Regsters::f12) << endl;	break;	}
		case 4: 
		{	cout << o_core.readRegister(Regsters::a0) << ends << endl;	break;	}
		case 5:
		{	int num; cin >> num; o_core.writeRegister(Regsters::v0, num)); break;	} // вопрос: откуда считывать, из памяти или с... экрана?
		case 6:
		{	float num; cin >> num; o_core.writeRegister(Regsters::f0, num)); break;	}
		case 7:
		{	double num; cin >> num; o_core.writeRegister(Regsters::f0, num)); break;	}
		case 8:
		{	 char[o_core.readRegister(Regsters::a1)] str; cin >> str; o_core.writeRegister(Regsters::a0, str); break;	}
		case 9:
		{	 /* ??? */		}
		case 10:
		{	 /*как выходить из программы будем?! */		}
		case 11:
		{	 string num; cin >> num; o_core.writeRegister(Regsters::f0, num)); break;	}
		case 12:
		{	 char symb; cin >> symb; o_core.writeRegister(Regsters::a0, symb)); break;	}
		case 13, 14, 15, 16:
		{	 /* хз как делать, надо ли? */	}
	}
	o_core->advance_pc(4);	
};
//==================================================

//JUMP OPERATIONS
//==================================================
int J_(Core (&o_core), DecodedInstruction m_instruction) // Jump
{
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = (o_core->m_pc & 0xf0000000) | (target << 2);
}
int JAL (Core (&o_core), DecodedInstruction m_instruction) // Jump and link
{
	o_core.writeRegister(Registers::r31, /* что за регистр 31?! */ o_core->m_pc + 8) /*(or nPC + 4)*/; 
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = (o_core->m_pc & 0xf0000000) | (target << 2);
}
int JR (Core (&o_core), DecodedInstruction m_instruction) // Jump register
{
	o_core->m_pc = o_core->nPC; 
	o_core->nPC = o_core.readRegister(m_instruction.address);
}
//==================================================

//BRANCH OPERATIONS
//==================================================
int BEQ (Core (&o_core), DecodedInstruction) // Branch on equal 
{
	if (o_core.readRegister(m_instruction.rs) == o_core.readRegister(m_instruction.rt)) 
		o_core->advance_pc(m_instruction.immediate << 2);
	else o_core->advance_pc(4);
}

int BNE (Core (&o_core), DecodedInstruction); // Branch on not equal
{
	if (o_core.readRegister(m_instruction.rs) != o_core.readRegister(m_instruction.rt)) 
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

int BGEZ (Core (&o_core), DecodedInstruction); // Branch on greater than or equal to zero
{
	if (o_core.readRegister(m_instruction.rs) >= 0)
		o_core->advance_pc(offset << 2); 
	else o_core->advance_pc(4); 
}

int BGEZAL (Core (&o_core), DecodedInstruction); // Branch on greater than or equal to zero and link
{
	if (o_core.readRegister(m_instruction.rs) >= 0)
	{	
		o_core.writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(m_instruction.immediate << 2); 
	}
	else o_core->advance_pc(4); 
}

int BGTZ (Core (&o_core), DecodedInstruction); // Branch on greater than zero
{
	if (o_core.readRegister(m_instruction.rs) > 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

int BLEZ (Core (&o_core), DecodedInstruction); // Branch on less than or equal to zero
{
	if (o_core.readRegister(m_instruction.rs) <= 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}

int BLTZAL (Core (&o_core), DecodedInstruction); // Branch on less than zero and link 
{
	if (o_core.readRegister(m_instruction.rs) < 0)
	{	
		o_core.writeRegister(registers::r31, o_core->m_pc + 8);	
		o_core->advance_pc(m_instruction.immediate << 2); 
	}
	else o_core->advance_pc(4); 
}

int BLTZ (Core (&o_core), DecodedInstruction); // Branch on less than zero
{
	if (o_core.readRegister(m_instruction.rs) < 0)
		o_core->advance_pc(m_instruction.immediate << 2); 
	else o_core->advance_pc(4); 
}
//==================================================