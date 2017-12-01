# frequently changed config items
#KEEP_ASM=1
USE_BIG=1
SFX=1
PHOTON_TRACKLOADER=0
MUSIC_PARALLEL_LOAD=0
SCAN_CHECKER=0

ADF_DEFINES=-DTRACKLOADER=1 -DFASTRAM=0 -DGAME_CONFIG=CONFIG_OCS_512 -Iout/adf
FAST_DEFINES=-DTRACKLOADER=1 -DFASTRAM=1 -DGAME_CONFIG=CONFIG_OCS_FAST -DGAME_COMPRESS_DATA=1 -Iout/fast
WORKBENCH_DEFINES=-DTRACKLOADER=0 -DFASTRAM=0 -DGAME_CONFIG=CONFIG_WORKBENCH -DGAME_COMPRESS_DATA=1 -Iout/workbench

# programs and paths
CC=/usr/local/amiga/vgcc/bin/m68k-amigaosvasm-gcc
#CC=/usr/local/amiga/bebbo/bin/m68k-amigaosvasm-gcc
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
FAST=bin/$(GAME_NAME)_fast.adf
MODULE=$(GAME_NAME).s
PROGRAM_BIN=out/adf/main.bin
FAST_PROGRAM_BIN=out/fast/reloc.fast


# all the components that form the game
C_SRCS=init.c game.c gfx.c screen.c mouse.c tile.c palette.c level.c disk.c text.c sound.c music.c keyboard.c menu.c message.c record.c  string.c hw.c hiscore.c dos.c panic.c script.c object.c object_data.c sort.c player.c enemy.c logo.c fighter.c hand.c conductor.c alarm.c thing.c baloon.c level1.c level2.c level3.c level4.c star.c random.c player_select.c motorbike.c gunfighter.c
SRCS=blitmtext.s hw_asm.s trackloader.s palette_asm.s  P6112-Play.s music_asm.s keyboard_asm.s sg_trackdisk.s depacker_doynax.s
MODULES=P61.jojo_ingame.bin P61.intro.bin P61.jojo_boss.bin P61.level2.bin P61.level3.bin
SOUND_FILES=Terence_punch01.wav Terence_punch02.wav Bud_Punch01.wav enemy01.wav enemy02.wav enemy03.wav pop.wav coin1.wav rev.wav motorbike.wav shoot.wav baloon.wav
LEVELS=1 2 3 4
IMAGES=font8x8.png menu.png scoreboard.png logo.png player_select.png game_complete.png #popup.png


# program arguments
LINKER_OPTIONS=-T link.script.x

VASM_ARGS=-nowarn=62 -Fhunk -quiet -esc -DSFX=$(SFX) -allmp -DMAX_P61_SIZE=$(MAX_P61_SIZE) -DMUSIC_PARALLEL_LOAD=$(MUSIC_PARALLEL_LOAD) -DPHOTON_TRACKLOADER=$(PHOTON_TRACKLOADER)
VASM_C_ARGS=-ldots -Fhunk -quiet -no-opt

EXTRA_DEFINES=-DSFX=$(SFX) -DMAX_P61_SIZE=$(MAX_P61_SIZE) -DPHOTON_TRACKLOADER=$(PHOTON_TRACKLOADER) -DMUSIC_PARALLEL_LOAD=$(MUSIC_PARALLEL_LOAD)
REGPARAM=-mregparm=2
#VASM_OPTS=-Wa,-no-opt



ifeq ($(USE_BIG), 1)
WHOLE_PROGRAM=-fwhole-program
endif
C_OPT=-Ofast #-fbbb=sapcmfbvei #r #-fbbb=+v #-fbbb=iesapcmfbrv #-fbbb=+V #-fbbb=sapcmefbV
BIG_FLAGS=$(WHOLE_PROGRAM)
C_OPT_ASM_OUTPUT=-Ofast

