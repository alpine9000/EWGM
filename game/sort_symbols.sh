/usr/local/bin/gawk  '{val="0x" $2; sub("^0x0x","0x",val); print strtonum(val), $0 ;}' ./out/fast/reloc.fast.symbols | sort -k1n | less
