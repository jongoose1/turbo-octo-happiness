#include <stdio.h>
#include "network.h"

int main(void){
	network * net = new_network(1, 1, 1);
	mutation * mut = new_mutation_for_network(net);
	randomize(mut);
	mutate(net, mut);
	net->inputs[0] = 420.69;
	tick(net);

	print_weights(net);
	print_activations(net);
	print_biases(net);
	print_input(net);
	print_input_weights(net);

	save_network(net, "mynetwork.net");
	delete_network(net);

	network * net2 = load_network("mynetwork.net");

	print_weights(net2);
	print_activations(net2);
	print_biases(net2);
	print_input(net2);
	print_input_weights(net2);

	return 0;
}
