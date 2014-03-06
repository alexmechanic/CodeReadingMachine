#include "MemoryUnit.h"
#include <stdexcept>

MemoryUnit::MemoryUnit():   m_physicalMemory((byte*)malloc(config::SIZE_OF_RAM_MEMORY*sizeof(byte))), 
							m_swapMemory((byte*)malloc(config::SIZE_OF_SWAP_FILE*sizeof(byte))),
							m_PMPageLastUsed((PMPE*)malloc(config::NUMBER_OF_RAM_PAGES*sizeof(PMPE))),
							m_SWAPpageDistribution((int*)malloc(config::NUMBER_OF_SWAP_PAGES*sizeof(int))),
							tlb()
{
	logFile = fopen("log.txt", "w");
	if(logFile == NULL) { printf("CTOR: WARNING: Can\'t create file for log\n"); }
	if(m_physicalMemory == NULL) {throw std::bad_alloc("Can not allocate m_physicalMemory for MemoryUnit");}
	if(m_swapMemory == NULL) {throw std::bad_alloc("Can not allocate m_swapMemory for MemoryUnit");}
	if(m_PMPageLastUsed == NULL) {throw std::bad_alloc("Can not allocate m_PMPageLastUsed for MemoryUnit");}
	if(m_SWAPpageDistribution == NULL) {throw std::bad_alloc("Can not allocate m_SWAPpageDistribution for MemoryUnit");}
	int i;
	for(i = 0; i < config::NUMBER_OF_VIRTUAL_PAGES; ++i) {
		m_pageDistribution[i].location = 0;
		m_pageDistribution[i].inRAM = false;
		m_pageDistribution[i].inSWAP = false;
	}
	for(i = 0; i < config::NUMBER_OF_RAM_PAGES; ++i) {
		m_PMPageLastUsed[i].lastUsed = 0;
		m_PMPageLastUsed[i].used = false;
	}
	for(i = 0; i < config::NUMBER_OF_SWAP_PAGES; ++i)
		m_SWAPpageDistribution[i] = -1;
	log("CTOR: Allocated %d bytes for physical memory\n", config::SIZE_OF_RAM_MEMORY);
	log("CTOR: Allocated %d bytes for swap\n", config::SIZE_OF_SWAP_FILE);
}

MemoryUnit::~MemoryUnit() { 
	log("===\nShuttig down:\n");
	free(m_physicalMemory); 
	log("DTOR: Phisical memory freed\n");
	free(m_swapMemory);
	log("DTOR: Swap memory freed\n");

	log("DTOR: Ready to detach log file...\n");
	fclose(logFile);
}

