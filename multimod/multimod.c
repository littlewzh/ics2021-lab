#include <stdint.h>
#include<stdio.h>
int ain[128],bin[128],min[128];
uint64_t bintran(uint64_t z,int i){
  return (z>>i)&1;
}
uint64_t mod(uint64_t c,uint64_t mod){
  if(c<mod) {return c;}
  else {
    while(c>=mod){
      uint64_t modd=mod;
      while((c>=modd)&&(!bintran(modd,63))){
        modd=modd<<1;
        
      }
      c=c-(modd>>1);
    }
    return c;
  }
}

uint64_t addmod(uint64_t x,uint64_t y,uint64_t m){
  x=mod(x,m);
  y=mod(y,m);
  uint64_t s=x+y;
  if(x>(-1ULL-y)){                       //此时发生了溢出
    return addmod(s+1,-1ULL,m);
  }
else {
  return mod(s,m);
}
}
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result=0;
  int k=0;
  for(int i=0;i<64;i++){
    if(bintran(b,i)) {bin[i]=1;k=i;}
    else {bin[i]=0;}
  }
  int l=0;
  for(int j=0;j<64;j++){
    if(bintran(a,j)) {ain[j]=1;l=j;}
    else {ain[j]=0;}
  } 
  /*while(b>0){
    k=bintran(b);
    uint64_t sub=1;
    b=b-(sub<<k);
    if((l+k)<64){
      result=addmod(result,a<<k,m);
    }
    else {
      uint64_t presult=a;
      for(uint64_t i=0;i<k;i++){
          presult=addmod(presult,presult,m);
      }
      result=addmod(presult,result,m);
    }

  }*/
    for(int i=k;i>=0;i--){
      if(bin[i]){
        if((i+l)<64){
        result=addmod(result,a<<i,m);}
        else{
          uint64_t presult=a;
        for(int j=0;j<i;j++){
           presult=addmod(presult,presult,m);
         }  
      result=addmod(presult,result,m);
        }
      }
    }
  return result;
}
