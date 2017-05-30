#!/bin/bash
echo "CHIP RAM USAGE:" $((16#`cat debugger.syms | grep endRam | sed 's/0x//' | sed 's/endRam: //'`))
cat out/fast/bootblock.txt | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > /tmp/bootblock.symbols
echo "DISK USAGE:" $((16#`cat /tmp/bootblock.symbols | grep maxDisk | sed 's/0x//' | sed 's/maxDisk: //'`))

