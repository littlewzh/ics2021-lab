#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);
//extern uint32_t choose(uint32_t n);
static inline uint32_t choose(uint32_t n) { return rand() % n; }

static uint64_t cycle_cnt = 0;
static int ass;
static int total;
static int set_num;
static int hang_num;
static uint8_t cac[exp2(14)];
static bool valid[exp2(14-BLOCK_WIDTH)];           //每行的有效位
static uint32_t tag[exp2(14-BLOCK_WIDTH)];           //每行的标志位
static bool dirty[exp2(14-BLOCK_WIDTH)];              //每行的dirty位
void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

uint32_t cache_read(uintptr_t addr) {
  uint32_t tag_in=addr>>(BLOCK_WIDTH+set_num);
  uint32_t index=(addr<<(32-BLOCK_WIDTH-set_num))>>(32-set_num);
  uint32_t offset=addr&0x3c;
  int hit=0;
  uint32_t data_out;
  int i;
  for(i=0;i<ass;i++){
    if((tag[index*4+i]==tag_in)&&valid[index*4+i]) {hit=1;break;}
  }
  printf("hit:%d\n",hit);
  if(hit==1){
    data_out=((uint32_t)cac[(4*index+i)*64+offset])|((uint32_t)cac[(4*index+i)*64+offset+1]<<8)|((uint32_t)cac[(4*index+i)*64+offset+2]<<16)|((uint32_t)cac[(4*index+i)*64+offset+3]<<24);
  }
  else{
    
    int k=choose(4);
    if(dirty[index*4+k]==1){
      mem_write((tag[index*4+k]<<set_num)|index,(uint8_t *)(cac+(4*index+k)*64));
    }
    printf("reach here\n");
    mem_read(addr>>BLOCK_WIDTH,(uint8_t *)(cac+(4*index+k)*64));
    valid[index*4+k]=1;
    dirty[index*4+k]=0;
    data_out=((uint32_t)cac[(4*index+k)*64+offset])|((uint32_t)cac[(4*index+k)*64+offset+1]<<8)|((uint32_t)cac[(4*index+k)*64+offset+2]<<16)|((uint32_t)cac[(4*index+k)*64+offset+3]<<24);
  }
  printf("data_out:0x%08x\n",data_out);
  return data_out;
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t tag_in=addr>>(BLOCK_WIDTH+set_num);
  uint32_t index=(addr<<(32-BLOCK_WIDTH-set_num))>>(32-set_num);
  printf("%u\n",index);
  uint32_t offset=addr&0x3c;
  int hit=0;
  uint32_t data_in;
  //uint32_t data_out;
  int i;
  for(i=0;i<ass;i++){
    if((tag[index*4+i]==tag_in)&&valid[index*4+i]) {hit=1;break;}
  }
  printf("hit:%d\n",hit);
  if(hit==1){
    dirty[index*4+i]=1;
    cac[(4*index+i)*64+offset]=data&0xff;
    cac[(4*index+i)*64+offset+1]=data&0xff00>>8;
    cac[(4*index+i)*64+offset+2]=data&0xff0000>>16;
    cac[(4*index+i)*64+offset+3]=data&0xff000000>>24;
    //data_out=((uint32_t)cac[(4*index+i)*64+offset])|((uint32_t)cac[(4*index+i)*64+offset+1]<<8)|((uint32_t)cac[(4*index+i)*64+offset+2]<<16)|((uint32_t)cac[(4*index+i)*64+offset+3]<<24);
  }
  else{
    
    int k=choose(4);
    if(dirty[index*4+k]==1){
      mem_write((tag[index*4+k]<<set_num)|index,(uint8_t *)(cac+(4*index+k)*64));
    }
    printf("reach here\n");
    mem_read(addr>>BLOCK_WIDTH,(uint8_t *)(cac+(4*index+k)*64));
    valid[index*4+k]=1;
    dirty[index*4+k]=1;
    cac[(4*index+k)*64+offset]=data&0xff;
    cac[(4*index+k)*64+offset+1]=data&0xff00>>8;
    cac[(4*index+k)*64+offset+2]=data&0xff0000>>16;
    cac[(4*index+k)*64+offset+3]=data&0xff000000>>24;
    //data_out=((uint32_t)cac[(4*index+k)*64+offset])|((uint32_t)cac[(4*index+k)*64+offset+1]<<8)|((uint32_t)cac[(4*index+k)*64+offset+2]<<16)|((uint32_t)cac[(4*index+k)*64+offset+3]<<24);
  }
  //return data_out;
}

void init_cache(int total_size_width, int associativity_width) {
  total=exp2(total_size_width);                                  //cache total size;
  ass=exp2(associativity_width);                                  // the number of line for each set;
  set_num=total_size_width-BLOCK_WIDTH-associativity_width;  //组数
  hang_num=total_size_width-BLOCK_WIDTH;                   //行数
  //uint8_t cac[exp2(total_size_width)];
  //uint32_t path=exp2(associativity_width);
  //bool valid[exp2(total_size_width-BLOCK_WIDTH)];           //每行的有效位
  //uint32_t tag[exp2(total_size_width-BLOCK_WIDTH)];           //每行的标志位
}

void display_statistic(void) {
}
