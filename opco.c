#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define N 10
#define S 100

/*
	1. define a set of behaviors, each with an equal number of shares
	2. choose a behavior randomly, with each behavior having probability proportional to its number of shares
	3. observe the positive or negative consequence (reinforcement or punishment)
	4. increase (or decrease) the number of shares of the chosen behavior proportionally to the consequence's severity, except, the number of shares cannot be decreased to zero
	5. goto 2
*/

int choose_behavior(int * B, int n, int total){
	if (total <= 0){
		int i;
		/*total not known/passed, must calculate*/
		total = 0;
		for (i =0;i<n;i++) total = total + B[i];
	}
	int random_share = rand() % total;
	int cumulative_shares = 0, index = -1;
	while (cumulative_shares <= random_share)
		cumulative_shares = cumulative_shares + B[++index];
	return index;
}

void adjust(int * B, int index, int consequence, int * total_p){
	int initial = B[index];
	B[index] = B[index] + consequence;
	if(B[index] <= 0) B[index] = 1;
	*total_p = *total_p + B[index] - initial;
}

int main(void){
	/* 1. define behaviors */
	int B[N];
	int i;
	for (i = 0;i<N;i++) B[i] = S;
	int total = N*S;
	srand(time(NULL));
	while (1){
		for(i = 0;i<N;i++) printf("B[%d] = %d\n", i, B[i]);
		printf("Press any key to continue");
		getchar();
		
		/* 2. choose behavior */
		int random_share = rand() % total;
		int cumulative_shares = 0, index = -1;
		while (cumulative_shares <= random_share)
			cumulative_shares = cumulative_shares + B[++index];

		printf("Random share chosen: %d\n", random_share);
		printf("Corresponding bahavior: %d\n", index);

		/*3. observe consequence*/
		int consequence = rand() % (S/10) - (S/10);

		printf("Consequence: %d\n", consequence);

		/*4. adjust probabilities*/
		int initial = B[index];
		B[index] = B[index] + consequence;
		if(B[index] <= 0) B[index] = 1;
		total = total + B[index] - initial;
		printf("\n");
	}
}
