#Makefile 

# project name
TARGET = Regret

CC=gcc

# Compiling flags
CFLAGS = -Wall

# Proper directories 
SRCDIR = src
INCDIR = include

all: regret

regret : main input travel node
	$(CC)  $(CFLAGS) -o $@ $^  

#Run_program: program
#	./program

main: ${SRCDIR}/main.c input travel
	$(CC)  $(CFLAGS) -c $< -o $@ 

input: ${SRCDIR}/input.c ${INCDIR}/input.h
	$(CC)  $(CFLAGS) -c $< -o $@

travel: ${SRCDIR}/travel.c ${INCDIR}/travel.h node 
	$(CC)  $(CFLAGS) -c $< -o $@
	
node: ${SRCDIR}/node.c ${INCDIR}/node.h
	$(CC)  $(CFLAGS) -c $< -o $@
	
clean:
	rm -Rf *~ regret main input travel node *.o
