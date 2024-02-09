#include <time.h>
#include <float.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* 

real -> real min finder
gradient descent
with unreliable feedback
f(x) = (x^2 - x + 1) + gauss()

sample output
Iterations: 1000000000
Delta: 0.100000
Rate: 0.100000
Correct Guess: 0.500000
Average Guess: 0.449886
Minimum Guess: -14.291606
Maximum Guess: 14.906471
^ seems to hone in on about 0.45

min and max get further away with larger iterations, but curve flattens

smaller delta increases variance
dividing delta by 10 increases min and max by a factor of 10

starting guess doesnt affect avg guess significantly

large rate blows up everything

there is always a chance that the estimated slope will be huge
*/

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

#define DELTA 0.1
#define RATE 0.1

double gauss(void){
	double x,y;
	x = (double) rand() / RAND_MAX;
	y = (double) rand() / RAND_MAX;
	return sqrt(-2 * log(x)) * cos( 2 * M_PI * y);
}

double observe(double x){
	return x*x - x + 1 + gauss();
}

#define N 1000000000
int main(int argc, char ** argv){
	srand(time(NULL));
	double guess = 0.0;
	double slope, y2, y1;
	size_t i;
	double avg = 0,  min = DBL_MAX, max = DBL_MIN;
	for(i = 0; i < N; i++){
		y1 = observe(guess);
		y2 = observe(guess + DELTA);
		slope = (y2- y1) / DELTA;
		guess = guess - RATE * slope;

		avg = avg + guess;
		min = guess<min?guess:min;
		max = guess>max?guess:max;
	}
	avg = avg / N;
	printf("Iterations: %d\n", N);
	printf("Delta: %lf\n", DELTA);
	printf("Rate: %lf\n", RATE);
	printf("Correct Guess: %lf\n", 0.5);
	printf("Final Guess: %lf\n", guess);
	printf("Average Guess: %lf\n", avg);
	printf("Minimum Guess: %lf\n", min);
	printf("Maximum Guess: %lf\n", max);
	return 0;
}
