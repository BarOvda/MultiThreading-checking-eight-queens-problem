#include "v2.h"
static volatile int globalChecks = 0;
// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	char *solution = (char*) malloc(sizeof(char) * 64);
	int * checks = (int *) malloc(sizeof(int) * 64);
	int result = 1;
	void* threadReturn;
	pthread_t threadsArry[THREADS_NUMBER];
	threadArguments args;
	pthread_mutex_t sum_mutex;
	pthread_mutex_t checks_mutex;
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_mutex_init(&checks_mutex, NULL);
	int i;
	if (argc == 1) {
		readFromSTDIN(&solution);
	} else {
		readFile(argv[1], &solution);
	}
	for (i = 0; i < 46; i++) {
		checks[i] = 0;
	}
	args.threadFinishCount =  (int*) malloc(sizeof(int));
	args.threadFinishCount[0]=1;
	args.solution = solution;
	args.checks = checks;
	args.sumMutex = &sum_mutex;
	args.checksMutex = &checks_mutex;
	for (i = 0; i < THREADS_NUMBER; i++) {
		args.id = i;
		pthread_create(&(threadsArry[i]), NULL, eightQueenCheck,
				(void *) &args);
	}
	pthread_cond_wait(&cond1, &lock);

	if (globalChecks == 46)
		printf("Solution is legal\n");
	else
		printf("Solution is not legal\n");
	for (i = 0; i < THREADS_NUMBER; i++) {
		pthread_join(threadsArry[i], NULL);
	}
	pthread_mutex_destroy(&sum_mutex);
	pthread_mutex_destroy(&checks_mutex);
	pthread_mutex_destroy(&lock);
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
	int threadid = arguments->id, fixedId, i, worknumber, result;
	int local;

	while (1) {
		worknumber = -1;
		pthread_mutex_lock(((threadArguments *) arg)->checksMutex);
		for (i = 0; i < 46; i++) {
			if (((threadArguments*) arg)->checks[i] == 0) {
				((threadArguments*) arg)->checks[i] = 1;
				worknumber = i;
				break;
			}
		}
		pthread_mutex_unlock(((threadArguments *) arg)->checksMutex);
		if (worknumber
				== -1&&((threadArguments*)arg)->threadFinishCount[0]<THREADS_NUMBER) { //thread didnt find work, end of progress
			((threadArguments*) arg)->threadFinishCount[0]++;
			break;
		} else if (worknumber
				== -1&&((threadArguments*)arg)->threadFinishCount[0]==THREADS_NUMBER) { //all threads finished works
			pthread_cond_signal(&cond1);
			break;
		} else if (worknumber < 8) {
			result = rowsCheck(arguments->solution, worknumber);
		} else if (worknumber < 16) {
			fixedId = worknumber - 8;
			result = colsCheck(arguments->solution, fixedId);
		} else if (threadid < 31) {
			fixedId = worknumber - 16;
			result = rightCrossCheck(arguments->solution, fixedId);
		} else {
			fixedId = worknumber - 31;
			result = leftCrossCheck(arguments->solution, fixedId);
		}
		pthread_mutex_lock(((threadArguments *) arg)->sumMutex);
		local = globalChecks;
		local += result;
		globalChecks = local;
		pthread_mutex_unlock(((threadArguments *) arg)->sumMutex);
	}
	//printf("%d\n",globalChecks);
	return NULL;
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
