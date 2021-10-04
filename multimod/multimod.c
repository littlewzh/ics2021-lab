#include <stdint.h>
#include<stdio.h>
int bin[128];
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
    mod(s+1,-1,m);
  }
else {
  while(s>=m) {s=s-m;}
 }
  return s;
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t x=a;
  uint64_t y=b;
  uint64_t result=0;
  uint64_t k=0;
  uint64_t l=0;
  l=bintran(x); 
  while(y>0){
    k=bintran(y);
    uint64_t sub=(1<<k);
    y=y-sub;
    if((l+k)<64){
      result=mod(result,x<<k,m);
    }
    else {
      int s=(k+l)-64;
      uint64_t presult=mod(x<<(63-l),0,m);
      //result=mod(result,x,m);
      for(int i=0;i<=s;i++){
        if((presult>>63)&1==1) {presult=mod((presult<<1)+1,-1,m);}
        else {presult=(presult<<1);}
      }
      result=mod(result,presult,m);
    }

  }
  
  return result;
}
