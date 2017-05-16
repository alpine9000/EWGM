PHDRS {
    chip PT_LOAD;    
    fast PT_LOAD;
    disk PT_LOAD;
    lastTrack PT_LOAD;
}


SECTIONS
{
    . = 0;
    .chip : {
    	*(data_c)    
        *(DATA_C)
    } :chip

    .chip_bss (NOLOAD) : {
       _startRandomChip = .;
       *(random_c)
       _endRandomChip = .;	
       _startBSSChip = .;
    	*(bss_c)
        *(BSS_C)
       _endBSSChip = .;
       endRam = .;
    } 

    . = 0x100000;

    .fast : {
    	_startFast = .;
	*(.entry)
	*(.text)
	*(CODE)			
        *(.data)	
        *(data)
	*(DATA)
    } :fast

    .bss (NOLOAD) : {
	_startBSS = .;
	*(bss)
        *(BSS)
	_endBSS = .;
	_startRandom = .;
        *(random)
	*(stack)
	_endRandom = .;
	_endFast = .;
   } 

   . = 0x200000;
   
   .disk ALIGN(512) : {
   	_startDisk = .;
   	*(disk)
	endDisk = .;
   } :disk

   .lastTrack : {
        *(lastTrack)
   } :lastTrack
}
