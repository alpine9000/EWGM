MEMORY 
{
    disk: org = 0x4000, len = 901120-0x4000-(512*11)
    endDisk: org = 895488+0x3c00, len = 512*11
    ram: org = 0x00000, len = 0x80000*3
}

SECTIONS
{
    load : { 
    	. = 0x4000;
        startCode = .;
        _startCode = .;
	*(CODE)
        *(.text) 
        *(.data)
	*(data_c)
        *(DATA)
        *(DATA_C)
	*(CHIP_DATA)
	*(COMMON)
        endCode = .;
    } > disk

    noload  : {
	. = ALIGN(512);    
        startData = .;
        level_data_1.o(disk);
	. = ALIGN(512);	
        level_data_2.o(disk);
	. = ALIGN(512);
	*(disk)
	endData = .;
    } > disk

    lastTrack (LOAD) : {
        lastTrackStart = .;
       *(lastTrack)
       endDisk = .;
    } > endDisk;

    bss (NOLOAD) : {
        . = endCode;
	_startBSS = .;
        *(.bss)
	*(bss)
	*(bss_c)
        *(BSS)
        *(BSS_C)
	*(CHIP_BSS)
	_endBSS = .;
    } > ram;

    random (NOLOAD) : {
    	. = _endBSS;
	_startRandom = .;
    	*(random)
        *(random_c)
	_endRandom = .;	
	endRam = .;
    } > ram;
}
