#!/bin/bash
endFast=$((16#`cat debugger.syms | grep _endFast | sed 's/0x//' | sed 's/_endFast: //'`))
startFast=$((16#`cat debugger.syms | grep _startFast | sed 's/0x//' | sed 's/_startFast: //'`))
echo "CHIP RAM USAGE:" $((16#`cat debugger.syms | grep endRam | sed 's/0x//' | sed 's/endRam: //'`))
echo "FAST RAM USAGE:" $(($endFast-$startFast))
cat out/fast/bootblock.txt | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > /tmp/bootblock.symbols
echo "    DISK USAGE:" $((16#`cat /tmp/bootblock.symbols | grep maxDisk | sed 's/0x//' | sed 's/maxDisk: //'`))




