#ifndef TLB_H

#define TLB_H

#include "Config.h"
#include "LogKeeper.h"

// TLB m_entry links one virtual page to a pair of physical frames.
typedef struct TLBEntry {
	// === LOGICAL SECTION ===
	//bool  R;		//no descripton found - ??? purpose
	dword VPN2;		//virtual page number
	// Page is translated when [(ASID==pid)||G ]&&(VPN2&&mask == vpn)
	// But we have only one global process so they are not used
	//bool  G;		//global bit (not used)
	//byte  ASID;		//process id bit (not used)
	//NOTE: since we have only one process all entries will be global
	// === EVEN PAGE === 
	dword PFN0;		//physical frame number
	//bool  C0;		//cache coherency bit (not used since we have no RAM cache)
	bool  D0;		//dirty bit
	bool  V0;		//valid bit
	// === ODD PAGE === 
	dword PFN1;		//physical frame number
	//bool  C1;		//cache coherency bit (not used since we have no RAM cache)
	bool  D1;		//dirty bit
	bool  V1;		//valid bit
} TLBEntry;

class TLB {
private:
	TLBEntry *m_entry;
	LogKeeper *m_logKeeper;
protected:
public:
	TLB(LogKeeper *a_logKeeper);
	~TLB();
	// Actually this command should be implemented in hardware and run faster but idk 
	address translate(address adrs, unsigned int mode);
	void insertEntry(unsigned int vpn, unsigned int phpn);
	void invalidateEntry(unsigned int vpn);
};



#endif //TLB_H