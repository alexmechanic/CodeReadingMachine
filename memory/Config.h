
#ifndef CONFIG

#define CONFIG

#include <math.h> 

typedef unsigned char byte;
typedef byte BYTE;
typedef unsigned short word;
typedef word WORD;
typedef unsigned int dword;
typedef dword DWORD;

namespace config {

	const unsigned int SIZE_OF_SWAP = 4096; //size of "HDD" in bytes
	const unsigned int SIZE_OF_RAM_MEMORY = 4096; //size of RAM in bytes
	const unsigned int SIZE_OF_PAGE = 1024; //in bytes
	const unsigned int BIT_PAGE_BIAS = static_cast<int>( ceil( log( static_cast<double>(SIZE_OF_PAGE) )/log(2.0) ) ); //number of bytes allocated for bias in page
	const unsigned int BIT_PAGE_NUMBER = sizeof(byte *) - BIT_PAGE_BIAS; //number of bytes allocated for page number
	const unsigned int NUMBER_OF_VIRTUAL_PAGES = static_cast<int>( pow( 2.0, static_cast<double>(BIT_PAGE_NUMBER) ) ); //maximum number of pages
	const unsigned int SIZE_OF_TLB = 8; 
	 const unsigned int NUMBER_OF_RAM_PAGES = static_cast<int>( ceil( static_cast<double>(SIZE_OF_RAM_MEMORY)/SIZE_OF_PAGE ) );
	const long long MAX_ARRANGABLE_BITS = NUMBER_OF_VIRTUAL_PAGES*SIZE_OF_PAGE;

};

#endif //CONFIG
