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

typedef struct mutation {
	size_t input_size;
	size_t oph;
	double * biases;        /* 1D, oph */
	double * weights;       /* 2D, oph*oph */
	double * input_weights; /* 2D, oph*input_size */
} mutation;

network * new_network(size_t input_size, size_t output_size, size_t hidden_size);

mutation * new_mutation(size_t input_size, size_t oph);

mutation * new_mutation_for_network(network * net);

int randomize(mutation * mut);

int scale(mutation * mut,  double scalar);

int clear_input(network * net);

int tick(network * net);

int scratch_tick(network * net, double * scratch);

int scratch_tick_output_z(network * net, double * scratch);

int tick_output_z(network * net);

int tanh_output(network * net);

int print_input(network * net);

int print_output(network * net);

int print_activations(network * net);

int print_weights(network * net);

int print_input_weights(network * net);

int print_biases(network * net);

int expand_hidden(network * net, size_t h);

int expand_input(network * net, size_t n);

int expand_output(network * net, size_t n);

int descend(network * net, double rate, mutation * nabla);

int adjust(network * net, double delta);

int compatible(network * net, mutation * mut);

int mutate(network * net, mutation * mut);

double gauss(void);

int invert(mutation * mut);

#endif
