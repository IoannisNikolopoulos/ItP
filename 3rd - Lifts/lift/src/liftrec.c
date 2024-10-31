#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// M function of recursive method
int Mrec(int stops, int lastStop, intArray dests) {
	int bestCost = fwLastStop(0, dests);

	if (stops == 0) return bestCost;

	for (int k=0; k<=lastStop; k++) {
		int newCost = Mrec(stops-1, k, dests);
		newCost -= fwLastStop(k, dests);
		newCost += fw(k, lastStop, dests);
		newCost += fwLastStop(lastStop, dests);

		bestCost = min(bestCost, newCost);
	}

	return bestCost;
}

// solve function of recursive method
int solve(int nrid, int nst, int* dests) {
	intArray destinations = malloc(sizeof(*destinations));
	destinations->len = nrid;
	destinations->data = dests;

	int lastFloor = dests[nrid-1];
	int bestCost = fwLastStop(0, destinations);

	int lastStop = 0;
	for (int j=0; j<=lastFloor; j++) {
		int newCost = Mrec(nst, j, destinations);

		if (newCost < bestCost) {
			bestCost = newCost;
			lastStop = j;
		}
	}

	if (lastStop) {
		printf("Last stop at floor: %d\n", lastStop);
	}
	else {
		puts("No lift stops");
	}

	freeIntArray(destinations);

	return bestCost;
}
