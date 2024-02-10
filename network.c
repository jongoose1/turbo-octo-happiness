/*network.c*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"

network * new_network(size_t input_size, size_t output_size, size_t hidden_size){
	network * net = malloc(sizeof(network));
	if(!net) return NULL;
	net->input_size = input_size;
	net->output_size = output_size;
	net->hidden_size = hidden_size;
	net->oph = output_size+hidden_size;
	net->biases = calloc(net->oph, sizeof(double));
	net->weights = calloc(net->oph*net->oph,sizeof(double));
	net->input_weights = calloc(input_size * net->oph,sizeof(double));
	net->activations = calloc(net->oph, sizeof(double));
	net->inputs = calloc(input_size, sizeof(double));
	if(!net->input_weights || !net->weights || !net-> biases || !net->activations || !net->inputs){
		free(net->input_weights);
		free(net->weights);
		free(net->biases);
		free(net->activations);
		free(net);
		free(net->inputs);
		return NULL;
	}
	return net;
}

int clear_input(network * net){
	size_t i;
	for(i = 0; i<net->input_size; i++) net->inputs[i] = 0.0;
	return 0;
}

int tick(network * net){
	double * scratch = malloc(net->oph * sizeof(double));
	scratch_tick(net, scratch);
	free(scratch);
	return 0;
}

int scratch_tick(network * net, double * scratch){
	size_t i, j;
	if(!scratch) return 1;
	for(i = 0; i < net->oph; i++) {
		scratch[i] = net->biases[i];
		for(j = 0; j < net->input_size; j++){
			scratch[i] = scratch[i] +
			net->inputs[j] * net->input_weights[i*net->input_size + j];
		}
		for(j = 0; j < net->oph; j++){
			scratch[i] = scratch[i] +
			net->activations[i]*net->weights[i*net->oph + j];
		}
		scratch[i] = tanh(scratch[i]);
	}
	for(i = 0; i < net->oph; i++){
		net->activations[i] = scratch[i];
	}
	return 0;
}

int print_input(network * net){
	size_t i;
	for(i = 0; i < net->input_size; i++){
		printf("%lf\n", net->inputs[i]);
	}
	printf("\n");
	return 0;
}

int print_output(network * net){
	size_t i;
	for(i = 0; i < net->output_size; i++){
		printf("%lf\n", net->activations[i]);
	}
	printf("\n");
	return 0;
}

/*
double * network_output(network * net, size_t returnSize){
	*returnSize = output_size;
	return net->activations;
}
*/

/*
int expand(netowrk * net, size_t neurons){
}
*/
