#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// return min of two values
int min(int a, int b) {
	return a <= b ? a : b;
}

// compare function for sorting ints
int intSort(const void* a, const void* b) {
	int num1 = *((int*) a);
	int num2 = *((int*) b);
	return num1 - num2;
}


// calculate cost to go to floors between firstStop and secondStop
int fw(int firstStop, int secondStop, intArray dests) {
	int cost = 0;

	for (int i=0; i<dests->len; i++) {
		int dest = dests->data[i];
		if (dest > firstStop && dest <= secondStop) {
			cost += min(dest-firstStop, secondStop-dest);
		}
	}

	return cost;
}

// calculate cost to go to floors above stop
int fwLastStop(int stop, intArray dests) {
	int cost = 0;

	for (int i=0; i<dests->len; i++) {
		int dest = dests->data[i];
		if (dest > stop) {
			cost += dest-stop;
		}
	}

	return cost;
}


// print data of intArray struct
void printIntArray(intArray array) {
	for (int i=0; i<array->len; i++) {
		printf("%d ", array->data[i]);
	}
	putchar('\n');
}

// copy data from src array to dest array
void copyArray(int* dest, int* src, int len) {
	for (int i=0; i<len; i++) {
		dest[i] = src[i];
	}
}

void freeIntArray(intArray array) {
	free(array->data);
	free(array);
}


// print a table with height of rows and width of cols
void printTable(int** table, int rows, int cols) {
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			printf("%4d", table[i][j]);
		}
		putchar('\n');
	}
}

void freeTable(int** table, int len) {
	for (int i=0; i<len; i++) {
		free(table[i]);
	}

	free(table);
}
