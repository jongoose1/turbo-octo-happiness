#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "network.h"

int main(void){
	srand(time(NULL));
	network * net1 = new_network(1, 1, 1);
	network * net2 = new_network(1,1,1);
	mutation * mut = new_mutation_for_network(net1);
	randomize(mut);
	
	printf("mut_length(mut): %lf\n", mut_length(mut));
	printf("distance(net1, net2): %lf\n", distance(net1, net2));
	printf("Applying Mut1 to net1...\n");
	scale(mut, 2.0);
	mutate(net1, mut);
	printf("distance(net1, net2): %lf\n", distance(net1, net2));
	printf("Applying Mut2 to net2...\n");
	randomize(mut);
	mutate(net2, mut);
	printf("distance(net1, net2): %lf\n", distance(net1, net2));
	return 0;
}
