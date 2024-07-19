#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"

/* tic tac toe */

/*
evolution simulator

outputs are interpreted before tanh, so feedback can be more precise

INPUT: 2 maps representing the current state of the board
	0-8: -1: not X, 1: X
	9-17: -1: not O, 1: O

OUTPUT: 9 neurons representing the best square for O to play
	-1 = dont play
	 1 = play

HIDDEN: learned without supervision

keep inputing current state to give thinking time

1. population of N networks compete for shares
2. N networks are selected to reproduce with mutation, with each network having probability of being selected proportional to its number of shares (duplicates are given different mutations)
3. goto 1

also mutates size

*/

#define DELTA 10
#define HID 30
#define N 1000
#define ROUNDS 10000
#define GENERATIONS 1
#define THINK_TICKS 2
int flip_board(double * state){
	size_t i;
	for(i = 0; i<9; i++) state[i] = -1 * state[i];
	return 0;
}
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

int detect_win(double * state){
	/* -1: X wins
	    0: no win
	    1: O wins */
	size_t i;
	for(i =0; i < 3; i++){
		if(state[i] == state[i+3] && state[i] == state[i+6]){
			if(state[i] == 1.0) return 1;
			if(state[i] == -1.0) return -1;
		}
		if(state[i*3] == state[i*3 + 1] && state[i*3] == state[i*3 + 2]){
			if(state[i*3] == 1.0) return 1;
			if(state[i*3] == -1.0) return -1;
		}
	}
	if((state[4] == state[0] && state[4] == state[8])|| (state[4] == state[2] && state[4] == state[6])){
		if(state[4] ==1.0) return 1;
		if(state[4] == -1.0) return -1;
	}
	return 0;
}

int play_game(network * net1, network * net2, size_t * length){
	/* -1: net1 wins
	    0: tie
	    1: net2 wins */
	size_t i, k, j;
	int winner;
	/* state: -1 == X, 0 == empty, 1 == O */
	double state[9] = {0, 0, 0,
			   0, 0, 0,
			   0, 0, 0};
/*	print_state(state);*/
	for(j = 0; j < 5; j++){
		/* net1 turn */
		/*network doesnt know which side its playing, so we'll flip the board for X */
		flip_board(state);
		for(i = 0; i < 18; i++) net1->inputs[i] = -1.0;
		for(i = 0; i < 9; i++){
			if(state[i] == -1.0) net1->inputs[i] = 1.0;
			if(state[i] == 1.0) net1->inputs[i + 9] = 1.0;
		}
		for(i = 0; i < THINK_TICKS - 1; i++) tick(net1);
		tick_output_z(net1);
		k = 0;
		for(i = 1;i < 9; i++){
			if (net1->activations[i] > net1->activations[k]) k = i;
		}
		tanh_output(net1);
/*		printf("X chooses square %ld\n", k);*/
		if(state[k] != 0.0) {
/*			printf("X made an illegal move\n");*/
/*			printf("O wins!\n");*/
			*length = j * 2;
			return 1;
		}
		flip_board(state);
		state[k] = -1.0;
/*		print_state(state);*/
		winner = detect_win(state);
		if(winner == 1){
/*			printf("O wins!\n");*/
			*length = j * 2 + 1;
			return 1;
		} else if (winner == -1){
/*			printf("X wins!\n");*/
			*length = j * 2 + 1;
			return -1;
		}
		if(j == 4) {
/*			printf("TIE!\n");*/
			*length = 9;
			return 0;
		}

		/* net2 turn */
		for(i = 0; i < 18; i++) net2->inputs[i] = -1.0;
		for(i = 0; i < 9; i++){
			if(state[i] == -1.0) net2->inputs[i] = 1.0;
			if(state[i] == 1.0) net2->inputs[i + 9] = 1.0;
		}
		for(i = 0; i < THINK_TICKS - 1; i++) tick(net2);
		tick_output_z(net2);

		k = 0;
		for(i = 1;i < 9; i++){
			if (net2->activations[i] > net2->activations[k]) k = i;
		}
		tanh_output(net2);
/*		printf("O chooses square %ld\n", k);*/
		if(state[k] != 0.0) {
/*			printf("O made an illegal move\n");*/
/*			printf("X wins!\n");*/
			*length = j * 2 + 1;
			return -1;
		}
		state[k] = 1.0;
/*		print_state(state);*/
		winner = detect_win(state);
		if(winner == 1){
/*			printf("O wins!\n");*/
			*length = j * 2 + 2;
			return 1;
		} else if (winner == -1){
/*			printf("X wins!\n");*/
			*length = j * 2 + 2;
			return -1;
		}

	}
/*	printf("TIE!\n");*/
	*length = 9;
	return 0;
}

