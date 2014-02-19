#ifndef MEMORY_UNIT
#define MEMORY_UNIT

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h>

typedef unsigned char byte;
typedef byte BYTE;
typedef unsigned short word;
typedef word WORD;
typedef unsigned int dword;
typedef dword DWORD;

class MemoryUnit {
private:
	byte* m_phisicalMemory;
	size_t m_size;
	byte* translate(byte* ptr) const; //транслировать из виртуального адреса в физический; 
	// Пока что на этом месте заглушка, возвращающая сам ptr
	byte *buf_ptr; //буферный указатель, скорее всего понадобится в дальнейшем, сейчас используется только в loadProgram
	mutable std::string m_debug_log; //лог
	mutable char m_debug_bufer[128]; //буфер для печати в лог, т.к. нельзя (?) форматированно писать сразу в string
	mutable char m_main_bufer[128]; //буфер для всего остального, чтобы log работал нормально
	void log(const char *_Format, ...) const;

public:
	MemoryUnit(size_t size);
	~MemoryUnit();

	void loadProgram(const char* path);
	void writeBytes(byte *dst_ptr, byte *src_ptr, size_t size);
	void writeCharString(byte *dst_ptr, const char *src_ptr);

	template <typename T>
	inline void write(byte *dst_ptr, T& t) {writeBytes(dst_ptr, reinterpret_cast<byte*>(&t), sizeof(T));}

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
	
	byte* debug_getMemory() {return m_phisicalMemory;} //только для дебага
	std::string& debug_getLog() {return m_debug_log;} //дебаг, хотя можно оставить и на потом
};


#endif //MEMORY_UNIT

