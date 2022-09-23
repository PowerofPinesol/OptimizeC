
CPPFLAGS=-I./lib
CFLAGS=-Wall -Werror -pedantic -O3 $(shell pkg-config --cflags gtk+-3.0) 
LDFLAGS=-L./lib $(shell pkg-config --libs gtk+-3.0)   -g
LIBS= -llife -lm -lbsd


all: life

life: life.o 
	$(CC) $(CFLAGS) -o $@ $^  $(LIBS) -lm $(LDFLAGS)

life.o: life.c

check: all
	./life -nw

clean:
	@echo -n Cleaning...
	@rm  life *.o  *~ 2>/dev/null || true
	@echo done
