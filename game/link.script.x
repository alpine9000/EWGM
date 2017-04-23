MEMORY 
{
    disk: org = 0x4000, len = 901120-0x4000-(512*11)
    endDisk: org = 895488+0x3c00, len = 512*11
    ram: org = 0x00000, len = 0x80000*1
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

    noload ALIGN(512) : {
        startData = .;
        *(.noload)
        *(noload)
	.=ALIGN(512);
	*(music)
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
    	*(random_c)
	_endRandom = .;	
	endRam = .;
    } > ram;
}
