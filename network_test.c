#include "network.h"

int main(void){
	size_t i;
	network * net = new_network(1,1,1);
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
	
	for(i = 0; i < 3; i++) net->weights[i] = (double) i;
	print_activations(net);
	print_weights(net);
	expand_hidden(net, 3);
	print_activations(net);
	print_weights(net);
	tick(net);
	print_activations(net);
	return 0;
}
