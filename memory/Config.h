
#ifndef CONFIG

#define CONFIG

#include <math.h> 

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned int address;

namespace config {

	const unsigned int BITS_IN_BYTE = 8; //syntax sugar
	const bool COPY_LOG = true; //copies log on console if true

	const unsigned int SIZE_OF_SWAP_FILE = 16384; //size of swap file in bytes
	const unsigned int SIZE_OF_RAM_MEMORY = 16384; //size of RAM in bytes
	const unsigned int BIT_PAGE_NUMBER = 20; //number of bits allocated for page number
	const unsigned int BIT_PAGE_BIAS = BITS_IN_BYTE*sizeof(address) - BIT_PAGE_NUMBER; //number of bits allocated for bias in page
	const unsigned int NUMBER_OF_VIRTUAL_PAGES = static_cast<int>( 1 << BIT_PAGE_NUMBER ); //maximum number of pages
	const unsigned int SIZE_OF_PAGE = static_cast<unsigned int>( 1 << BIT_PAGE_BIAS ); //size of page in bytes
	const unsigned int SIZE_OF_TLB = 8; 
	const unsigned int NUMBER_OF_RAM_PAGES = SIZE_OF_RAM_MEMORY/SIZE_OF_PAGE;

};

#endif //CONFIG
