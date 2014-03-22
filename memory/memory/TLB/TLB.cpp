#include "TLB.h"

using config::ADDRESS_ERROR;
using config::TLB_NOT_FOUND_ERROR;
using config::INVALID_ENTRY_ERROR;
using config::TLB_FULL_ERROR;
using config::KERNEL_MODE;
using config::SUPERVISOR_MODE;
using config::USER_MODE;
using config::SIZE_OF_TLB;
using config::SIZE_OF_PAGE;


TLB::TLB(LogKeeper *a_logKeeper):m_logKeeper(a_logKeeper), 
								 m_entry( static_cast<TLBEntry*>(malloc(config::NUMBER_OF_VIRTUAL_PAGES*sizeof(TLBEntry))))
{
	unsigned int counter1;
	for(counter1 = 0; counter1 < SIZE_OF_TLB; ++counter1) {
		//Fills TLB with invalid entries
		m_entry[counter1].VPN2  = counter1*config::SIZE_OF_PAGE;
		//m_entry[counter1].G	= true;
		m_entry[counter1].V0    = false;
		m_entry[counter1].V1    = false;
	}
	m_logKeeper->log("TLB CTOR: TLB ready\n");
}

TLB::~TLB() {
	free(m_entry);
}

void TLB::insertEntry(unsigned int vpn, unsigned int phpn) {
	m_logKeeper->log("TLB: Inserting new entry\n");
	unsigned int counter1, counter2;
	counter2 = -1;
	bool isOdd = vpn & 1;
	vpn = vpn & (UINT_MAX-1); //zerofying last bit because we map one virtual page to two phisical
	for(counter1 = 0; counter1 < SIZE_OF_TLB; ++counter1) {
		if(m_entry[counter1].VPN2 == vpn)
			break;
		if( (m_entry[counter1].V0 == false)&&(m_entry[counter1].V1 == false) ) 
			counter2 = counter1; //invalid entries are pretenders for replacing
	}
	if(counter1 == SIZE_OF_TLB) {
		m_logKeeper->log("TLB: Same vpn not found, replacing invalid entry\n");
		counter1 = counter2;
		if(counter1 == -1) {
			m_logKeeper->log("TLB: TLB is full, replacing last entry\n");
			m_logKeeper->log("[[[WARNING: IT CAUSES ERRORS SINCE WE HAVE NO PAGE TABLE]]]\n");
			counter1 = SIZE_OF_TLB - 1;
			m_entry[counter1].V0 = false; //invalidating both linked pages
			m_entry[counter1].V1 = false;
		}
	} else m_logKeeper->log("TLB: Same vpn found, refilling it\n");
	m_entry[counter1].VPN2 = vpn;
	if(isOdd) {
		m_entry[counter1].PFN1 = phpn;
		m_entry[counter1].V1 = true;
		m_entry[counter1].D1 = false;
	} else {
		m_entry[counter1].PFN0 = phpn;
		m_entry[counter1].V0 = true;
		m_entry[counter1].D0 = false;
	}
}

void TLB::invalidateEntry(unsigned int vpn) {
	unsigned int counter1;
	m_logKeeper->log("TLB: Invalidating vpn %u\n", vpn);
	bool isOdd = vpn & 1;
	vpn = vpn & (UINT_MAX-1); //zerofying last bit because we map one virtual page to two phisical
	for(counter1 = 0; counter1 < SIZE_OF_TLB; ++counter1) {
		if(m_entry[counter1].VPN2 == vpn) {
			m_logKeeper->log("TLB: vpn successfully invalidated\n");
			if(isOdd) 
				m_entry[counter1].V1 = false;
			else
				m_entry[counter1].V0 = false;
			break;
		}
	}
	m_logKeeper->log("TLB: vpn to invalidate not found\n");
}


// Actually this command should be implemented in hardware and run faster but idk 
address TLB::translate(address adrs, unsigned int mode) {
	//TODO: add instruction check
	//TODO: add mode check
	unsigned int vpn = config::extractPageNumber(adrs);
	bool isOdd = config::isOddPage(adrs);
	vpn = vpn & (UINT_MAX-1); //zerofying last bit because we map one virtual page to two phisical
	if (adrs&config::KERNEL_MAPPED_MASK) {
		m_logKeeper->log("TLB: Kernel mapped segment hit\n");
		if(mode == KERNEL_MODE) {
			// ...
		} else {
			m_logKeeper->log("Adress error\n");
			return ADDRESS_ERROR;
		}
	} else if (adrs&config::SUPERVISOR_MAPPED_MASK) {
		m_logKeeper->log("TLB: Supervisor mapped segment hit\n");
		if(mode == SUPERVISOR_MODE) {
			// ...
		} else {
			m_logKeeper->log("TLB: Adress error\n");
			return ADDRESS_ERROR;
		}
	} else if (adrs&config::KERNEL_UNMAPPED_UNCACHED_MASK) { 
		m_logKeeper->log("TLB: Kernel unmapped uncached segment hit\n");
		return ADDRESS_ERROR; //shoud not be acessed via TLB
	} else if (adrs&config::KERNEL_UNMAPPED_MASK) {
		m_logKeeper->log("TLB: Kernel unmapped segment hit\n");
		return config::ADDRESS_ERROR; //shoud not be acessed via TLB either
	} else {
		m_logKeeper->log("TLB: User segment hit\n");
		unsigned int counter1;
		for(counter1 = 0; counter1 < SIZE_OF_TLB; ++counter1) {
			if( m_entry[counter1].VPN2 == vpn ) {
				//Here should be ASID check, but we have only one process now
				if( isOdd ) {
					if( !(m_entry[counter1].V1) ) {
						m_logKeeper->log("TLB: Invalid entry hit\n");
						return INVALID_ENTRY_ERROR;
					}
					m_logKeeper->log("TLB: Successful odd hit\n");
					return SIZE_OF_PAGE*m_entry[counter1].PFN0 + config::extractPageBias(adrs);
				} else {
					if( !(m_entry[counter1].V0) ) {
						m_logKeeper->log("TLB: Invalid entry hit\n");
						return INVALID_ENTRY_ERROR;
					}
					m_logKeeper->log("TLB: Successful even hit\n");
					return SIZE_OF_PAGE*m_entry[counter1].PFN0 + config::extractPageBias(adrs);
				}
			}
		}
	}
	m_logKeeper->log("TLB: TLB miss\n");
	return TLB_NOT_FOUND_ERROR;
}
