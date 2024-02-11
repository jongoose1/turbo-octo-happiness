#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>

typedef struct network {
	size_t input_size;
	size_t output_size;
	size_t hidden_size;
	size_t oph;
	double * biases;        /* 1D, oph */
	double * weights;       /* 2D, oph*oph */
	double * input_weights; /* 2D, oph*input_size */
	double * activations;   /* 1D, oph */
	double * inputs;        /* 1D, input_size */
} network;

network * new_network(size_t input_size, size_t output_size, size_t hidden_size);

int clear_input(network * net);

int tick(network * net);

int scratch_tick(network * net, double * scratch);

int print_input(network * net);

int print_output(network * net);

int print_activations(network * net);

int print_weights(network * net);

int print_input_weights(network * net);

int print_biases(network * net);

int expand_hidden(network * net, size_t h);

int expand_input(network * net, size_t n);

int expand_output(network * net, size_t n);

#endif
