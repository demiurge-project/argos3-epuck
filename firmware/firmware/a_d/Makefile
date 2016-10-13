# Makefile
# ==============================================================================
# 07.02.2002 cmc :  Created based on sample from Kevin Nickels.
# 07.15.2004 cmc :  Updated and applied to sn_sim project.
# 08.05.2004 cmc :  Added tags to removed files in distclean, added "doc" target
# 12.16.2004 cmc :  Remove output files with distclean.
# ------------------------------------------------------------------------------
#  TODO: Figure out why LD won't link C++ object files with standard args.
# ==============================================================================


# ==============================================================================
# Local variables
# ------------------------------------------------------------------------------
#  first, find all the pertinent filenames
ASMSRCS = $(wildcard *.s)
CSRCS = $(wildcard *.c)
SRCS = $(CSRCS) $(ASMSRCS)
HDRS = $(wildcard *.h) $(wildcard *.inc)
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CTAGS    = ctags
CC       = pic30-elf-gcc
LD 	 = pic30-elf-ld
ASM	 = pic30-elf-as
CFLAGS   = -g -Wall -mcpu=30F6014A 
CPPFLAGS = -I../ -I../std_microchip/support/h -I../std_microchip/include -I../uart -I../motor_led -I../I2C -I../codec -I../a_d 


# ==============================================================================
# Dependencies & rules
# ------------------------------------------------------------------------------
liba_d.a: $(OBJS)
	ar rsc $@ $(OBJS)

%.o: %.s
	$(ASM) -g -I../std_microchip/support/inc -I/usr/share/pic30-support/inc -p=30F6014A $< -o $@
# ==============================================================================
# Clean up directory
# ------------------------------------------------------------------------------
.PHONY:	clean
clean:	
	- $(RM) $(OBJS) *~ core.* *.rpo

.PHONY: distclean
distclean: clean
	- $(RM) $(DEPS) tags *.a *.so $(OUTPUT)
# ==============================================================================


# ==============================================================================
# make tags files for vim
# ------------------------------------------------------------------------------
tags:	$(SRCS) $(HDRS)
	$(CTAGS) $(SRCS) $(HDRS)
# ==============================================================================

# ==============================================================================
# end of Makefile
