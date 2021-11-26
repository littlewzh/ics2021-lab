#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
 asm(
     "mov %%rdi,%%rax;"
     "add %%rsi,%%rax;"
    // "mov %%rsi,%%rax;"
     :"=r"(a)
     :"r"(a),"r"(b)
 );
  return a;
}

int asm_popcnt(uint64_t x) {
  /*int s = 0;
  for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }*/
  int s=0;
  //int i=0;
  asm(
      "mov $0x0,%%ecx;"           //int i=0
      "mov $0x0,%%edx;"          // int s=0
      ".loop1:"
      "mov %%rdi,%%rax;"         // x->rdi
      "shr %%cl,%%rax;"
      "and $0x1,%%eax;"
      "incl %%ecx;"
      "add %%eax,%%edx;"
      "cmp $0x40,%%ecx;"
      "jl .loop1;"
      "mov %%edx,%%eax;"
   
      :"=r"(s)
      :"r"(s),"r"(x)
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
 /*char *pout=(char *)dest;
 char *pin=(char *)src;
 while(n--){
   *pout++=*pin++;
 }*/
 asm(                       // dest->rdi   src->rsi   n->rdx
    "mov %%rdx,%%rcx;"
    "mov $0x0,%%eax;"
    ".loop:"
    "cmp %%rcx,%%rax;"
    "je .retq;"
    "movzbl (%%rsi,%%rax,1),%%edx;"
    "mov %%dl,(%%rdi,%%rax,1);"
    "add $0x1,%%rax;"
    "jmp .loop;"
    ".retq:"
    "mov %%rdi,%%rax;"
    :"=r"(dest)
    :"r"(dest),"r"(src),"r"(n)
 );
 return dest;//memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env) {
  asm volatile(
               "push %%rbp;"
               "mov %%rsp,%%rbp;"
               "mov (%%rbp),%%rax;"
               "mov %%rax, (%%rdi);"
               "lea 10(%%rsp),%%rax;"
               "mov %%rax,8(%%rdi);"
               "mov %%rbx,10(%%rdi);"
               "mov 8(%%rbp),%%rax;"
               "mov %%rax,18(%%rdi);"
               "mov %%rcx,20(%%rdi);"
               "mov %%rdx,28(%%rdi);"
               "mov %%rsi,30(%%rdi);"
               "pop %%rbp;"
               :
               :
  );
  return 0;//setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  //longjmp(env, val);
  asm volatile(
             "mov %%rsi,%%rax;"
             "mov 20(%%rdi),%%rcx;"
             "mov 28(%%rdi),%%rdx;"
             "mov 30(%%rdi),%%rsi;"
             //"mov 18(%%rdi),"
             "jmp *18(%%rdi);"
            :
            :"rsi"(val)
            :

  );  
}
