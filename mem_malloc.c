#include "mem_malloc.h"

static struct _m_mallco_dev mallco_dev=
{
  simple_mem_init, 
  simple_mem_perused,
  {0},   
  {0},   
  0,     
};

static uint32_t mem_malloc(uint32_t size)  
{  
  signed long offset=0;  
  uint16_t nmemb;
  uint16_t cmemb=0;
  uint32_t i;
  
  if(!mallco_dev.memrdy)
    mallco_dev.init();
  
  if(size==0)
    return 0XFFFFFFFF;
  
  nmemb = size / MEM_BLOCK_SIZE;
  if ( size % MEM_BLOCK_SIZE )
    nmemb++;
  
  for ( offset=MEM_ALLOC_TABLE_SIZE-1;offset>=0;offset-- )
  {
    if(!mallco_dev.memmap[offset])
      cmemb++;
    else 
      cmemb = 0;

    if( cmemb == nmemb ){
      for(i=0;i<nmemb;i++){  
        mallco_dev.memmap[offset+i]=nmemb;  
      }  
      return (offset*MEM_BLOCK_SIZE);
    }
  }
  return 0XFFFFFFFF;
}  

static uint8_t mem_free(uint32_t offset)  
{  
  int i;
  
  if(!mallco_dev.memrdy){
    mallco_dev.init();    
      return 1;
  }
  
  if(offset<MAX_MEM_SIZE){  
    int index = offset/MEM_BLOCK_SIZE;
    int nmemb = mallco_dev.memmap[index];
    for(i=0;i<nmemb;i++){  
      mallco_dev.memmap[index+i] = 0;  
    }  
    return 0;  
  }else 
    return 2;
}

void simple_memcpy(void *des,void *src,uint32_t n)  
{  
  uint8_t *xdes=des;
  uint8_t *xsrc=src; 
  while(n--)*xdes++=*xsrc++;  
}  

void simple_memset(void *s,uint8_t c,uint32_t count)  
{  
  uint8_t *xs = s;  
  while(count--)*xs++=c;  
}    

void simple_mem_init(void)  
{
  simple_memset(mallco_dev.membase, 0, sizeof(mallco_dev.membase));
  mallco_dev.memrdy=1;
}  

uint8_t simple_mem_perused(void)  
{  
  uint16_t used=0;  
  uint32_t i;  
  for(i=0;i<MEM_ALLOC_TABLE_SIZE;i++)  
  {  
    if(mallco_dev.memmap[i])
      used++; 
  }  
  return used*100/MEM_ALLOC_TABLE_SIZE;  
}  

void simple_free(void *ptr)  
{  
  uint32_t offset; 
  
  if (ptr == NULL)
    return;
  
  offset=(uint32_t)ptr-(uint32_t)&mallco_dev.membase;  
  mem_free(offset);
}  

void *simple_malloc(uint32_t size)  
{  
  uint32_t offset;  
  offset=mem_malloc(size);  
  if ( offset == 0XFFFFFFFF ) {
    WICED_BT_TRACE("no mem\n");
    return NULL;  
  }
  else 
    return (void*)((uint32_t)&mallco_dev.membase+offset);  
}  

void *simple_realloc(void *ptr,uint32_t size)  
{  
  uint32_t offset = mem_malloc(size);  

  if(offset==0XFFFFFFFF)
    return NULL;     
  else{  
    simple_memcpy((void*)((uint32_t)&mallco_dev.membase+offset),ptr,size);
    simple_free(ptr);
    return (void*)((uint32_t)&mallco_dev.membase+offset);
  }  
} 

uint8_t simple_memcmp(uint8_t *buffer1, uint8_t *buffer2, uint16_t len)
{  //ok...
    const uint8_t *pSrc1;
    const uint8_t *pSrc2;

    pSrc1 = buffer1;
    pSrc2 = buffer2;
    //compare...
    while(len --)
    {
        if(*pSrc1 ++ != *pSrc2 ++)
        {
            return 1;
        }
    }
    return 0;
}
