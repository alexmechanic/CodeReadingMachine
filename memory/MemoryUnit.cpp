#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit(LogKeeper *a_logKeeper):   
		m_physicalMemory((byte*)malloc(config::SIZE_OF_RAM_MEMORY*sizeof(byte))), 
		m_pageUsed((byte*)malloc(config::NUMBER_OF_RAM_PAGES*sizeof(byte))),
		m_logKeeper(a_logKeeper),
		m_tlb(a_logKeeper)
{
	if(m_physicalMemory == NULL) {throw std::bad_alloc("CTOR: Can not allocate m_physicalMemory for MemoryUnit");}
	if(m_pageUsed == NULL) {throw std::bad_alloc("CTOR: Can not allocate m_pageUsed for MemoryUnit");}
	for(unsigned int i = 0; i < config::NUMBER_OF_RAM_PAGES; ++i)
		m_pageUsed[i] = 0;
	m_logKeeper->log("CTOR: Allocated %d bytes for physical memory\n", config::SIZE_OF_RAM_MEMORY);
	m_logKeeper->log("CTOR: Allocated %d bytes for physical memory used array\n", (config::NUMBER_OF_RAM_PAGES/config::BITS_IN_BYTE+1));
}

MemoryUnit::~MemoryUnit() { 
	m_logKeeper->log("===\nShuttig down:\n");
	free(m_physicalMemory); 
	m_logKeeper->log("DTOR: Phisical memory freed\n");
	free(m_pageUsed);
	m_logKeeper->log("DTOR: Memory used array freed\n");
}

int MemoryUnit::writeBytes(address dstAdrs, byte *srcPtr, size_t size) {
	m_logKeeper->log("Writing at %p (%u)\n", dstAdrs, dstAdrs);
	byte *realPtr; //adress on real machine
	dstAdrs = translate(dstAdrs);
	if(dstAdrs == config::RAM_FULL_ERROR) {
		return config::RAM_FULL_ERROR;
	}
	realPtr = convertIntoRealPhysicalMemoryPtr(dstAdrs);
	memcpy(realPtr, srcPtr, size);
	m_logKeeper->log("%d bytes were written at %p (real: %p) from %p\n\n", size, dstAdrs, realPtr, srcPtr);
	return 0;
}

int MemoryUnit::readBytes(address srcAdrs, byte *dstPtr, size_t size) {
	m_logKeeper->log("Reading at %p (%u)\n", srcAdrs, srcAdrs);
	byte *realPtr; //adress on real machine
	srcAdrs = translate(srcAdrs);
	if(srcAdrs == config::RAM_FULL_ERROR) {
		return config::RAM_FULL_ERROR;
	}
	realPtr = convertIntoRealPhysicalMemoryPtr(srcAdrs);
	memcpy(dstPtr, realPtr, size);
	m_logKeeper->log("%d bytes were read from %p (real: %p) to %p\n\n", size, srcAdrs, realPtr, dstPtr);
	return 0;
}


address MemoryUnit::translate(address adrs) {
	m_logKeeper->log("--- New translating operation ---\n");
	m_logKeeper->log("Translating %p (%u)\nLooking into cash...\n", adrs, adrs);
	address bufAdrs = m_tlb.translate(adrs, config::USER_MODE);  
	if( (bufAdrs == config::TLB_NOT_FOUND_ERROR)||(bufAdrs == config::INVALID_ENTRY_ERROR) ) {
		m_logKeeper->log("Not found or invalid entry. Placing new entry in TLB\n");
		unsigned int phpn = findEmptyPhisicalPage();
		//Insert here check for mode errors
		if(phpn == config::RAM_FULL_ERROR) {
			m_logKeeper->log("ERROR: Out of RAM\n");
			return config::RAM_FULL_ERROR; //now OS should handle this and swap some pages
		}
		m_logKeeper->log("Inserting new entry into TLB\n");
		m_tlb.insertEntry( config::extractPageNumber( adrs ), phpn );
		m_pageUsed[phpn] = 1;
		m_logKeeper->log("Retranslating\n");
		return m_tlb.translate(adrs, config::USER_MODE);
	}
	return bufAdrs;
}

byte *MemoryUnit::convertIntoRealPhysicalMemoryPtr(address adrs) const {
	m_logKeeper->log("RAM address %p translated into real address %p\n", adrs, adrs + m_physicalMemory);
	return adrs + m_physicalMemory;
}

int MemoryUnit::loadProgram(const char* path, address adrs) {
	FILE *src_file = NULL;
	unsigned int bias = 0;
	char mainBuffer[256]; //buffer for formated print
	if( (src_file = fopen(path, "r")) == NULL) {
		m_logKeeper->log("Can\'t open file with programm\n");
		return -1;
	}
	else {
		m_logKeeper->log("Loading program:\n===\n");
		while(!feof(src_file)) {
			if ( fgets (mainBuffer, sizeof(mainBuffer), src_file) == NULL ) 
				break;
			writeBytes( adrs + bias, reinterpret_cast<byte*>(mainBuffer), strlen(mainBuffer));
			bias += strlen(mainBuffer);
		}
		m_logKeeper->log("===\n");
		if( fclose (src_file) == EOF) {
			m_logKeeper->log("Can\'t close file with programm\n");
			return -2;
		}
		else
			return 0;
	}
}

unsigned int MemoryUnit::findEmptyPhisicalPage() {
	unsigned int counter1;
	for(counter1 = 0; counter1 < config::NUMBER_OF_RAM_PAGES; ++counter1)
		if( *(m_pageUsed + counter1) == 0 )
			return counter1;
	return config::RAM_FULL_ERROR; //now OS should handle this and swap some pages
}