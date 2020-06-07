#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
int numberOfBlocks;
int INITVECTOR[4] = {0, 0, 0, 0};

// Hash lookup structure
struct Hash
{
  char key;
  int value;
};

// Hashtable for lookup
struct Hash hashtable[26] = {
    {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}};

// function that compare the character in the hash table
int compareHashByChar(const void *_a, const void *_b)
{
  const struct Hash *a = _a, *b = _b;
  if (a->key < b->key)
    return -1;
  else if (a->key > b->key)
    return 1;
  else
    return 0;
}

// function that compare the values in the hash table
int compareHashByInt(const void *_a, const void *_b)
{
  const struct Hash *a = _a, *b = _b;
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
  struct Hash key[1] = {{c, 0}};
  struct Hash *result =
      bsearch(key, hashtable, sizeof(hashtable) / sizeof(hashtable[0]),
              sizeof(hashtable[0]), compareHashByChar);
  return result ? result->value : -1;
}

// Find corresponding character for the integer from the hash table
char getHashedCharacter(int i)
{
  struct Hash key[1] = {{'0', i}};
  struct Hash *result =
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

// Test the block of message created by function!
void printBlocksOfMessage(char **blockArray)
{
  for (int i = 0; i < numberOfBlocks; i++)
  {
    printf("Block %d: ", i);
    printf("%s", blockArray[i]);
    printf("\n");
  }
}

// Test the number block that is converted
void printNumberBlock(int **numBlock)
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

// cut the message into block of 16
char **createBlockArray(char *message)
{
  numberOfBlocks = ceil((double)strlen(message) / (double)16);
  int endPointOfMessage = strlen(message) - 1;
  int iterator = 0;
  printf("Number of Blocks: %d\n", numberOfBlocks);

  char **blockArray = (char **)malloc(numberOfBlocks * sizeof(char *));
  for (int i = 0; i < numberOfBlocks; i++)
  {
    int k = 0;
    blockArray[i] = (char *)calloc(17, sizeof(char));
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
int **createNumberBlock(char *block)
{
  int iterator = 0;
  int numberBlock[4][4];
  if (strlen(block) == 16)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        numberBlock[i][j] = getHashedInteger(block[iterator]);
      }
    }
  }
  else
  {
    perror("Something wrong with blocks of message!");
  }
}

// compression function Round 1 and Round 2
void compression(char *block)
{
  int **numBlock = createNumberBlock(block);
  printNumberBlock(numBlock);
}

void computeTTH(char *message)
{
  char **blocksOfMessage = createBlockArray(message);
  printf("Value of F: %d\n", getHashedInteger('F'));
  printf("Value of 2: %c\n", getHashedCharacter(2));
  printBlocksOfMessage(blocksOfMessage);
  for (int i = 0; i < numberOfBlocks; i++)
  {
    compression(blocksOfMessage[i]);
  }
  free(blocksOfMessage);
}

int main()
{
  char *message = "ABCDEFGHIJKLMNOP";
  computeTTH(message);
}