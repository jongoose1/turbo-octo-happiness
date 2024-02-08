#include <math.h>

typedef struct neuron neuron;
typedef struct connection_list_node connection_list_node;
typedef connection_list_node cln;

struct neuron {
	double bias;
	double activation;
	connection_list_node * connections;
};
struct connection_list_node {
	double weight;
	neuron * from;
	connection_list_node * next;
};

int main(void){
	return 0;
}
