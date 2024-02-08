#include <math.h>

typedef struct neuron neuron;
typedef struct connection_list_node connection_list_node;
typedef connection_list_node cln;

struct neuron {
	double bias;
	double activation;
	connection_list_node * connections_head;
};
struct connection_list_node {
	double weight;
	neuron * from;
	connection_list_node * next;
};

double evaluate(neuron * n){
	/* tanh(sum(w*x) + b) */

	double a;
	int i;
	cln * curr = n->connections_head;
	a = n->bias;
	while(curr){
		a = a + curr->weight * curr->from->activation
		curr = curr->next;
	}

	a = tanh(a);
	*(n->activation) = a;
	return a;
}

cln * add_connection(neuron * from, neuron * to,double weight){
	/* caller frees */

	cln * new_conn = malloc(sizeof(cln));
	new_conn->weight = weight;
	new_conn->from = from;
	new_conn->next = to->connections_head;
	to->connections_head = new_conn;
}

/*remove*/
int main(void){
	return 0;
}
