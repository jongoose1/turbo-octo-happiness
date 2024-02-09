CC := gcc
OUTS := opco inator network_test
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

neuron.o: neuron.c
	$(CC) -c -o neuron.o neuron.c $(CFLAGS)

network.o: network.c
	$(CC) -c -o network.o network.c $(CFLAGS)

network_test: network_test.o network.o
	$(CC) -o network_test network_test.o network.o -lm $(LDFLAGS)
network_test.o: network_test.c
	$(CC) -c -o network_test.o network_test.c $(CFLAGS)


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
