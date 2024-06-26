#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFSIZE 50

typedef struct darr{
	int *shares;
	char *names;
	size_t size;
	size_t max_size;
} darr;

int print_usage(char * exe){
	printf("Usage: %s file.txt\n", exe);
	return 0;
}

int insert(darr * d, int x, char * name){
	if (!d) return 1;
	if (d->size >= d->max_size) {
		d->max_size = d->max_size + 32;
		d->shares = realloc(d->shares, d->max_size*sizeof(int));
		d->names = realloc(d->names, d->max_size*BUFFSIZE);
		if (!d->shares || !d->names) return 1;
	}

	d->shares[d->size] = x;
	strncpy(d->names+d->size*BUFFSIZE, name, BUFFSIZE);
	d->size = d->size + 1;
	return 0;
}

int main(int argc, char **argv) {
	srand(time(0));
	if (argc != 2) {
		print_usage(argv[0]);
		return 1;
	}
	FILE * f = fopen(argv[1], "r");
	if (!f) {
		print_usage(argv[0]);
		return 1;
	}
	darr arr = {0, 0, 0, 0};
	char buff[BUFFSIZE];
	int shares, total = 0;
	while (fscanf(f, "%s %d", buff, &shares) == 2) {
		insert(&arr, shares, buff);
		total = total + shares;
	}
	int random_share = rand() % total;
	int cumulative_shares = 0, index = -1;
	while (cumulative_shares <= random_share)
		cumulative_shares = cumulative_shares + arr.shares[++index];
	printf("%s chosen.", arr.names+index*BUFFSIZE);
	fclose(f);
	return 0;
}
