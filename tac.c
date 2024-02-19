#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*calculates avg game length of tic tac toe, if u make illegal move game ends */ 

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
size_t play_game(void){
	size_t j, k;
	double state[9] = {0,0,0,0,0,0,0,0,0};
	for(j = 0; j < 5; j++){
		k = rand() % 9;
		if(state[k] != 0.0) return j * 2;
		state[k] = -1.0;
		if(detect_win(state) != 0.0) return j * 2 + 1;
		if(j == 4) return 9;

		k = rand() % 9;
		if(state[k]!=0.0) return j * 2 + 1;
		state[k] = 1.0;
		if(detect_win(state) != 0.0) return j * 2 + 2;
	}
	return 9;
}
int main(void){
	srand(time(NULL));
	size_t sum, games;
	sum = 0;
	games = 0;
	while(1){
		sum = sum + play_game();
		printf("%ld / %ld\n", sum, ++games);
	}
}
