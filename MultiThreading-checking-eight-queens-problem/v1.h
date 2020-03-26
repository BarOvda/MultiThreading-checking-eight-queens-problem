/*
 * v1.h
 *
 *  Created on: Dec 22, 2019
 *      Author: afeka
 */

#ifndef V1_H_
#define V1_H_
#define THREADS_NUMBER 46
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
	int id;
} threadArguments;

#endif /* V1_H_ */
