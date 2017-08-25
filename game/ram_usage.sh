#!/bin/bash
echo "CHIP RAM USAGE:" $((16#`cat debugger.syms | grep endRam | sed 's/0x//' | sed 's/endRam: //'`))
