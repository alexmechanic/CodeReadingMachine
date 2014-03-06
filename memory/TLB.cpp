#include "TLB.h"


void TLB::insertEntry(unsigned int vpn, unsigned int phpn) {
	unsigned int counter1, counter2;
	counter2 = 0;
	for(counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1) {
		if( m_isEmptyEntry[counter1] == true )
			break;
		if( m_lastHit[counter1] < m_lastHit[counter2] )
			counter2 = counter1;
	}
	if(counter1 == config::SIZE_OF_TLB) 
		counter1 = counter2; //TLB is full, replacing least used entry
	m_virtualPageNumber[counter1] = vpn;
	m_phisycalPageNumber[counter1] = phpn;
	m_isEmptyEntry[counter1] = false; 
	m_lastHit[counter1] = time(NULL);
}

TLB::TLB() {
	unsigned int counter1;
	for(counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1) {
		m_virtualPageNumber[counter1] = -1;
		m_phisycalPageNumber[counter1] = -1;
		m_isEmptyEntry[counter1] = true;
		m_lastHit[counter1] = time(NULL);
	}
}

// Actually this command should be implemented in hardware and run faster but idk 
int TLB::translateVirtualPage(unsigned int vpn) {
	if ( vpn >= config::NUMBER_OF_VIRTUAL_PAGES )
		return -2; 
	for(int counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1)
		if( m_virtualPageNumber[counter1] == vpn )
			return m_phisycalPageNumber[counter1];
	return -1;
}
