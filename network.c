/*network.c*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "network.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

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

mutation * new_mutation(size_t input_size, size_t oph){
	mutation * mut = malloc(sizeof(mutation));
	if(!mut) return NULL;
	mut->input_size = input_size;
	mut->oph = oph;
	mut->input_weights = calloc(mut->input_size*mut->oph,sizeof(double));
	mut->biases = calloc(mut->oph,sizeof(double));
	mut->weights = calloc(mut->oph * mut->oph, sizeof(double));
	if(!mut->input_weights || !mut->biases || !mut->weights){
		free(mut->input_weights);
		free(mut->biases);
		free(mut->weights);
		free(mut);
		return NULL;
	}
	return mut;
}

mutation * new_mutation_for_network(network * net){
	return new_mutation(net->input_size, net->oph);
}

int randomize(mutation * mut){
	size_t i, j;
	double magnitude = 0;
	for(i = 0; i < mut->oph; i++){
		mut->biases[i] = gauss();
		magnitude = magnitude + mut->biases[i] * mut->biases[i];
		for(j = 0; j < mut->input_size; j++){
			mut->input_weights[i*mut->input_size + j] = gauss();
			magnitude = magnitude + 
			mut->input_weights[i*mut->input_size + j]*
			mut->input_weights[i*mut->input_size + j];
		}
		for(j = 0; j < mut->oph; j++){
			mut->weights[i*mut->oph + j] = gauss();
			magnitude = magnitude +
			mut->weights[i*mut->oph + j]*
			mut->weights[i*mut->oph + j];
		}
	}
	magnitude = sqrt(magnitude);

	/* normalize */
	scale(mut, 1.0 / magnitude);
	return 0;
}