ifeq ($(SCAN_CHECKER), 1)
WHOLE_PROGRAM=
VASM_OPTS=
BASE_CFLAGS=-Wno-overlength-strings $(REGPARAM) $(VASM_OPTS) -fomit-frame-pointer  -pedantic-errors -Wfatal-errors -Wall -Werror -Wextra -Wshadow -Wuninitialized  -Wundef -Wbad-function-cast -Wparentheses $(INCLUDE) $(EXTRA_DEFINES) -DSCAN_CHECKER
else
VASM_OPTS=-Wa,-opt-speed #-Wa,-opt-speed -Wa,-showcrit -Wa,-opt-allbra -Wa,-opt-brajmp -Wa,-opt-lsl  -Wa,-opt-movem -Wa,-opt-mul -Wa,-opt-div -Wa,-opt-pea #-Wa,-opt-speed
BASE_CFLAGS=-Wmissing-field-initializers -funsafe-loop-optimizations -Wno-overlength-strings $(REGPARAM) $(VASM_OPTS) -fomit-frame-pointer -noixemul -mcpu=68000 -m68000 -pedantic-errors -Wfatal-errors -Wall -Wunused-but-set-variable -Werror -Wextra -Wshadow -Wuninitialized -Wduplicated-cond -Wframe-larger-than=1563 -Wstack-usage=1563 -Wundef -Wbad-function-cast -Wparentheses -Wlogical-op -Wnull-dereference $(INCLUDE) $(EXTRA_DEFINES)
endif

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
LEVEL_FAST_BIN_FILES=$(addsuffix .bin,$(addprefix out/fast/level_c_data_, $(LEVELS))) $(addsuffix .bin,$(addprefix out/fast/level_f_data_, $(LEVELS)))
LEVEL_WB_BIN_FILES=$(addsuffix .bin,$(addprefix out/workbench/level_data_, $(LEVELS)))
MAX_P61_SIZE=$(shell ls -Sl assets/P61*.bin | awk '{print $$5}' | head -1)
MODULE_HEADERS=$(addprefix out/, $(MODULES:.bin=.lz.h))
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
ADF_LEVEL_UNCOMPRESSED=$(addsuffix .c,$(addprefix out/adf/leveldata_f_, $(LEVELS))) $(addsuffix .c,$(addprefix out/adf/leveldata_c_, $(LEVELS)))
ADF_OBJS=$(addprefix out/adf/, $(OBJS))
FAST_C_OBJS=$(addprefix out/fast/, $(C_OBJS))
FAST_LEVEL_OBJS=$(addsuffix .o,$(addprefix out/fast/leveldata_f_, $(LEVELS))) $(addsuffix .o,$(addprefix out/fast/leveldata_c_, $(LEVELS)))
FAST_OBJS=$(addprefix out/fast/, $(OBJS))
FAST_LEVEL_COMPRESSED=$(addsuffix .c,$(addprefix out/fast/leveldata_f_, $(LEVELS))) $(addsuffix .c,$(addprefix out/fast/leveldata_c_, $(LEVELS)))
WORKBENCH_C_OBJS=$(addprefix out/workbench/, $(C_OBJS))
WORKBENCH_LEVEL_COMPRESSED=$(addsuffix .c,$(addprefix out/workbench/leveldata_f_, $(LEVELS))) $(addsuffix .c,$(addprefix out/workbench/leveldata_c_, $(LEVELS)))
WORKBENCH_OBJS=$(addprefix out/workbench/, $(OBJS))
ALL_OBJS=out/adf/main.o $(ADF_OBJS)
ALL_FAST_OBJS=out/fast/main.o $(FAST_OBJS)
ALL_WORKBENCH_OBJS=out/workbench/main.o $(WORKBENCH_OBJS)
ALL_DEPENDS=$(ALL_OBJS:.o=.d) $(ALL_WORKBENCH_OBJS:.o=.d) $(ALL_FAST_OBJS:.o=.d)


# rules
all: | out fast wb #adf

include ../shared/externals.mk

adf: libgcc/out/gcc.lib version bin $(ALL_TOOLS) $(ADF)

fast: libgcc/out/gcc.lib version bin $(ALL_TOOLS) $(FAST)

.PHONY: version
version:
	@echo $(ALL_TOOLS)
	-@#./version/version.sh

gdrive: fast
	cp $(FAST) ~/Google\ Drive

