PREFIX = /usr

CC = cc
CFLAGS 	= -std=c99 -pedantic -Wall -Werror

SRC = brainduck.c
OBJ = $(SRC:.c=.o)

brainduck: $(OBJ)
	$(CC) -o $@ $(OBJ)

$(OBJ):
%.o: %.c
	$(CC) -c $(CFLAGS) $<

install: brainduck
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f brainduck $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/brainduck

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/brainduck

clean:
	rm -f brainduck $(OBJ)

.PHONY: install uninstall clean