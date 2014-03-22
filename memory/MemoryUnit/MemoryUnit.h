#ifndef MEMORY_UNIT_H
#define MEMORY_UNIT_H


#include "TLB.h"

class MemoryUnit {
private:
	byte * m_physicalMemory;
	byte * m_pageUsed;

	TLB m_tlb; //TODO: reconsider location, it should be in Stas' part
	LogKeeper *m_logKeeper;

	address translate(address ptr); //translation from virtual address into phisical

public:

	MemoryUnit(LogKeeper *a_logKeeper);
	~MemoryUnit();

	byte *convertIntoRealPhysicalMemoryPtr(address adrs) const;

	int loadProgram(const char* path, address adrs);

	int writeBytes(address dstAdrs, byte *srcPtr, size_t size);
	// Facade function for any type of data...
	template <typename T>
	inline int write(address dstAdrs, T *t) 
		{return writeBytes(dstAdrs, reinterpret_cast<byte*>(t), sizeof(T));}
	// ...and several well-known
	inline int writeByte(address dstAdrs, byte *srcByte)
		{return writeBytes(dstAdrs, srcByte, sizeof(byte));}
	inline int writeWord(address dstAdrs, word *srcWord) 
		{return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcWord), sizeof(word));}
	inline int writeDword(address dstAdrs, dword *srcDword) 
		{return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcDword), sizeof(dword));}
	inline int writeInt(address dstAdrs, int *srcInt)
		{return writeBytes(dstAdrs, reinterpret_cast<byte*>(srcInt), sizeof(int));}

	int readBytes(address srcAdrs, byte *dstPtr, size_t size);
	// Facade function for any type of data...
	template <typename T>
	inline int read(address srcAdrs, T *t) { 
		return readBytes(srcAdrs, reinterpret_cast<byte*>(t), sizeof(T));}
	// ...and several well-known
	inline int readByte(address srcPtr, byte* dstByte) { return read(srcPtr, dstByte); }
	inline int readWord(address srcPtr, word* dstWord) { return read(srcPtr, dstWord); }
	inline int readDword(address srcPtr, dword* dstDword) { return read(srcPtr, dstDword); }
	inline int readInt(address srcPtr, int* dstInt) { return read(srcPtr, dstInt); }
	
	unsigned int findEmptyPhisicalPage();
	inline void invalidatePhisicalPage(unsigned int phpn) { m_pageUsed[phpn] = 0; }

	//TODO: написать функцию взятия дампа памяти
};


#endif //MEMORY_UNIT_H

