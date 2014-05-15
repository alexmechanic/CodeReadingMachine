#include <string>
#include "proto.h"
using namespace std;
//ARIPHMETICAL OPERATIONS
//==================================================
//commands ADD and its derivatives
//--------------------------------------------------
int ADD (Core &o_core, DecodedInstruction m_instruction)  //ADD + ADD immediate
// константные ссылки. необходима для последнего аргумента чтобы работало сложение с константыой, вторая - до кучи, и потому как не нужно измеять значение второго регистра
//при случае вызова ADDI имя функции тупо заменяется на ADD, ибо нечего 2 одинаковых функции заводить
{
	uint32_t a = o_core.readRegister((Registers)m_instruction.rs), b = o_core.readRegister((Registers)m_instruction.rt);
	uint32_t sum = a + b;
	uint32_t ssd = a ^ b;
	if ((ssd >= 0) && ((ssd ^ sum) < 0))
		return -2; //overflow exception
	else  
	{
		o_core.writeRegister((Registers)m_instruction.rd, sum);	
		return 0; 
		o_core.advance_pc (4);
	}
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

int ADDU(Core &o_core, DecodedInstruction m_instruction)
{	
	uint32_t sum = o_core.readRegister((Registers)m_instruction.rs) + o_core.readRegister((Registers)m_instruction.rt);
	o_core.writeRegister((Registers)m_instruction.rd, sum);
	o_core.advance_pc (4);
	return 0;
}

int ADDUI(Core &o_core, DecodedInstruction m_instruction)
{	
	uint32_t sum = o_core.readRegister((Registers)m_instruction.rs) + m_instruction.immediate;
	o_core.writeRegister((Registers)m_instruction.rt, sum);
	o_core.advance_pc (4);
	return 0;
}
//--------------------------------------------------

//commands SUB and its derivatives
//--------------------------------------------------
int SUB(Core &o_core, DecodedInstruction m_instruction)  //sub
{
	uint32_t a = o_core.readRegister((Registers)m_instruction.rs), b = o_core.readRegister((Registers)m_instruction.rt);
	uint32_t diff = a - b;
	if (diff <= a)
	{
		o_core.writeRegister((Registers)m_instruction.rd, diff);	
		o_core.advance_pc (4);
	}
	else return -2; //overflow exception
	return 0;
}

int SUBU(Core &o_core, DecodedInstruction m_instruction)
{
	uint32_t diff = o_core.readRegister((Registers)m_instruction.rs) - o_core.readRegister((Registers)m_instruction.rt);
	o_core.writeRegister((Registers)m_instruction.rd, diff);
	o_core.advance_pc (4);
	return 0;
}
//--------------------------------------------------

//commands DIV and its derivatives
//--------------------------------------------------
int DIV(Core &o_core, DecodedInstruction m_instruction)
{
	o_core.writeRegister( Registers::LO, o_core.readRegister((Registers)m_instruction.rs) / o_core.readRegister((Registers)m_instruction.rt) );
	o_core.writeRegister( Registers::HI, o_core.readRegister((Registers)m_instruction.rs) % o_core.readRegister((Registers)m_instruction.rt) );
	o_core.advance_pc (4);
	return 0;
}

int DIVU(Core &o_core, DecodedInstruction m_instruction)
{
	o_core.writeRegister( Registers::LO, (o_core.readRegister((Registers)m_instruction.rs) / o_core.readRegister((Registers)m_instruction.rt)) );
	o_core.writeRegister( Registers::HI, (o_core.readRegister((Registers)m_instruction.rs) % o_core.readRegister((Registers)m_instruction.rt)) );
	o_core.advance_pc (4);
	return 0;
}
//--------------------------------------------------

//commands MULT and its derivatives
//--------------------------------------------------
int MULT(Core &o_core, DecodedInstruction m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(o_core.readRegister((Registers)m_instruction.rs) * o_core.readRegister((Registers)m_instruction.rt));
	o_core.writeRegister( Registers::LO, ((result << 32) >> 32) );
	o_core.writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
	o_core.advance_pc (4);
	return 0;
}

int MULTU(Core &o_core, DecodedInstruction m_instruction)
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)(o_core.readRegister((Registers)m_instruction.rs) * o_core.readRegister((Registers)m_instruction.rt));
	o_core.writeRegister( Registers::LO, ((result << 32) >> 32) );
	o_core.writeRegister( Registers::HI, (result >> 32) ); // нужно ли помещать старшую часть в HI, если такого нет в reference?
	o_core.advance_pc (4);
	return 0;
}
//--------------------------------------------------
//==================================================