.PHONY: emulate-adf
emulate-adf: adf
	cp $(ADF) ~/Projects/amiga/test.adf

.PHONY: emulate-fast
emulate-fast: fast
	cp $(FAST) ~/Projects/amiga/test.adf

libgcc/out/gcc.lib:
	make -C libgcc

.PHONY: test
test: test-fast #test-adf

.PHONY: test-adf
test-adf: emulate-adf
	cp adf_debugger.syms ~/Projects/amiga/debugger.syms
	-@rm -f ~/Documents/FS-UAE/Save\ States/A500/test.sdf
	cp test/test.lua default.lua
	cd test; rm -f screenshots; ln -sf adf_screenshots screenshots; cd ..
	 $(RUN_SCRIPT)
	-@grep FAIL lua.log || true
	@grep PASSED lua.log

.PHONY: test-fast
test-fast: emulate-fast
	cp fast_debugger.syms ~/Projects/amiga/debugger.syms
	-@rm -f ~/Documents/FS-UAE/Save\ States/A500/test.sdf
	cp test/test.lua default.lua
	cd test; rm -f screenshots; ln -sf fast_screenshots screenshots; cd ..
	 $(RUN_SCRIPT)
	-@grep FAIL lua.log || true
	@grep PASSED lua.log
	-@rm -f default.lua

go: emulate-adf
	cp adf_debugger.syms ~/Projects/amiga/debugger.syms
	#@rm -f default.lua
	 $(RUN_SCRIPT)

speed: emulate-fast
	cp fast_debugger.syms ~/Projects/amiga/debugger.syms
	-@rm -f ~/Documents/FS-UAE/Save\ States/A500/test.sdf
	cp test/speed.lua default.lua
	$(RUN_SCRIPT)

gf: emulate-fast
	cp fast_debugger.syms ~/Projects/amiga/debugger.syms
	#@rm -f default.lua
	 $(RUN_SCRIPT)

goaga: emulate-adf
	 $(A1200T_RUN_SCRIPT)

bin:
	mkdir bin

out:
	mkdir out
	mkdir out/adf
	mkdir out/fast
	mkdir out/workbench

$(ADF): out/bootblock.bin
	$(MAKEADF) out/bootblock.bin > $(ADF)
	@ls -lh out/bootblock.bin

$(FAST): out/fast/bootblock.bin
	$(MAKEADF) out/fast/bootblock.bin > $(FAST)
	@./disk_usage.sh

wb: | out version libgcc/out/gcc.lib $(ALL_TOOLS) bin/$(GAME_NAME)

out/adf/star.o: star.c out/star-sprite.h
out/adf/level.o: out/demo.h $(ADF_LEVEL_UNCOMPRESSED) level.c $(LEVEL_FILES)
out/fast/star.o: star.c out/star-sprite.h
out/fast/level.o: out/demo.h $(FAST_LEVEL_COMPRESSED) level.c
out/workbench/star.o: star.c out/star-sprite.h
out/workbench/level.o: out/demo.h $(WORKBENCH_LEVEL_COMPRESSED) level.c

out/baloon.raw: assets/baloon.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

out/shoot.raw: assets/shoot.wav
	sox  -R -v 1.2 $< -b 8 -c 1 -r 11025 -e signed $@

out/rev.raw: assets/rev.wav
	sox  -R -v 1.2 $< -b 8 -c 1 -r 11025 -e signed $@

out/motorbike.raw: assets/motorbike_2.wav
	sox  -R -v 1.0 $< -b 8 -c 1 -r 11025 -e signed $@

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
out/fast/sprite.o: $(HSPRITE_HEADERS)
out/workbench/sprite.o: $(HSPRITE_HEADERS)

out/background_%-map.c: assets/background_%.tmx #out
	$(MAPGEN) --c --depth=5 --input=$<
	@mv background_$*-map.c out
	@rm background_$*-indexes.c
	@rm background_$*-sprites.c

