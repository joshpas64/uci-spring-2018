#include "simulator.h"
#include "queue.h"

#include <math.h>


//Generate a random number between a fixed range [Minumum,Maximum]
int uniformRange(int min,int max){
	int delta = max - min;
	unsigned int num = (rand() % delta) + min;
	return num;
}

//Generate a random gaussian number with a 
static double randGauss(double mean,double sigma){
	double x = (double) random() / RAND_MAX;
	double y = (double) random() / RAND_MAX;

	double z = mean + (sqrt(-2 * log(x)) * cos(2 * M_PI * y) * sigma);
	return z;
}

//Initialize a memory queue data structure for holding
//the processors that are still waiting to access the resource.
void init_queue(memoryQueue* memQueue){
	memQueue->attachedProcess = -1;
	memQueue->queue = NULL;
}

//Method for adding a process to a memory module's waiting queue.
//Is FIFO or first-come first-served meaning an incoming process must first 
//wait for the processes that were there waiting before it to access the memory
//module.
void pushMemQueue(memoryQueue* memQueue,int process){
	if(memQueue->queue == NULL){
		node* insertionNode = createNode(process);
		memQueue->queue = insertionNode;
	} else {
		push(&(memQueue->queue),process);
	}
}

//A way to check if a memory module can give access to a certain process
bool check_availability(int process,memoryQueue* memQueue){
	//A memory module can give access to a processed if its wait queue is empty, the memory queue has just been
	//initialized or if the memoryQueue was scheduled to service that process beforehand.
	if(memQueue == NULL || memQueue->attachedProcess == -1 || memQueue->attachedProcess == process){
		return true;
	} 

	return false;
}

//A simulator cycle will be used to calculate the average wait time 
//for a system with (k) processors and (m) memory modules.

//We use and initialize a simulator struct (defined in 'simulator.h')
// to do this.
void setup_simulator(simulator* sim,int processCount, int modules){
	//Set the simulator's processor count and number of memory modules.
	sim->processCount = processCount;
	sim->moduleCount = modules;

	//Allocate an array for storing each processor's current access request 
	sim->processes = (int*) malloc(processCount * sizeof(int));

	//Allocate an array for storing each processor's total amount of cycles it has had
	//to wait in the simulation
	sim->waitTimes = (int*) malloc(processCount * sizeof(int));

	//Allocate an array for storing each processor's priority in case of concurrent access clashes.
	sim->priorities = (int*) malloc(processCount * sizeof(int));

	//Allocate an array of memory modules and its corresponding waiting queues
	sim->memories = (int*) malloc(modules * sizeof(int));	//Used to indicate (with 0 or 1) if the memory module is currently available
	sim->queues = (memoryQueue*) calloc(modules,sizeof(memoryQueue)); //Used for prioritizing processors that have been waiting longer to access a memory module.

	int i;
	for(i = 0; i < processCount; i++){
		sim->processes[i] = -1;
		sim->waitTimes[i] = 0; //All processes start out having never waited for access to a memory resource
		sim->priorities[i] = i;//Have the priorities simply be the processor's index in the processor array
	}

	for(i = 0; i < modules; i++){
		sim->memories[i] = 0;//All memory modules begin as available
		init_queue(&(sim->queues[i]));//Initialize the pointers for their waiting queues.
	}	
}

