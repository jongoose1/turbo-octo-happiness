#ifndef NETWORK2_H
#define NETWORK2_H

#include <stdlib.h>

typedef struct connection {
	size_t from;
	size_t to;
	double weight;
} connection;

typedef struct network {
	size_t size_neurons;
	size_t max_neurons;
	size_t size_connections;
	size_t max_connections;
	double * biases;
	double * left_activations;
	double * right_activations;
	connection * connections;
} network;


network * new_network(size_t size_neurons);
size_t push_neuron(network * net, double bias);
double pop_neuron(network * net);
size_t push_connection(network * net, size_t from, size_t to, double weight);
connection pop_connection(network * net);
connection remove_connection(network * net, size_t i);
void tick(network * net);
double average_connectivity(network * net);
double saturation(network * net);
int find_connection(network * net, size_t from, size_t to);

#endif
