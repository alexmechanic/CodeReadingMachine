#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit(): m_size(config::SIZE_OF_RAM_MEMORY), 
									m_phisicalMemory((byte*)malloc(config::SIZE_OF_RAM_MEMORY)), 
									m_swapMemory((byte*)malloc(config::SIZE_OF_SWAP)),
									tlb(),
									m_debug_log("Initialisation\n") {
	log("Allocated %d bytes for physical memory\n", m_size);
	if(m_phisicalMemory == NULL) {throw std::bad_alloc("Can not allocate phisicalMemory for MemoryUnit");}
}

MemoryUnit::~MemoryUnit() { free(m_phisicalMemory); }

int MemoryUnit::writeBytes(byte *dst_ptr, byte *src_ptr, size_t size) {
	try {dst_ptr = translate(dst_ptr);} catch(...) {return -1;} 
	memcpy(dst_ptr, src_ptr, size);
	log("%d bytes were written at %p from %p\n", size, dst_ptr, src_ptr);
	return 0;
}

int MemoryUnit::writeCharString(byte *dst_ptr, const char *src_ptr) {
	try {dst_ptr = translate(dst_ptr);} catch(...) {return -1;}
	strcpy( reinterpret_cast<char*>(dst_ptr), src_ptr);
	log("%d bytes of chars were written at %p from %p\n", strlen(src_ptr), dst_ptr, src_ptr);
	return 0;
}

byte* MemoryUnit::translate(byte* ptr) const {
	log("Translating %p\n", ptr);
	int buf = tlb.translateVirtualPage( extractPageNumber(ptr) );
	if( buf == -1) {
		log("TLB entry not found. Allocating new entry. \n");
		buf = tlb.placeIntoMemory( extractPageNumber(ptr) );
		if( buf < 0 )
			return NULL; //temporarily
		log("...allocated RAM-page #%d\n", buf);
	} else
		log("Found TLB entry with. Phisical memory page number %d.\n", buf);
	return m_phisicalMemory + buf*config::SIZE_OF_PAGE + extractPageBias(ptr);
	//return ptr; //заглушка
}

void MemoryUnit::log(const char *_Format, ...) const {
	va_list args;
	va_start(args, _Format);
	vsnprintf_s(m_debug_buffer, sizeof(m_debug_buffer), sizeof(m_debug_buffer), _Format, args);
	m_debug_log.append(m_debug_buffer);
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
			if ( fgets (m_main_buffer, sizeof(m_main_buffer), src_file) == NULL ) 
				break;
			m_debug_log.append(m_main_buffer);
			writeCharString( translate(m_phisicalMemory + bias), m_main_buffer);
			bias += strlen(m_main_buffer);
		}
		log("===\n");
		fclose (src_file);
	}
}