out/sprite_%.h: assets/sprite_%.png palettes/palette_%.pal
	$(CROPPA) --width=320 --height=1038 --input=assets/sprite_$*.png --output=out/sprite_$*-resized.png
	$(IMAGECON)  --quantize --colors=32 --full-color-palette --input=out/sprite_$*-resized.png --output=out/sprite_$* --output-bitplanes --output-png --output-palette --output-grey-palette-asm --output-palette-asm --use-palette=palettes/palette_$*.pal --output-mask
	$(IMAGECON) --onebitplane-mask --quantize --colors=32 --full-color-palette --input=out/sprite_$*-resized.png --output=out/sprite_1bp_$* --output-bitplanes --output-png --output-palette --output-grey-palette-asm --output-palette-asm --use-palette=palettes/palette_$*.pal --output-mask
	cat out/sprite_$*.bin | $(BIN2C) > out/sprite_$*.h
	cat out/sprite_$*-mask.bin | $(BIN2C) > out/sprite_$*-mask.h
	cat out/sprite_1bp_$*-mask.bin | $(BIN2C) > out/sprite_1bp_$*-mask.h


out/tile_%.h: assets/tile_%.png palettes/palette_%.pal
	$(IMAGECON) --quantize --colors=32 --full-color-palette --input=assets/tile_$*.png --output=out/tile_$* --output-bitplanes --output-png --output-palette --output-palette-asm --use-palette=palettes/palette_$*.pal
	cat out/tile_$*.bin | $(BIN2C) > out/tile_$*.h


out/level_recording_%.h: recordings/level%.raw
	echo recordings/level$*.raw
	cat recordings/level$*.raw | $(BIN2C) > out/level_recording_$*.h

out/demo.h: recordings/demo.raw
	echo recordings/demo.raw
	cat recordings/demo.raw | $(BIN2C) > out/demo.h

out/%.lz.h: assets/%.bin
	../tools/doynamite68k/out/lz -o out/$*.bin.lz assets/$*.bin
	cat out/$*.bin.lz | $(BIN2C) > out/$*.lz.h

out/%.h: assets/%.bin
	cat assets/$*.bin | $(BIN2C) > out/$*.h

out/adf/music.o: music.c $(MODULE_HEADERS)
out/fast/music.o: music.c $(MODULE_HEADERS)
out/workbench/music.o: music.c $(MODULE_HEADERS)

out/bootblock.bin: out/bootblock.o

out/game_complete.bin: assets/game_complete.png palettes/palette_4.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/game_complete.png --output=out/game_complete --output-bitplanes --output-png  --output-palette --use-palette=palettes/palette_4.pal
	../tools/doynamite68k/out/lz -o out/game_complete.bin.lz out/game_complete.bin

out/menu.bin: assets/menu.png palettes/menu.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/menu.png --output=out/menu --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/menu.pal
	../tools/doynamite68k/out/lz -o out/menu.bin.lz out/menu.bin

out/player_select.bin: assets/player_select.png palettes/player_select.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/player_select.png --output=out/player_select --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/player_select.pal
	../tools/doynamite68k/out/lz -o out/player_select.bin.lz out/player_select.bin

out/logo.bin: assets/logo.png palettes/logo.pal
	$(IMAGECON) --colors=16 --full-color-palette --input=assets/logo.png --output=out/logo --output-bitplanes --output-png  --output-palette --use-palette=palettes/logo.pal
	../tools/doynamite68k/out/lz -o out/logo.bin.lz out/logo.bin

out/scoreboard.bin: assets/scoreboard.png palettes/palette_1.pal
	$(IMAGECON) --colors=32 --full-color-palette --input=assets/scoreboard.png --output=out/scoreboard --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/palette_1.pal

#out/popup.bin: assets/popup.png palettes/default.pal
#	$(IMAGECON) --colors=32 --full-color-palette --input=assets/popup.png --output=out/popup --output-bitplanes --output-png --output-palette-asm --output-palette --use-palette=palettes/default.pal

out/bootblock.bin: out/bootblock.o
	$(LD) -brawbin1 $< -o $@ -M > out/adf/bootblock.txt

out/fast/bootblock.bin: out/fast/bootblock.o
	$(LD) -Tlink.bootblock.x -brawbin1 $< -o $@ -M > out/fast/bootblock.txt