void run_simulator(simulator* sim,distribution dist,FILE* file){
	int i,process_idx,k,sample;

	//Each processor will have its own local mean if it generates memory access requests 
	//using a Gaussian distribution.

	//The means for each processor will stay the same during the whole simulation
	// as a way to simulate locality of reference for memory access.
	int* processorMeans = (int*) malloc(sim->processCount * sizeof(int));

	//In the case that the simulation wants to generate memory module requests with
	//a Gaussian distribution.
	//The sigma will be the number of memory modules divided by 5.0.
	double sigma = (double) (sim->moduleCount) / 3.0;

	//Create the first batch of memory requests
	for(i = 0; i < sim->processCount; i++){
		if(dist == Uniform){
			//If the distribution needs to be uniform for access requests
			//generate them using a Uniform distribution.
			sample = uniformRange(0,sim->moduleCount) % sim->moduleCount;
		} else if(dist == Gaussian){
			//If the distribution needs to be gaussian for access requests
			//generate them using a Gaussian distribution.
			
			//The mean for the processor is selected using a uniform distribution
			int mean = uniformRange(0,sim->moduleCount);

			//The gaussian number representing the access requests will be generated
			//with the recently created mean and previously defined sigma (variance).
			sample = abs((int) (randGauss(mean,sigma)) % sim->moduleCount);

			//Store the mean for the corresponding processor for later use in later request cycles.
			processorMeans[i] = mean;
		}

		//Assign the memory module to the processor
		sim->processes[i] = sample;
	}


	//Initializers for checking the simulation termination condition
	// (when the past average is different from the current wait time average by less than 0.02%)
	double pastAverage = -1.0;
	double currentAverage = -1.0;
	double percentDiff = 1.0;
	i = 1;

	//Simulate access requests until the past waiting average differs from the current by less than 0.02%
	while(i++){
		//Set past average to the last cycle's current average
		pastAverage = currentAverage;

		//Check if each processor got access to the memory module it request
		for(process_idx = 0; process_idx < sim->processCount; process_idx++){

			//If the memory module the process accessed was available (==0) and is available (meaning its wait queue) is empty
			//then the process got access to the memory module and can generate another access request.
			if(sim->memories[sim->processes[process_idx]] == 0 || check_availability(process_idx,&(sim->queues[sim->processes[process_idx]]))){
				//Generate a new memory module to request from using either a Uniform or Gaussian distribution.
				if(dist == Uniform){
					sample = uniformRange(0,sim->moduleCount) % sim->moduleCount;
				} else if(dist == Gaussian){
					sample = abs((int) (randGauss(processorMeans[process_idx],sigma)) % sim->moduleCount);
				}

				//Assign the memory module to that process
				sim->processes[process_idx] = sample;

				//Indicate that the memory module's currently attached process is the newly assigned process
				sim->queues[sample].attachedProcess = process_idx;

				//Indicate that the memory module is now in use
				sim->memories[sample] = 1;
			} else {

				//In the case that the memory module is not available to the process
				//then the memory module must now wait, thus adding to the total amount of times
				//the process has had to wait for access to resources.
				sim->waitTimes[process_idx]++;

				//Add the process to the memory module's waiting queue if it is not already in there.
				if(!contains(&(sim->queues[sim->processes[process_idx]].queue),process_idx)){
					pushMemQueue(&(sim->queues[sim->processes[process_idx]]),process_idx);
				}
			}
		}

		//Each cycle, each memory module must determine which process it should give access to next.
		//Usually this goes to the process at the front of its waiting queue.

		//The assignment is done by setting the 'attachedProcess' field to the process number (or index)
		//In the case that the memory module's wait queue is empty, simply choose the process with the lower
		// wait queue.

		for(k = 0; k < sim->moduleCount; k++){
			if(sim->queues[k].queue != NULL){
				//Get process id / index of the process at the front of the process's wait queue.
				//and assign to the current memory module.
				int nextProcess = pop(&(sim->queues[k].queue));
				sim->queues[k].attachedProcess = nextProcess;
			}

			//If the wait queue is empty, mark the memory module as immediately available to process that may 
			//request it in the next cycle
			if(empty(&(sim->queues[k].queue))){
				sim->memories[k] = 0;
			} 
		}

		//Calculate the average waiting time for all processors to access a memory module.
		currentAverage = getAverageWaitTime(sim,i);

		if(pastAverage >= 0){
			//This ternary operations are simply to prevent the case of when the past 
			//and current wait time averages are both 0. It is to prevent a 0/0 undefined error
			pastAverage = pastAverage == 0 ? 0.1 : pastAverage;
			currentAverage = currentAverage == 0 ? 0.1 : currentAverage;

			//Calculate the percentage difference between the current wait time average and the past one.
			percentDiff = fabs(1.0 - (currentAverage / pastAverage));
		}

		//Terminate when the wait times hit an asymptote or a point where they do not change anymore
		//A difference in values of < 0.02%

		if(percentDiff < 0.0002){
			break;
		}
	}

	//Write all the data in CSV row format so an outside library (in this case Python's Matplotlib)
	//can use it as a data source for a line graph
	fprintf(file, "%d,%d,%f\n",sim->processCount,sim->moduleCount,getAverageWaitTime(sim,i));

	//Free array for processor means
	free(processorMeans);
}


