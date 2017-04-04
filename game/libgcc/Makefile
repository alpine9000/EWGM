C_SRCS=
SRCS=gcc.s

VASM=/usr/local/amiga/vgcc/bin/vasmm68k_mot
LD=/usr/local/amiga/vgcc/bin/vlink
EXTRA_DEFINES=

CC=/usr/local/amiga/vgcc/bin/m68k-amigaos-gcc
REGPARAM=-mregparm=2
#VASM_OPTS=-Wa,-no-opt
VASM_OPTS=-Wa,-opt-allbra -Wa,-opt-brajmp -Wa,-opt-lsl  -Wa,-opt-movem -Wa,-opt-mul -Wa,-opt-div -Wa,-opt-pea #-Wa,-opt-speed
C_OPT=-O3 
CFLAGS=$(C_OPT) $(REGPARAM) $(VASM_OPTS) -fomit-frame-pointer -noixemul -mcpu=68000 -m68000 -pedantic-errors -Wfatal-errors -Wall -Werror -Wextra -Wshadow -I /usr/local/amiga/m68k-amigaos/ndk/include/ $(EXTRA_DEFINES)
VASM_ARGS=-Fhunk -quiet -esc  -allmp
ASM_OBJS=$(SRCS:.s=.o)
C_OBJS=$(C_SRCS:.c=.o)
OBJS=$(ASM_OBJS) $(C_OBJS) 
ALL_OBJS=$(addprefix out/, $(OBJS)) 
ALL_DEPENDS=$(ALL_OBJS:.o=.d) 


all: out out/gcc.lib

out:
	mkdir out

out/gcc.lib: out $(ALL_OBJS)
	$(LD) $(ALL_OBJS) -r -o out/gcc.lib

out/%.o: %.s out
	$(VASM) $(VASM_ARGS)  $< -o $@ 


out/%.o: %.c out
	-@$(CC) -S $(CFLAGS) -DTRACKLOADER=1 -I$(AMIGA_INCLUDE) $< -o out/adf/$*.s 
	$(CC) -c $(CFLAGS) -DTRACKLOADER=1 -I$(AMIGA_INCLUDE) $< -o out/adf/$*.o
	@$(CC) -MM $(CFLAGS) -DTRACKLOADER=1 -I$(AMIGA_INCLUDE) $< -o out/adf/$*.d
	@mv -f out/adf/$*.d $*.d.tmp
	@sed -e 's|.*:|out/adf/$*.o:|' < $*.d.tmp > out/adf/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> out/adf/$*.d
	@rm -f $*.d.tmp

.PHONY: clean
clean:	
	rm -rf out bin *~

depend:
	echo $(ALL_DEPENDS)

-include $(ALL_DEPENDS)
$(ALL_OBJS): Makefile
