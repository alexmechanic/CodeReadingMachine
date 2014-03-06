#ifndef MEMORY_UNIT_H
#define MEMORY_UNIT_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include "TLB.h"

typedef struct PageTableEntry {
	address location;
	bool inRAM;
	bool inSWAP;
	//bool pid; //since we have one-process system pid always 0
	//bool modified; //later
	//bool protected; //later
//something else going here
} PTE; 

typedef struct PhysicalMemoryPageEntry {
	time_t lastUsed;
	bool used;
} PMPE;

class MemoryUnit {
private:
	byte * const m_physicalMemory;
	byte * const m_swapMemory;
	PMPE * const m_PMPageLastUsed; //for effective swaping
	//m_SWAPpageDistribution: i'th element contains number of virtual page in i'th page of swap file
	int * const m_SWAPpageDistribution; 
	PTE m_pageDistribution[config::NUMBER_OF_VIRTUAL_PAGES]; //MMU table

	TLB tlb; //TODO reconsider location

	//byte *bufPtr; //buffer pointner, will be used in several functions later
	FILE *logFile;

	address translate(address ptr); //translation from virtual address into phisical
	void log(const char *_Format, ...) const;	

	void swapPage(byte* dst_page, byte* src_page);
	void exchangePages(byte* page1, byte* page2);

public:

	int findVirtualPageFromRAMPage(unsigned int RAMpageNumber) const;
	int findEmptyRAMPage() const;
	int findEmptySWAPPage() const;
	int findRAMPageForSwap() const;
	int findSWAPPageForSwap(unsigned int vpn) const;
	address findInTable(address adrs);

//public:

	MemoryUnit();
	~MemoryUnit();

	byte *convertIntoRealPhysicalMemoryPtr(address adrs) const;

	int loadProgram(const char* path, address adrs);
	int writeBytes(address dstAdrs, byte *srcPtr, size_t size);

	template <typename T>
	inline int write(address dstAdrs, T *t) {return writeBytes(dstAdrs, reinterpret_cast<byte*>(t), sizeof(T));}
	
	inline int writeByte(address dstAdrs, byte *srcByte) {return writeBytes(dstAdrs, srcByte, sizeof(byte));}
	inline int writeWord(address dstAdrs, word *srcWord) {return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcWord), sizeof(word));}
	inline int writeDword(address dstAdrs, dword *srcDword) {return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcDword), sizeof(dword));}
	inline int writeInt(address dstAdrs, int *srcInt) {return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcInt), sizeof(int));}

	template <typename T>
	int read(address srcAdrs, T *t) { 
		log("Reading at virtual address %p\n", srcAdrs);
		srcAdrs = translate(srcAdrs);
		/* // I'm not sure if we need this
		if( (srcAdrs = translate(srcAdrs)) == NULL ) {
			log("Trying to read at NULL/n");
			return -1;
			//exit(-1); //will be fixed later
		} */
		T* bufSrcPtr = reinterpret_cast<T*>(convertIntoRealPhysicalMemoryPtr(srcAdrs));
		*t = *bufSrcPtr;
		log("\n");
		return 0;
	}
	
	inline int readByte(address srcPtr, byte* dstByte) { return read(srcPtr, dstByte); }
	inline int readWord(address srcPtr, word* dstWord) { return read(srcPtr, dstWord); }
	inline int readDword(address srcPtr, dword* dstDword) { return read(srcPtr, dstDword); }
	inline int readInt(address srcPtr, int* dstInt) { return read(srcPtr, dstInt); }
	
	//TODO: написать функцию взятия дампа памяти
	//TODO: написать функцию освобождения страницы памяти
};


#endif //MEMORY_UNIT_H

