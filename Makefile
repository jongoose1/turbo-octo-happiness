OUTS := opco inator network_test gaussinator expansion_test
DEPS := neuron.h network.h
CC := gcc
CFLAGS := -O3 -flto -ansi -Wall
LDFLAGS := -O3 -Wall -flto
LDLIBS := -lm
#CFLAGS := -ansi -Wall -fsanitize=undefined -g
#LDFLAGS := -Wall -fsanitize=undefined
BINDIR := ~/bin
RM := rm -f

.PHONY: all
all: $(addsuffix .x, $(OUTS))

opco.x: opco.o
inator.x: inator.o
network_test.x: network_test.o network.o
expansion_test.x: expansion_test.o network.o
gaussinator.x: gaussinator.o

$(addsuffix .x, $(OUTS)):
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

$(OUTS): %: %.x
	cp $^ $@

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(addsuffix .x, $(OUTS))
	$(RM) $(OUTS)

.PHONY: install
install: $(OUTS)
	mkdir -p $(BINDIR)
	install $(OUTS) $(BINDIR)
	$(RM) $(OUTS)

.PHONY: uninstall
uninstall:
	$(RM) $(addprefix $(BINDIR)/, $(OUTS))
