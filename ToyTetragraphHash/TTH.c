#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
int numberOfBlocks;
int INITVECTOR[4] = { 0, 0, 0, 0 };
char FINALRESULT[4];

// Hash lookup structure
struct Hash
{
	char key;
	int value;
};

// Hashtable for lookup
struct Hash hashtable[26] = {
	{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25} };

// function that compare the character in the hash table
int compareHashByChar(const void* _a, const void* _b)
{
	const struct Hash* a = _a, * b = _b;
	if (a->key < b->key)
		return -1;
	else if (a->key > b->key)
		return 1;
	else
		return 0;
}

// function that compare the values in the hash table
int compareHashByInt(const void* _a, const void* _b)
{
	const struct Hash* a = _a, * b = _b;
	if (a->value < b->value)
		return -1;
	else if (a->value > b->value)
		return 1;
	else
		return 0;
}

// Find corresponding integer for the character from Hash table
int getHashedInteger(char c)
{
	struct Hash key[1] = { {c, 0} };
	struct Hash* result =
		bsearch(key, hashtable, sizeof(hashtable) / sizeof(hashtable[0]),
			sizeof(hashtable[0]), compareHashByChar);
	return result ? result->value : -1;
}

// Find corresponding character for the integer from the hash table
char getHashedCharacter(int i)
{
	struct Hash key[1] = { {'0', i} };
	struct Hash* result =
		bsearch(key, hashtable, sizeof(hashtable) / sizeof(hashtable[0]),
			sizeof(hashtable[0]), compareHashByInt);
	return result ? result->key : '0';
}

// check if character is a letter (A-Z)
bool checkIfCharacterIsAlphabet(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	else
		return false;
}

//compute number of blocks for the message
int computeNumberOfBlocks(char* message) {
	int characters = 0;
	for (int i = 0; i < strlen(message); i++) {
		if (checkIfCharacterIsAlphabet(message[i])) {
			characters++;
		}
	}
	return ceil((double)characters / (double)16);
}

// Test the block of message created by function!
void printBlocksOfMessage(char** blockArray)
{
	for (int i = 0; i < numberOfBlocks; i++)
	{
		printf("Block %d: ", i);
		printf("%s", blockArray[i]);
		printf("\n");
	}
}

// Test the number block that is converted
void printNumberBlock(int** numBlock)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d,", numBlock[i][j]);
		}
	}
	printf("\n");
}

//check Result
void printVector(int round) {
	printf("Result after Round %d: (%d,%d,%d,%d)\n",
		round,
		INITVECTOR[0],
		INITVECTOR[1],
		INITVECTOR[2],
		INITVECTOR[3]);
}

// cut the message into block of 16
char** createBlockArray(char* message)
{
	numberOfBlocks = computeNumberOfBlocks(message);
	int endPointOfMessage = strlen(message) - 1;
	int iterator = 0;
	printf("Number of Blocks: %d\n", numberOfBlocks);

	char** blockArray = (char**)malloc(numberOfBlocks * sizeof(char*));
	for (int i = 0; i < numberOfBlocks; i++)
	{
		int k = 0;
		blockArray[i] = (char*)calloc(17, sizeof(char));
		while (k < 16)
		{
			if (iterator <= endPointOfMessage &&
				checkIfCharacterIsAlphabet(message[iterator]))
			{
				blockArray[i][k] = toupper(message[iterator]);
				iterator++;
				k++;
			}
			else if (iterator > endPointOfMessage)
			{
				blockArray[i][k] = '0';
				iterator++;
				k++;
			}
			else
			{
				iterator++;
			}
		}
	}
	return blockArray;
}

// create a single number block
int** createNumberBlock(char* block)
{
	int iterator = 0;
	int** numberBlock = (int**)malloc(4 * sizeof(int*));
	if (strlen(block) == 16)
	{
		for (int i = 0; i < 4; i++)
		{
			numberBlock[i] = (int*)malloc(4 * sizeof(int));
			for (int j = 0; j < 4; j++)
			{
				numberBlock[i][j] = getHashedInteger(block[iterator]);
				iterator++;
			}
		}
		return numberBlock;
	}
	else
	{
		perror("Something wrong with blocks of message!");
		return NULL;
	}
}

//rearrange block (round 2 step)
void rearrangeBlock(int** numBlock) {
	//Rotate first row
	int temp = numBlock[0][0];
	numBlock[0][0] = numBlock[0][1];
	numBlock[0][1] = numBlock[0][2];
	numBlock[0][2] = numBlock[0][3];
	numBlock[0][3] = temp;

	//Rotate second row
	temp = numBlock[1][0];
	int temp1 = numBlock[1][1];
	numBlock[1][0] = numBlock[1][2];
	numBlock[1][1] = numBlock[1][3];
	numBlock[1][2] = temp;
	numBlock[1][3] = temp1;

	//Rotate third row
	temp = numBlock[2][3];
	numBlock[2][3] = numBlock[2][2];
	numBlock[2][2] = numBlock[2][1];
	numBlock[2][1] = numBlock[2][0];
	numBlock[2][0] = temp;

	//Rotate fourth row
	temp = numBlock[3][0];
	temp1 = numBlock[3][1];
	numBlock[3][0] = numBlock[3][3];
	numBlock[3][1] = numBlock[3][2];
	numBlock[3][2] = temp1;
	numBlock[3][3] = temp;
}

//computing each column and adding to Initial vector
void computeBlock(int** numBlock) {
	int sum[4] = { 0,0,0,0 };
	//Adding each column
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			sum[j] += numBlock[i][j];
		}
	}
	//mod26 each column then, adding it to the Result and then mod 26 again!
	for (int i = 0; i < 4; i++) {
		INITVECTOR[i] = ((INITVECTOR[i] + (sum[i] % 26)) % 26);
	}
}

// compression function Round 1 and Round 2
void compression(char* block)
{
	//Round1
	int** numBlock = createNumberBlock(block);
	printNumberBlock(numBlock);
	computeBlock(numBlock);
	printVector(1);

	//Round2
	rearrangeBlock(numBlock);
	printNumberBlock(numBlock);
	computeBlock(numBlock);
	printVector(2);

	free(numBlock);
}

//convert the final result to letter
void convertToLetters() {
	for (int i = 0; i < 4; i++) {
		FINALRESULT[i] = getHashedCharacter(INITVECTOR[i]);
	}
}


void computeTTH(char* message)
{
	char** blocksOfMessage = createBlockArray(message);
	printf("Value of F: %d\n", getHashedInteger('F'));
	printf("Value of 2: %c\n", getHashedCharacter(2));
	printBlocksOfMessage(blocksOfMessage);
	for (int i = 0; i < numberOfBlocks; i++)
	{
		compression(blocksOfMessage[i]);
	}
	convertToLetters();
	free(blocksOfMessage);
}

void printFinalResult(char* message) {
	printf("Original Message: %s\n", message);
	printf("Hash result (TTH): (");
	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
			printf("%c,",FINALRESULT[i]);
		else
			printf("%c)", FINALRESULT[i]);
	}
}

int main()
{
	char* message = "I leave twenty million dollars to my friendly cousin Bill.";
	computeTTH(message);
	printFinalResult(message);
}