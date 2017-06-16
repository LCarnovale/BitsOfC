// Author: Leo Carnovale (Leo.carnovale@gmail.com)
// Date: 16/06/2017
// Converts any integer from one integer base to another 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Any number can be represented as multiples of it's base.
// For example, 123 in base 10:
// 
// 			 123 = 1*10^2 + 2*10^1 + 3*10^0
// 
// in base B, the number 4865 is:
// 
// 			4865 = 4*B^3 + 8*B^2 + 6*B^1 + 5*B^0
// 
// To convert from one base to another, we have an equality as follows,
// 		
// 		a*B^0 + b*B^1 + c*B^2 + ... = x*A^0 + y*A^1 + z*A^2 + ... 
// 
// The first digit in a number always has the highest place value, as is clear
// in the above examples. To get the first digit in the new base, we simply find 
// the largest power of the new base that is less than the digit. Essentially,
// where N is the highest power of the base, and therefore 1 less than the length 
// of the new number;
// Converting 123 base 10 to base 3, 
// 		3^N < 1*10^2 + 2*10^1 + 3*10^0, greatest possible integer N
// 		N*ln(3) < ln(123)
// 		N < ln(123) / ln(3) = log_3(123)
// ie, 	N = floor(ln(123) / ln(3)) or floor(log_3(123))
// 
// Converting 1001 base 2 to base 6,
// 		6^N < 1*2^3 + 0*2^2 + 0*2^1 + 1*2^0
// 		N*ln(6) < ln(value)
// As we can see, when doing calculations with different bases it doesn't suffice to say
// x < y when x and y have different bases. 10 < 8 is true if 10 is in base 2, 3, 4, up to 7.
// As a result, the value 
// in general,
// 		N = floor(ln(Value)/ln(<NewBase>))
// the first digit is then the floor or integer part of (value / NewBase^N)
// Following this first conversion, we subtract the value of the first digit,
// ie digit*(NewBase^N), from the value and perform the same operation on the
// remaining amount, showing a somewhat recursive nature to the process.
// Note that, regrettably, at the moment I do have to briefly change the number to base 10
// in order to carry out calculations. (Sorry) 


char  getNumChar(int number);
int   getCharNum(char c);
int   power     (int base, int power);

char* baseConvert(int value, int startBase, int endBase) {
	if (endBase > 62) {
		printf("Warning, endBase is greater than 62. \n");
		printf("Currently can not represent single digits greater than 62 ('z'),\n");
		printf("Successful conversion not guaranteed.\n");
	}

	int index = 0;
	if (startBase != 10) {
		// Sadly I have to bring the number to base 10 to do calculations. 
		// Hopefully I eventually work out some totally meta way to convert
		// numbers without doing this
		int tempRemain = value, tempDiff = 0;//, power = 0;
		int startValue = power(10, (int) log(tempRemain) + 1); // This kind of 
		value = startValue;
		for (index = (int) log(tempRemain); index >= 0; index--) {
			
			// printf("10 power: %d, %d power: %d\n", 
			// 	(int) (power(10, index)), 
			// 	startBase, power(startBase, index));
			
			tempDiff = (int) (tempRemain / power(10, index)) * power(startBase, index);
			value += tempDiff;
			tempRemain = tempRemain % (int) power(10, index);
		}
		value -= startValue;

		index = 0;
	}
	printf("value: %d\n", value);

	if (value == 0) {
		return "0"; // The algorithm uses logs, log of 0 is undefined thus the program wouldn't work
	}

	int digit = 0, remaining = value;
	int N = 0;  										// N -> Highest power in the new number. (See above)
	N = (int) (log(value) / log(endBase));
	char *returnString = malloc((N + 2) * sizeof (char));// A string that will hold the converted digits. N is known to be 1 less than
                                                         // the length of the digit, so N + 2 gives an extra space for the terminating character.
	for (int i = 0; i < N + 1; i++) {
		returnString[i] = '0';                            // Initialise the string. The last value marks the end of the string,
	}                                                    // and won't be changed.
	returnString[N + 1] = 0;
	int numLength = N;
	char numChar = 0;
	while (remaining > 0) {
		// printf("Pass %d\n", index);
		N = (int) (log(remaining * endBase) / log(endBase) - 1);
		// printf("N: %d\n", N);
		digit = remaining / power(endBase, N);
		numChar = getNumChar(digit);
		if (numChar) {
			// printf("Digit: %d\n", digit);
			returnString[numLength - N] = numChar;
			index++;
		} else {
			printf("Unable to complete conversion. Aborting.\n");
			exit(1);
		}
		remaining -= (digit * power(endBase, N));
		// printf("Remaining: %d\n", remaining);
	}
	return returnString;
}


// Returns the appropriate number or letter character for a digit.
// For example, if 0 <= number <= 9, will return the ascii for number.
// if number > 9, returns a letter. eg. number = 12, returns 'C'.
// After 'Z' comes 'a', through to 'z'. 
// This limits the highest base to 62.
char getNumChar(int number) {
	if (0 <= number && number <= 9) {
		return ('0' + number);
	} else if (10 <= number && number <= 35) {
		return ('A' + number - 10);
	} else if (36 <= number && number <= 61) {
		return ('a' + number - 36);
	} else {
		printf("Unable to make digit for face value: %d.\n", number);
		return 0;
	}
}

// The reverse of getNumChar
int getCharNum(char c) {
	if ('0' <= c && c <= '9') {
		return (c - '0');
	} else if ('A' <= c && c <= 'Z') {
		return (c - 'A' + 10);
	} else if ('a' <= c && c <= 'z') {
		return (c - 'z' + 36);
	} else {
		printf("Unrecognised digit: %c.\n", c);
		return 0;
	}
}

int power (int base, int power) {
	int result = 1;
	for (int i = power; i > 0; i--) {
		result *= base;
	}
	return result;
}
