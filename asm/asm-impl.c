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
  int val=0; 
  asm (
     "mov %%rax, (%%rdi);"
     "mov %%rbx, 4(%%rdi);"
     "mov %%rcx, 8(%%rdi);"
     "mov %%rdx, 12(%%rdi);"
     "mov %%rdi, 16(%%rdi);"
     "mov %%rip, 20(%%rdi);"
     "mov %%rsp,%%rbx;"
     "add  $0x4,%%rbx;"
     "mov %%rbx, 24(%%rdi);"
     "mov (%%rbp),%%rbx;"
     "mov %%rbx,28(%%rdi);"
     "mov 4(%%rbp),%%rbx;"
     "mov %%rbx,32(%%rdi);"
   //  "movq 40(%%rdi),%%rax;"
     :"=r"(env)
     :"r"(env),"r"(val)
     :
      );
  return val;
  //return setjmp(env);
}

void asm_longjmp(asm_jmp_buf *env, int val) {
  asm ( 
        "call %%rbx"
	"mov (%%rdi),%%rax\n\t"	
	"mov 8(%%rdi),%%rcx\n\t"
	"mov 12(%%rdi),%%rdx\n\t"
	"mov 24(%%rdi),%%rsp\n\t"
	"mov 28(%%rdi),%%rbp\n\t"
	//"movq 64(%%rdi),%%rbx\n\t"
	//"pushq %%rbx\n\t"			//push eip
	"mov 4(%%rdi),%%rbx\n\t"
	"mov 16(%%rdi),%%rdi\n\t"
	"mov %%rsi,20(%%rdi)\n\t"
	"jmp %%rbp;"
        "ret\n\t"					//pop eip
	:"=r"(val)
	:"r"(env),"r"(val)
        );  

  //longjmp(env, val);
}
