CC=g++
CFLAGS=-c -Wall
INCDIR=-Iinclude
LDFLAGS=
OBJDIR=obj
SRCDIR=src
BINDIR=bin
SOURCES=main.cpp memcard.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=memrex
PATHOBJECTS=$(addprefix $(OBJDIR)/,$(OBJECTS))
PATHSOURCES=$(addprefix $(SRCDIR)/,$(SOURCES))

all: $(PATHSOURCES) $(EXECUTABLE)

debug: debugflags $(PATHSOURCES) $(EXECUTABLE)

clean:
	rm -f $(PATHOBJECTS) $(BINDIR)/$(EXECUTABLE)

debugflags:
	$(eval DEBUG := -g3 -gdwarf-2 -g -DDEBUG)

$(EXECUTABLE): $(PATHOBJECTS)
	$(CC) $(LDFLAGS) $(PATHOBJECTS) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(DEBUG) $< -o $@
