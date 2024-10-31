#ifndef UTILS_H
#define UTILS_H

int min(int a, int b);

int intSort(const void* a, const void* b);

// struct to hold an array of ints
typedef struct {
	int* data; // actual array
	int len; // length of the array
} *intArray;

int fw(int firstStop, int secondStop, intArray dests);
int fwLastStop(int stop, intArray dests);

void printIntArray(intArray array);
void copyArray(int* dest, int* src, int len);
void freeIntArray(intArray array);

void printTable(int** table, int heigth, int width);
void freeTable(int** table, int len);

#endif // UTILS_H
