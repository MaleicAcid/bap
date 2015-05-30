SRCDIR=./src/
SOURCES:=$(wildcard $(SRCDIR)*.cc)

OBJDIR=./obj/
OBJECTS:=$(SOURCES:$(SRCDIR)%.cc=$(OBJDIR)%.o)

BINDIR=./bin/
TARGET=bercos

CC = g++
CFLAGS = -Wall -O3 -DDEBUG -ggdb -std=c++11 #-pg
LIBS = 
INCLUDE = -I./inc

all: $(OBJECTS)
	@echo Building executable
	@${CC} -o $(BINDIR)$(TARGET) ${CFLAGS} ${LIBS} $(OBJECTS)

$(OBJECTS): $(OBJDIR)%.o: $(SRCDIR)%.cc
	@echo Building $<
	@$(CC) $(INCLUDE) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(OBJDIR)*.o
	rm -f $(BINDIR)*
 
