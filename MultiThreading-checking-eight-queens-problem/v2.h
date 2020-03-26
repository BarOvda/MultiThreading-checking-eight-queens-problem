/*
 * v2.h
 *
 *  Created on: Dec 31, 2019
 *      Author: afeka
 */

#ifndef V2_H_
#define V2_H_
#define THREADS_NUMBER 4
#define ROWS_COLS 8
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
int readFile(char fileName[], char** solution);
int readFromSTDIN(char** solution);
int rowsCheck(char* solution, int threadId);
void * eightQueenCheck(void * arg);
int colsCheck(char* solution, int threadId);
int leftCrossCheck(char* solution, int threadId);
int rightCrossCheck(char* solution, int threadId);

typedef struct {
	char *solution;
	int * checks;
	int id;
	int* threadFinishCount;
	pthread_mutex_t *sumMutex;
	pthread_mutex_t *checksMutex;
} threadArguments;



#endif /* V2_H_ */
