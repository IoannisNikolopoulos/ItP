#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// M function of memoization method
int Mmem(int stops, int lastStop, intArray dests, int** mem) {
	int bestCost = fwLastStop(0, dests);

	if (stops == 0) return bestCost;

	for (int k=0; k<=lastStop; k++) {
		// if value of M function for inputs stops-1 and k has not been calculated, then calculate and store it
		if (mem[stops-1][k] == -1) {
			mem[stops-1][k] = Mmem(stops-1, k, dests, mem);
		}

		int newCost = mem[stops-1][k];
		newCost -= fwLastStop(k, dests);
		newCost += fw(k, lastStop, dests);
		newCost += fwLastStop(lastStop, dests);

		bestCost = min(bestCost, newCost);
	}

	return bestCost;
}

// solve function of memoization method
int solve(int nrid, int nst, int* dests) {
	intArray destinations = malloc(sizeof(*destinations));
	destinations->len = nrid;
	destinations->data = dests;

	int lastFloor = dests[nrid-1];
	int bestCost = fwLastStop(0, destinations);

	// initialize table to store previous values of M function
	int** mem = malloc((nst+1) * sizeof(*mem));

	for (int i=0; i<=nst; i++) {
		mem[i] = malloc((lastFloor+1) * sizeof(**mem));
		for (int j=0; j<=lastFloor; j++) {
			mem[i][j] = -1;
		}
	}

	int lastStop = 0;
	for (int j=0; j<=lastFloor; j++) {
		int newCost = Mmem(nst, j, destinations, mem);

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

	freeTable(mem, nst+1);
	freeIntArray(destinations);

	return bestCost;
}
