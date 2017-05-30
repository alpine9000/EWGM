MEMORY 
{
    disk: org = 0x0, len = 901120
}

SECTIONS
{
    .disk : {
    	 *(.text)
	 maxDisk = .;
	 . = 895488;
	 *(.lastTrack)
    } > disk
}