out/font%.bin: assets/font%.png palettes/font.pal
	$(IMAGECON) --input $< $(IMAGECON_ARGS) --output-bitplanes --output-grey-palette-asm --output-palette-asm --output-palette --output out/font$* --colors=2 --use-palette=palettes/font.pal --full-color-palette-file --output-mask --transparent-color=0,0,0 --output-png


out/fast/level_c_data_1.o: $(LEVEL_FILES)
out/fast/level_f_data_1.o: $(LEVEL_FILES)
out/fast/level_c_data_2.o: $(LEVEL_FILES)
out/fast/level_f_data_2.o: $(LEVEL_FILES)
out/fast/level_c_data_3.o: $(LEVEL_FILES)
out/fast/level_f_data_3.o: $(LEVEL_FILES)
out/fast/level_c_data_4.o: $(LEVEL_FILES)
out/fast/level_f_data_4.o: $(LEVEL_FILES)
.SECONDARY : $(LEVEL_FAST_BIN_FILES)
out/fast/level_c_data_%.bin: out/fast/level_c_data_%.o
ifeq ($(SCAN_CHECKER), 1)
	echo "" > out/fast/level_c_data_$*.bin
else
	$(LD) -brawbin1 out/fast/level_c_data_$*.o -o out/fast/level_c_data_$*.bin
endif
out/fast/leveldata_c_%.c: out/fast/level_c_data_%.bin
	../tools/doynamite68k/out/lz -o out/fast/level_c_data_$*.bin.lz out/fast/level_c_data_$*.bin
	cat out/fast/level_c_data_$*.bin.lz | $(BIN2C) > $@
out/fast/level_f_data_%.bin: out/fast/level_f_data_%.o
ifeq ($(SCAN_CHECKER), 1)
	echo "" > out/fast/level_f_data_$*.bin
else
	$(LD) -brawbin1 out/fast/level_f_data_$*.o -o out/fast/level_f_data_$*.bin
endif
out/fast/leveldata_f_%.c: out/fast/level_f_data_%.bin
	../tools/doynamite68k/out/lz -o out/fast/level_f_data_$*.bin.lz out/fast/level_f_data_$*.bin
	cat out/fast/level_f_data_$*.bin.lz | $(BIN2C) > $@

out/workbench/level_data_f_1.o: $(LEVEL_FILES)
out/workbench/level_data_f_2.o: $(LEVEL_FILES)
out/workbench/level_data_f_3.o: $(LEVEL_FILES)
out/workbench/level_data_f_4.o: $(LEVEL_FILES)
out/workbench/level_data_c_1.o: $(LEVEL_FILES)
out/workbench/level_data_c_2.o: $(LEVEL_FILES)
out/workbench/level_data_c_3.o: $(LEVEL_FILES)
out/workbench/level_data_c_4.o: $(LEVEL_FILES)
.SECONDARY: $(LEVEL_WB_BIN_FILES)
out/workbench/level_c_data_%.bin: out/workbench/level_c_data_%.o
	$(LD) -brawbin1 out/workbench/level_c_data_$*.o -o out/workbench/level_c_data_$*.bin
out/workbench/level_f_data_%.bin: out/workbench/level_f_data_%.o
	$(LD) -brawbin1 out/workbench/level_f_data_$*.o -o out/workbench/level_f_data_$*.bin
out/workbench/leveldata_c_%.c: out/workbench/level_c_data_%.bin
	../tools/doynamite68k/out/lz -o out/workbench/level_c_data_$*.bin.lz out/workbench/level_c_data_$*.bin
	cat out/workbench/level_c_data_$*.bin.lz | $(BIN2C) > $@
out/workbench/leveldata_f_%.c: out/workbench/level_f_data_%.bin
	../tools/doynamite68k/out/lz -o out/workbench/level_f_data_$*.bin.lz out/workbench/level_f_data_$*.bin
	cat out/workbench/level_f_data_$*.bin.lz | $(BIN2C) > $@

