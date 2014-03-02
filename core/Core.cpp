#include "Core.h"

Core::Core() {
	m_basicBlockIterator = 0;
	m_currentBasicBlock = 0;
	m_registers[Registers::zero] = 0;
}

uint32_t Core::readRegister(Registers reg) {
	return m_registers[reg];
}

int Core::writeRegister(Registers reg, uint32_t value) {
	switch (reg) {
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
	m_memory->readDword((byte *)m_pc, m_currentInstr);
	m_decodedInstr = decode(m_currentInstr);
	if (m_basicBlockIterator >= BASICBLOCKSIZE)
		m_basicBlockIterator = 0;
	else if (m_basicBlockIterator == 0) {
		m_currentBasicBlock = calculateBasicBlockPosition(); // пока просто 0
		m_basicBlockCache[m_currentBasicBlock].pc = m_pc;
		m_basicBlockCache[m_currentBasicBlock].N = 1;
	}
	else if (m_basicBlockIterator == (BASICBLOCKSIZE - 1)) {
		// проверка (не до конца понял)
	}
	call(m_decodedInstr);
	m_basicBlockCache[m_currentBasicBlock].inst[m_basicBlockIterator] = m_decodedInstr;
	++m_basicBlockIterator;
	m_pc += 4;
	return 0;
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
	return 0;
}

int Core::call(DecodedInstruction instruction) {
	return 0;
}