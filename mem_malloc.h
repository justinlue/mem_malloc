#pragma once
#include "stdint.h"
 
#ifndef NULL
#define NULL 0
#endif
#define MEM_BLOCK_SIZE    96
#define MAX_MEM_SIZE      6144 // 10k MAX managed memory size
#define MEM_ALLOC_TABLE_SIZE MAX_MEM_SIZE/MEM_BLOCK_SIZE
#define MEM_CHECK       0

struct _m_mallco_dev
{
  void (*init)(void);
  uint8_t (*perused)(void);
  uint8_t  membase[MAX_MEM_SIZE];
  uint16_t memmap[MEM_ALLOC_TABLE_SIZE];
  uint8_t  memrdy;
};

uint8_t simple_mem_perused(void);      
void simple_memset(void *s, uint8_t c,  uint32_t count);
void simple_memcpy(void *des,void *src, uint32_t n);
uint8_t simple_memcmp(uint8_t *buffer1, uint8_t *buffer2, uint16_t len);
void simple_mem_init(void);
void simple_free(void *ptr);
void *simple_malloc(uint32_t size);
void *simple_realloc(void *ptr, uint32_t size);
