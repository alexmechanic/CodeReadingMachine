#include "core.h"

class Core {
public:
	Core();
	void setdebugoptions(uint32_t);
	int execute();
private:
	uint32_t m_pc;
	uint32_t m_debugoptions;
	uint32_t m_currentinstr, m_previousinstr;
	Memory *m_memory;
};

Core::Core() {
}
void Core::setdebugoptions(uint32_t debugoptions) {
	m_debugoptions = debugoptions;
}
int Core::execute() {
	m_memory->read4(m_pc, m_currentinstr);
	m_pc += 4;
}