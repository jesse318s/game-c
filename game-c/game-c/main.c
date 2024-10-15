#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECIMAL_RADIX 10

static void freeMemory(int** target, int** guess);
static int initializeMemory(const int length, int** target, int** guess);
static int reallocateMemory(const int length, int** target, int** guess);

static int prepareGame(int* length, int** target, int** guess);
static int playGame(const int length, int* target, int* guess);

int main() {
	srand((unsigned int)time(NULL));

	int length = 0;
	int playAgain = 1;
	int* target = NULL;
	int* guess = NULL;

	while (playAgain) {
		if (prepareGame(&length, &target, &guess)) {
			freeMemory(&target, &guess);

			return 1;
		}

		if (playGame(length, target, guess)) {
			freeMemory(&target, &guess);

			return 1;
		}

		printf("Do you want to play again? (1 for Yes, 0 for No): ");

		if (scanf_s("%d", &playAgain) != 1 || (playAgain != 0 && playAgain != 1)) {
			fprintf(stderr, "Invalid input. Exiting the game.\n");
			freeMemory(&target, &guess);

			return 1;
		}
	}

	freeMemory(&target, &guess);

	return 0;
}

static void freeMemory(int** target, int** guess) {
	if (target && *target) {
		free(*target);
		*target = NULL;
	}

	if (guess && *guess) {
		free(*guess);
		*guess = NULL;
	}
}

static int initializeMemory(const int length, int** target, int** guess) {
	*target = (int*)calloc(length, sizeof(int));
	*guess = (int*)calloc(length, sizeof(int));

	if (*target == NULL || *guess == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		freeMemory(target, guess);

		return 1;
	}

	return 0;
}

static int reallocateMemory(const int length, int** target, int** guess) {
	int* tempTarget = (int*)realloc(*target, length * sizeof(int));
	int* tempGuess = (int*)realloc(*guess, length * sizeof(int));

	if (tempTarget == NULL || tempGuess == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		freeMemory(&tempTarget, &tempGuess);

		return 1;
	}

	*target = tempTarget;
	*guess = tempGuess;

	return 0;
}

static int prepareGame(int* length, int** target, int** guess) {
	printf("Enter the length of the sequence to guess: ");

	if (scanf_s("%d", length) != 1 || *length <= 0) {
		fprintf(stderr, "Invalid input. Please enter a positive integer.\n");

		return 1;
	}

	if (*target == NULL && *guess == NULL) { 
		if (initializeMemory(*length, target, guess)) return 1; 
	}
	else { 
		if (reallocateMemory(*length, target, guess)) return 1; 
	}

	for (int i = 0; i < *length; i++) {
		(*target)[i] = rand() % DECIMAL_RADIX;
	}

	return 0;
}

static int playGame(const int length, int* target, int* guess) {
	int correct = 0;

	printf("Guess the sequence of %d numbers (each between 0 and 9):\n", length);

	while (1) {
		for (int i = 0; i < length; i++) {
			if (scanf_s("%d", &guess[i]) != 1) {
				fprintf(stderr, "Invalid input. Please enter integers only.\n");

				return 1;
			}
		}

		correct = 1;

		for (int i = 0; i < length; i++) {
			if (guess[i] != target[i]) {
				correct = 0;

				break;
			}
		}

		if (correct) {
			printf("Congratulations! You guessed the sequence.\n");

			break;
		}
		else {
			printf("Incorrect sequence. Try again:\n");
		}
	}

	return 0;
}