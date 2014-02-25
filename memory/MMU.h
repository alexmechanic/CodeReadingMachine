#ifndef MMUGUARD

#define MMUGUARD

#include <hash_map>
#include "Config.h"

unsigned int extractPageNumber(byte* ptr);
unsigned int extractPageBias(byte* ptr);

class TLB {
private:
	// Insert here m_PID array when we'll have more processes
	int m_virtualPageNumber[config::SIZE_OF_TLB];
	int m_phisycalPageNumber[config::SIZE_OF_TLB];
	bool m_phisycalPageEmpty[config::SIZE_OF_TLB]; //place into MMU
	unsigned int counter1, counter2;
protected:
public:
	TLB();
	// Actually this command should be implemented in hardware and run faster but idk 
	int translateVirtualPage(unsigned int vpn);
	int placeIntoMemory(unsigned int vpn);
};

/*
typedef struct PageTableEntry {
	bool present;
	bool pid;
	bool modified;
	//something else going here
} PTE; */

/*
class MMU {
private:
	byte * const m_phisicalMemory; //given by MemoryUnit in ctor
	unsigned int m_pageDistribution[SIZE_OF_TLB];
	//Composed with hash_map, because actually it should be implemented in harware
	hash_map<int, int*> TLB;
	int i;
protected:
public:
	MMU(byte * const a_phisicalMemory): m_phisicalMemory(a_phisicalMemory) {
		for(int i = 0; i < SIZE_OF_TLB; ++i)
			m_pageDistribution[i] = NULL;
	}
	
	int findEmpty() {
		for(int i = 0; i < SIZE_OF_TLB; ++i)
			if(m_pageDistribution[i] == NULL)
				return i;
		return -1;
	}
};
*/

#endif //MMUGUARD