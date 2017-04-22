# frequently changed config items
#KEEP_ASM=1
USE_BIG=1
SFX=1
PHOTON_TRACKLOADER=0
MUSIC_PARALLEL_LOAD=0
ONEBITPLANE_MASK=--onebitplane-mask

# programs and paths
CC=/usr/local/amiga/vgcc/bin/m68k-amigaos-gcc
VASM=/usr/local/amiga/vgcc/bin/vasmm68k_mot
LD=/usr/local/amiga/vgcc/bin/vlink
NDK=/usr/local/amiga/vgcc/ndk/
INCLUDE=-I$(NDK)include/


# userstack used by bootblock
USERSTACK_ADDRESS=7fffc
# if you change this you must also change link.script.x
BASE_ADDRESS=4000
# note: this must be high enough not to conflict with MFMbufE
BOOTBLOCK_ASM=alpine_bootblock.s


# top level stuff
GAME_NAME=newgame
ADF=bin/$(GAME_NAME).adf
MODULE=$(GAME_NAME).s
PROGRAM_BIN=out/main.bin


# all the components that form the game
C_SRCS=init.c game.c gfx.c screen.c mouse.c tile.c palette.c level.c disk.c text.c sound.c music.c keyboard.c menu.c message.c record.c  string.c hw.c hiscore.c dos.c panic.c script.c object.c object_data.c sort.c player.c enemy.c logo.c fighter.c hand.c conductor.c alarm.c thing.c level1.c star.c
SRCS=blitmtext.s hw_asm.s trackloader.s palette_asm.s  P6112-Play.s music_asm.s keyboard_asm.s sg_trackdisk.s
MODULES=P61.jojo_ingame.bin P61.jojo_boss.bin
SOUND_FILES=Terence_punch01.wav Terence_punch02.wav Bud_Punch01.wav enemy01.wav enemy02.wav enemy03.wav pop.wav coin1.wav
LEVELS=1
IMAGES=font8x8.png menu.png scoreboard.png logo.png #popup.png


# program arguments
LINKER_OPTIONS=-T link.script.x

VASM_ARGS=-Fhunk -quiet -esc -DSFX=$(SFX) -allmp -DMAX_P61_SIZE=$(MAX_P61_SIZE) -DMUSIC_PARALLEL_LOAD=$(MUSIC_PARALLEL_LOAD) -DPHOTON_TRACKLOADER=$(PHOTON_TRACKLOADER)
VASM_C_ARGS=-ldots -Fhunk -quiet -no-opt

EXTRA_DEFINES=-DSFX=$(SFX) -DMAX_P61_SIZE=$(MAX_P61_SIZE) -DPHOTON_TRACKLOADER=$(PHOTON_TRACKLOADER) -DMUSIC_PARALLEL_LOAD=$(MUSIC_PARALLEL_LOAD)
REGPARAM=-mregparm=2
#VASM_OPTS=-Wa,-no-opt
VASM_OPTS=-Wa,-opt-allbra -Wa,-opt-brajmp -Wa,-opt-lsl  -Wa,-opt-movem -Wa,-opt-mul -Wa,-opt-div -Wa,-opt-pea #-Wa,-opt-speed 
ifeq ($(USE_BIG), 1)
WHOLE_PROGRAM=-fwhole-program
endif
C_OPT=-Ofast
BIG_FLAGS=$(WHOLE_PROGRAM)
C_OPT_ASM_OUTPUT=-Ofast
BASE_CFLAGS=-funsafe-loop-optimizations -Wno-overlength-strings $(REGPARAM) $(VASM_OPTS) -fomit-frame-pointer -noixemul -mcpu=68000 -m68000 -pedantic-errors -Wfatal-errors -Wall -Werror -Wextra -Wshadow -Wuninitialized -Wduplicated-cond -Wframe-larger-than=512 -Wstack-usage=512 -Wundef -Wbad-function-cast -Wparentheses $(INCLUDE) $(EXTRA_DEFINES)
C_LIBS=libgcc/out/gcc.lib
C_LIBS_WB=$(NDK)/lib/amiga.lib libgcc/out/gcc.lib
CFLAGS=$(C_OPT) $(BASE_CFLAGS)
CFLAGS_CHECK = $(BASE_CFLAGS)
CFLAGS_ASM_OUTPUT=$(C_OPT_ASM_OUTPUT) $(BASE_CFLAGS)


