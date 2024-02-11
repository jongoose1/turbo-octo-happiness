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
	int r;
	double * scratch = malloc(net->oph * sizeof(double));
	r = scratch_tick(net, scratch);
	free(scratch);
	return r;
}

int scratch_tick(network * net, double * scratch){
	if(!scratch) return 1;
	size_t i, j;
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

int print_activations(network * net){
	size_t i;
	for(i = 0; i < net->oph; i++){
		printf("%lf\n", net->activations[i]);
	}
	printf("\n");
	return 0;
}

int print_weights(network * net){
	size_t i, j;
	for(i = 0; i < net->oph; i++){
		for(j = 0; j < net->oph; j++){
			printf("%lf ", net->weights[i*net->oph + j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int expand_hidden(network * net, size_t h){
	/* adds h hidden neurons */
	if(h == 0) return 0;
	
	network tmp;
	size_t i, j;

	tmp.input_size = net->input_size;
	tmp.output_size = net->output_size;
	tmp.hidden_size = net->hidden_size + h;
	tmp.oph = net->oph + h;
	tmp.biases = realloc(net->biases,tmp.oph * sizeof(double));
	if (!tmp.biases) return 1;
	tmp.weights = calloc(tmp.oph * tmp.oph, sizeof(double));
	if (!tmp.weights) return 1;
	tmp.input_weights = realloc(net->input_weights, tmp.oph*tmp.input_size * sizeof(double));
	if (!tmp.input_weights) return 1;
	tmp.activations = realloc(net->activations,tmp.oph * sizeof(double));
	if (!tmp.activations) return 1;
	tmp.inputs = net->inputs;

	for (i = net->oph; i <tmp.oph;i++){
		tmp.biases[i] = 0.0;
		tmp.activations[i] = 0.0;
		for(j = 0; j < tmp.input_size;j++){
			tmp.input_weights[i*tmp.input_size + j] = 0.0;
		}
	}

	for(i = 0; i < net->oph; i++){
		for(j = 0; j < net->oph; j++){
			tmp.weights[i*tmp.oph + j] = net->weights[i*net->oph + j];
		}
	}
	free(net->weights);
	*net = tmp;
	return 0;
}


