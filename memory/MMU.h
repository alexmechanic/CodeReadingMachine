
#ifndef MMUGUARD

#define MMUGUARD


/*
typedef struct PageTableEntry {
bool present;
bool pid;
bool modified;
//something else going here
} PTE; */

/*
class MMU {
private:
byte * const m_phisicalMemory; //given by MemoryUnit in ctor
unsigned int m_pageDistribution[SIZE_OF_TLB];
//Composed with hash_map, because actually it should be implemented in harware
hash_map<int, int*> TLB;
int i;
protected:
public:
MMU(byte * const a_phisicalMemory): m_phisicalMemory(a_phisicalMemory) {
for(int i = 0; i < SIZE_OF_TLB; ++i)
m_pageDistribution[i] = NULL;
}
int findEmpty() {
for(int i = 0; i < SIZE_OF_TLB; ++i)
if(m_pageDistribution[i] == NULL)
return i;
return -1;
}
};
*/


#endif //MMUGUARD