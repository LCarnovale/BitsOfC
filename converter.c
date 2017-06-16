#include <stdio.h>
#include <stdlib.h>
#include "BaseConvert.h"


int main(int argc, char const *argv[]) {
	int number, startBase, endBase;
	printf("Type in a number, start base then end base, separated by spaces.\n>>> ");
	scanf("%d %d %d", &number, &startBase, &endBase);
	// printf("Num: %d\n", number);
	// printf("startBase: %d\n", startBase);
	// printf("endBase: %d\n", endBase);
	char *result = baseConvert(number, startBase, endBase);
	printf("Result: %s\n", result);
	return 0;
}