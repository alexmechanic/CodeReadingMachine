#include <string>
using namespace std;
//ARIPHMETICAL OPERATIONS
//==================================================
//commands ADD and its derivatives
//--------------------------------------------------
void ADD (uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  //ADD + ADD immediate
// константные ссылки. необходима для последнего аргумента чтобы работало сложение с константыой, вторая - до кучи, и потому как не нужно измеять значение второго регистра
//при случае вызова ADDI имя функции тупо заменяется на ADD, ибо нечего 2 одинаковых функции заводить
{	r_to = r1+r2;	};

void ADDU(uint32_t  (&r_to), const uint32_t   (&r1), const uint32_t   (&r2))
{	r_to = (uint32_t)(r1+r2);	};

void ADDUI(uint32_t  (&r_to), const uint32_t   (&r1), const int32_t C)
{	r_to = (uint32_t)(r1+C);	};
//--------------------------------------------------

//commands SUB and its derivatives
//--------------------------------------------------
void SUB(uint32_t (&r_to), const uint32_t (&r1), const uint32_t (&r2))  //sub
// константные ссылки. необходима для последнего аргумента потому как не нужно измеять значение ни второго, ни первого регистров
{
	uint32_t diff = r1-r2; // потом обработать случаи переполнения (?)
	r_to = diff;

};

void SUBU(uint32_t (&r_to), const uint32_t (&r1), const uint32_t (&r2))
// константные ссылки. необходима для последнего аргумента потому как не нужно измеять значение ни второго, ни первого регистров
{
	uint32_t diff = r1-r2; // потом обработать случаи переполнения (?)
	r_to = diff;
};
//--------------------------------------------------

//commands DIV and its derivatives
//--------------------------------------------------
void DIV(const uint32_t (&r1), const uint32_t (&r2))
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	LO = r1 / r2;
	HI = r1 % r2;
};

void DIVU(const uint32_t (&r1), const uint32_t (&r2))
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	LO = r1 / r2;
	HI = (r1 % r2);
};
//--------------------------------------------------

//commands MULT and its derivatives
//--------------------------------------------------
void MULT(const uint32_t (&r1), const uint32_t (&r2))
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)r1 * r2;
	LO = (result << 32) >> 32;
	HI = result >> 32; // нужно ли помещать старшую часть в HI, если такого нет в reference?
};

void MULTU(const uint32_t (&r1), const uint32_t (&r2))
// константные ссылки. необходимы потому как не нужно изменять значение ни второго, ни первого регистров, плюс - передача по ссылке
{
	int64_t result = (int64_t)r1 * r2;
	LO = (result << 32) >> 32;
	HI = result >> 32; //нужно ли помещать старшую часть в HI, если такого нет в reference?
};
//--------------------------------------------------
//==================================================

//LOGICAL OPERATIONS
//==================================================

//commands AND and its derivatives
//--------------------------------------------------
void AND(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))
{	r_to = r1 & r2;		};

void ANDI(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t  C)
{	r_to = r1 & C;		};
//--------------------------------------------------

