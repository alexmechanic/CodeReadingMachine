#include "Config.h"

namespace config {

	const unsigned int BITS_IN_BYTE = 8; //syntax sugar
	const bool COPY_LOG = true; //copies log on console if true

	const address KERNEL_MAPPED_MASK = 0xE000000; // 1110 in first quad
	const address SUPERVISOR_MAPPED_MASK = 0xC000000; // 1100 in first quad
	const address KERNEL_UNMAPPED_UNCACHED_MASK = 0xA000000; // 1010 in first quad
	const address KERNEL_UNMAPPED_MASK = 0x8000000; // 1000 in first quad
	const address& KSEG3 = KERNEL_MAPPED_MASK;
	const address& KSSEG = SUPERVISOR_MAPPED_MASK;
	const address& KSEG1 = KERNEL_UNMAPPED_UNCACHED_MASK;
	const address& KSEG0 = KERNEL_UNMAPPED_MASK;

	const int ADDRESS_ERROR = -1;
	const int TLB_NOT_FOUND_ERROR = -2;
	const int INVALID_ENTRY_ERROR = -3;
	const int TLB_FULL_ERROR = -10;
	const int RAM_FULL_ERROR = -20;
	const int UNSUCCESSFUL_WRITING_ERROR = -30;

	const unsigned int KERNEL_MODE = 0;
	const unsigned int SUPERVISOR_MODE = 1;
	const unsigned int USER_MODE = 2;

	const unsigned int SIZE_OF_RAM_MEMORY = 16384; //size of RAM in bytes
	const unsigned int BIT_PAGE_NUMBER = 20; //number of bits allocated for page number
	const unsigned int BIT_PAGE_BIAS = BITS_IN_BYTE*sizeof(address) - BIT_PAGE_NUMBER; 
	//number of bits allocated for bias in page; 20 bpn ==> 12 bpb
	const unsigned int SIZE_OF_PAGE = static_cast<unsigned int>( 1 << BIT_PAGE_BIAS ); 
	//size of page in bytes
	const unsigned int NUMBER_OF_VIRTUAL_PAGES = static_cast<unsigned int>( 1 << BIT_PAGE_NUMBER ); 
	//maximum number of pages
	const unsigned int NUMBER_OF_RAM_PAGES = SIZE_OF_RAM_MEMORY/SIZE_OF_PAGE;
	const unsigned int SIZE_OF_TLB = 1;
	
	//Extracts from given virtual machine address virtual page number and bias in this page
	//Example with given BIT_PAGE_NUMBER = 20 ==> SIZE_OF_PAGE = 4096:
	//extractPageNumber(16383) = 3
	//extractPageBias(16383) = 4095
	unsigned int extractPageNumber(address adrs) { return adrs >> config::BIT_PAGE_BIAS;  };
	unsigned int extractPageBias(address adrs) {	return adrs & ( config::SIZE_OF_PAGE - 1); };
	bool isOddPage(address adrs) { return extractPageNumber(adrs) & 1; }
};