//LOGICAL OPERATIONS
//==================================================

//commands AND and its derivatives
//--------------------------------------------------
int AND(Core &o_core, DecodedInstruction m_instruction)
{	o_core.writeRegister((Registers)m_instruction.rd, (o_core.readRegister((Registers)m_instruction.rs) & o_core.readRegister((Registers)m_instruction.rt)) );
	o_core.advance_pc (4);
	return 0;
}

int ANDI(Core &o_core, DecodedInstruction m_instruction)
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) & m_instruction.immediate) );
	o_core.advance_pc (4);
	return 0;
}
//--------------------------------------------------

//commands OR and its derivatives
//--------------------------------------------------
int OR(Core &o_core, DecodedInstruction m_instruction)  //Bitwise or
{	o_core.writeRegister((Registers)m_instruction.rd, o_core.readRegister((Registers)m_instruction.rs) | o_core.readRegister((Registers)m_instruction.rt) );		return 0;
	o_core.advance_pc (4);
}

int ORI(Core &o_core, DecodedInstruction m_instruction)  // Bitwise or immediate
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) | m_instruction.immediate) );		return 0;
	o_core.advance_pc (4);
}

int XOR(Core &o_core, DecodedInstruction m_instruction)  //Bitwise exclusive or
{	o_core.writeRegister((Registers)m_instruction.rd, o_core.readRegister((Registers)m_instruction.rs) ^ o_core.readRegister((Registers)m_instruction.rt) );		return 0;
	o_core.advance_pc (4);
}

int XORI(Core &o_core, DecodedInstruction m_instruction)  //Bitwise exclusive or
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) ^ m_instruction.immediate) );		return 0;
	o_core.advance_pc (4);
}

int NOR(Core &o_core, DecodedInstruction m_instruction)  // Bitwise exclusive or immediate
{	o_core.writeRegister((Registers)m_instruction.rd, (~(o_core.readRegister((Registers)m_instruction.rs) | o_core.readRegister((Registers)m_instruction.rt))) );		return 0;
	o_core.advance_pc (4);
}
//==================================================

//SHIFT OPERATIONS
//==================================================
int SLL(Core &o_core, DecodedInstruction m_instruction) // Shift left logical
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) << m_instruction.immediate) );		return 0;
	o_core.advance_pc (4);
}

int SRL(Core &o_core, DecodedInstruction m_instruction) // Shift right logical
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) >> m_instruction.immediate) );		return 0;
	o_core.advance_pc (4);
}

int SLLV(Core &o_core, DecodedInstruction m_instruction) // Shift left logical variable
{	o_core.writeRegister((Registers)m_instruction.rd, (o_core.readRegister((Registers)m_instruction.rs) << o_core.readRegister((Registers)m_instruction.rt)) );	return 0;
	o_core.advance_pc (4);
}

int SRLV(Core &o_core, DecodedInstruction m_instruction) // Shift right logical variable
{	o_core.writeRegister((Registers)m_instruction.rd, (o_core.readRegister((Registers)m_instruction.rs) >> o_core.readRegister((Registers)m_instruction.rt)) );	return 0;
	o_core.advance_pc (4);
}

int SRA(Core &o_core, DecodedInstruction m_instruction) // Shift right arithmetic
{	o_core.writeRegister((Registers)m_instruction.rt, (o_core.readRegister((Registers)m_instruction.rs) >> m_instruction.immediate) );		return 0;
	o_core.advance_pc (4);
} // в чем разница с Shift right logical?!
//==================================================

//SET OPERATIONS
//==================================================
int SLT(Core &o_core, DecodedInstruction m_instruction)  // Set on less than (signed)
{	o_core.writeRegister((Registers)m_instruction.rd, ( \
						(o_core.readRegister((Registers)m_instruction.rs) < o_core.readRegister((Registers)m_instruction.rt)) ? 1 : 0 \
						) );	
	o_core.advance_pc (4);
	return 0;
}

