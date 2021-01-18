PROYECTO=principal
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm
CC=gcc
SRCC=$(wildcard *.c)
OBJS=$(SRCC:.c=.o)

all: $(PROYECTO)

%.o:	%.c
	$(CC) -c $< $(CFLAGS) 

$(PROYECTO): $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

uninstall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean:
	rm -f *.o $(PROYECTO)
