// Author: Leo Carnovale (leo.carnovale@gmail.com)
// Date: 16/06/2017
// Takes in a not nicely formatted table text file and makes a nicely 
// 	formatted table text file. 

#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define MAX_LENGTH 8192

typedef struct columnNode *columnNode;
typedef struct columns 	  *columns;

struct columnNode {
	struct columnNode *next;
	int width;
};

struct columns {
	struct columnNode *head;
	int size;
};



columnNode indexCols(columns c, int i);
void append(columns c, int columnWidth);

int strLength(char *string);
void clearCharArray(char *array, int length);


int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(0);
	}

	char delim = ',';
	char gap = ' ';

	if (argc > 2) {
		if (argv[2][3] == 0) {
			if (argv[2][0] == argv[2][2] && argv[2][0] == '\'') {
				gap = argv[2][1];
			} else {
				printf("Example of usage for gap parameter: %s %s '.'\n", argv[0], argv[1]);			
				printf("Using default of ' ' (space)\n");
			}

		} else {
			printf("Example of usage for gap parameter: %s %s '.'\n", argv[0], argv[1]);
			printf("Using default of ' ' (space)\n");
		}
	}

	columns cols = malloc(sizeof (struct columns));
	cols->size = 0;
	cols->head = NULL;



	FILE *f = fopen(argv[1], "r");
	char C = fgetc(f);
	int lineCount = 1, columnWidth = 0, lineLength = 0,
		columnIndex = 0, maxLineLength = 0;
	int counter = 0;

	while (C != -1) {
		if (C == '\n') {
			if (lineLength > maxLineLength) {
				maxLineLength = lineLength;
			}
			lineCount++;
			columnWidth = 0;
			columnIndex = 0;
			lineLength = 0;
		} else if (C == delim) {
			// Start of a new column
			columnWidth++; // This value is now technically 1 space wider than the column
			if (columnIndex >= cols->size) {
				// New column, add it to the list.
				append(cols, columnWidth);
			} else if (indexCols(cols, columnIndex)->width < columnWidth) {
				// Resize this column
				indexCols(cols, columnIndex)->width = columnWidth;
			}
			columnWidth = 0;
			columnIndex++;
		} else {
			lineLength++;
			columnWidth++;
		}
		// putchar(C);
		C = fgetc(f);
	}
	// Now all the columnNodes in cols have the maximum width of each column.
	// rewind(f);
	fclose(f);

	// Create the new file string:
	int stringLength = strLength(argv[1]) + 5;
	printf("Length of string: %d\n", stringLength);
	char fileString[stringLength + 1];

	int w = 0, x = 0;

	for (; w <= stringLength; w++) {
		if (argv[1][w] == '.') {
			for (; x < 5; x++) {
				fileString[w + x] = "Nice."[x];
			}
			x--;
		} else {
			fileString[w + x] = argv[1][w];
		}
	}
	printf("FileString: %s\n", fileString);

	FILE *new = fopen(fileString, "w");
	FILE *source = fopen(argv[1], "r");

	// fputs("hello world", new);
	int newMaxLineLength = 0;
	for (int i = 0; i < cols->size; i++) {
		newMaxLineLength += indexCols(cols, i)->width + 1;
	}
	// If the predicted new length is shorter than the current, make the new max length the current max length instead.  
	newMaxLineLength = newMaxLineLength ? (newMaxLineLength > maxLineLength) : maxLineLength; 

	char line[maxLineLength];
	char newLine[newMaxLineLength];

	columnIndex = 0;
	columnWidth = 0;
	int shift = 0;

	for (int i = 0; i <= lineCount; i++) {
		fgets(line, maxLineLength, source);
		columnWidth = indexCols(cols, 0)->width;
		columnStart = i;
		for (int j = 0; j <= newMaxLineLength; j++) {
			if (line[j] == delim) {

			}		
		}


		clearCharArray(line, maxLineLength);
		clearCharArray(newLine, newMaxLineLength);
	}




	fclose(source);
	fclose(new);
	return 0;
}


// Returns the column node at index 'i' in the list of columnNodes 
// starting at c->head
columnNode indexCols(columns c, int i) {
	if (i < 0) {
		if (-1 * i > c->size) {
			return NULL;
		} 

		i = i + c->size;
	}

		
	if (i >= c->size) {
		return NULL;
	}
	
	columnNode temp = c->head;
	
	while (i > 0 && temp != NULL) {
		temp = temp->next;
		i--;
	}

	return temp;

}

// Adds a new columnNode onto the list starting at c->head, with width 'columnWidth'
void append(columns c, int columnWidth) {

	columnNode end = c->head;
	if (end) {
		while (end->next != NULL) {
			end = end->next;
		}
	}

	columnNode newNode = malloc(sizeof (struct columnNode));
	newNode->next = NULL;
	newNode->width = columnWidth;
	if (!end) {
		// Special case where the list is empty.
		c->head = newNode;
	} else {
		end->next = newNode;
	}
	c->size++;

}

int strLength(char *string) {
	int i = 0;
	for (; string[i] != 0; i++) {
		if (i > MAX_LENGTH) {
			printf("Warning: Max length exceeded when finding length of string.\n");
			return -1;
		}
	}

	return i;
}

// Clears an array of char, making each value 0.
void clearCharArray(char *array, int length) {
	for (int i = 0; i < length; i++) {
		array[i] = 0;
	}
}
