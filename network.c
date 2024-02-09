/*network.c*/

#include "network.h"
#include <math.h>

network * new_network(size_t input_size, size_t output_size, size_t hidden_size){
	size_t oph;
	network * net = malloc(sizeof(network));
	if(!net) return NULL;
	oph = output_size+hidden_size;
	net->biases = calloc(oph, sizeof(double));
	net->weights = calloc(oph*oph,sizeof(double));
	net->input_weights = calloc(input_size * oph,sizeof(double));
	net->activations = calloc(oph, sizeof(double));
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
	for(i = 0; i<net->input_size; i++) inputs[i] = 0.0;
	return 0;
}

int tick(network * net){
	size_t i, oph, j;
	oph = net->output_size + net->hidden_size;
	double * scratch = malloc(opt * sizeof(double));
	if(!scratch) return 1;
	for(i = 0; i < oph; i++) {
		scratch[i] = net->biases[i];
		for(j = 0; j < net->input_size; j++){
			scratch[i] = scratch[i] + net->inputs[i][j] * net->input_weights[i][j];
		}
		for(j = 0; j < oph; j++){
			scratch[i] = scratch[i] + net->activations[i]*net->weights[i][j];
		}
		scratch[i] = tanh(scratch[i]);
	}
	for(i = 0; i < oph; i++){
		activations[i] = scratch[i];
	}
	free(scratch);
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
