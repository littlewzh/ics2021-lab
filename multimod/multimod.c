#include <stdint.h>
#include<stdio.h>
uint64_t bintran(uint64_t z){

  uint64_t k=0;                     //记录2的mici
  while((z>=(1<<k))&&k<64){
    k++;
  }
  return k-1; 
}
uint64_t mod(uint64_t x,uint64_t y,uint64_t m){
  while(x>=m){
    x=x-m;
  }
  while(y>=m){
     y=y-m;
   }
  uint64_t s=x+y;
  if(s<x){                       //此时发生了溢出
    mod(s+1,-1ULL,m);
  }
else {
  while(s>=m) {s=s-m;}
 }
  return s;
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result=0;
  uint64_t k=0;
  uint64_t l=0;
  l=bintran(a); 
  while(b>0){
    k=bintran(b);
    uint64_t sub=(1<<(k-1));
    b=b-sub;
    if((l+k)<64){
      result=mod(result,a<<k,m);
    }
    else {
      uint64_t presult=a;
      for(int i=0;i<k;i++){
          presult=mod(presult,presult,m);
      }
      result=mod(result,presult,m);
    }

  }
  
  return result;
}
