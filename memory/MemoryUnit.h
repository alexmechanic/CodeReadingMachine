#ifndef MEMORY_UNIT
#define MEMORY_UNIT

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include "MMU.h"

class MemoryUnit {
private:
	byte * const m_phisicalMemory;
	byte * const m_swapMemory;
	mutable TLB tlb; //R

	byte *bufPtr; //buffer pointner, used in several functions
	//mutable std::string m_debug_log; //log
	FILE *logFile;
	mutable char m_debug_buffer[128]; //buffer for formated print into log file
	mutable char m_mainBuffer[128]; //common buffer for formated print

	byte* translate(byte* ptr) const; //translation from virtual address into phisical
	void log(const char *_Format, ...) const;	

public:
	MemoryUnit();
	~MemoryUnit();

	void loadProgram(const char* path);
	int writeBytes(byte *dstPtr, byte *srcPtr, size_t size);
	int writeCharString(byte *dstPtr, const char *srcPtr);

	template <typename T>
	inline int write(byte *dstPtr, T *t) {return writeBytes(dstPtr, reinterpret_cast<byte*>(t), sizeof(T));}
	inline int writeByte(byte *dstPtr, byte *srcByte) {return writeBytes(dstPtr, srcByte, sizeof(byte));}
	inline int writeWord(byte *dstPtr, word *srcWord) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcWord), sizeof(word));}
	inline int writeDword(byte *dstPtr, dword *srcDword) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcDword), sizeof(dword));}
	inline int writeInt(byte *dstPtr, int *srcInt) {return writeBytes(dstPtr, reinterpret_cast<byte*>(srcInt), sizeof(int));}

	template <typename T>
	inline int read(byte *srcPtr, T *t) const { //TODO сделать не inline (?)
		log("Reading at virtual address %p\n", srcPtr);
		T* bufSrcPtr = reinterpret_cast<T*>(translate(srcPtr));
		if (bufSrcPtr != NULL) {
			*t = *bufSrcPtr;
			return 0;
		} else {
			log("Error at reading\n");
			return -1;
		}
	}
	inline int readByte(byte* srcPtr, byte* dstByte) const { return read(srcPtr, dstByte); }
	inline int readWord(byte* srcPtr, word* dstWord) const { return read(srcPtr, dstWord); }
	inline int readDword(byte* srcPtr, dword* dstDword) const { return read(srcPtr, dstDword); }
	inline int readInt(byte* srcPtr, int* dstInt) const { return read(srcPtr, dstInt); }
	
	//byte* debug_getMemory() {return m_phisicalMemory;} //только для дебага
	//std::string& debug_getLog() {return m_debug_log;} //дебаг, хотя можно оставить и на потом
	//TODO: написать функцию дампа памяти
};


#endif //MEMORY_UNIT

