#include "network.h"
#include <stdio.h>

int main(void){
	network * net = new_network(1,1,2);
	print_input(net);
	print_output(net);

	tick(net);
	print_input(net);
	print_output(net);

	net->inputs[0] = 1.0;
	net->input_weights[0] = 1.0;
	tick(net);
	print_input(net);
	print_output(net);
	
	print_activations(net);
	print_weights(net);
	expand_hidden(net, 3);
	print_activations(net);
	print_weights(net);
	printf("is  %ld\n", net->input_size);
	printf("os  %ld\n", net->output_size);
	printf("hs  %ld\n", net->hidden_size);
	printf("oph %ld\n", net->oph);
	tick(net);
	print_activations(net);
	return 0;
}