int scale(mutation * mut, double scalar){
	size_t i, j;
	for(i = 0; i < mut->oph; i++){
		mut->biases[i] = mut->biases[i] * scalar;
		for(j=0;j<mut->input_size;j++){
			mut->input_weights[i*mut->input_size + j] =
			mut->input_weights[i*mut->input_size + j] *
			scalar;
		}
		for(j=0;j<mut->oph;j++){
			mut->weights[i*mut->oph + j] =
			mut->weights[i*mut->oph + j] *
			scalar;
		}
	}
	return 0;
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
int print_biases(network * net){
	size_t i;
	for(i = 0; i < net->oph; i++){
		printf("%lf\n", net->biases[i]);
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

int print_input_weights(network * net){
	size_t i, j;
	for(i = 0; i < net->oph; i++){
		for(j = 0; j < net->input_size; j++){
			printf("%lf ", net->input_weights[i*net->input_size + j]);
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
	tmp.weights = calloc(tmp.oph * tmp.oph, sizeof(double));
	tmp.input_weights = realloc(net->input_weights, tmp.oph*tmp.input_size * sizeof(double));
	tmp.activations = realloc(net->activations,tmp.oph * sizeof(double));
	if (!tmp.activations || !tmp.input_weights || !tmp.weights || !tmp.biases){
		free(tmp.activations);
		free(tmp.biases);
		free(tmp.input_weights);
		free(tmp.weights);
		return 1;
	}
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

int expand_input(network * net, size_t n){
	if(n==0) return 0;
	network tmp;
	size_t i, j;
	tmp.input_size = net->input_size + n;
	tmp.output_size = net->output_size;
	tmp.hidden_size = net->hidden_size;
	tmp.oph = net->oph;
	tmp.biases = net->biases;
	tmp.weights = net->weights;
	tmp.activations = net->activations;
	tmp.input_weights = calloc(tmp.oph*tmp.input_size, sizeof(double));
	tmp.inputs = realloc(net->inputs, tmp.input_size * sizeof(double));
	if(!tmp.input_weights || !tmp.inputs) {
		free(tmp.inputs);
		free(tmp.input_weights);
		return 1;
	}
	for(i = net->input_size; i < tmp.input_size;i++){
		tmp.inputs[i] = 0.0;
	}
	for(i = 0; i < net->oph; i++){
		for(j = 0; j < net->input_size; j++){
			tmp.input_weights[i*tmp.input_size + j] = net->input_weights[i*net->input_size + j];
		}
	}
	free(net->input_weights);
	*net = tmp;
	return 0;
}
int expand_output(network * net, size_t n){
	if(n==0) return 0;
	network tmp;
	size_t i, j;
	tmp.input_size = net->input_size;
	tmp.output_size = net->output_size + n;
	tmp.hidden_size = net->hidden_size;
	tmp.oph = net->oph + n;
	tmp.biases = calloc(tmp.oph, sizeof(double));
	tmp.weights = calloc(tmp.oph * tmp.oph, sizeof(double));
	tmp.activations = calloc(tmp.oph, sizeof(double));
	tmp.input_weights = calloc(tmp.oph*tmp.input_size, sizeof(double));
	tmp.inputs = net->inputs;
	if(!tmp.biases || !tmp.weights || !tmp.activations || !tmp.input_weights){
		free(tmp.biases);
		free(tmp.weights);
		free(tmp.activations);
		free(tmp.input_weights);
		return 1;
	}
	for(i = 0; i < net->output_size;  i++){
		tmp.biases[i] = net->biases[i];
		tmp.activations[i] = net->activations[i];
		for(j = 0; j < tmp.input_size; j++){
			tmp.input_weights[i*tmp.input_size + j] = net->input_weights[i*net->input_size + j];
		}
	}
	for(i = tmp.output_size; i < tmp.oph; i++){
		tmp.biases[i] = net->biases[i - n];
		tmp.activations[i] = net->activations[i - n];
		for(j = 0; j < tmp.input_size; j++){
			tmp.input_weights[i*tmp.input_size + j] = net->input_weights[(i - n)*net->input_size + j];
		}
	}
	for(i = 0; i < net->output_size; i++){
		for(j = 0; j < net->output_size; j++){
			tmp.weights[i*tmp.oph + j] = net->weights[i*net->oph + j];
		}
		for(j = tmp.output_size; j < tmp.oph; j++){
			tmp.weights[i*tmp.oph + j]= net->weights[i*net->oph + j - n];
		}
	}
	for(i = tmp.output_size; i < tmp.oph; i++){
		for(j = 0; j < net->output_size; j++){
			tmp.weights[i*tmp.oph + j] = net->weights[(i-n)*net->oph + j];
		}
		for(j = tmp.output_size; j < tmp.oph; j++){
			tmp.weights[i*tmp.oph + j]= net->weights[(i-n)*net->oph + j - n];
		}
	}
	free(net->biases);
	free(net->weights);
	free(net->activations);
	free(net->input_weights);
	*net = tmp;
	return 0;
}

int descend(network * net, double rate, mutation * nabla){
	if(!compatible(net, nabla)) return 1;
	size_t i, j;
	for(i = 0; i < net->oph; i++){
		net->biases[i] = net->biases[i] - rate * nabla->biases[i];
		for(j = 0; j < net->input_size; j++){
			net->input_weights[i*net->input_size + j] = net->input_weights[i*net->input_size + j] - rate * nabla->input_weights[i*net->input_size + j];
		}
		for(j = 0; j < net->oph; j++){
			net->weights[i*net->oph + j] = net->weights[i*net->oph + j] - rate * nabla->weights[i*net->oph + j];
		}
	}
	return 0;
}

int adjust(network * net, double delta){
	/* dont know when i would ever use this */
	size_t i, j;
	for(i = 0; i < net->oph; i++){
		net->biases[i] = net->biases[i] + delta;
		for(j = 0; j < net->input_size; j++){
			net->input_weights[i*net->input_size + j] = net->input_weights[i*net->input_size + j] + delta;
		}
		for(j = 0; j < net->oph; j++){
			net->weights[i*net->oph + j] = net->weights[i*net->oph + j] + delta;
		}
	}
	return 0;
}

int compatible(network * net, mutation * mut){
	return mut->oph==net->oph && mut->input_size==net->input_size;
}

int mutate(network * net, mutation * mut){
	if(!compatible(net, mut)) return 1;
	size_t i, j;
	for(i = 0; i < net->oph; i++){
		net->biases[i] = net->biases[i] + mut->biases[i];
		for(j = 0; j < net->input_size; j++){
			net->input_weights[i*net->input_size + j] =
			net->input_weights[i*net->input_size + j] +
			mut->input_weights[i*mut->input_size + j];
		}
		for(j = 0; j < net->oph; j++){
			net->weights[i*net->oph + j] =
			net->weights[i*net->oph + j] +
			mut->weights[i*mut->oph + j];
		}
	}
	return 0;
}


double gauss(void){
	double x,y;
	x = (double) rand() / RAND_MAX;
	y = (double) rand() / RAND_MAX;
	return sqrt(-2 * log(x)) * cos( 2 * M_PI * y);
}

int invert(mutation * mut){
	size_t i, j;
	int r = 0;
	for(i = 0; i < mut->oph; i++){
		if (mut->biases[i] == 0){
			r = 1;
		} else {
			mut->biases[i] = 1.0 / mut->biases[i];
		}
		for(j = 0; j < mut->input_size; j++){
			if(mut->input_weights[i*mut->input_size + j] ==0) {
				r = 1;
			} else {
				mut->input_weights[i*mut->input_size + j] = 1.0 /
				mut->input_weights[i*mut->input_size + j];
			}
		}
		for(j = 0; j < mut->oph; j++){
			if(mut->weights[i*mut->oph + j] == 0){
				r = 1;
			} else {
				mut->weights[i*mut->oph + j] = 1.0 /
				mut->weights[i*mut->oph + j];
			}
		}
	}
	return r;
}
