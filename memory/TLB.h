#ifndef TLB_H

#define TLB_H

#include "Config.h"

class TLB {
private:
	// Insert here m_PID array when we'll have more processes
	int m_virtualPageNumber[config::SIZE_OF_TLB];
	int m_phisycalPageNumber[config::SIZE_OF_TLB];
	bool m_isEmptyEntry[config::SIZE_OF_TLB]; //place into MMU
	time_t m_lastHit[config::SIZE_OF_TLB]; //the one which was used first will be changed in next TLB miss
	
protected:
public:
	TLB();
	// Actually this command should be implemented in hardware and run faster but idk 
	int translateVirtualPage(unsigned int vpn);
	void insertEntry(unsigned int vpn, unsigned int phpn);
};



#endif //TLB_H