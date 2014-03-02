#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit(): m_phisicalMemory((byte*)malloc(config::SIZE_OF_RAM_MEMORY)), 
									m_swapMemory((byte*)malloc(config::SIZE_OF_SWAP_FILE)),
									tlb() {
	logFile = fopen("log.txt", "w");
	if(logFile == NULL) { printf("WARNING: Can\'t create file for log\n"); }
	if(m_phisicalMemory == NULL) {throw std::bad_alloc("Can not allocate phisicalMemory for MemoryUnit");}
	log("Allocated %d bytes for physical memory\n", config::SIZE_OF_RAM_MEMORY);
	log("Allocated %d bytes for swap\n", config::SIZE_OF_SWAP_FILE);
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

int MemoryUnit::writeBytes(address dstPtr, byte *srcPtr, size_t size) {
	byte *realPtr; //adress on real machine
	log("Writing from real address %p to virtual address %p\n", srcPtr, dstPtr);
	if( (dstPtr = translate(dstPtr)) == NULL ) {
		log("Trying to write at NULL/n");
		return -1;
		//exit(-1); //will be fixed later
	}
	realPtr = convertIntoRealPtr(dstPtr);
	memcpy(realPtr, srcPtr, size);
	log("%d bytes were written at %p (real: %p) from %p\n", size, dstPtr, realPtr, srcPtr);
	return 0;
}


address MemoryUnit::translate(address ptr) {

	log("Translating %p\n", ptr);

	unsigned int bufVirtualPage = tlb.translateVirtualPage( extractPageNumber(ptr) );
	if( bufVirtualPage == -1) {
		log("TLB entry not found. Allocating new entry. \n");
		bufVirtualPage = tlb.placeIntoMemory( extractPageNumber(ptr) );
		if( bufVirtualPage < 0 )
			return NULL; //temporarily
		log("...allocated RAM-page #%d\n", bufVirtualPage);
	} else
		log("Found TLB entry with phisical memory page number %d.\n", bufVirtualPage);
	log("Translated to RAM address %p\n", bufVirtualPage*config::SIZE_OF_PAGE + extractPageBias(ptr));
	return bufVirtualPage*config::SIZE_OF_PAGE + extractPageBias(ptr);
	//return ptr; //заглушка
}

byte *MemoryUnit::convertIntoRealPtr(address adrs) const {
	log("RAM address %p translated into real address %p\n", adrs, adrs + m_phisicalMemory);
	return adrs + m_phisicalMemory;
}

void MemoryUnit::log(const char *_Format, ...) const {
	//Insert here possible return because of disabled logging
	va_list args;
	va_start(args, _Format);
	char debug_buffer[256];
	vsnprintf_s(debug_buffer, sizeof(debug_buffer), sizeof(debug_buffer), _Format, args);
	fwrite(reinterpret_cast<void*>(debug_buffer), sizeof(char), strlen(debug_buffer), logFile);
	if(config::COPY_LOG)
		printf(debug_buffer);
	fflush(logFile);
	//fprintf(logFile, m_debug_buffer);
	va_end(args);
}

int MemoryUnit::loadProgram(const char* path, address adrs) {
	FILE *src_file = NULL;
	unsigned int bias = 0;
	char mainBuffer[256]; //buffer for formated print
	if( (src_file = fopen(path, "r")) == NULL) {
		log("Can\'t open file with programm\n");
		return -1;
	}
	else {
		log("Loading program:\n===\n");
		while(!feof(src_file)) {
			if ( fgets (mainBuffer, sizeof(mainBuffer), src_file) == NULL ) 
				break;
			writeBytes( adrs + bias, reinterpret_cast<byte*>(mainBuffer), strlen(mainBuffer));
			bias += strlen(mainBuffer);
		}
		log("===\n");
		if( fclose (src_file) == EOF) {
			log("Can\'t close file with programm\n");
			return -2;
		}
		else
			return 0;
	}
}