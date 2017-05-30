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

    . = 0x203250;    

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

   . = 0x400000;
   
   .disk ALIGN(512) : {
   	_startDisk = .;
        startData = .;
        level_data_1.o(disk);
	. = ALIGN(512);	
        level_data_2.o(disk);
	. = ALIGN(512);
        level_data_3.o(disk);
	. = ALIGN(512);
	*(disk)
	endDisk = .;
   } :disk

   .lastTrack : {
        *(lastTrack)
   } :lastTrack
}
