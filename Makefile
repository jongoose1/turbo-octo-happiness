OUTS := opco inator network_test gaussinator expansion_test chessinator \
nandinator newtoninator newtons_nandinator chessinator2
DEPS := network neuron
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

network_test.x: network.o
expansion_test.x: network.o
chessinator.x: network.o
chessinator2.x: network.o
nandinator.x: network.o
newtons_nandinator.x: network.o

$(addsuffix .x, $(OUTS)): %.x: %.o

%.x: %.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(addsuffix .o, $(DEPS)): %.o: %.h

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
