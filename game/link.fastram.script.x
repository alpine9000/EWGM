PHDRS {
    chip PT_LOAD;    
    fast PT_LOAD;
    disk PT_LOAD;
}


SECTIONS
{
    . = 0;
    .chip : {
        *(entry)
    	*(data_c)    
        *(DATA_C)
	*(CHIP_DATA)
    } :chip

    .chip_bss (NOLOAD) : {
       _startRandomChip = .;
       *(random_c)
       _endRandomChip = .;	
       _startBSSChip = .;
    	*(bss_c)
        *(BSS_C)
	*(CHIP_BSS)
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
   	*(.noload)
   	*(noload)	    
	. = ALIGN(512);
	*(music)
	endDisk = .;
   } :disk

}
