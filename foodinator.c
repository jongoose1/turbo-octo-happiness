#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "network.h"

/*
see e/e2.txt
*/

#define DELTA 10
#define HID 30
#define N 1000
char dtc(double x){
	if(x== 1.0) return 'O';
	if(x==-1.0) return 'X';
	return ' ';
}

int print_state(double * state){
	size_t i, j;
	printf("---------------------\n");
	for(i = 0; i < 10; i++){
		printf("|");
		for(j = 0; j < 10; j++){
			printf("%c|", dtc(state[i*10+j]));
		}
		printf("\n");
	}
	printf("---------------------\n");
	return 0;
}

int find_food(network * net) {
	size_t i, k, j;
	int pos, food = 0;
	/* state: -1 == my position, 0, = empty, 1 == food */
	double state[100] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/* place food randomly */
	i = 0;
	while (i < 10) {
		pos = rand() % 100;
		if (state[pos] == 0) {
			state[pos] = 1;
			i++;
		}
	}
	/* place starting point randomly */
	do {
		pos = rand() % 100;
	} while (state[pos] == 1);
	state[pos] = -1;
/*	printf("Initial State:\n");*/
/*	print_state(state);*/
	/* initial activations = 0 */
	for(j = 0; j < net->oph; j++) net->activations[j] = 0;
	/* make moves */
	for(j = 0; j < 30; j++){
/*		printf("move #%ld\n", j+1);*/
		for(i = 0; i < 100; i++) net->inputs[i] = state[i];
		tick_output_z(net);
		k = 0;
		for (i = 1; i < net->output_size; i++) {
			if (net->activations[i] > net->activations[k]) k = i;
		}
		tanh_output(net);
/*		printf("net chooses to move ");*/
		switch(k) {
		case 0:
/*			printf("up\n");*/
			if (pos-10 < 0) {
/*				printf("... which is illegal, no move taken\n");*/
			} else {
				state[pos] = 0;
				pos = pos - 10;
				if (state[pos] == 1) {
					food++;
/*					printf("food found!\n");*/
				}
				state[pos] = -1;
			}
			break;
		case 1:
/*			printf("down\n");*/
			if (pos+10 > 99) {
/*				printf("... which is illegal, no move taken\n");*/
			} else {
				state[pos] = 0;
				pos = pos + 10;
				if (state[pos] == 1) {
					food++;
/*					printf("food found!\n");*/
				}
				state[pos] = -1;
			}
			break;
		case 2:
/*			printf("left\n");*/
			if (pos%10 == 0) {
/*				printf("... which is illegal, no move taken\n");*/
			} else {
				state[pos] = 0;
				pos = pos - 1;
				if (state[pos] == 1) {
					food++;
/*					printf("food found!\n");*/
				}
				state[pos] = -1;
			}
			break;
		case 3:
/*			printf("right\n");*/
			if (pos%10 == 9) {
/*				printf("... which is illegal, no move taken\n");*/
			} else {
				state[pos] = 0;
				pos = pos + 1;
				if (state[pos] == 1) {
					food++;
/*					printf("food found!\n");*/
				}
				state[pos] = -1;
			}
			break;
		}
/*		print_state(state);*/
	}
	
	return food;
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
	size_t i, selection, gen = 0;
	double avg;
	char c = 'c';
	int x;
	network * nets[N];
	network * scratch[N];
	size_t shares[N];
	for(i = 0; i < N; i++){
		nets[i] = new_network(100,4,HID);
		if(!nets[i]) {
			printf("Failed to allocate networks\n");
			return 1;
		}
	}
	do {

		avg = 0;
		/* 1. compete for shares */
		for(i = 0; i < N; i++) {
			x = find_food(nets[i]);
			avg = avg + x;
			shares[i] = 1 + x;
/*			printf("Gen #%ld Network #%ld found %d food.\n",gen ,i, x);*/
		}
		/* 2. selection */
		double avg_size = 0;
		for(i = 0; i < N; i++){
			avg_size = avg_size + nets[i]->hidden_size;
		}
		avg_size = avg_size / N;
		for(i = 0; i < N; i++){
			selection = select(shares, N);
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
		avg = avg / N;
		printf("Thus ends generation:                %ld\n", gen);
		printf("Average food found this generation:  %lf\n", avg);
		printf("Average size:                        %lf\n", avg_size);
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
