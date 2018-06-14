#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define DEFAULT_MAX_MEMORY_MODULES 2048
#define PROCESSOR_CONFIGURATION_COUNT 6

typedef enum  {
	Uniform = 0,
	Gaussian = 1
} distribution;

typedef struct memoryQueue {
	int attachedProcess;
	node* queue;
} memoryQueue;

typedef struct simulator {
	int* processes;
	int* waitTimes;
	int* priorities;
	int* memories;
	memoryQueue* queues;

	int processCount;
	int moduleCount;
} simulator;

int uniformRange(int min, int max);
static double randGauss(double mean,double sigma);

void init_queue(memoryQueue* memQueue);
void pushMemQueue(memoryQueue* memQueue,int process);

bool check_availability(int process,memoryQueue* memQueue);

void setup_simulator(simulator* sim,int processCount,int modules);
void run_simulator(simulator* sim,distribution dist,FILE* file);
void free_simulator(simulator* sim);

double getAverageWaitTime(simulator* sim,int requests);
void run_session(int* processorConfigs,int configSize,int modules,const char* uniformLogs,const char* gaussianLogs);

#endif
