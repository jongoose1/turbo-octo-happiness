CC := gcc
OUTS := opco inator neuron
CFLAGS := -O3 -flto -ansi -Wall
LDFLAGS := -O3 -Wall -flto
BINDIR := ~/bin

.PHONY: all
all: $(OUTS)

opco: opco.o
	$(CC) -o opco opco.o $(LDFLAGS)

opco.o: opco.c
	$(CC) -c -o opco.o opco.c $(CFLAGS)

inator: inator.o
	$(CC) -o inator inator.o $(LDFLAGS)

inator.o: inator.c
	$(CC) -c -o inator.o inator.c $(CFLAGS)

neuron: neuron.o
	$(CC) -o neuron neuron.o $(LDFLAGS)

neuron.o: neuron.c
	$(CC) -c -o neuron.o neuron.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(OUTS)

.PHONY: install
install: all
	mkdir -p $(BINDIR)
	install $(OUTS) $(BINDIR)

.PHONY: uninstall
uninstall:
	rm -f $(addprefix $(BINDIR)/, $(OUTS))
