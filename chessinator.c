#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"

/* square choosing game player */

/*
outputs are interpreted before tanh, so feedback can be more precise

would learn much faster with automized feedback based on data rather than my
guess about how well it played 
*/

#define DELTA 0.1
#define RATE 0.1

#define INP 9
#define OUT 9
#define HID 10

size_t player_input(void){
	size_t r;
	printf("choose a square: ");
	scanf("%ld", &r);
	return r;
}

double get_feedback(void){
	double r;
	printf("on a scale from 0-100 (0 = perfect), how bad did the computer play? ");
	scanf("%lf",&r);
	return r;
}

int play_game(network * net){
	size_t i, j, k;
	for(i = 0; i < 4; i++){
		/*players turn*/
		k = player_input();
		for(j =  0; j < INP; j++){
			if(j == k) net->inputs[j] = 1.0;
			else net->inputs[j] = -1.0;
		}
		/* computers turn */
		tick_output_z(net);
		/* dont tanh output, so we can give nuanced feedback */
		printf("Output z's: \n");
		print_output(net);
		k = 0;
		for(j = 1;j < OUT; j++){
			if (net->activations[j] > net->activations[k]) k = j;
		}
		printf("Therefore, computer chooses square %ld\n", k);
		tanh_output(net);
	}
	return 0;
}

int main(void){
	srand(time(NULL));
	double y2, y1;
	network * net = new_network(INP, OUT, HID);
	mutation * mut = new_mutation_for_network(net);
	while(1){
		printf("Game 1\n");
		play_game(net);
		y1 = get_feedback();
		printf("Adjusting strategy...\n");
		randomize(mut);
		scale(mut, DELTA);
		mutate(net, mut);
		printf("Game 2\n");
		play_game(net);
		y2 = get_feedback();
		if(invert(mut)) printf("inversion failed\n");
		scale(mut, y2-y1);
		printf("Descending...");
		descend(net, RATE, mut);
		printf("New Weights: \n");
		print_weights(net);
		print_input_weights(net);
		printf("New Biases: \n");
		print_biases(net);
	}
	return 0;
}
