#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"
#include <math.h>

/* 
nandinator , but using newtons method instead of gradient descent

newtons method cant iterate if the change in cost is less than the precision of doubles.
in this case, the size of the random step taken is doubled until a change in cost is noticed

this allows the network to launch out of local minimums, which doesnt happen in gradient descent
*/

#define N 100000
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
	if(p) printf("-1,  1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;
	/* 1, -1 -> 1 */
	net->inputs[0] = 1.0;
	net->inputs[1] = -1.0;
	y = 1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf(" 1, -1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;
	/* 1, 1 -> -1 */
	net->inputs[0] = 1.0;
	net->inputs[1] = 1.0;
	y = -1.0;
	tick(net);
	a = net->activations[0];
	if(p) printf(" 1,  1 -> %lf\n", a);
	c = c + fabs(y - a) * fabs(y - a) / 2;

	c =  c / 4.0;
	if(p) printf("total cost: %lf\n", c);
	return c;
}

int main(void){
	srand(time(NULL));
	double y2, y1, delta;
	network * net = new_network(2, 1, 0); /* set 100 hidden neurons for comically large brain, it stil works */
	mutation * mut = new_mutation_for_network(net);
	char c;
	size_t i;
	int p;
	size_t total = 0;
	do {
		for(i = 0; i < N; i++){
			p = (i == N - 1);
			if(p){
				printf("New Weights: \n");
				print_weights(net);
				print_input_weights(net);
				printf("New Biases: \n");
				print_biases(net);
			}
			y1 = cost(net, p);
			if (y1 ==0) break;
			delta = 1;
			randomize(mut);
			mutate(net, mut);
			y2 = cost(net, p);

			/* bit of a hack to make sure dy and nabla arent 0 so that newton can iterate */
			while (y2 == y1){
				scale(mut, -1);
				mutate(net, mut);
				randomize(mut);
				delta = delta * 2;
				scale(mut, delta);
				mutate(net, mut);
				y2 = cost(net,p);
			}
			/* end hack */

			if(p && nabla(mut, y2-y1)) printf("nabla failed\n");
			if(p){
				printf("nabla:\n");
				print_mut_weights(mut);
				print_mut_input_weights(mut);
				print_mut_biases(mut);
			}
			if(p && newton_iteration(net, mut, y2)) printf("newton failed\n");
			total = total + 1;
		}
		if (y1 ==0) break;
		printf("press c<enter> to continue\n");
		c = getchar();
		getchar();
	} while(c == 'c' && y1 != 0);
	
	if(y1 == 0){
		cost(net, 1);
		printf("Root Found !!\n");
		printf("Weights: \n");
		print_weights(net);
		print_input_weights(net);
		printf("Biases: \n");
		print_biases(net);
		printf("total_iters %ld\n",total );
	}
	delete_network(net);
	delete_mutation(mut);
	return 0;
}