# transformations and calculations
LEVEL_FILES=$(addsuffix .h,$(addprefix out/sprite_, $(LEVELS)))\
	    $(addsuffix .h,$(addprefix out/tile_, $(LEVELS)))\
	    $(addsuffix -map.c,$(addprefix out/background_, $(LEVELS)))\
	    $(addsuffix .h,$(addprefix out/level_recording_, $(LEVELS)))
MAX_P61_SIZE=$(shell ls -Sl assets/P61*.bin | awk '{print $$5}' | head -1)
MODULE_HEADERS=$(addprefix out/, $(MODULES:.bin=.h))
IMAGEDATA=$(addprefix out/, $(IMAGES:.png=.bin))
RAW_SOUND_FILES=$(addprefix out/, $(SOUND_FILES:.wav=.raw))
ASSETS=$(IMAGEDATA) $(RAW_SOUND_FILES)
ASM_OBJS=$(SRCS:.s=.o)
C_OBJS=$(C_SRCS:.c=.o)
ifeq ($(USE_BIG), 1)
OBJS=$(ASM_OBJS) big.o
else
OBJS=$(ASM_OBJS) $(C_OBJS) 
endif
ADF_C_OBJS=$(addprefix out/adf/, $(C_OBJS)) 
ADF_OBJS=$(addprefix out/adf/, $(OBJS)) 
WORKBENCH_C_OBJS=$(addprefix out/workbench/, $(C_OBJS)) 
WORKBENCH_OBJS=$(addprefix out/workbench/, $(OBJS)) 
ALL_OBJS=out/adf/main.o $(ADF_OBJS)
ALL_WORKBENCH_OBJS=out/workbench/main.o $(WORKBENCH_OBJS)
ALL_DEPENDS=$(ALL_OBJS:.o=.d) $(ALL_WORKBENCH_OBJS:.o=.d)


# rules
all: | out adf 

include ../shared/externals.mk

adf: libgcc/out/gcc.lib version bin $(ALL_TOOLS) $(ADF)

.PHONY: version
version:
	@echo $(ALL_TOOLS)
	-@#./version/version.sh

gdrive: all
	cp $(ADF) ~/Google\ Drive

testadf: all
	cp $(ADF) ~/Projects/amiga/test.adf

libgcc/out/gcc.lib:
	make -C libgcc

.PHONY: test
test: testadf
	-@rm -f ~/Documents/FS-UAE/Save\ States/A500/test.sdf
	cp test/test.lua default.lua
	 $(RUN_SCRIPT)
	-@grep FAIL lua.log || true
	@grep PASSED lua.log

go: testadf
	@rm -f default.lua
	 $(RUN_SCRIPT)


goaga: testadf
	 $(A1200T_RUN_SCRIPT)

bin:
	mkdir bin

out:
	mkdir out
	mkdir out/adf
	mkdir out/workbench

$(ADF): out/bootblock.bin
	$(MAKEADF) out/bootblock.bin > $(ADF)
	@ls -lh out/bootblock.bin

wb: | out version libgcc/out/gcc.lib $(ALL_TOOLS) bin/$(GAME_NAME)

out/adf/star.o: star.c out/star-sprite.h
out/adf/level.o: level.c $(LEVEL_FILES)
out/workbench/level.o: level.c $(LEVEL_FILES)