size_t select(size_t * shares, size_t n){
	size_t i, total = 0, random_share, cumulative_shares = 0, index = 0;
	for(i = 0; i < n; i++) total = total + shares[i];
	random_share = rand() % total;
	while(cumulative_shares <= random_share)
		cumulative_shares = cumulative_shares + shares[index++];
	return index - 1; 
}


int main(void){
	srand(time(NULL));
	size_t i, a, b, selection, gen = 0, length;
	double avg;
	char c = 'c';
	int x;
	network * nets[N];
	network * scratch[N];
	size_t shares[N];
	for(i = 0; i < N; i++){
		nets[i] = new_network(18,9,HID);
		if(!nets[i]) {
			printf("Failed to allocate networks\n");
			return 1;
		}
	}
	do {

		avg = 0;
		/* 1. compete for shares */
		for(i = 0; i < N; i++) shares[i] = 1;
		for(i = 0; i < ROUNDS; i++) {
/*			printf("Gen #%ld Round #%ld\n",gen ,i);*/
			a = rand() % N;
			while((b = rand() % N) == a);
/*			printf("Network %ld VS Network %ld\n", a, b);*/
			x = play_game(nets[a], nets[b], &length);
			avg = avg + length;
			switch(x){
				case -1:
					shares[a]++;
/*					printf("Network %ld wins!\n", a);*/
					break;
				case 1:
					shares[b]++;
/*					printf("Network %ld wins!\n", b);*/
					break;
			}
		}
		/* print distance matrix */
		/*
		size_t j;
		printf("Distances:\n");
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
				printf("%lf ", distance(nets[i], nets[j]));
			}
			printf("\n");

		}
		*/
		/* 2. selection */
		double avg_size = 0;
		for(i = 0; i < N; i++){
			/*printf("Shares[%ld] = %ld\n", i, shares[i]);*/
			avg_size = avg_size + nets[i]->hidden_size;
		}
		avg_size = avg_size / N;
/*		printf("\n");*/
		for(i = 0; i < N; i++){
			selection = select(shares, N);
			/*printf("Network %ld has been selected\n", selection);*/
			scratch[i] = copy_network(nets[selection]);
			if(!scratch[i]){
				printf("Failed to copy networks\n");
				return 1;
			}
			int expand = rand() % 2;
			int diff = 0.01 * scratch[i]->hidden_size;
			if (diff < 1) diff = 1;
			if(expand) expand_hidden(scratch[i], diff);
			else contract_hidden(scratch[i], diff);
			mutation * mut = new_mutation_for_network(scratch[i]);
			if(!mut) {
				printf("Failed to allocate mutation\n");
				return 1;
			}
			randomize(mut);
			scale(mut, DELTA);
			mutate(scratch[i], mut);
			delete_mutation(mut);
		}
		for(i = 0; i < N; i++){
			delete_network(nets[i]);
			nets[i] = scratch[i];
		}
		avg = avg / ROUNDS;
		printf("Thus ends generation:                %ld\n", gen);
		printf("Average game length this generation: %lf\n", avg);
		printf("Average size:                        %lf\n", avg_size);
/*		if(gen % GENERATIONS == 0){*/
/*			printf("Press c<enter> to run more generations\n");*/
/*			c = getchar(); getchar();*/
/*		}*/
		gen++;
	} while (c == 'c');
	/*here*/
	size_t max_index = 0;
	for(i = 0; i < N; i++){
		if (shares[i] > shares[max_index]) max_index = i;
		delete_network(nets[i]);
	}
	return 0;
}
