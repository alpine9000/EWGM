MEMORY 
{
    disk: org = 0x0, len = 901120
}

SECTIONS
{
    .disk : {
    	 *(.text)
	 . = 895488;
	 *(.lastTrack)
    } > disk
}