out/adf/level_data_1.o: $(LEVEL_FILES)
out/adf/level_data_2.o: $(LEVEL_FILES)
out/adf/level_data_3.o: $(LEVEL_FILES)
out/adf/level_data_4.o: $(LEVEL_FILES)
out/adf/level_data_%.bin: out/adf/level_data_%.o
	$(LD) -brawbin1 out/adf/level_data_$*.o -o out/adf/level_data_$*.bin
out/adf/leveldata_%.c: out/adf/level_data_%.bin
	cat out/adf/level_data_$*.bin | $(BIN2C) > $@
	#../tools/doynamite68k/out/lz -o out/adf/level_data_$*.bin.lz out/adf/level_data_$*.bin
	#cat out/adf/level_data_$*.bin.lz | $(BIN2C) > $@

#./out/logo.pal: out/logo.bin
#./out/menu.pal: out/menu.bin

#out/bootblock_relocate.s: bootblock_relocate.c
#	$(CC) -fomit-frame-pointer -mregparm=2 -noixemul -mcpu=68000 -m68000 -pedantic-errors -Wfatal-errors -Wall -Werror -Wextra -Wshadow -Wuninitialized -Wduplicated-cond -Wundef -Wbad-function-cast -Wparentheses -Os -S bootblock_relocate.c -o out/bootblock_relocate.s

out/bootblock.o: bootblock_relocate.s $(BOOTBLOCK_ASM) $(PROGRAM_BIN)
	$(VASM) -no-opt -showopt $(VASM_ARGS) -DFASTRAM=0 -DUSERSTACK_ADDRESS="\$$$(USERSTACK_ADDRESS)" -DBASE_ADDRESS="\$$$(BASE_ADDRESS)"   $(BOOTBLOCK_ASM) -o $@ $(INCLUDE)

out/fast/bootblock.o: bootblock_relocate.s $(BOOTBLOCK_ASM) $(FAST_PROGRAM_BIN)
	$(VASM) -no-opt -showopt $(VASM_ARGS) -DFASTRAM=1 -DUSERSTACK_ADDRESS="\$$$(USERSTACK_ADDRESS)" -DBASE_ADDRESS="\$$$(BASE_ADDRESS)"   $(BOOTBLOCK_ASM) -o $@ $(INCLUDE)


out/adf/main.o: $(MODULE) $(EXTRA) $(ASSETS) $(IMAGEDATA)
	$(VASM)  $(ADF_DEFINES)  $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make  $(ADF_DEFINES) $(VASM_ARGS) $< -o $@ $(INCLUDE) > $*.d

out/fast/main.o: $(MODULE) $(EXTRA) $(ASSETS) $(IMAGEDATA)
	$(VASM)  $(FAST_DEFINES)  $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make $(FAST_DEFINES)  $(VASM_ARGS) $< -o $@ $(INCLUDE) > $*.d

out/workbench/main.o: $(MODULE) $(EXTRA) $(ASSETS)
	$(VASM)  $(WORKBENCH_DEFINES)  $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make  $(WORKBENCH_DEFINES) $(VASM_ARGS) $< -o $@ $(INCLUDE) > $*.d

out/adf/%.o: %.s
	$(VASM) $(VASM_ARGS) $(ADF_DEFINES) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make $(ADF_DEFINES) $(VASM_ARGS) $< -o $@ $(INCLUDE) > out/adf/$*.d

out/adf/%.o: out/%.c
ifeq ($(KEEP_ASM),1)
	-@$(CC) -S $(CFLAGS_ASM_OUTPUT) $(ADF_DEFINES) $< -o out/adf/$*.s
endif
	$(CC) -c $(CFLAGS) $(ADF_DEFINES) $< -o out/adf/$*.o

out/fast/%.o: %.s
	$(VASM) $(VASM_ARGS) $(FAST_DEFINES)  $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make $(FAST_DEFINES) $(VASM_ARGS) $< -o $@ $(INCLUDE) > out/fast/$*.d

out/fast/%.o: out/%.c
ifeq ($(KEEP_ASM),1)
	-@$(CC) -S $(CFLAGS_ASM_OUTPUT) $(FAST_DEFINES) $< -o out/fast/$*.s
endif
	$(CC) -E -c $(CFLAGS) $(FAST_DEFINES) $< -o out/fast/$*.o


