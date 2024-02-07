CC := gcc
OUTS := opco
CFLAGS := -O3 -flto -ansi -Wall
LDFLAGS := -O3 -Wall -flto
BINDIR := ~/bin

.PHONY: all
all: $(OUTS)

opco: opco.o
	$(CC) -o opco opco.o $(LDFLAGS)

inator: inator.o
	$(CC) -o inator inator.o $(LDFLAGS)

opco.o: opco.c
	$(CC) -c -o opco.o opco.c $(CFLAGS)

inator.o: inator.c
	$(CC) -c -o inator.o inator.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.0
	rm -f $(OUTS)

.PHONY: install
install: all
	mkdir -p $(BINDIR)
	install $(OUTS) $(BINDIR)

.PHONY: uninstall
uninstall:
	rm -f $(addprefix $(BINDIR)/, $(OUTS))
