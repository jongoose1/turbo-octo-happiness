#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/*
real -> real min finder
using gradient descent
*/

#define DELTA 0.1
#define RATE 0.1

double observe(double x){
	double y;
	printf("f(%lf) = ", x);
	scanf("%lf", &y);
	return y;
}

int main(int argc, char ** argv){
	srand(time(NULL));
	double guess = 0.0;
	double slope, y2, y1;
	while(1){
		y1 = observe(guess);
		y2 = observe(guess + DELTA);
		slope = (y2- y1) / DELTA;
		guess = guess - RATE * slope;
	}
}
