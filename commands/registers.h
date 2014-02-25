//32 основных регистра + 2 HI & LO регистры для 64битных операций

//всегда хранит 0
#define $zero r0
//временный регистр для языка ассемблера
#define $$at r1
//значения функций и выражений
#define $v0 r2
#define $v1 r3
//аргументы функций
#define $a0 r4
#define $a1 r5
#define $a2 r6
#define $a3 r7
// временные
#define $t0 r8
#define $t1 r9
#define $t2 r10
#define $t3 r11
#define $t4 r12
#define $t5 r13
#define $t6 r14
#define $t7 r15
//сохраненные временные значения
#define $s0 r16
#define $s1 r17
#define $s2 r18
#define $s3 r19
#define $s4 r20
#define $s5 r21
#define $s6 r22
#define $s7 r23
// временные
#define $t8 r24
#define $t9 r25
// зарезервирована для ядра операционной системы
#define $k0 r26
#define $k1 r27
// глобальный указатель
#define $gp r28
// указатель стека
#define $sp r29
// указатель фрейма
#define $fp r30 
// возвращает адрес
#define $ra r31

//всегда хранит 0
const uint32_t  r0 = 0;
//временный регистр для языка ассемблера
const uint32_t  r1 = 0;
//значения функций и выражений
uint32_t  r2 = 0;
uint32_t  r3 = 0;
//аргументы функций
uint32_t  r4 = 0;
uint32_t  r5 = 0;
uint32_t  r6 = 0;
uint32_t  r7 = 0;
// временные
uint32_t  r8 = 0;
uint32_t  r9 = 0;
uint32_t  r10 = 0;
uint32_t  r11 = 0;
uint32_t  r12 = 0;
uint32_t  r13 = 0;
uint32_t  r14 = 0;
uint32_t  r15 = 0;
//сохраненные временные значения
uint32_t  r16 = 0;
uint32_t  r17 = 0;
uint32_t  r18 = 0;
uint32_t  r19 = 0;
uint32_t  r20 = 0;
uint32_t  r21 = 0;
uint32_t  r22 = 0;
uint32_t  r23 = 0;
// временные
uint32_t  r24 = 0;
uint32_t  r25 = 0;
// зарезервирована для ядра операционной системы
const uint32_t  r26 = 0;
const uint32_t  r27 = 0;
// глобальный указатель
uint32_t  r28 = 0;
// указатель стека
const uint32_t  r29 = 0;
// указатель фрейма
uint32_t  r30 = 0;
// возвращает адрес
uint32_t  r31 = 0;

//два регистра для хранения результатов умножения/деления : HI, LO
uint32_t  HI = 0;
uint32_t  LO = 0;
