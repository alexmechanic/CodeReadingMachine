
#ifndef CONFIG_H

#define CONFIG_H

#include <cmath> 
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned int address;

namespace config {

	extern const unsigned int BITS_IN_BYTE; //syntax sugar
	extern const bool COPY_LOG; //copies log on console if true

	extern const address KERNEL_MAPPED_MASK; // 1110 in first quad
	extern const address SUPERVISOR_MAPPED_MASK; // 1100 in first quad
	extern const address KERNEL_UNMAPPED_UNCACHED_MASK; // 1010 in first quad
	extern const address KERNEL_UNMAPPED_MASK; // 1000 in first quad
	extern const address& KSEG3;
	extern const address& KSSEG;
	extern const address& KSEG1;
	extern const address& KSEG0;

	extern const int ADDRESS_ERROR;
	extern const int TLB_NOT_FOUND_ERROR;
	extern const int INVALID_ENTRY_ERROR;
	extern const int TLB_FULL_ERROR;
	extern const int RAM_FULL_ERROR;
	extern const int UNSUCCESSFUL_WRITING_ERROR;

	extern const unsigned int KERNEL_MODE;
	extern const unsigned int SUPERVISOR_MODE;
	extern const unsigned int USER_MODE;

	extern const unsigned int SIZE_OF_RAM_MEMORY; //size of RAM in bytes
	extern const unsigned int BIT_PAGE_NUMBER; //number of bits allocated for page number
	extern const unsigned int BIT_PAGE_BIAS; 
	//number of bits allocated for bias in page; 20 bpn ==> 12 bpb
	extern const unsigned int SIZE_OF_PAGE; 
	//size of page in bytes
	extern const unsigned int NUMBER_OF_VIRTUAL_PAGES; 
	//maximum number of pages
	extern const unsigned int NUMBER_OF_RAM_PAGES;
	extern const unsigned int SIZE_OF_TLB;
	
	//Extracts from given virtual machine address virtual page number and bias in this page
	//Example with given BIT_PAGE_NUMBER = 20 ==> SIZE_OF_PAGE = 4096:
	//extractPageNumber(16383) = 3
	//extractPageBias(16383) = 4095
	unsigned int extractPageNumber(address adrs);
	unsigned int extractPageBias(address adrs);
	bool isOddPage(address adrs);
};

#endif //CONFIG_H
