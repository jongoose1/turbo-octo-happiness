#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct neuron neuron;
typedef struct connection_list_node connection_list_node;
typedef struct neuron_list_node neuron_list_node;
typedef neuron_list_node nln;
typedef connection_list_node cln;

struct neuron {
	double bias;
	double activation;
	connection_list_node * head;
};

struct connection_list_node {
	double weight;
	neuron * from;
	connection_list_node * next;
};

struct neuron_list_node {
	neuron n;
	neuron_list_node * next;
};

struct network {
	size_t connections;
	size_t neurons;
	neuron_list_node * head;
};

double evaluate(neuron * n){
	/* tanh(sum(w*x) + b) */
	cln * curr = n->head;
	double a = n->bias;
	while(curr){
		a = a + curr->weight * curr->from->activation;
		curr = curr->next;
	}
	a = tanh(a);
	n->activation = a;
	return a;
}

cln * add_connection(neuron * from, neuron * to,double weight){
	cln * new_conn = malloc(sizeof(cln));
	new_conn->weight = weight;
	new_conn->from = from;
	new_conn->next = to->head;
	to->head = new_conn;
	return new_conn;
}

void remove_connection(neuron * n){
	/* LIFO */
	cln * old_head = n->head;
	if(!old_head) return;
	n->head = old_head->next;
	free(old_head);
	return;
}

void add_random_connection(network * net) {
	/* add connection to random node */
	size_t from = rand() % net->neurons;
	size_t to = rand() % net->neurons;
	
}

double average_connectivity