//In C all dynamically allocated memory must be manually freed by the programmer
//to prevent memory leaks. This function simply releases all the memory that would be
//stored in a our simulator object (struct).
void free_simulator(simulator* sim){
	int i;

	//Free the arrays for the processors, wait times, and priorities
	free(sim->processes);
	free(sim->waitTimes);
	free(sim->priorities);
	
	//Free the memory allocated to each node in the memory's wait queue
	for(i = 0; i < sim->moduleCount; i++){
		destroyQueue(&(sim->queues[i].queue));
	}

	//Free the arrays for the memory modules and the memory's modules
	//corresponding wait queues.
	free(sim->memories);
	free(sim->queues);
}

//Way to calculate the average wait time for (N) processes given
//(k) request cycles.
double getAverageWaitTime(simulator* sim,int requests){
	double average = 0;
	int i;

	for(i = 0; i < sim->processCount; i++){
		average += ((double) (sim->waitTimes[i]) / requests); //Increment by the processor's average wait time.
	}

	//Average all wait time averages by dividing by number of processors.
	average /= sim->processCount;
	return average;
}


//This will run the whole simulation session for different processor configurations (defined in parameter 'processorConfigs')
//It will run simulation cycles from configurations of 1 to (modules) memory modules.

//It will write the data into log files (*.csv files) for future reference that can be used by outside libraries to create plots
void run_session(int* processorConfigs,int configSize,int modules,const char* uniformLogs,const char* gaussianLogs){
	int i,moduleCount;

	//Run with both Uniform and Gaussian distributions
	distribution uniform = Uniform;
	distribution gaussian = Gaussian;

	FILE* uniformFile;

	//Open log file for writing.
	//This file will be used for storing results from simulations where the distribution of 
	//memory module access requests is Uniform.
	uniformFile = fopen(uniformLogs,"w");
	fprintf(uniformFile,"processors,memory modules,wait-times\n");

	//Run one simulation cycle each for each processor and memory module configuration
	for(i = 0; i < configSize;i++){
		for(moduleCount = 1;moduleCount < modules + 1;moduleCount++){


			//Setup run, and free a simulation cycle and write the data to the data files.
			simulator sim;
			setup_simulator(&sim,processorConfigs[i],moduleCount);

			run_simulator(&sim,uniform,uniformFile);

			free_simulator(&sim);
		}
	}

	//Close file.
	fclose(uniformFile);

	FILE* gaussianFile;

	//This file will be used for storing results from simulations where the distribution of 
	//memory module access requests is Uniform.
	gaussianFile = fopen(gaussianLogs,"w");
	fprintf(gaussianFile, "processors,memory modules,wait-times\n");

	//Run one simulation cycle each for each processor and memory module configuration
	for(i = 0; i < configSize;i++){
		for(moduleCount = 1;moduleCount < modules + 1;moduleCount++){

			//Setup run, and free a simulation cycle and write the data to the data files for gaussian distributions.
			simulator sim;
			setup_simulator(&sim,processorConfigs[i],moduleCount);

			run_simulator(&sim,gaussian,gaussianFile);

			free_simulator(&sim);
		}
	}

	//Close file.
	fclose(gaussianFile);
}