out/workbench/%.o: out/%.c
	$(CC) $(CFLAGS) $(WORKBENCH_DEFINES) -c $< -o $@

adf_big.c: $(ADF_C_OBJS) $(C_SRCS)
	cat $(C_SRCS) > adf_big.c

out/adf/big.o: adf_big.c
	$(CC) -c $(CFLAGS) $(BIG_FLAGS) $(ADF_DEFINES) adf_big.c -o $*.o
	-@$(CC) -MM $(CFLAGS_CHECL) $(ADF_DEFINES) adf_big.c -o $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
ifeq ($(KEEP_ASM), 1)
	-@$(CC) -c $(CFLAGS) $(BIG_FLAGS) $(ADF_DEFINES) adf_big.c -S -o out/adf/big.s
endif
	-@#rm adf_big.c

fast_big.c: $(FAST_C_OBJS) $(C_SRCS)
	-@etags *.c *.h
	cat $(C_SRCS) > fast_big.c

out/fast/big.o: fast_big.c
	$(eval MAX_LEVEL_SIZE=$(shell ls -Sl out/fast/level*.bin.lz | awk '{print $$5}' | head -1))
	$(CC) -c -DMAX_LEVEL_SIZE=$(MAX_LEVEL_SIZE) $(CFLAGS) $(BIG_FLAGS) $(FAST_DEFINES) fast_big.c -o $*.o
	-@$(CC) -MM $(CFLAGS_CHECL) $(FAST_DEFINES) fast_big.c -o $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
ifeq ($(KEEP_ASM), 1)
	-@$(CC) -DMAX_LEVEL_SIZE=$(MAX_LEVEL_SIZE) -c $(CFLAGS) $(BIG_FLAGS)  $(FAST_DEFINES) fast_big.c -S -o out/fast/big.s
endif
	-@#rm fast_big.c


workbench_big.c: $(WORKBENCH_C_OBJS) $(C_SRCS)
	cat $(C_SRCS) > workbench_big.c

out/workbench/big.o: workbench_big.c
	$(eval MAX_LEVEL_SIZE=$(shell ls -Sl out/workbench/level*.bin.lz | awk '{print $$5}' | head -1))
	$(CC) -c -DMAX_LEVEL_SIZE=$(MAX_LEVEL_SIZE) $(CFLAGS) $(BIG_FLAGS) $(WORKBENCH_DEFINES)  workbench_big.c -o $*.o
	-@$(CC) -MM $(CFLAGS_CHECK)  $(WORKBENCH_DEFINES)  workbench_big.c -o $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
ifeq ($(KEEP_ASM), 1)
	-@$(CC) -DMAX_LEVEL_SIZE=$(MAX_LEVEL_SIZE) -c $(CFLAGS) $(BIG_FLAGS) $(WORKBENCH_DEFINES) workbench_big.c -S -o out/workbench/big.s
endif
	-@#rm workbench_big.c

out/adf/%.o: %.c
ifeq ($(KEEP_ASM),1)
	$(CC) -S $(CFLAGS_ASM_OUTPUT) $(ADF_DEFINES) $< -o out/adf/$*.s
endif
ifeq ($(USE_BIG), 1)
	$(CC) -c $(CFLAGS_CHECK) $(ADF_DEFINES)  $< -o out/adf/$*.o
else
	$(CC) -c $(CFLAGS) $(ADF_DEFINES)  $< -o out/adf/$*.o
endif
	@$(CC) -MM $(CFLAGS_CHECK) $(ADF_DEFINES)  $< -o out/adf/$*.d
	@mv -f out/adf/$*.d $*.d.tmp
	@sed -e 's|.*:|out/adf/$*.o:|' < $*.d.tmp > out/adf/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/adf/$*.d
	@rm -f $*.d.tmp


out/fast/%.o: %.c
ifeq ($(KEEP_ASM),1)
	$(CC) -S $(CFLAGS_ASM_OUTPUT) $(FAST_DEFINES) $< -o out/fast/$*.s
endif
ifeq ($(USE_BIG), 1)
	$(CC) -c $(CFLAGS_CHECK) $(FAST_DEFINES) $< -o out/fast/$*.o
