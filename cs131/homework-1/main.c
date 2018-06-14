#include "simulator.h"

int main(int argc, char** argv){
	printf("Program: %s\n",argv[0]);	
	printf("Successfully included all files");

	//Get name of *.csv files to redirect to when the simulation is done
	//These files will hold the data to create the gaussian and uniform distribution plots.

	const char* uniformLog = argc > 1 ? argv[1] : "logs/uniformLogs.csv";
	const char* gaussianLog = argc > 2 ? argv[2] : "logs/gaussianLogs.csv";
	const int seed = argc > 3 ? atoi(argv[3]) : 1;


	printf("Setting up random number generator with seed %d\n",seed);
	//Initialize random number generator with predefined seed.
	srand(seed);	

	//We will use a total of 6 processor configurations for this simulation
	//2 simulations will be tested for memory modules of 1 - 2048 memory modules for 2 processors requesting memory access.
	//One simulation will be done with the requested memory module being generated from a gaussian distribution and
	//another being generated from a uniform distribution.

	//Another 2 simulations will be done the same way for a processor configuration of 4 processors, than 8, etc.

	int processorConfigs[PROCESSOR_CONFIGURATION_COUNT] = {2,4,8,16,32,64};
	
	//Run simulation for all possible memory module configurations for each of the 6 different processor counts.
	run_session(processorConfigs,PROCESSOR_CONFIGURATION_COUNT,DEFAULT_MAX_MEMORY_MODULES,uniformLog,gaussianLog);
		
	return 0;
}
