OUTS := opco inator network_test gaussinator expansion_test chessinator \
nandinator newtoninator newtons_nandinator chessinator2 netio_test \
chessinator3 tac distance_test choose chessinator4 foodinator net2
DEPS := network neuron network2
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
distance_test.x: network.o
netio_test.x: network.o
expansion_test.x: network.o
chessinator.x: network.o
foodinator.x: network.o
chessinator2.x: network.o
chessinator3.x: network.o
chessinator4.x: network.o
nandinator.x: network.o
newtons_nandinator.x: network.o
memtest.x: network.o
net2.x: network2.o

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
