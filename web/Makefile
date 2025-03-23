TGTDIR  := out
TARGET  ?= snek.html

SHELL_FILE := snek_shell.html

SRCEXT  := .cpp
INCEXT  := .hpp
OBJEXT  := .o

SRCDIR  := ../src
OBJDIR  := build

EMFLAGS := -s USE_SDL=2 

CC      := em++
LD      := em++
CCMACRO ?=
LDFLAGS := $(EMFLAGS) --bind --use-preload-plugins --shell-file $(SHELL_FILE)
CCFLAGS := -g $(EMFLAGS)

SRCS  = $(shell find $(SRCDIR) -type f -name '*$(SRCEXT)')
OBJS  = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS:.cpp=.o))

all: $(TARGET) | $(TGTDIR)
	@mv $(TARGET) $(TGTDIR)/
	@mv $(TARGET:.html=.js) $(TGTDIR)/ 2>/dev/null || true
	@mv $(TARGET:.html=.wasm) $(TGTDIR)/ 2>/dev/null || true

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%$(OBJEXT): $(SRCDIR)/%$(SRCEXT) | $(OBJDIR)
	$(CC) $(CCFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TGTDIR):
	mkdir -p $(TGTDIR)

clean:
	rm -rf $(OBJDIR)/* $(TGTDIR)/*

.PHONY: all clean