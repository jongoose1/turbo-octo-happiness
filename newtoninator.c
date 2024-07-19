#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/*
real -> real root finder
using slightly modified newtons method

we cant find slope exactly, so use 2 points and extrapolate that line to 0
repeat
can also use the average of the two points with estimated slope
*/

double observe(double x){
	double y;
	printf("f(%lf) = ", x);
	scanf("%lf", &y);
	return y;
}

int main(int argc, char ** argv){
	srand(time(NULL));
	double prev_guess = 0.0, guess = 1.0;
	double slope, y2, y1;
	y1 = observe(prev_guess);
	while(1){
		y2 = observe(guess);
		slope = (y2- y1) / (guess - prev_guess);
		prev_guess = guess;
		guess = guess - y2 / slope; 
		y1 = y2;
	}
}
