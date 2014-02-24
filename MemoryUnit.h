#ifndef MEMORY_UNIT
#define MEMORY_UNIT

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include "MMU.h"

class MemoryUnit {
private:
	const size_t m_size;
	byte * const m_phisicalMemory;
	byte * const m_swapMemory;
	mutable TLB tlb;

	byte *buf_ptr; //buffer pointner, used in several functions
	mutable std::string m_debug_log; //log
	mutable char m_debug_buffer[128]; //buffer for formated print into log 
	mutable char m_main_buffer[128]; //common buffer for formated print

	byte* translate(byte* ptr) const; //транслировать из виртуального адреса в физический; 
	// ѕока что на этом месте заглушка, возвращающа€ сам ptr
	void log(const char *_Format, ...) const;	

public:
	MemoryUnit();
	~MemoryUnit();

	void loadProgram(const char* path);
	int writeBytes(byte *dst_ptr, byte *src_ptr, size_t size);
	int writeCharString(byte *dst_ptr, const char *src_ptr);

	template <typename T>
	inline int write(byte *dst_ptr, T& t) {return writeBytes(dst_ptr, reinterpret_cast<byte*>(&t), sizeof(T));}

	template <typename T>
	inline int read(byte *src_ptr, T& t) const { //TODO сделать не inline (?)
		T* buf_src_ptr = reinterpret_cast<T*>(translate(src_ptr));
		try {
			log("Reading at %p\n", buf_src_ptr);
			t = *buf_src_ptr;
			return 0;
		} catch (std::exception &e){
			log("Exception during reading at %p:\n", buf_src_ptr);
			log(e.what());
			return -1;
		} catch (...) {
			log("Error during reading at %p:\n", buf_src_ptr);
			return -1;
		}
	}
	inline int readByte(byte* src_ptr, byte& dst_byte) const { return read(src_ptr, dst_byte); }
	inline int readWord(byte* src_ptr, word& dst_word) const { return read(src_ptr, dst_word); }
	inline int readDword(byte* src_ptr, dword& dst_dword) const { return read(src_ptr, dst_dword); }
	inline int readInt(byte* src_ptr, int& dst_int) const { return read(src_ptr, dst_int); }
	
	byte* debug_getMemory() {return m_phisicalMemory;} //только дл€ дебага
	std::string& debug_getLog() {return m_debug_log;} //дебаг, хот€ можно оставить и на потом
	//TODO: написать функцию дампа пам€ти
};


#endif //MEMORY_UNIT

