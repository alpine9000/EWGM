#!/bin/bash
endFast=$((16#`cat debugger.syms | grep _endFast | sed 's/0x//' | sed 's/_endFast: //'`))
startFast=$((16#`cat debugger.syms | grep _startFast | sed 's/0x//' | sed 's/_startFast: //'`))
fastUsed=$(($endFast-$startFast))
chipUsed=$((16#`cat debugger.syms | grep endRam | sed 's/0x//' | sed 's/endRam: //'`))
cat out/fast/bootblock.txt | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > /tmp/bootblock.symbols
diskUsed=$((16#`cat /tmp/bootblock.symbols | grep maxDisk | sed 's/0x//' | sed 's/maxDisk: //'`))

echo "CHIP RAM USAGE:" $chipUsed" ("$((524288-$chipUsed))" free)"
echo "FAST RAM USAGE:" $fastUsed" ("$((524288-$fastUsed))" free)"
echo "    DISK USAGE:" $diskUsed" ("$((895488-$diskUsed))" free)" #901120 - hiscore track




