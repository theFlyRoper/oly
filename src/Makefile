SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

DEFS =

OBJ = main.o \
			print_text_range.o \
			print_each_forward.o \
			print_each_backward.o \
			print_at.o \
			print_first.o \
			print_last.o \
			get_rules.o \
			xmalloc.o \
			xstrdup.o \
			error.o

LIB	= -licuio -licuuc -licudata

PROG = break

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

.c.o:
	$(CC) $(CFLAGS) $(DEFS) -c $<

.PHONY: all clean dev

clean:
	rm $(OBJ) $(PROG)

dev: DEFS += -DREVDEV 
dev: CFLAGS += -g 
dev: all
		
all: $(PROG)
