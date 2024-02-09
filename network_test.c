#include "network.h"

int main(void){
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
	return 0;
}
