#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int Mdp(int stops, int lastStop, intArray dests, int** mem, int** bestStops) {
	int bestCost = fwLastStop(0, dests);

	if (stops == 0) return bestCost;

	for (int k=0; k<=lastStop; k++) {
		int newCost = mem[stops-1][k];
		newCost -= fwLastStop(k, dests);
		newCost += fw(k, lastStop, dests);
		newCost += fwLastStop(lastStop, dests);

		if (newCost < bestCost) {
			bestCost = newCost;
			bestStops[stops][lastStop] = k;
		}
	}

	return bestCost;
}

int solve(int nrid, int nst, int* dests) {
	intArray destinations = malloc(sizeof(*destinations));
	destinations->len = nrid;
	destinations->data = dests;

	int lastFloor = dests[nrid-1];
	int bestCost = fwLastStop(0, destinations);

	// calculate and store the values of M function for all possible inputs
	int** mem = malloc((nst+1) * sizeof(*mem));
	int** bestStops = malloc((nst+1) * sizeof(*bestStops));

	for (int i=0; i<=nst; i++) {
		mem[i] = malloc((lastFloor+1) * sizeof(**mem));
		bestStops[i] = malloc((lastFloor+1) * sizeof(**bestStops));

		for (int j=0; j<=lastFloor; j++) {
			if (i==0) {
				mem[i][j] = bestCost;
				continue;
			}

			mem[i][j] = Mdp(i, j, destinations, mem, bestStops);
		}
	}

	// find best cost for nst number of stops from table mem
	int lastStop = 0;
	for (int j=0; j<=lastFloor; j++) {
		int newCost = mem[nst][j];

		if (newCost < bestCost) {
			bestCost = newCost;
			lastStop = j;
		}
	}

	printTable(mem, nst+1, lastFloor+1);

	if (lastStop) {
		printf("Lift stops are:");

		// find at what floors the elevator stopped and print them
		int stopsToPrint[nst];
		stopsToPrint[0] = lastStop;
		stopsToPrint[1] = bestStops[nst][lastFloor];

		int prevStop = stopsToPrint[1];
		for (int i=2; i<nst; i++) {
			stopsToPrint[i] = bestStops[nst][prevStop];
			prevStop = stopsToPrint[i];
		}

		for (int i=nst-1; i>=0; i--) {
			if (stopsToPrint[i] != 0) printf(" %d", stopsToPrint[i]);
		}
		putchar('\n');
	}
	else {
		puts("No lift stops");
	}

	freeTable(mem, nst+1);
	freeIntArray(destinations);

	return bestCost;
}
