#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
 asm("addl %%edx,%%eax"
     :"=r"(a)
     :"r"(a),"r"(b)
 );
  return a;
}

int asm_popcnt(uint64_t x) {
 /* int s = 0;
  for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }*/
  int s=0;
  //int i=0;
  asm(
     //"mov $0x0,%[xs];"
      "mov %%edx %[as]"
      "mov $0x0 %[ai]"
      "mov %%edi, %[ax];"
     ".loop:"
      "cmp $0x40,%[ai];"
      "jge  .ret;"
      "incl %[ai];"
      "shr $0x1,%[ax];"
      "mov %[ax],%%eax;"
      "and $0x1,%%eax;"
      "cmp $0x1,%%eax;"
      "jz .loop;"
      "incl %[as];"
      "jmp .loop;"
      :[as]"=r"(s)
      :[as]"r"(s),[ax]"r"(x)
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
