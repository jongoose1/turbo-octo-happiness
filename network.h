#ifndef NETWORK_H
#define NETWORK_H

typedef struct network {
	size_t input_size;
	size_t output_size;
	size_t hidden_size;
	double * biases;        /* 1D, oph */
	double * weights;       /* 2D, oph*oph */
	double * input_weights; /* 2D, input_size*oph */
	double * activations;   /* 1D, oph */
	double * inputs;        /* 1D, input_size */
} network;

network * new_network(size_t input_size, size_t output_size, size_t hidden_size);

int tick(network * net);

#endif