else
	$(CC) -c $(CFLAGS) $(FAST_DEFINES)  $< -o out/fast/$*.o
endif
	@$(CC) -MM $(CFLAGS_CHECK) $(FAST_DEFINES) $< -o out/fast/$*.d
	@mv -f out/fast/$*.d $*.d.tmp
	@sed -e 's|.*:|out/fast/$*.o:|' < $*.d.tmp > out/fast/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/fast/$*.d
	@rm -f $*.d.tmp


out/workbench/%.o: %.c
ifeq ($(USE_BIG), 1)
	$(CC) -c $(CFLAGS_CHECK) $(WORKBENCH_DEFINES)  $< -o out/workbench/$*.o
else
	$(CC) -c $(CFLAGS) $(WORKBENCH_DEFINES) $< -o out/workbench/$*.o
endif
	@$(CC) -MM $(CFLAGS_CHECK) $(WORKBENCH_DEFINES)  $< -o out/workbench/$*.d
	@mv -f out/workbench/$*.d $*.d.tmp
	@sed -e 's|.*:|out/workbench/$*.o:|' < $*.d.tmp > out/workbench/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/workbench/$*.d
	@rm -f $*.d.tmp
	-@$(CC) -S $(CFLAGS) $(WORKBENCH_DEFINES)  $< -o out/workbench/$*.s

out/workbench/%.o: %.s
	$(VASM) $(WORKBENCH_DEFINES) $(VASM_ARGS) $< -o $@ $(INCLUDE)
	@$(VASM) -depend=make  $(WORKBENCH_DEFINES) $(VASM_ARGS) $(INCLUDE) $< -o $@  > out/workbench/$*.d

out/adf/main.bin: out/adf/main.o $(ADF_OBJS)
	$(LD) $(LINKER_OPTIONS) -brawbin1 $< $(ADF_OBJS) $(C_LIBS) -o $@ -M > out/adf/main.txt
	@cat out/adf/main.txt | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > $@.symbols
	@cp $@.symbols adf_debugger.syms
	@cp adf_debugger.syms debugger.syms
	./ram_usage.sh
ifeq ($(USE_BIG), 1)
	-@#rm out/adf/big.o
endif

out/fast/reloc.fast: out/fast/main.o $(FAST_OBJS)
	$(LD) -brawseg -q -Tlink.fastram.script.x $< $(FAST_OBJS) $(C_LIBS) -o out/fast/reloc -M > out/fast/reloc.txt
	@cat out/fast/reloc.txt | grep ", value " | cut -d " " -f3,7 | cut -d "," -f1 > $@.symbols
	@cp $@.symbols fast_debugger.syms
	@cp fast_debugger.syms debugger.syms
ifeq ($(USE_BIG), 1)
	-@#rm out/fast/big.o
endif

bin/$(GAME_NAME): bin $(ALL_TOOLS)  $(ALL_WORKBENCH_OBJS)
	$(LD)  $(C_LIBS_WB) -hunkattr bss_c=2 -hunkattr code_c=2 -hunkattr data_c=2  $(ALL_WORKBENCH_OBJS) -o $@ -M > out/workbench/newgame.txt #-s
ifeq ($(USE_BIG), 1)
	-@#rm out/workbench/big.o
endif
	-cp $@ ~/A1200/BlockySkies
	-cp $@ ~/A4000/BlockySkies
	#rm -f bin/vegeta.adf
	#-../tools/amitools/xdftool.py bin/vegeta.adf format "vegeta" + write bin/newgame
	#-../tools/amitools/xdftool.py bin/vegeta.adf boot install
	#-../tools/amitools/xdftool.py bin/vegeta.adf makedir s
	#-../tools/amitools/xdftool.py bin/vegeta.adf write support/EndRun
	#-../tools/amitools/xdftool.py bin/vegeta.adf write support/startup-sequence s
	@ls -l bin/$(GAME_NAME)

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
$(ALL_FAST_OBJS): Makefile
$(ALL_WORKBENCH_OBJS): Makefile


include ../shared/run.mk
