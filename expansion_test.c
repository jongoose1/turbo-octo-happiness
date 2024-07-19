#include "network.h"

#define HID 4
#define OUT 3
#define INP 2

int main(void){
	size_t i;
	network * net1 = new_network(INP,OUT,HID);
	network * net2 = new_network(INP,OUT,HID);
	network * net3 = new_network(INP,OUT,HID);
	for(i = 0; i < net1->oph*net1->oph; i++) {
		net1->weights[i] = (double) i;
		net2->weights[i] = (double) i;
		net3->weights[i] = (double) i;
	}
	for(i = 0; i < net1->input_size; i++) {
		net1->inputs[i] = (double) i;
		net2->inputs[i] = (double) i;
		net3->inputs[i] = (double) i;
	}
	for(i = 0; i < net1->oph; i++) {
		net1->biases[i] = (double) i;
		net2->biases[i] = (double) i;
		net3->biases[i] = (double) i;
	}
	for(i = 0; i < net1->oph * net1->input_size; i++) {
		net1->input_weights[i] = (double) i;
		net2->input_weights[i] = (double) i;
		net3->input_weights[i] = (double) i;
	}
	for(i = 0; i < net1->oph; i++) {
		net1->activations[i] = (double) i;
		net2->activations[i] = (double) i;
		net3->activations[i] = (double) i;
	}

	print_activations(net1);
	print_weights(net1);
	print_input_weights(net1);
	print_biases(net1);
	print_input(net1);
	
	expand_input(net1, 2);

	print_activations(net1);
	print_weights(net1);
	print_input_weights(net1);
	print_biases(net1);
	print_input(net1);
	
	expand_output(net2, 2);

	print_activations(net2);
	print_weights(net2);
	print_input_weights(net2);
	print_biases(net2);
	print_input(net2);
	
	expand_hidden(net3, 2);

	print_activations(net3);
	print_weights(net3);
	print_input_weights(net3);
	print_biases(net3);
	print_input(net3);

	return 0;
}
