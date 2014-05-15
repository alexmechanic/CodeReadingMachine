#include "Core.h"
#include "proto.h"

Core::Core() {
	instructionsArray[0x020] = ADD;
	instructionsArray[0x021] = ADDU;
	instructionsArray[0x240] = ADDUI;
	instructionsArray[0x022] = SUB;
	instructionsArray[0x023] = SUBU;
	instructionsArray[0x01A] = DIV;
	instructionsArray[0x01B] = DIVU;
	instructionsArray[0x018] = MULT;
	instructionsArray[0x019] = MULTU;
	instructionsArray[0x024] = AND;
	instructionsArray[0x300] = ANDI;
	instructionsArray[0x025] = OR;
	instructionsArray[0x340] = ORI;
	instructionsArray[0x026] = XOR;
	instructionsArray[0x380] = XORI;
	instructionsArray[0x027] = NOR;
	instructionsArray[0x000] = SLL;
	instructionsArray[0x002] = SRL;
	instructionsArray[0x004] = SLLV;
	instructionsArray[0x006] = SRLV;
	instructionsArray[0x003] = SRA;
	instructionsArray[0x02A] = SLT;
	instructionsArray[0x280] = SLTI;
	instructionsArray[0x02B] = SLTU;
	instructionsArray[0x2C0] = SLTUI;
	instructionsArray[0x012] = MFLO;
	instructionsArray[0x010] = MFHI;
	instructionsArray[0x3C0] = LUI;
	instructionsArray[0xA00] = SB;
	instructionsArray[0xAC0] = SW;
	instructionsArray[0x800] = LB;
	instructionsArray[0x8C0] = LW;
	instructionsArray[0x100] = BEQ;
	instructionsArray[0x140] = BNE;
	instructionsArray[0x040] = BGEZ;
	//instructionsArray[0x] = BGEZAL;
	instructionsArray[0x1C0] = BGTZ;
	instructionsArray[0x180] = BLEZ;
	//instructionsArray[0x] = BLTZAL;
	//instructionsArray[0x] = BLTZ;
	instructionsArray[0x080] = J_;
	instructionsArray[0x0C0] = JAL;
	instructionsArray[0x008] = JR;
	//instructionsArray[0xFFF] = NOOP;	// нет
	//instructionsArray[0x00C] = SYSCALL;
	m_currentBasicBlock = 0;
	m_registers[Registers::zero] = 0;
}

uint32_t Core::readRegister(Registers reg) {
	return m_registers[reg];
}

int Core::writeRegister(Registers reg, uint32_t value) {
	switch(reg) {
	case Registers::zero:
		return -1;
	default:
		m_registers[reg] = value;
		return m_registers[reg];
	}
}

void Core::setDebugOptions(uint32_t debugOptions) {
	m_debugOptions = debugOptions;
}
int Core::execute() {
	if(!m_instructionsLeft) {
		m_pc = m_nextPC;
		if(!findInBasicBlockCache(m_pc))
			fillBasicBlock();
		m_nextPC = m_basicBlockCache[m_currentBasicBlock].next_pc;
	}
	call(m_decodedInstr);
	--m_instructionsLeft;
	return 0;
}

void Core::advance_pc (word offset)
{
    m_pc  =  m_nPC;
	m_nPC  += offset;
}

DecodedInstruction Core::decode(uint32_t instruction) {
	Instruction instr;
	DecodedInstruction decodedInstr;
	uint32_t opcd;
	instr.code = instruction;
	opcd = instr.opcode;
	decodedInstr.internalOpcode = opcd;
	
	if (opcd == 0x00 || opcd == 0x10) {
		decodedInstr.format = InstructionFormat::R;
		decodedInstr.additionalData = 0;
		decodedInstr.rs = instr.rs;
		decodedInstr.rt = instr.rt;
		decodedInstr.rd = instr.rd;
		decodedInstr.shamt = instr.shamt;
		// добавлена функция со смещением (всего 12 бит)
		decodedInstr.internalOpcode = decodedInstr.internalOpcode | (instr.function << 6);
	}
	else if (opcd == 0x02 || opcd == 0x03) {
		decodedInstr.format = InstructionFormat::J;
		decodedInstr.additionalData = 0;
		decodedInstr.address = instr.address;
	}
	else {
		decodedInstr.format = InstructionFormat::I;
		decodedInstr.additionalData = 0;
		decodedInstr.rs = instr.rs;
		decodedInstr.rt = instr.rt;
		decodedInstr.immediate = instr.immediate;
	}
	return decodedInstr;
}

uint32_t Core::calculateBasicBlockPosition() {
	// что нужно затереть (дописать выбор)
	return 0;
}

int Core::call(DecodedInstruction instruction) {
	(instructionsArray[instruction.internalOpcode])(*this, instruction);
	m_pc += 4;
	return 0;
}

int Core::fillBasicBlock() {
	m_currentBasicBlock = calculateBasicBlockPosition(); // пока просто 0
	//m_basicBlockCache[m_currentBasicBlock].N = 0;		// если не инициализируется в конструкторе (а оно инициализируется)
	m_basicBlockCache[m_currentBasicBlock].pc = m_pc;
	m_basicBlockCache[m_currentBasicBlock].next_pc = m_pc + 4 * BASICBLOCKSIZE;
	for(uint32_t i = m_basicBlockCache[m_currentBasicBlock].pc; i < m_basicBlockCache[m_currentBasicBlock].next_pc; i += 4) {
		m_memory->readDword((byte *)i, m_currentInstr);
		m_decodedInstr = decode(m_currentInstr);
		if(isBranch(m_decodedInstr))
			if(i == BASICBLOCKSIZE - 1) {
				m_basicBlockCache[m_currentBasicBlock].next_pc = i;
				break;
			}
			else {
				m_basicBlockCache[m_currentBasicBlock].next_pc = i + 8;
			}
		m_basicBlockCache[m_currentBasicBlock].inst[i] = m_decodedInstr;
		++m_basicBlockCache[m_currentBasicBlock].N;
	}
	m_instructionsLeft = m_basicBlockCache[m_currentBasicBlock].N;
	return 0;
}

char Core::isBranch(DecodedInstruction instr) {
	// переделать
	if(instr.format == J)
		return 1;
	else
		return 0;
}

char Core::findInBasicBlockCache(uint32_t _pc) {
	int i;
	for(i = 0; i < CACHESIZE; ++i) {
		if(m_basicBlockCache[i].N && (m_basicBlockCache[i].pc == _pc)) {
			m_currentBasicBlock = i;
			return 1;
		}
	}
	return 0;
}

int Core::setPC(uint32_t _pc) {
	m_pc = _pc;
	return 0;
}