int SLTI(Core &o_core, DecodedInstruction m_instruction)  // Set on less than immediate (signed)
{	o_core.writeRegister((Registers)m_instruction.rt, ( \
						(o_core.readRegister((Registers)m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	
	o_core.advance_pc (4);
	return 0;
}

int SLTU(Core &o_core, DecodedInstruction m_instruction)  // Set on less than (unsigned)
{	o_core.writeRegister((Registers)m_instruction.rd, ( \
						(o_core.readRegister((Registers)m_instruction.rs) < o_core.readRegister((Registers)m_instruction.rt)) ? 1 : 0 \
						) );	
	o_core.advance_pc (4);
	return 0;
}

int SLTUI(Core &o_core, DecodedInstruction m_instruction)  // Set on less than immediate (unsigned)
{	o_core.writeRegister((Registers)m_instruction.rt, ( \
						(o_core.readRegister((Registers)m_instruction.rs) < m_instruction.immediate) ? 1 : 0 \
						) );	
	o_core.advance_pc (4);
	return 0;
}
//==================================================

//MOVE OPERATIONS
//==================================================
int MFLO(Core &o_core, DecodedInstruction m_instruction)  // Move from LO
{	o_core.writeRegister((Registers)m_instruction.rd, o_core.readRegister(Registers::LO) );	
	o_core.advance_pc (4);
	return 0;
}

int MFHI(Core &o_core, DecodedInstruction m_instruction)  // Move from HI
{	o_core.writeRegister((Registers)m_instruction.rd, o_core.readRegister(Registers::HI) );	
	o_core.advance_pc (4);
	return 0;
}
//==================================================

//LUI OPERATION
//==================================================
int LUI(Core &o_core, DecodedInstruction m_instruction)  // load upper immediate
{	o_core.writeRegister((Registers)m_instruction.rt, (m_instruction.immediate << 16) ); 
	o_core.advance_pc (4);
	return 0;
}
//==================================================

//MEMORY OPERATIONS
//==================================================
int LB(Core &o_core, DecodedInstruction m_instruction) //Load byte
{
	byte temp;
	if (o_core.m_memory->readByte((byte *)o_core.readRegister((Registers)m_instruction.rs)+m_instruction.shamt, temp) == 0 )
	{	return -2;	}
	else 
	{	
		o_core.writeRegister((Registers)m_instruction.rt, temp); 
		return 0;
	}
	o_core.advance_pc (4);
};

int LW(Core &o_core, DecodedInstruction m_instruction) //Load word
{	
	word temp;
	if (o_core.m_memory->readWord ((byte *)o_core.readRegister((Registers)m_instruction.rs)+m_instruction.immediate, temp)!= 0 ) /*(оттуда ли брать Offset?)*///, (Registers)m_instruction.rt) != 0 )
	{	return -2;	}
	else 
	{
		o_core.writeRegister((Registers)m_instruction.rt, temp); 
		return 0;
	}
	o_core.advance_pc (4);
	
};

int SB(Core &o_core, DecodedInstruction m_instruction) // Store byte
{
	//if (o_core.m_memory->writeBytes((byte *)o_core.readRegister((Registers)m_instruction.rs)+m_instruction.immediate, (byte *)(0xff & o_core.readRegister((Registers)m_instruction.rt)), 1) != 0 )
	//{	/*error*/	}
	o_core.advance_pc (4);
	return 0;
}

int SW(Core &o_core, DecodedInstruction m_instruction) // Store word
{
	//if (writeWord(o_core.readRegister((Registers)m_instruction.rs)+offset, (0xff & o_core.readRegister((Registers)m_instruction.rt))) != 0 )
	//{	/*error*/	}
	o_core.advance_pc (4);
	return 0;
}
//==================================================

//NOOP OPERATION
//==================================================
int NOOP(Core &o_core) // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
{	o_core.advance_pc(4);
	return 0;
}
//==================================================

//SYSCALL OPERATION
//==================================================
int SYSCALL(Core &o_core)
{	
	int tmp_reg = o_core.readRegister(Registers::v0);
	switch(tmp_reg)
	{/*
		case 1: 
		{	cout << o_core.readRegister(Registers::a0) << endl;	break;	} // куда выводить - решим, переопределить поток cout придется
		case 2: 
		{	cout << o_core.readRegister(Registers::f12) << endl;	break;	}
		case 3: 
		{	cout << o_core.readRegister(Registers::f12) << endl;	break;	}
		case 4: 
		{	cout << o_core.readRegister(Registers::a0) << ends << endl;	break;	}
		case 5:
		{	int num; cin >> num; o_core.writeRegister(Registers::v0, num)); break;	} // вопрос: откуда считывать, из памяти или с... экрана?
		case 6:
		{	float num; cin >> num; o_core.writeRegister(Registers::f0, num)); break;	}
		case 7:
		{	double num; cin >> num; o_core.writeRegister(Registers::f0, num)); break;	}
		case 8:
		{	 char[o_core.readRegister(Registers::a1)] str; cin >> str; o_core.writeRegister(Registers::a0, str); break;	}
		case 9:
		{		}
		case 10:
		{}
		case 11:
		{	 string num; cin >> num; o_core.writeRegister(Registers::f0, num)); break;	}
		case 12:
		{	 char symb; cin >> symb; o_core.writeRegister(Registers::a0, symb)); break;	}
		case 13, 14, 15, 16:
		{}*/
	}
	o_core.advance_pc(4);
	return 0;
}
//==================================================

//JUMP OPERATIONS
//==================================================
int J_(Core &o_core, DecodedInstruction m_instruction) // Jump
{
	o_core.m_pc = o_core.m_nPC; 
	o_core.m_nPC = (o_core.m_pc & 0xf0000000) | (m_instruction.address << 2);
	return 0;
}
int JAL (Core &o_core, DecodedInstruction m_instruction) // Jump and link
{
	o_core.writeRegister(Registers::ra, o_core.m_pc + 8) /*(or m_nPC + 4)*/; 
	o_core.m_pc = o_core.m_nPC; 
	o_core.m_nPC = (o_core.m_pc & 0xf0000000) | (m_instruction.address << 2);
	return 0;
}
int JR (Core &o_core, DecodedInstruction m_instruction) // Jump register
{
	o_core.m_pc = o_core.m_nPC; 
	o_core.m_nPC = o_core.readRegister((Registers)m_instruction.address);
	return 0;
}
//==================================================

//BRANCH OPERATIONS
//==================================================
int BEQ (Core &o_core, DecodedInstruction m_instruction)  //Branch on equal 
{
	if (o_core.readRegister((Registers)m_instruction.rs) == o_core.readRegister((Registers)m_instruction.rt)) 
		o_core.advance_pc(m_instruction.immediate << 2);
	else o_core.advance_pc(4);
	return 0;
}

int BNE (Core &o_core, DecodedInstruction m_instruction) //Branch on not equal
{
	if (o_core.readRegister((Registers)m_instruction.rs) != o_core.readRegister((Registers)m_instruction.rt)) 
		o_core.advance_pc(m_instruction.immediate << 2); 
	else o_core.advance_pc(4); 
	return 0;
}

int BGEZ (Core &o_core, DecodedInstruction m_instruction)  //Branch on greater than or equal to zero
{
	if (o_core.readRegister((Registers)m_instruction.rs) >= 0)
		o_core.advance_pc(m_instruction.immediate << 2); 
	else o_core.advance_pc(4); 
	return 0;
}

int BGEZAL (Core &o_core, DecodedInstruction m_instruction)  //Branch on greater than or equal to zero and link
{
	if (o_core.readRegister((Registers)m_instruction.rs) >= 0)
	{	
		o_core.writeRegister(Registers::ra, o_core.m_pc + 8) /*(or m_nPC + 4)*/; 
		o_core.advance_pc(m_instruction.immediate << 2); 
	}
	else o_core.advance_pc(4);
	return 0;
}

int BGTZ (Core &o_core, DecodedInstruction m_instruction)  //Branch on greater than zero
{
	if (o_core.readRegister((Registers)m_instruction.rs) > 0)
		o_core.advance_pc(m_instruction.immediate << 2); 
	else o_core.advance_pc(4); 
	return 0;
}

int BLEZ (Core &o_core, DecodedInstruction m_instruction) // Branch on less than or equal to zero
{
	if (o_core.readRegister((Registers)m_instruction.rs) <= 0)
		o_core.advance_pc(m_instruction.immediate << 2); 
	else o_core.advance_pc(4); 
	return 0;
}

int BLTZAL (Core &o_core, DecodedInstruction m_instruction) // Branch on less than zero and link 
{
	if (o_core.readRegister((Registers)m_instruction.rs) < 0)
	{	
		o_core.writeRegister(Registers::ra, o_core.m_pc + 8) /*(or m_nPC + 4)*/; 	
		o_core.advance_pc(m_instruction.immediate << 2); 
	}
	else o_core.advance_pc(4);
	return 0;
}

int BLTZ (Core &o_core, DecodedInstruction m_instruction) // Branch on less than zero
{
	if (o_core.readRegister((Registers)m_instruction.rs) < 0)
		o_core.advance_pc(m_instruction.immediate << 2); 
	else o_core.advance_pc(4);
	return 0;
}
//==================================================