out/Terence_punch01.raw: assets/Terence_punch01.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/Terence_punch02.raw: assets/Terence_punch02.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/Bud_Punch01.raw: assets/Bud_Punch01.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/enemy01.raw: assets/enemy01.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/enemy02.raw: assets/enemy02.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/enemy03.raw: assets/enemy03.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@	

#http://opengameart.org/content/10-8bit-coin-sounds
out/coin1.raw: assets/coin1.wav
	sox -R -v 1.0 $<  -b 8 -c 1 -r 11025 $@	

#http://opengameart.org/content/bubbles-pop
out/pop.raw: assets/pop.wav
	sox -R -v 1.0 $<  -b 8 -c 1 -r 11025 $@

out/star-sprite.h: assets/star.png
	$(IMAGECON) --input $< --colors=4 --output=out/star --output-sprite --output-png

out/adf/sprite.o: $(HSPRITE_HEADERS)

out/workbench/sprite.o: $(HSPRITE_HEADERS)

out/background_%-map.c: assets/background_%.tmx out
	$(MAPGEN) --c --depth=5 --input=$<
	@mv background_$*-map.c out
	@mv item_$*-sprites.c out
	@rm background_$*-indexes.c
	@rm item_$*-indexes.c
	@rm item_$*-map.c
	@rm background_$*-sprites.c

out/sprite_%.h: assets/sprite_%.png palettes/palette_%.pal
	$(CROPPA) --width=320 --height=563 --input=assets/sprite_$*.png --output=out/sprite_$*-resized.png
	$(IMAGECON) $(ONEBITPLANE_MASK) --quantize --colors=32 --full-color-palette --input=out/sprite_$*-resized.png --output=out/sprite_$* --output-bitplanes --output-png --output-palette --output-palette-asm --use-palette=palettes/palette_$*.pal --output-mask
	cat out/sprite_$*.bin | $(BIN2C) > out/sprite_$*.h
	cat out/sprite_$*-mask.bin | $(BIN2C) > out/sprite_$*-mask.h


out/tile_%.h: assets/tile_%.png palettes/palette_%.pal
	$(IMAGECON) --quantize --colors=32 --full-color-palette --input=assets/tile_$*.png --output=out/tile_$* --output-bitplanes --output-png --output-palette --output-palette-asm --use-palette=palettes/palette_$*.pal
	cat out/tile_$*.bin | $(BIN2C) > out/tile_$*.h


out/level_recording_%.h: recordings/level%.raw
	echo recordings/level$*.raw 
	cat recordings/level$*.raw | $(BIN2C) > out/level_recording_$*.h

out/%.h: assets/%.bin
	cat assets/$*.bin | $(BIN2C) > out/$*.h

out/adf/music.o: music.c $(MODULE_HEADERS)
out/workbench/music.o: music.c $(MODULE_HEADERS)

out/bootblock.bin: out/bootblock.o

out/menu.bin: assets/menu.png palettes/menu.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/menu.png --output=out/menu --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/menu.pal

out/logo.bin: assets/menu.png palettes/logo.pal
	$(IMAGECON) --colors=16 --full-color-palette --input=assets/logo.png --output=out/logo --output-bitplanes --output-png  --output-palette --use-palette=palettes/logo.pal

out/scoreboard.bin: assets/scoreboard.png palettes/palette_1.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/scoreboard.png --output=out/scoreboard --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/palette_1.pal

#out/popup.bin: assets/popup.png palettes/default.pal
#	$(IMAGECON) --colors=32 --full-color-palette --input=assets/popup.png --output=out/popup --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/default.pal

out/bootblock.bin: out/bootblock.o
	$(LD) -brawbin1 $< -o $@

out/font%.bin: assets/font%.png palettes/font.pal
	$(IMAGECON) --input $< $(IMAGECON_ARGS) --output-bitplanes --output-grey-palette-asm --output-palette-asm --output-palette --output out/font$* --colors=2 --use-palette=palettes/font.pal --full-color-palette-file --output-mask --transparent-color=0,0,0 --output-png


