#include <stdio.h>
#include <stdlib.h>

#include "lift.h"
#include "utils.h"

int main() {
	int nrid, nst;
	scanf("%d %d", &nrid, &nst);

	int* dests = malloc(nrid * sizeof(*dests));
	for (int i=0; i<nrid; i++) {
		scanf("%d", dests+i);
	}

	qsort(dests, nrid, sizeof(*dests), intSort);

	int result = solve(nrid, nst, dests);
	printf("Cost is: %d\n", result);
}
