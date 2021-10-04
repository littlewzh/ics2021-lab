#include <stdint.h>
#include<stdio.h>
uint64_t bintran(uint32_t a){
  uint64_t k=0;                     //记录2的mici
  while(a>=(1<<k)){
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
    mod(s+1,(1<<64)-1,m);
  }
  return x;
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t x=a<b?a:b,y=a>b?a:b;
  
  uint64_t result=0;
  int k,l;
  while(y>0){
    k=bintran(y);
    y=y-(1<<k);
    l=bintran(x);
    if((l+k)<64){
      result=mod(result,x<<k,m);
    }
    else {
      x=x<<(63-l);
      int s=(k+l)-64;
      uint64_t presult=mod(x,0,m);
      //result=mod(result,x,m);
      for(int i=0;i<=s;i++){
        if((presult>>(63-i)&)1) {presult=mod((presult<<1)+1,(1<<64)-1,m);}
        else {continue;}
      }
      result=mod(result,presult,m);
    }

  }
  
  return result;
}
