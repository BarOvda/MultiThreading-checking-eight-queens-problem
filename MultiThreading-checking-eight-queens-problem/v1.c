#include "v1.h"

int main(int argc, char *argv[]) {
	char *solution = (char*) malloc(sizeof(char) * 64);
	int result = 1;
	void* threadReturn;
	int * re = (int*)malloc(sizeof(int));
	pthread_t threadsArry[64];
	threadArguments args;

	int i;
	if (argc == 1) {
		readFromSTDIN(&solution);

	} else {
		readFile(argv[1], &solution);
	}

	args.solution = solution;
	for (i = 0; i < THREADS_NUMBER; i++) {
		args.id = i;
		pthread_create(&(threadsArry[i]), NULL, eightQueenCheck,
				(void *) &args);
	}
	for (i = 0; i < THREADS_NUMBER; i++) {
		pthread_join(threadsArry[i], &threadReturn);
		re =(int*)threadReturn;
		if (*re == 0)
			result = 0;
	}
	if (result == 0)
		printf("Solution is not legal\n");
	else
		printf("Solution is legal\n");
	exit(EXIT_SUCCESS);
}
int readFromSTDIN(char** solution) {
	int i = 0;
	char ch;
	while (1) {
		ch = getchar();
		if (ch == EOF) {
			break;
		} else if (ch == '.' || ch == 'Q') {
			(*solution)[i] = ch;
			i++;
		} else if (isspace(ch) == 0 && ch != ' ' && ch != '\n') {
			printf("the char:%c\n", ch);
			printf("illegal file format");
			return 0;
		}
	}
	return 1;
}
int readFile(char fileName[], char** solution) {
	FILE *filePointer;
	char ch;
	int i = 0;
	filePointer = fopen(fileName, "r");
	if (filePointer == NULL) {
		printf("File is not available \n");
		return 0;
	} else {
		while ((ch = fgetc(filePointer)) != EOF) {
			if (ch == '.' || ch == 'Q') {
				(*solution)[i] = ch;
				i++;
			} else if (isspace(ch) == 0 && ch != ' ' && ch != '\n') {
				printf("the char:%c illegal\n", ch);
				printf("illegal file format");
				return 0;
			}
		}
	}
	fclose(filePointer);
	return 1;
}

void * eightQueenCheck(void * arg) {
	threadArguments *arguments = (threadArguments*) arg;
	int threadid = arguments->id, fixedId;
	int * result = (int*)malloc(sizeof(int));
	int workn=threadid;
	
	if (threadid < 8) {
		*result = rowsCheck(arguments->solution, threadid);
	} else if (threadid < 16) {
		fixedId = threadid - 8;
		*result = colsCheck(arguments->solution, fixedId);
	} else if (threadid < 31) {
		fixedId = threadid - 16;
		*result = rightCrossCheck(arguments->solution, fixedId);
	} else {
		fixedId = threadid - 31;
		*result = leftCrossCheck(arguments->solution, fixedId);
	}

	return (void*) result;
}
int rowsCheck(char* solution, int threadId) {
	int col, count = 0;
	for (col = 0; col < 8; col++) {
		if (solution[threadId * ROWS_COLS + col] == 'Q')
			count++;
	}
	if (count != 1)
		return 0;
	return 1;
}
int colsCheck(char* solution, int threadId) {
	int row, count = 0;
	for (row = 0; row < 8; row++) {
		if (solution[row * ROWS_COLS + threadId] == 'Q')
			count++;
	}
	if (count != 1)
		return 0;
	return 1;
}

int leftCrossCheck(char* solution, int threadId) {
	int i, col, row, count = 0, rowzero, colzero;
	if (threadId < 7) {
		colzero = 7;
		for (row = threadId; row < ROWS_COLS; row++) {
			if (solution[row * ROWS_COLS + colzero] == 'Q')
				count++;
			colzero--;
		}
		if (count > 1)
			return 0;
	} else {
		int id = threadId - ROWS_COLS + 1; //
		rowzero = 0;
		for (col = ROWS_COLS - id - 1; col >= 0; col--) {
			if (solution[rowzero * ROWS_COLS + col] == 'Q')
				count++;
			rowzero++;
		}
		if (count > 1)
			return 0;
	}
	return 1;
}
int rightCrossCheck(char* solution, int threadId) {
	int i, col, row, count = 0, rowzero, colzero;
	if (threadId < 7) {
		colzero = 0;
		for (row = threadId; row < ROWS_COLS; row++) {
			if (solution[row * ROWS_COLS + colzero] == 'Q')
				count++;
			colzero++;
		}
		if (count > 1)
			return 0;
	} else {
		int id = threadId - ROWS_COLS + 1; //
		rowzero = 0;
		for (col = id; col < ROWS_COLS; col++) {
			if (solution[rowzero * ROWS_COLS + col] == 'Q')
				count++;
			rowzero++;
		}
		if (count > 1)
			return 0;
	}
	return 1;
}
