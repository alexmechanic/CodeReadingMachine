#ifndef MEMORY_UNIT
#define MEMORY_UNIT

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include "TLB.h"

class MemoryUnit {
private:
	byte * const m_phisicalMemory;
	byte * const m_swapMemory;
	TLB tlb; //later <------------

	byte *bufPtr; //buffer pointner, will be used in several functions later
	FILE *logFile;

	address translate(address ptr); //translation from virtual address into phisical
	void log(const char *_Format, ...) const;	

public:
	MemoryUnit();
	~MemoryUnit();

	byte *convertIntoRealPtr(address adrs) const;

	int loadProgram(const char* path, address adrs);
	int writeBytes(address dstPtr, byte *srcPtr, size_t size);

	template <typename T>
	inline int write(address dstPtr, T *t) {return writeBytes(dstPtr, reinterpret_cast<byte*>(t), sizeof(T));}
	
	inline int writeByte(address dstPtr, byte *srcByte) {return writeBytes(dstPtr, srcByte, sizeof(byte));}
	inline int writeWord(address dstPtr, word *srcWord) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcWord), sizeof(word));}
	inline int writeDword(address dstPtr, dword *srcDword) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcDword), sizeof(dword));}
	inline int writeInt(address dstPtr, int *srcInt) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcInt), sizeof(int));}

	template <typename T>
	int read(address srcAdrs, T *t) { 
		log("Reading at virtual address %p\n", srcAdrs);
		if( (srcAdrs = translate(srcAdrs)) == NULL ) {
			log("Trying to read at NULL/n");
			return -1;
			//exit(-1); //will be fixed later
		}
		T* bufSrcPtr = reinterpret_cast<T*>(convertIntoRealPtr(srcAdrs));
		*t = *bufSrcPtr;
		return 0;
	}
	
	inline int readByte(address srcPtr, byte* dstByte) { return read(srcPtr, dstByte); }
	inline int readWord(address srcPtr, word* dstWord) { return read(srcPtr, dstWord); }
	inline int readDword(address srcPtr, dword* dstDword) { return read(srcPtr, dstDword); }
	inline int readInt(address srcPtr, int* dstInt) { return read(srcPtr, dstInt); }
	
	//TODO: написать функцию взятия дампа памяти
};


#endif //MEMORY_UNIT

