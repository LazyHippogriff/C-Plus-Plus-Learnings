#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#define SUCCESS 0
#define FAILURE -1
#define NUMBER_OF_INPUT_IN_FILE 5
#define STEEL_SECTION_NAME_MAX_LENGTH 100
#define MAX_RESULT_SIZE 10000
#define PI 3.14159

extern char output[MAX_RESULT_SIZE];
extern char steelSectionName[STEEL_SECTION_NAME_MAX_LENGTH];
extern int steelSectionNameSize;
extern uintmax_t Zx;
extern uintmax_t Iy;
extern uintmax_t J;
extern uintmax_t Iw;


void test();
void printValues();
int initialiseValuesFromFile(FILE* inputFile);
int createOutputString(double ratios[10][11]);
int createOutputFile(const char* fileName);
void calculateArray();
