#include "MMU.h"

unsigned int extractPageNumber(byte* ptr) {
	return reinterpret_cast<unsigned int>(ptr) >> config::BIT_PAGE_BIAS; 
};

unsigned int extractPageBias(byte* ptr) {
	int buf = reinterpret_cast<unsigned int>(ptr) & (config::SIZE_OF_PAGE - 1);
	return buf;
};

int TLB::placeIntoMemory(unsigned int vpn) {
	for(counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1)
		if( m_virtualPageNumber[counter1] == -1 )
			break;
	if(counter1 == config::SIZE_OF_TLB) 
		return -1; //later
	else {
		m_virtualPageNumber[counter1] = vpn;
		for(counter2 = 0; counter2 < config::NUMBER_OF_RAM_PAGES; ++counter2)
			if( m_phisycalPageEmpty[counter2] == true)
				break;
		if(counter2 == config::NUMBER_OF_RAM_PAGES)
			return -2; //all pages are used?
		m_phisycalPageNumber[counter2] = counter1;
		m_phisycalPageEmpty[counter2] = false;
		return counter1;
	}
}


TLB::TLB() {
	for(counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1) {
		m_virtualPageNumber[counter1] = -1;
		m_phisycalPageNumber[counter1] = -1;
	}
	for(counter1 = 0; counter1 < config::NUMBER_OF_RAM_PAGES; ++counter1)
		m_phisycalPageEmpty[counter1] = true;
}

// Actually this command should be implemented in hardware and run faster but idk 
int TLB::translateVirtualPage(unsigned int vpn) {
	if ( vpn >= config::NUMBER_OF_VIRTUAL_PAGES )
		return -2; 
	for(counter1 = 0; counter1 < config::SIZE_OF_TLB; ++counter1)
		if( m_virtualPageNumber[counter1] == vpn )
			return m_phisycalPageNumber[counter1];
	return -1;
}
