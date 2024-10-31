#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// calculate the cost of stopping at all the floors in the table stops
int calcCost(int* stops, int stopsLen, intArray dests) {
    int cost = 0;

    cost += fw(0, stops[0], dests);
    for (int i=0; i<stopsLen-1; i++) {
        cost += fw(stops[i], stops[i+1], dests);
    }
    cost += fwLastStop(stops[stopsLen-1], dests);

    return cost;
}

// utility function to generate combinations of length limit for all the numbers from start to end
intArray combinationUtil(int* minCost, int* data, int start, int end, int index, int limit, intArray dests) {

    // Current combination is ready to be used
    if (index == limit) {
        int cost = calcCost(data, limit, dests);
        if (cost < *minCost) {
            *minCost = cost;

            intArray array = malloc(sizeof(*array));
            array->len = limit;
            array->data = malloc(array->len * sizeof(*(array->data)));

            copyArray(array->data, data, array->len);

            return array;
        }

        return NULL;
    }

    // replace index with all possible elements. The condition
    // end-i+1 >= limit-index makes sure that including one element
    // at index will make a combination with remaining elements
    // at remaining positions
    intArray combination = NULL;
    for (int i = start; i <= end && end - i + 1 >= limit - index; i++)
    {
        data[index] = i;
        intArray nextCombination = NULL;
        nextCombination = combinationUtil(minCost, data, i + 1, end, index + 1, limit, dests);

        if (nextCombination != NULL) {
            if (combination != NULL) freeIntArray(combination);
            combination = nextCombination;
        }
    }

    return combination;
}

// solve function for brute force method
int solve(int nrid, int nst, int* dests) {
    intArray destinations = malloc(sizeof(*destinations));
    destinations->len = nrid;
    destinations->data = dests;

	int lastFloor = dests[nrid-1];
    int minCost = fwLastStop(0, destinations);

    int lastMinCost = minCost;
    intArray stops = NULL;
	for (int i=1; i<=nst; i++) {
        int data[i];
        intArray newStops = combinationUtil(&minCost, data, 1, lastFloor, 0, i, destinations);

        // if minCost has not changed discard newStops
        if (minCost == lastMinCost) {
            freeIntArray(newStops);
            continue;
        }

        // otherwise update lastMinCost and keep newStops data
        lastMinCost = minCost;
        if (stops != NULL) freeIntArray(stops);
        stops = newStops;
	}

    if (stops->len > 0) {
        printf("Lift stops are: ");
        printIntArray(stops);
    }
    else {
        puts("No lift stops");
    }

    freeIntArray(stops);
    freeIntArray(destinations);

	return minCost;
}
