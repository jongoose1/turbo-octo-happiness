#include <math.h>
#include <stdlib.h>
#include "network2.h"

network * new_network(size_t size_neurons) {
	network * net =  calloc(1, sizeof(network));
	if(size_neurons == 0) return net;
	net->size_neurons = size_neurons;
	net->max_neurons = size_neurons;
	net->biases = calloc(size_neurons, sizeof(double));
	net->left_activations = calloc(size_neurons, sizeof(double));
	net->right_activations = calloc(size_neurons, sizeof(double));
	return net;
}

size_t push_neuron(network * net, double bias) {
	if (net->size_neurons >= net->max_neurons) {
		net->max_neurons += 10;
		net->biases = realloc(net->biases, net->max_neurons*sizeof(double));
		net->left_activations = realloc(net->left_activations, net->max_neurons*sizeof(double));
		net->right_activations = realloc(net->right_activations, net->max_neurons*sizeof(double));
	}
	net->biases[net->size_neurons] = bias;
	net->left_activations[net->size_neurons] = 0.0;
	net->right_activations[net->size_neurons] = 0.0;
	return ++net->size_neurons;
}

double pop_neuron(network * net){
	double bias = net->biases[net->size_neurons - 1];
	if (2 * net->size_neurons < net->max_neurons) {
		net->max_neurons /= 2;
		net->biases = realloc(net->biases, net->max_neurons*sizeof(double));
		net->left_activations = realloc(net->left_activations, net->max_neurons*sizeof(double));
		net->right_activations = realloc(net->right_activations, net->max_neurons*sizeof(double));
	}
	size_t i;
	for (i = 0; i < net->size_connections; i++){
		if (net->connections[i].to == net->size_neurons - 1 ||
			net->connections[i].from == net->size_neurons - 1) {
			remove_connection(net, i);
			i--;
		}
	}
	net->size_neurons--;
	return bias;
}

size_t push_connection(network * net, size_t from, size_t to, double weight) {
	/* do nothing if already exists */
	int c = find_connection(net, from, to);
	if (c != -1) return net->size_connections;
	if (net->size_connections >= net->max_connections) {
		net->max_connections += 100;
		net->connections = realloc(net->connections, net->max_connections*sizeof(connection));
	}
	connection new_connection;
	new_connection.to = to;
	new_connection.from = from;
	new_connection.weight= weight;
	net->connections[net->size_connections] = new_connection;
	return ++net->size_connections;
}

static void resize_connections(network * net) {
	if (2 * net->size_connections < net->max_connections) {
		net->max_connections/=2;
		net->connections = realloc(net->connections, net->max_connections*sizeof(connection));
	}
	return;
}

connection pop_connection(network * net) {
	resize_connections(net);
	return net->connections[--net->size_connections];
}

connection remove_connection(network * net, size_t i) {
	resize_connections(net);
	connection c = net->connections[i];
	for (;i < net->size_connections; i++) {
		net->connections[i] = net->connections[i + 1];
	}
	net->size_connections--;
	return c;
}

void tick(network * net) {
	size_t i;
	double *tmp;
	for(i = 0; i < net->size_neurons; i++) {
		net->right_activations[i] = net->biases[i];
	}
	for(i = 0; i < net->size_connections; i++) {
		net->right_activations[net->connections[i].to] +=
			net->connections[i].weight * net->left_activations[net->connections[i].from];
	}
	for(i = 0; i < net->size_neurons; i++) {
		net->right_activations[i] = tanh(net->right_activations[i]);
	}

	tmp = net->left_activations;
	net->left_activations = net->right_activations;
	net->right_activations = tmp;
	return;
}

double average_connectivity(network * net) {
	return (double) net->size_connections / net->size_neurons;
}

double saturation(network * net) {
	return average_connectivity(net) / net->size_neurons;
}

int find_connection(network * net, size_t from, size_t to){
	size_t i;
	for (i = 0; i < net->size_connections; i++)
		if (net->connections[i].to == to && net->connections[i].from == from)
			return i;
	return -1;
}