address MemoryUnit::findInTable(address adrs) {
	int RAMpageNumber, SWAPpageNumber, oldvpn, newvpn; 
	// newvpn - number of virtual page going to be loaded to RAM
	// oldvpn - number of virtual page going to be swapped
	// RAMpageNumber - number of RAM page containg virtual page #oldvpn
	// SWAPpageNumber - number of SWAP page containg virtual page #newvpn
	newvpn = config::extractPageNumber(adrs);
	if(m_pageDistribution[newvpn].inRAM == true) { // page is already in RAM memory 
		log("MMU ==> Page was alredy in RAM\n");
		RAMpageNumber = config::extractPageNumber(m_pageDistribution[newvpn].location);
	}
	else if (m_pageDistribution[newvpn].inSWAP == true) { // page in swap file
		log("MMU ==> Page was in swap-file\n");
		SWAPpageNumber = findSWAPPageForSwap(newvpn); //extracting virtual page 
		//number and looking for it in swap table for exchange
		if(SWAPpageNumber == -1) {
			log("ERROR: Can not find page in swap file\n"); //error: virtual page said to be in swap-file not there
			exit(413);								 //should not happen actually
		}
		RAMpageNumber = findEmptyRAMPage();
		if(RAMpageNumber == -1) { //there is no free space in RAM
			RAMpageNumber = findRAMPageForSwap(); //looking for least used page in RAM
			oldvpn = findVirtualPageFromRAMPage(RAMpageNumber);

			exchangePages(m_physicalMemory + RAMpageNumber*config::SIZE_OF_PAGE, 
						  m_swapMemory	   + SWAPpageNumber*config::SIZE_OF_PAGE); //physically swapping content
			//Old page
			m_SWAPpageDistribution[SWAPpageNumber] = oldvpn; 
			m_pageDistribution[oldvpn].inRAM = false;
			m_pageDistribution[oldvpn].inSWAP = true;
			m_pageDistribution[oldvpn].location = SWAPpageNumber*config::SIZE_OF_PAGE;
		} else { //found free page in RAM
			swapPage(m_physicalMemory + RAMpageNumber*config::SIZE_OF_PAGE,
					 m_swapMemory	  + SWAPpageNumber*config::SIZE_OF_PAGE);
		}
		m_PMPageLastUsed[RAMpageNumber].lastUsed = time(NULL); //renewing timestamp
		m_pageDistribution[newvpn].inRAM = true;
		m_pageDistribution[newvpn].inSWAP = false;
		m_pageDistribution[newvpn].location = RAMpageNumber*config::SIZE_OF_PAGE;
	} else { // page have been cleared or not used at all, now we should (re)allocate it
		log("MMU ==> Page was clear\n");

		
		/*printf("!!!!!!!!!!\n");
		for(int k = 0; k < config::NUMBER_OF_RAM_PAGES; ++k) 
			printf("%16d %d\n", m_PMPageLastUsed[k].lastUsed, m_PMPageLastUsed[k].used);
		printf("!!!!!!!!!!\n"); */

		RAMpageNumber = findEmptyRAMPage();
		if( RAMpageNumber == -1) { //no free space in RAM
			log("MMU ==> No free space in RAM, trying to swap least used page\n");
			if( (SWAPpageNumber = findEmptySWAPPage()) == -1 ) {
				log("There is no place for new page\n"); //error: there is no physical
				exit(612);								 //memory for new virtual page
			} //TODO consider remove error
			RAMpageNumber = findRAMPageForSwap(); //stashing least used in swap file
			swapPage(m_swapMemory     + SWAPpageNumber*config::SIZE_OF_PAGE, 
					 m_physicalMemory + RAMpageNumber*config::SIZE_OF_PAGE);
			int oldvpn = findVirtualPageFromRAMPage(RAMpageNumber);
			m_SWAPpageDistribution[SWAPpageNumber] = oldvpn; 
			m_pageDistribution[oldvpn].inRAM = false;
			m_pageDistribution[oldvpn].inSWAP = true;
			m_pageDistribution[oldvpn].location = SWAPpageNumber*config::SIZE_OF_PAGE;
			log("MMU ==> Page #%u have been swapped to #ui page of swap\n", RAMpageNumber, SWAPpageNumber);
		}  
		m_PMPageLastUsed[RAMpageNumber].used = true; //marking this page as used
		m_PMPageLastUsed[RAMpageNumber].lastUsed = time(NULL);//renewing timestamp
		m_pageDistribution[newvpn].inRAM = true;
		m_pageDistribution[newvpn].inSWAP = false;
		m_pageDistribution[newvpn].location = RAMpageNumber*config::SIZE_OF_PAGE;
		log("MMU ==> Page #%u have been reloaded\n", RAMpageNumber);		
	}

	log("MMU ==> Puting (vpn: %u, phpn: %u) into TLB\n", newvpn, RAMpageNumber);
	tlb.insertEntry(newvpn, RAMpageNumber);
	return m_pageDistribution[newvpn].location + config::extractPageBias(adrs);
}

int MemoryUnit::findVirtualPageFromRAMPage(unsigned int RAMpageNumber) const {
	for(int i = 0; i < config::NUMBER_OF_VIRTUAL_PAGES; ++i) 
		if( (m_pageDistribution[i].inRAM == true )&&
			(m_pageDistribution[i].location == RAMpageNumber*config::SIZE_OF_PAGE) )
				return i;
	return -1;
}
int MemoryUnit::findEmptyRAMPage() const {
	for(int i = 0; i < config::NUMBER_OF_RAM_PAGES; ++i) 
		if( m_PMPageLastUsed[i].used == false )
			return i;
	return -1;
}
int MemoryUnit::findEmptySWAPPage() const {
	for(int i = 0; i < config::NUMBER_OF_SWAP_PAGES; ++i)
		if(m_SWAPpageDistribution[i] == -1)
			return i;
	return -1;
}
int MemoryUnit::findRAMPageForSwap() const {
	int j = findEmptyRAMPage(); //is there a free physical page
	if( j == -1 ) { //no free page, choosing page for swapping
		j = 0;
		for(int i = 1; i < config::NUMBER_OF_RAM_PAGES; ++i) 
			if(m_PMPageLastUsed[i].lastUsed < m_PMPageLastUsed[j].lastUsed)
				j = i;
	}
	return j;
}
int MemoryUnit::findSWAPPageForSwap(unsigned int vpn) const {
	unsigned int i;
	for(i = 0; i < config::NUMBER_OF_SWAP_PAGES; ++i)
		if( m_SWAPpageDistribution[i] == vpn )
			return i;
	return -1; //that actually should not happen
}

