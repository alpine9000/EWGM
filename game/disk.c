#include "game.h"

#if TRACKLOADER==1

#if PHOTON_TRACKLOADER==1

extern void
LoadMFMB(__REG("a0", void* dest), __REG("d0", uint32_t startSector), __REG("d1", int16_t nrsecs), __REG("a1", void* lastAddr));

#else

void
td_init(void);

uint32_t 
td_selectdisk(__REG("d0", uint32_t diskId));

void
td_motoroff(void);

uint32_t
td_write(__REG("d0", uint16_t firstBlock), __REG("d1", uint16_t numBlocks), __REG("a0", void* source));

uint32_t
td_format(__REG("d0", uint16_t trackNum), __REG("a0", void* src), __REG("a4", uint16_t size));

uint32_t
td_read(__REG("d0", uint16_t firstBlock), __REG("d1", uint16_t numBlocks), __REG("a0", void* destination));

static uint16_t td_initialised = 0;

static void
td_doInit(void)
{
  if (!td_initialised) {
    td_init();
    td_selectdisk(0x19731973);
    td_initialised = 1;
  }
}
#endif
#endif

__EXTERNAL
__NOINLINE uint16_t
disk_loadData(void* dest, void* src, int32_t size)
{
#if TRACKLOADER==1
  uint16_t error = 0;
#if FASTRAM==0
  int32_t startSector = ((((uint32_t)src)-((uint32_t)&startCode))>>9)+2; // +2 for bootblock  
#else
  uint8_t* start;  
  start = disk_dataStart;
  int32_t startSector = ((((uint32_t)src)+((uint32_t)start))>>9);
#endif

  int16_t numSectors = (size+512)>>9;
#if PHOTON_TRACKLOADER==1
  LoadMFMB(dest, startSector, -numSectors, ((char*)dest)+size);
  return error;
#else

#ifdef DEBUG
  if (size < 512) {
    PANIC("panic: disk_loadData: < 512 read");
  }
#endif

  td_doInit();
  error = td_read(startSector+(numSectors-1), 1, dest);

  if (!error) {
    
    volatile char* d = (char*)dest+((numSectors-1)*512);
    char* s = dest;
    for (int16_t i = 0; i < 512 && d < ((char*)dest)+size; i++, d++, s++) {
      *d = *s;
    }
    if (numSectors > 1) {
      error = td_read(startSector, numSectors-1, dest);
    }
  }

  td_motoroff();
  return error;
#endif

#else

  char* d = (char*)dest;
  char* s = (char*)src;
  for (int32_t i = 0; i < size; i++) {
    *d++ = *s++;
  }

  return 0;
#endif
}


uint16_t
disk_read(void* dest, void* src, int32_t size)
{
  uint16_t error = 0;

 retry:
  error = disk_loadData(dest, src, size);
  
  if (error) {
    if (message_ask(I18N_DISK_READ_RTRY)) {
      goto retry;
    }
  }

  return error;
}


uint16_t
disk_write(void* dest, void* src, int16_t numBlocks)
{
  uint32_t err = 0;
#if TRACKLOADER==1
#if FASTRAM==0
  int32_t startBlock = ((((uint32_t)dest)-((uint32_t)&startCode))>>9)+2; // +2 for bootblock  
#else
  int32_t startBlock = ((uint32_t)dest)>>9;
#endif  

#if PHOTON_TRACKLOADER==0

#ifdef DEBUG
  if (((startBlock/11)*11) != startBlock) {
    PANIC("disk_write to non track aligned address");
  }
#endif

  //if ((err = td_write(startBlock, numBlocks, src)) != 0) {
  if ((err = td_format(startBlock/11, src, 512*numBlocks)) != 0) {
  } else {
    td_motoroff();
  }

#endif
#endif
  __USE(src);
  __USE(dest);
  __USE(numBlocks);
  return err;
}