//commands OR and its derivatives
//--------------------------------------------------
void OR(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  //Bitwise or
{	r_to = r1 | r2;		};

void ORI(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t  C)  // Bitwise or immediate
{	r_to = r1 | C;		};

void XOR(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  //Bitwise exclusive or
{	r_to = r1 ^ r2;		};

void XORI(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  C)  //Bitwise exclusive or
{	r_to = r1 ^ C;		};

void NOR(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  // Bitwise exclusive or immediate
{	r_to = ~(r1 | r2);	};
//==================================================

//SHIFT OPERATIONS
//==================================================
void SLL(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t C) // Shift left logical
{	r_to = r1 << C;		};

void SRL(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t C) // Shift right logical
{	r_to = r1 >> C;		};

void SLLV(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t (&r2)) // Shift left logical variable
{	r_to = r1 << r2;	};

void SRLV(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t (&r2)) // Shift right logical variable
{	r_to = r1 >> r2;	};

void SRA(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t C) // Shift right arithmetic
{	r_to = r1 >> C;		}; // в чем разница с Shift right logical?!
//==================================================

//SET OPERATIONS
//==================================================
void SLT(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  // Set on less than (signed)
{	r_to = (r1 < r2) ? 1 : 0;	};

void SLTI(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t C)  // Set on less than immediate (signed)
{	r_to = (r1 < C) ? 1 : 0;	};

void SLTU(uint32_t  (&r_to), const uint32_t  (&r1), const uint32_t  (&r2))  // Set on less than (unsigned)
{	r_to = (r1 < r2) ? 1 : 0;	};

void SLTUI(uint32_t  (&r_to), const uint32_t  (&r1), const int32_t C)  // Set on less than immediate (unsigned)
{	r_to = (r1 < C) ? 1 : 0;	};
//==================================================

//MOVE OPERATIONS
//==================================================
void MFLO(uint32_t  (&r_to))  // Move from LO
{	r_to = LO;	};

void MFHI(uint32_t  (&r_to))  // Move from HI
{	r_to = HI;	};
//==================================================

//LUI OPERATION
//==================================================
void LUI(uint32_t  (&r_to), const int32_t C)  // load upper immediate
{	r_to = C << 16;	};
//==================================================

//MEMORY OPERATIONS
//==================================================
void LB(uint32_t  (&) r_to, const int32_t (&) r1, const uint32_t offset) //Load byte
{	if (readByte (*(r1+offset), r_to) != 0 )
	{	/*error*/	}
};

void LW(uint32_t  (&r_to), const int32_t (&r1), const uint32_t offset) //Load byte
{	if (readWord (*(r1+offset), r_to) != 0 )
	{	/*error*/	}
};

void SB(uint32_t  (&r_from), const uint32_t (&r1), const uint32_t offset); // Store byte
{
	if (write(*(r1+offset), r_from) != 0 )
	{	/*error*/	}
};

void SW(uint32_t  (&r_from), const uint32_t (&r1), const uint32_t offset); // Store word
{
	if (write(*(r1+offset), r_from) != 0 )
	{	/*error*/	}
};
//==================================================

//NOOP OPERATION
//==================================================
void NOOP() // Note: The encoding for a NOOP represents the instruction SLL $0, $0, 0 which has no side effects.
{	/*advance_pc (4);*/	};
//==================================================

//SYSCALL OPERATION
//==================================================
void SYSCALL()
{	
int tmp_reg = readRegister($v0); //readRegister пока не существует, исправить позже!
switch(tmp_reg)
{
	case 1: 
	{	cout << readRegister($a0) << endl;	break;	} // куда выводить - решим, переопределить поток cout придется
	case 2: 
	{	cout << readRegister($f12) << endl;	break;	}
	case 3: 
	{	cout << readRegister($f12) << endl;	break;	}
	case 4: 
	{	cout << readRegister($a0) << ends << endl;	break;	}
	case 5:
	{	int num; cin >> num; writeRegister($v0, num)); break;	} // вопрос: откуда считывать, из памяти или с... экрана?
	case 6:
	{	float num; cin >> num; writeRegister($f0, num)); break;	}
	case 7:
	{	double num; cin >> num; writeRegister($f0, num)); break;	}
	case 8:
	{	 char[readRegister($a1)] str; cin >> str; writeRegister($a0, str)); break;	}
	case 9:
	{	 /* ??? */		}
	case 10:
	{	 /*как выходить из программы будем?! */		}
	case 11:
	{	 string num; cin >> num; writeRegister($f0, num)); break;	}
	case 12:
	{	 char symb; cin >> symb; writeRegister($a0, symb)); break;	}
	case 13, 14, 15, 16:
	{	 /* хз как делать, надо ли? */	}
}
/*advance_pc (4);*/	
};
//==================================================