#define __REG(reg, arg) register arg asm(reg)

typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;


extern void LoadMFMB(__REG("a0", void* dest), __REG("d0", uint32_t startSector), __REG("d1", int16_t nrsecs));
//void relocate(__REG("a0", void *fastram));

extern uint8_t startFastRam;
extern uint8_t endFastRam;
extern uint8_t startChipRam;
extern uint8_t endChipRam;
extern uint8_t startDisk;
extern uint8_t relocateFast;
extern uint8_t relocateChip;

void
relocateSection(__REG("a0", uint8_t* baseAddress), __REG("a1", uint32_t* table), __REG("d0", uint32_t offset))
{
  uint32_t n = *((uint32_t*)table);
  table++;
  
  for (uint32_t i = 0; i < n; i++, table++) {
    uint32_t data = *table;
    uint32_t* address = (uint32_t*)(baseAddress+data);
    *(address) += offset;
    //baseAddress[data/4] += offset;
  }
}

void
relocate(__REG("a0", void * fastram), __REG("a1", void* chipram))
{
  int32_t startSector = ((((uint32_t)&startFastRam)))>>9;
  int16_t numSectors = ((&endFastRam-&startFastRam)+512)>>9;
  LoadMFMB(fastram, startSector, -numSectors);

  uint32_t* d = fastram;
  *d = (uint32_t)&startDisk-(uint32_t)&startFastRam;
  
  startSector = ((((uint32_t)&startChipRam)))>>9;
  numSectors = ((&endChipRam-&startChipRam)+512)>>9;
  LoadMFMB(chipram, startSector, numSectors);  

  relocateSection(fastram, (uint32_t*)((uint8_t*)fastram+(&relocateFast-&startFastRam)), (uint32_t)fastram);
  relocateSection(fastram, (uint32_t*)((uint8_t*)fastram+(&relocateChip-&startFastRam)), (uint32_t)chipram);  
}
