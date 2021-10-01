#include <stdint.h>

uint64_t bintran(uint32_t a){
  uint64_t k=0;                     //记录2的mici
  while(a>=(1<<k)){
    k++;
  }
  return k-1; 
}
uint64_t mod(uint64_t x,uint64_t m){
  while(x>=m){
    x=x-m;
  }
  return x;
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t x=a<b?a:b,y=a>b?a:b;
  
  uint64_t result=0;
  uint64_t k;
  while(y>0){
    k=bintran(y);
    y=y-(1<<k);
    result+=mod(x<<k,m);
  }
  
  return result;
}
