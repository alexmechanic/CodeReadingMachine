#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit(size_t asize): m_size(asize), m_debug_log("Initialisation\n") {
	m_phisicalMemory = (byte*)malloc(m_size); 
	log("Allocated %d bytes for physical memory\n", m_size);
	if(m_phisicalMemory == NULL) {throw std::bad_alloc("Can not allocate phisicalMemory for MemoryUnit");}
}

MemoryUnit::~MemoryUnit() { free(m_phisicalMemory); }

void MemoryUnit::writeBytes(byte *dst_ptr, byte *src_ptr, size_t size) {
	try {dst_ptr = translate(dst_ptr);} catch(...) {throw;} //пока что просто перебрасываем исключение
	memcpy(dst_ptr, src_ptr, size);
	log("%d bytes were written at %p from %p\n", size, dst_ptr, src_ptr);
}

void MemoryUnit::writeCharString(byte *dst_ptr, const char *src_ptr) {
	try {dst_ptr = translate(dst_ptr);} catch(...) {throw;} //пока что просто перебрасываем исключение
	strcpy( reinterpret_cast<char*>(dst_ptr), src_ptr);
	log("%d bytes of chars were written at %p from %p\n", strlen(src_ptr), dst_ptr, src_ptr);
}

byte* MemoryUnit::translate(byte* ptr) const {
	if((ptr < m_phisicalMemory)||(ptr >= m_phisicalMemory + m_size)) {
		sprintf_s(m_debug_bufer, sizeof(m_debug_bufer), "translate() error: trying to access invalid pointer %p", ptr);
		throw std::invalid_argument(m_debug_bufer);
	}
	return ptr; //заглушка
}

void MemoryUnit::log(const char *_Format, ...) const {
	va_list args;
	va_start(args, _Format);
	vsnprintf_s(m_debug_bufer, sizeof(m_debug_bufer), sizeof(m_debug_bufer), _Format, args);
	m_debug_log.append(m_debug_bufer);
	va_end(args);
}

void MemoryUnit::loadProgram(const char* path) {
	FILE *src_file = NULL;
	unsigned int bias = 0;
	if( (src_file = fopen(path, "r")) == NULL)
		throw std::invalid_argument("Can not open code file\n");
	else {
		log("Loading program:\n===\n");
		while(!feof(src_file)) {
			if ( fgets (m_main_bufer, sizeof(m_main_bufer), src_file) == NULL ) 
				break;
			m_debug_log.append(m_main_bufer);
			writeCharString( translate(m_phisicalMemory + bias), m_main_bufer);
			bias += strlen(m_main_bufer);
		}
		log("===\n");
		fclose (src_file);
	}
}