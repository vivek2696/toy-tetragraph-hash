#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h> 

bool checkIfCharacterIsAlphabet(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	else
		return false;
}

void computeTTH(char* message) {
	int numberOfBlocks = ceil((double)strlen(message) / (double)16);
	int endPointOfMessage = strlen(message) - 1;
	int iterator = 0;
	printf("Number of Blocks: %d\n", numberOfBlocks);

	char** blockArray = (char**)malloc(numberOfBlocks * sizeof(char*));
	for (int i = 0; i < numberOfBlocks; i++)
	{
		int k = 0;
		blockArray[i] = (char*)calloc(16, sizeof(char));
		while (k < 16) {
			if (iterator <= endPointOfMessage && checkIfCharacterIsAlphabet(message[iterator])) {
				blockArray[i][k] = toupper(message[iterator]);
				iterator++;
				k++;
			}
			else if (iterator > endPointOfMessage) {
				blockArray[i][k] = '0';
				iterator++;
				k++;
			}
			else {
				iterator++;
			}
		}
	}

	for (int i = 0; i < numberOfBlocks; i++)
	{
		printf("Block %d: ", i);
		for (int j = 0; j < 16; j++) {
			printf("%c,", blockArray[i][j]);
		}
		printf("\n");
	}

}

int main() {
	char* message = "ABCDEFGHIJKLMNOP";

	computeTTH(message);

}