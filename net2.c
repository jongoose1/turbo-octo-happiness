#include <stdio.h>
#include "network2.h"

void set_input(network * net, int c);

int main(void) {
	network *net; 
	int max, i, c;
	net = new_network(54);
	
	while(1) {
		c = getchar();
		if (c == '\n') {
			putchar('\n');
			continue;
		}
		set_input(net, c);
		tick(net);
		max = 27;
		for(i = 28; i < 54; i++)
			if (net->left_activations[i] > net->left_activations[max])
				max = i;
		if (max == 53)
			putchar(' ');
		else
			putchar('a' + max - 27);
	}
}

void set_input(network * net, int c){
	int i;
	for(i = 0; i < 27; i++)
		net->left_activations[i] = 0.0;
	if (c == ' ') 
		net->left_activations[26] = 1.0;
	else if (c >= 'a' && c <= 'z')
		net->left_activations[c-'a'] = 1.0;
	else if (c >= 'A' && c <= 'Z')
		net->left_activations[c-'A'] = 1.0;
	return;
}
