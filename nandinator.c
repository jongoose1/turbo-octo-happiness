#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"
#include <math.h>

/* 
problem: local minimums at cost = 1.5, 1.0, and 0.5

it will find another minimum with some probability due to random mutation (it kinda just walks around at the min)
but may jump out near cost = 0, for the same reason D:

i could scale delta with cost to make that less likely
*/


#define DELTA 1
#define RATE 0.01
#define N 10000
double cost(network * net, int p){
	double c = 0, a, y;
	/* -1, -1 -> 1 */
	net->inputs[0] = -1.0;
	net->inputs[1] = -1.0;
	y = 1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf("-1, -1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;
	/* -1, 1 -> 1 */
	net->inputs[0] = -1.0;
	net->inputs[1] = 1.0;
	y = 1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf("-1, 1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;
	/* 1, -1 -> 1 */
	net->inputs[0] = 1.0;
	net->inputs[1] = -1.0;
	y = 1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf("1, -1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;
	/* 1, 1 -> -1 */
	net->inputs[0] = 1.0;
	net->inputs[1] = 1.0;
	y = -1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf("1, 1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;

	c =  c / 4.0;
	if(p) printf("total cost: %lf\n", c);
	return c;
}

int main(void){
	srand(time(NULL));
	double y2, y1;
	network * net = new_network(2, 1, 0);
	mutation * mut = new_mutation_for_network(net);
	char c;
	size_t i;
	int p;
	do {
		for(i = 0; i < N; i++){
			p = i == N - 1;
			y1 = cost(net, p);
			if(y1 == 0) break;
			randomize(mut);
			scale(mut, DELTA);
			mutate(net, mut);
			y2 = cost(net, p);
			if(invert(mut)) printf("inversion failed\n");
			scale(mut, y2-y1);
			descend(net, RATE, mut);
			
			if(p){
				printf("New Weights: \n");
				print_weights(net);
				print_input_weights(net);
				printf("New Biases: \n");
				print_biases(net);
			}
		}
		if(y1 == 0) break;
		printf("press c<enter> to continue\n");
		c = getchar();
		getchar();
	} while(c == 'c');
	if(y1 == 0){
		cost(net, 1);
		printf("Root Found !!\n");
		printf("Weights: \n");
		print_weights(net);
		print_input_weights(net);
		printf("Biases: \n");
		print_biases(net);
	}
	delete_network(net);
	delete_mutation(mut);
	return 0;
}
