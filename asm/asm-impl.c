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

int asm_setjmp(asm_jmp_buf *env) {
  //int val=0; 
  asm (
    // "mov -8(%%rsp),%%rax;"
     "movq %%rax, (%%rdi);"
     "movq %%rbx, 8(%%rdi);"
     "movq %%rcx, 16(%%rdi);"
    // "mov (%%rsp), %%rcx;"
     "movq %%rdx,24(%%rdi);"
    // "lea 16(%%rsp),%%rcx;"
     "movq %%rdi, 32(%%rdi);"
    // "mov 8(%%rsp),%%rcx;"
    "movq %%rsi, 40(%%rdi);"
     "movq %%rsp,%%rbx;"
     "add $0x10,%%rbx;"
     "movq %%rbx, 48(%%rdi);"
     "movq (%%rbp),%%rbx;"
     "movq %%rbx,56(%%rdi);"
     "movq 8(%%rbp),%%rbx;"
     "movq %%rbx,64(%%rdi);"
     //"movq 40(%%rdi),%%rax;"
     :
     :"r"(env)
     :
      );
  return 0;
  //return setjmp(env);
}

void asm_longjmp(asm_jmp_buf *env, int val) {
  asm (
	"movq 0(%%rdi),%%rax\n\t"	
	"movq 16(%%rdi),%%rcx\n\t"
	"movq 24(%%rdi),%%rdx\n\t"
	"movq 48(%%rdi),%%rsp\n\t"
	"movq 56(%%rdi),%%rbp\n\t"
	"movq 64(%%rdi),%%rbx\n\t"
	"pushq %%rbx\n\t"			//push eip
	"movq 8(%%rdi),%%rbx\n\t"
	"movq 32(%%rdi),%%rdi\n\t"
	"movq %%rsi,40(%%rdi)\n\t"
	"ret\n\t"					//pop eip
	:
	:
        );  

  //longjmp(env, val);
}