#./out/logo.pal: out/logo.bin
#./out/menu.pal: out/menu.bin

out/bootblock.o: $(BOOTBLOCK_ASM) $(PROGRAM_BIN)
	$(VASM) $(VASM_ARGS)  -DUSERSTACK_ADDRESS="\$$$(USERSTACK_ADDRESS)" -DBASE_ADDRESS="\$$$(BASE_ADDRESS)"  $< -o $@ $(INCLUDE)

out/adf/main.o: $(MODULE) $(EXTRA) $(ASSETS) $(IMAGEDATA)
	$(VASM)  -DTRACKLOADER=1  $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make -DTRACKLOADER=1 $(VASM_ARGS) $< -o $@ $(INCLUDE) > $*.d

out/workbench/main.o: $(MODULE) $(EXTRA) $(ASSETS)
	$(VASM)  -DTRACKLOADER=0 $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make -DTRACKLOADER=0 $(VASM_ARGS) $< -o $@ $(INCLUDE) > $*.d

out/adf/%.o: %.s
	$(VASM) $(VASM_ARGS) -DTRACKLOADER=1 $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make -DTRACKLOADER=1 $(VASM_ARGS) $< -o $@ $(INCLUDE) > out/adf/$*.d

out/adf/%.o: out/%.c
ifeq ($(KEEP_ASM),1)
	-@$(CC) -S $(CFLAGS_ASM_OUTPUT) -DTRACKLOADER=1 $< -o out/adf/$*.s 
endif
	$(CC) -c $(CFLAGS) -DTRACKLOADER=1 $< -o out/adf/$*.o

out/workbench/%.o: out/%.c
	$(CC) $(CFLAGS) -DTRACKLOADER=0 -c $< -o $@ 

adf_big.c: $(ADF_C_OBJS) $(C_SRCS)
	cat $(C_SRCS) > adf_big.c

out/adf/big.o: adf_big.c
	$(CC) -c $(CFLAGS) $(BIG_FLAGS) -DTRACKLOADER=1 adf_big.c -o $*.o
	-@$(CC) -MM $(CFLAGS_CHECL) -DTRACKLOADER=1 adf_big.c -o $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
ifeq ($(KEEP_ASM), 1)
	-@$(CC) -c $(CFLAGS) $(BIG_FLAGS) -DTRACKLOADER=1 adf_big.c -S -o out/adf/big.s
endif
	-@#rm adf_big.c

workbench_big.c: $(WORKBENCH_C_OBJS) $(C_SRCS)
	cat $(C_SRCS) > workbench_big.c

out/workbench/big.o: workbench_big.c
	$(CC) -c $(CFLAGS) $(BIG_FLAGS) -DTRACKLOADER=0  workbench_big.c -o $*.o
	-@$(CC) -MM $(CFLAGS_CHECK) -DTRACKLOADER=1 workbench_big.c -o $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
ifeq ($(KEEP_ASM), 1)
	-@$(CC) -c $(CFLAGS) $(BIG_FLAGS) -DTRACKLOADER=0  workbench_big.c -S -o out/workbench/big.s
endif
	-@#rm workbench_big.c

out/adf/%.o: %.c
ifeq ($(KEEP_ASM),1)
	$(CC) -S $(CFLAGS_ASM_OUTPUT) -DTRACKLOADER=1 $< -o out/adf/$*.s 
endif
ifeq ($(USE_BIG), 1)	
	$(CC) -c $(CFLAGS_CHECK) -DTRACKLOADER=1  $< -o out/adf/$*.o
else
	$(CC) -c $(CFLAGS) -DTRACKLOADER=1  $< -o out/adf/$*.o
endif
	@$(CC) -MM $(CFLAGS_CHECK) -DTRACKLOADER=1  $< -o out/adf/$*.d
	@mv -f out/adf/$*.d $*.d.tmp
	@sed -e 's|.*:|out/adf/$*.o:|' < $*.d.tmp > out/adf/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/adf/$*.d
	@rm -f $*.d.tmp

