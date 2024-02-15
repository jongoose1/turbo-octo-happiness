#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"

/* tic tac toe */

/*
outputs are interpreted before tanh, so feedback can be more precise

INPUT: 9 neurons with the following states representing the current state of the board
	-1 = X
	 0 = unplayed
	 1 = O

OUTPUT: 9 neurons representing the best square for O to play
	-1 = dont play
	 1 = play

HIDDEN: learned without supervision

keep inputing current state to give thinking time

a little more supervied than i would like. we are keeping track of the state for the computer, instead of it learning to do that on its own

when thinking it will typically either reach equilibrium, or get stuck in a loop, longest loop period i had was 4
*/

#define DELTA 0.1
#define RATE 0.1

#define INP 9
#define OUT 9
#define HID 10

char dtc(double x){
	if(x== 1.0) return 'O';
	if(x==-1.0) return 'X';
	return ' ';
}

int print_state(double * state){
	size_t i, j;
	printf("-------\n");
	for(i = 0; i < 3; i++){
		printf("|");
		for(j = 0; j < 3; j++){
			printf("%c|", dtc(state[i*3+j]));
		}
		printf("\n");

	}
	printf("-------\n");
	return 0;
}
size_t player_input(void){
	size_t r;
	printf("choose a square: ");
	scanf("%ld", &r);
	return r;
}

int detect_win(double * state){
	/* 0: no win
	   1: O wins
	   2: X wins */
	size_t i;
	for(i =0; i < 3; i++){
		if(state[i] == state[i+3] && state[i] == state[i+6]){
			if(state[i] == 1.0) return 1;
			if(state[i] == -1.0) return 2;
		}
		if(state[i*3] == state[i*3 + 1] && state[i*3] == state[i*3 + 2]){
			if(state[i*3] == 1.0) return 1;
			if(state[i*3] == -1.0) return 2;
		}
	}
	if((state[4] == state[0] && state[4] == state[8])|| (state[4] == state[2] && state[4] == state[6])){
		if(state[4] ==1.0) return 1;
		if(state[4] == -1.0) return 2;
	}
	return 0;
}

double get_feedback(void){
	double r;
	printf("on a scale from 0-100 (0 = perfect), how bad did the computer play? ");
	scanf("%lf",&r);
	return r;
}

int play_game(network * net){
	/* 1: player wins
	   2: computer wins */
	size_t i, k;
	char s[5];
	int winner, first = 1;
	clear_input(net);
	while(!(winner = detect_win(net->inputs))){
		print_state(net->inputs);
		/*computers turn first*/
		do {
			if(!first) tanh_output(net);
			tick_output_z(net);
			printf("Output z's: \n");
			print_output(net);
			printf("keep thinking? (y/n): ");
			scanf("%s", s);
			first = 0;
		} while (s[0] == 'y' || s[0] == 'Y');
		k = 0;
		for(i = 1;i < 9; i++){
			if (net->activations[i] > net->activations[k]) k = i;
		}
		tanh_output(net);
		printf("Therefore, computer chooses square %ld\n", k);
		if(net->inputs[k] != 0.0) {
			printf("Computer made an illegal move\n");
			return 1;
		}
		net->inputs[k] = -1.0;
		print_state(net->inputs);
		/*players turn*/
		do {
			k = player_input();
		} while(net->inputs[k] != 0.0);
		net->inputs[k] = 1.0;
	}
	if(winner == 1) printf("Player wins!\n");
	if(winner == 2) printf("Computer wins!\n");
	return winner;
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
