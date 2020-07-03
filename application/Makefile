IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined")
endif

CROSS?=RISCV32
all: dhrystone.$(CROSS).elf

-include $(IMPERAS_HOME)/lib/$(IMPERAS_ARCH)/CrossCompiler/$(CROSS).makefile.include
ifeq ($($(CROSS)_CC),)
    IMPERAS_ERROR := $(error "Please install the $(CROSS)_CC toolchain")
endif

dhrystone.$(CROSS).elf: dhrystone.$(CROSS).o
	$(V) echo "Linking Application $@"
	$(V) $(IMPERAS_LINK) -g -o $@  $^  $(IMPERAS_LDFLAGS)

dhrystone.$(CROSS).o: dhrystone.c
	$(V) echo "Compiling Application $@"
	$(V) $(IMPERAS_CC) -g -c  $(IMPERAS_CFLAGS) -D$(CROSS)  -o $@  $<

clean:
	- rm -f dhrystone.$(CROSS).elf dhrystone.$(CROSS).o
