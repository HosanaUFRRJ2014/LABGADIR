ARCH:=$(shell getconf LONG_BIT)
#ARCH:=32

# ---------------------------------------------------------------------
# Directories
# ---------------------------------------------------------------------

# Source codes
WKSPCDIR:=$(HOME)/Grafos_e_Algoritmos/aa
SRCDIR:=$(WKSPCDIR)/LABGADIR
GRAFALGDIR:=$(SRCDIR)/grafalg16.2
TRABSDIR:=$(GRAFALGDIR)
MAINLIBDIR:=$(SRCDIR)/lib
GRAPHDIR:=$(SRCDIR)/graph
HEAPDIR:=$(SRCDIR)/datast

# Object codes
LIBDIR=$(GRAFALGDIR)/lib

# Executable codes
BINDIR=$(GRAFALGDIR)/bin

# Source codes
INCTRABSDIR:=$(GRAFALGDIR)/include
SHARELIBDIR:=$(GRAFALGDIR)/lib

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

COMPILER= gcc
COPT=-m$(ARCH) -O -fpic -fexceptions -DNDEBUG -Wno-unused-result
DEFS= -D_ARCH$(ARCH)_
INCFLAGS = -I$(HEAPDIR) -I$(GRAPHDIR)
LIBFLAGS = -L$(SHARELIBDIR) -Wl,-rpath=$(SHARELIBDIR)

# ---------------------------------------------------------------------
# Entries
# ---------------------------------------------------------------------

# Build a list of the object files to create, based on the .cs found
AUTORDIRS:=$(patsubst $(GRAFALGDIR)/autor_%,autor_%,$(wildcard $(GRAFALGDIR)/autor_*))
AUTOROBJS:=$(patsubst $(GRAFALGDIR)/autor_%.c,$(LIBDIR)/autor_%_$(ARCH).o,$(wildcard $(GRAFALGDIR)/autor_*/trab*.c))

.SECONDEXPANSION:

$(AUTOROBJS):$(LIBDIR)/autor_%_$(ARCH).o: $$(GRAFALGDIR)/autor_$$*.c
	$(COMPILER) ${COPT} ${INCFLAGS} ${DEFS} -c $< -o $@

$(AUTORDIRS):autor_%: $(AUTOROBJS)
	$(COMPILER) ${COPT} $(LIBFLAGS) ${DEFS} $^ -o $(BINDIR)/$@_$(ARCH) -ltrab_main_$(ARCH)

alltrabs: $(AUTORDIRS)

clearall:
	rm $(BINDIR)/*
	rm $(AUTOROBJS)	
