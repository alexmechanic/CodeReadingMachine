#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit(): m_phisicalMemory((byte*)malloc(config::SIZE_OF_RAM_MEMORY)), 
									m_swapMemory((byte*)malloc(config::SIZE_OF_SWAP)),
									tlb() {
	logFile = fopen("log.txt", "w");
	if(logFile == NULL) { printf("WARNING: Can\'t create file for log\n"); }
	if(m_phisicalMemory == NULL) {throw std::bad_alloc("Can not allocate phisicalMemory for MemoryUnit");}
	log("Allocated %d bytes for physical memory\n", config::SIZE_OF_RAM_MEMORY);
	log("Allocated %d bytes for swap\n", config::SIZE_OF_SWAP);
}

MemoryUnit::~MemoryUnit() { 
	log("===\nShuttig down:\n");
	free(m_phisicalMemory); 
	log("Phisical memory freed:\n");
	free(m_swapMemory);
	log("Swap memory freed:\n");

	log("Ready to detach log file...\n");
	fclose(logFile);
}

int MemoryUnit::writeBytes(byte *dstPtr, byte *srcPtr, size_t size) {
	try {dstPtr = translate(dstPtr);} catch(...) {return -1;} 
	memcpy(dstPtr, srcPtr, size);
	log("%d bytes were written at %p from %p\n", size, dstPtr, srcPtr);
	return 0;
}

int MemoryUnit::writeCharString(byte *dstPtr, const char *srcPtr) {
	try {dstPtr = translate(dstPtr);} catch(...) {return -1;}
	strcpy( reinterpret_cast<char*>(dstPtr), srcPtr);
	log("%d bytes of chars were written at %p from %p\n", strlen(srcPtr), dstPtr, srcPtr);
	return 0;
}

byte* MemoryUnit::translate(byte* ptr) const {

	log("Translating %p\n", ptr);

	int bufVirtualPage = tlb.translateVirtualPage( extractPageNumber(ptr) );
	if( bufVirtualPage == -1) {
		log("TLB entry not found. Allocating new entry. \n");
		bufVirtualPage = tlb.placeIntoMemory( extractPageNumber(ptr) );
		if( bufVirtualPage < 0 )
			return NULL; //temporarily
		log("...allocated RAM-page #%d\n", bufVirtualPage);
	} else
		log("Found TLB entry with phisical memory page number %d.\n", bufVirtualPage);
	return m_phisicalMemory + bufVirtualPage*config::SIZE_OF_PAGE + extractPageBias(ptr);
	//return ptr; //заглушка
}

void MemoryUnit::log(const char *_Format, ...) const {
	//Insert here possible return because of disabled logging
	va_list args;
	va_start(args, _Format);
	vsnprintf_s(m_debug_buffer, sizeof(m_debug_buffer), sizeof(m_debug_buffer), _Format, args);
	fwrite(reinterpret_cast<void*>(m_debug_buffer), sizeof(char), strlen(m_debug_buffer), logFile);
	fflush(logFile);
	//fprintf(logFile, m_debug_buffer);
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
			if ( fgets (m_mainBuffer, sizeof(m_mainBuffer), src_file) == NULL ) 
				break;
			writeCharString( translate(m_phisicalMemory + bias), m_mainBuffer);
			bias += strlen(m_mainBuffer);
		}
		log("===\n");
		fclose (src_file);
	}
}