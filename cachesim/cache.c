#include "common.h"
#include <inttypes.h>
#include <string.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);
//extern uint32_t choose(uint32_t n);
static inline uint32_t choose(uint32_t n) { return rand() % n; }
//extern uint8_t mem[MEM_SIZE];
static uint64_t cycle_cnt = 0;
static int ass;
static int total;
static int set_num;
static int hang_num;
static uint8_t cac[exp2(24)];
static bool valid[exp2(24-BLOCK_WIDTH)];           //每行的有效位
static uint32_t tag[exp2(24-BLOCK_WIDTH)];           //每行的标志位
static bool dirty[exp2(24-BLOCK_WIDTH)];              //每行的dirty位
void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

uint32_t cache_read(uintptr_t addr) {
  //uint32_t addrin=addr&~0x3;
  uint32_t tag_in=addr>>(BLOCK_WIDTH+set_num);
  //printf("0x%08x\n",tag_in);
  uint32_t index=((addr<<(64-BLOCK_WIDTH-set_num))>>(64-set_num));
  //printf("%d\n",set_num);
  //printf("0x%08x\n",index);
  uint32_t offset=addr&0x3c;
  //printf("0x%08x\n",offset);
  int hit=0;
  uint32_t data_out;
  int i;
  for(i=0;i<ass;i++){
    if((tag[index*ass+i]==tag_in)&&valid[index*ass+i]) {hit=1;break;}
  }
  //printf("hit:%d\n",hit);
  if(hit==1){
    data_out=((uint32_t)cac[(ass*index+i)*64+offset])|((uint32_t)cac[(ass*index+i)*64+offset+1]<<8)|((uint32_t)cac[(ass*index+i)*64+offset+2]<<16)|((uint32_t)cac[(ass*index+i)*64+offset+3]<<24);
  }
  else{
    
    int k=choose(ass);
    if(dirty[index*ass+k]==1){
      mem_write((tag[index*ass+k]<<set_num)|index,(uint8_t *)(cac+(ass*index+k)*64));

    }
    //printf("dirty:%d\n",dirty[index*4+k]);
    mem_read(addr>>BLOCK_WIDTH,(uint8_t *)(cac+(ass*index+k)*64));
    valid[index*ass+k]=1;
    tag[index*ass+k]=tag_in;
    dirty[index*ass+k]=0;
    /*printf("0x%02x\n",cac[(4*index+k)*64+offset]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+1]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+2]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+3]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+4]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+5]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+6]);
    printf("0x%02x\n",cac[(4*index+k)*64+offset+7]);*/
    data_out=((uint32_t)cac[(ass*index+k)*64+offset])|((uint32_t)cac[(ass*index+k)*64+offset+1]<<8)|((uint32_t)cac[(ass*index+k)*64+offset+2]<<16)|((uint32_t)cac[(ass*index+k)*64+offset+3]<<24);
  }
  //printf("data_out2:0x%08x\n",data_out2);
  //printf("data_out:0x%08x\n",data_out);
  return data_out;
  //return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  //uint32_t addr=addrin&~0x3;
  uint32_t tag_in=addr>>(BLOCK_WIDTH+set_num);
  uint32_t index=((addr<<(64-BLOCK_WIDTH-set_num))>>(64-set_num));
  //printf("0x%08x\n",index);
  uint32_t offset=addr&0x3c;
  //printf("0x%08x\n",offset);
  int hit=0;
  uint32_t data_in;
  //uint32_t data_out;
 //*p = (*p & ~wmask) | (data & wmask);
  int i;
  for(i=0;i<ass;i++){
    if((tag[index*ass+i]==tag_in)&&valid[index*ass+i]) {hit=1;break;}
  }
  //printf("hit:%d\n",hit);
  if(hit==1){
    dirty[index*ass+i]=1;
    *(uint32_t *)(cac+(ass*index+i)*64+offset)=(*(uint32_t *)(cac+(ass*index+i)*64+offset)&~wmask)|(data & wmask);
    /*cac[(4*index+i)*64+offset]=data&0xff;
    cac[(4*index+i)*64+offset+1]=(data&0xff00)>>8;
    cac[(4*index+i)*64+offset+2]=(data&0xff0000)>>16;
    cac[(4*index+i)*64+offset+3]=(data&0xff000000)>>24;*/
    //data_out=((uint32_t)cac[(4*index+i)*64+offset])|((uint32_t)cac[(4*index+i)*64+offset+1]<<8)|((uint32_t)cac[(4*index+i)*64+offset+2]<<16)|((uint32_t)cac[(4*index+i)*64+offset+3]<<24);
  }
  else{
    
    int k=choose(ass);
    if(dirty[index*ass+k]==1){
      mem_write((tag[index*ass+k]<<set_num)|index,(uint8_t *)(cac+(ass*index+k)*64));
    }
   //printf("reach here\n");
    mem_read(addr>>BLOCK_WIDTH,(uint8_t *)(cac+(ass*index+k)*64));
    valid[index*ass+k]=1;
    dirty[index*ass+k]=1;
    tag[index*ass+k]=tag_in;
    *(uint32_t *)(cac+(ass*index+k)*64+offset)=(*(uint32_t *)(cac+(ass*index+k)*64+offset)&~wmask)|(data & wmask);
    /*cac[(4*index+k)*64+offset]=data&0xff;
    cac[(4*index+k)*64+offset+1]=(data&0xff00)>>8;
    cac[(4*index+k)*64+offset+2]=(data&0xff0000)>>16;
    cac[(4*index+k)*64+offset+3]=(data&0xff000000)>>24;*/
    //data_out=((uint32_t)cac[(4*index+k)*64+offset])|((uint32_t)cac[(4*index+k)*64+offset+1]<<8)|((uint32_t)cac[(4*index+k)*64+offset+2]<<16)|((uint32_t)cac[(4*index+k)*64+offset+3]<<24);
  }
}

void init_cache(int total_size_width, int associativity_width) {
  total=exp2(total_size_width);                                  //cache total size;
  ass=exp2(associativity_width);                                  // the number of line for each set;
  set_num=total_size_width-BLOCK_WIDTH-associativity_width;  //组数
  hang_num=total_size_width-BLOCK_WIDTH;                   //行数
  memset(cac,0,sizeof(cac));
  memset(dirty,0,sizeof(dirty));
  memset(valid,0,sizeof(valid));
  memset(tag,0,sizeof(tag));
  //uint8_t cac[exp2(total_size_width)];
  //uint32_t path=exp2(associativity_width);
  //bool valid[exp2(total_size_width-BLOCK_WIDTH)];           //每行的有效位
  //uint32_t tag[exp2(total_size_width-BLOCK_WIDTH)];           //每行的标志位
}

void display_statistic(void) {
}