void MemoryUnit::swapPage(byte* dst_page, byte* src_page) {
	for(int i = 0; i < config::SIZE_OF_PAGE; ++i)
		 *(dst_page + i) = *(src_page + i);
}

void MemoryUnit::exchangePages(byte* page1, byte* page2) {
	byte bufbyte;
	for(int i = 0; i < config::SIZE_OF_PAGE; ++i) {
		bufbyte = *(page1 + i);
		*(page1 + i) = *(page2 + i);
		*(page2 + i) = bufbyte;
	}
}

int MemoryUnit::writeBytes(address dstAdrs, byte *srcPtr, size_t size) {
	byte *realPtr; //adress on real machine
	dstAdrs = translate(dstAdrs);
	log("Writing from real address %p to virtual address %p\n", srcPtr, dstAdrs);
	/* // I'm not sure if we need this
	if( (dstAdrs = translate(dstAdrs)) == NULL ) {
		log("Trying to write at NULL/n");
		return -1;
		//exit(-1); //will be fixed later
	} */
	realPtr = convertIntoRealPhysicalMemoryPtr(dstAdrs);
	memcpy(realPtr, srcPtr, size);
	log("%d bytes were written at %p (real: %p) from %p\n\n", size, dstAdrs, realPtr, srcPtr);
	return 0;
}


address MemoryUnit::translate(address adrs) {
	log("--- New translating operation ---\n");
	log("Translating %p (%u)\nLooking into cash...\n", adrs, adrs);
	unsigned int vpn = config::extractPageNumber(adrs);
	int bufRAMPage = tlb.translateVirtualPage(vpn);  
	if( bufRAMPage == -1) { //not found
		log("   ...not found. Looking in PageTable\n");
		adrs = findInTable(adrs);
		bufRAMPage = config::extractPageNumber(adrs); // TODO consider replacing with recursive translation() call
	} else
		log("   ...found TLB entry with phisical memory page number %d.\n", bufRAMPage);
	log("Translated to RAM address %p\n", bufRAMPage*config::SIZE_OF_PAGE + 
										  config::extractPageBias(adrs));
	log("--- --- ---\n");
	m_PMPageLastUsed[bufRAMPage].lastUsed = time(NULL);
	return bufRAMPage*config::SIZE_OF_PAGE + config::extractPageBias(adrs);
}

byte *MemoryUnit::convertIntoRealPhysicalMemoryPtr(address adrs) const {
	log("RAM address %p translated into real address %p\n", adrs, adrs + m_physicalMemory);
	return adrs + m_physicalMemory;
}

void MemoryUnit::log(const char *_Format, ...) const {
	//Insert here possible return because of disabled logging
	va_list args;
	va_start(args, _Format);
	char debug_buffer[256];
	vsnprintf_s(debug_buffer, sizeof(debug_buffer), sizeof(debug_buffer), _Format, args);
	fwrite(reinterpret_cast<void*>(debug_buffer), sizeof(char), strlen(debug_buffer), logFile);
	if(config::COPY_LOG)
		printf(debug_buffer);
	fflush(logFile);
	//fprintf(logFile, m_debug_buffer);
	va_end(args);
}

int MemoryUnit::loadProgram(const char* path, address adrs) {
	FILE *src_file = NULL;
	unsigned int bias = 0;
	char mainBuffer[256]; //buffer for formated print
	if( (src_file = fopen(path, "r")) == NULL) {
		log("Can\'t open file with programm\n");
		return -1;
	}
	else {
		log("Loading program:\n===\n");
		while(!feof(src_file)) {
			if ( fgets (mainBuffer, sizeof(mainBuffer), src_file) == NULL ) 
				break;
			writeBytes( adrs + bias, reinterpret_cast<byte*>(mainBuffer), strlen(mainBuffer));
			bias += strlen(mainBuffer);
		}
		log("===\n");
		if( fclose (src_file) == EOF) {
			log("Can\'t close file with programm\n");
			return -2;
		}
		else
			return 0;
	}
}