out/workbench/%.o: %.c
ifeq ($(USE_BIG), 1)	
	$(CC) -c $(CFLAGS_CHECK) -DTRACKLOADER=0  $< -o out/workbench/$*.o
else
	$(CC) -c $(CFLAGS) -DTRACKLOADER=0  $< -o out/workbench/$*.o
endif
	@$(CC) -MM $(CFLAGS_CHECK) -DTRACKLOADER=0  $< -o out/workbench/$*.d
	@mv -f out/workbench/$*.d $*.d.tmp
	@sed -e 's|.*:|out/workbench/$*.o:|' < $*.d.tmp > out/workbench/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/workbench/$*.d
	@rm -f $*.d.tmp
	-@$(CC) -S $(CFLAGS) -DTRACKLOADER=0  $< -o out/workbench/$*.s 

out/workbench/%.o: %.s
	$(VASM)  -DTRACKLOADER=0 $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make  -DTRACKLOADER=1 $(VASM_ARGS) $(INCLUDE) $< -o $@  > out/workbench/$*.d

out/main.bin: out/adf/main.o $(ADF_OBJS)
	$(LD) $(LINKER_OPTIONS) -brawbin1 $< $(ADF_OBJS) $(C_LIBS) -o $@ 
	@$(LD) $(LINKER_OPTIONS) -brawbin1 $< $(ADF_OBJS) $(C_LIBS) -o $@  -M -o /tmp/main.bin > out/adf/main.txt
	@$(LD) $(LINKER_OPTIONS) -brawbin1 $< $(ADF_OBJS) $(C_LIBS) -M -o /tmp/main.bin | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > $@.symbols
	@cp $@.symbols ~/Projects/amiga/debugger.syms
	@echo "RAM USAGE:" $$((16#`cat ~/Projects/amiga/debugger.syms | grep endRam | sed 's/0x//' | sed 's/endRam: //'`)) bytes
ifeq ($(USE_BIG), 1)	
	-@#rm out/adf/big.o
endif

bin/$(GAME_NAME): bin $(ALL_TOOLS)  $(ALL_WORKBENCH_OBJS) 
	$(LD) $(C_LIBS_WB) -hunkattr random_c=2 -hunkattr bss_c=2 -hunkattr code_c=2 -hunkattr data_c=2  $(ALL_WORKBENCH_OBJS) -o $@
	@$(LD) $(C_LIBS_WB) -hunkattr random_c=2 -hunkattr bss_c=2 -hunkattr code_c=2 -hunkattr data_c=2  $(ALL_WORKBENCH_OBJS) -M -o /tmp/main.bin > out/workbench/newgame.txt
ifeq ($(USE_BIG), 1)	
	-@#rm out/workbench/big.o
endif
	cp $@ ~/A1200/BlockySkies
	cp $@ ~/A4000/BlockySkies
	rm -f bin/vegeta.adf
	../tools/amitools/xdftool.py bin/vegeta.adf format "vegeta" + write bin/newgame
	../tools/amitools/xdftool.py bin/vegeta.adf boot install
	../tools/amitools/xdftool.py bin/vegeta.adf makedir s
	../tools/amitools/xdftool.py bin/vegeta.adf write support/EndRun
	../tools/amitools/xdftool.py bin/vegeta.adf write support/startup-sequence s

        #cp assets/BlockySkies.info bin
        #cp assets/BlockySkiesDrawer.info bin
        #cp assets/BlockySkies.info ~/A1200
        #cp assets/BlockySkies.info ~/A4000
        #ls -lh bin/BlockySkies

.PHONY: clean
clean:	
	rm -rf out bin *~
	make -C libgcc clean
	make out

depend:
	echo $(ALL_DEPENDS)

-include $(ALL_DEPENDS)
$(ALL_OBJS): Makefile
$(ALL_WORKBENCH_OBJS): Makefile


include ../shared/run.mk
