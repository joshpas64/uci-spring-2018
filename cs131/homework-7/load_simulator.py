#!/usr/bin/python

from __future__ import (absolute_import, division,
                        print_function, unicode_literals)

import random
import sys
import csv

MAX_PROCESSORS = 100

MIN_LOAD = 10
MAX_LOAD = 1000
MIN_TICK_INTERVAL = 100
MAX_TICK_INTERVAL = 1000

MAX_CYCLES = 1000000
TRIAL_COUNT = 10


def show_definitions():
	"""Definition of Key Terms used for detailing how the this Load Balancing Simulator is Implemented"""
	print("\n\n\n")

	terms = {
		"Connection Network": "Every processor is connected to two other processor, one on the left and another on the right." + \
		"\nThey are connected to form a ring, the first processor is connected on the left to the last processor." + \
		"\nProcessor 0 of n processors is connected to Processor n - 1 on the left and Processor 1 on the right.",
		"Locally Balanced": "A processor is locally balanced if the load difference between it and its left and right neighbor processor is less than or equal to 1.",
		"System Balanced": "The system is considered 'balanced' when all the processors are locally balanced",
		"Distributed Load Balancing": "Processors will at random points check their neighbors, and give or take load to be balanced with its neighbors",
		"Centralized Load Balancing": "The load of all processors will be checked and an average range is generated" + \
		"\neach new processor will now have that load value, by either giving or taking load from other processors",
		"Strict Balance":"System is not balanced until all processors have a certain load amount",
		"General Balance": "System is not balanced until all processors are 'locally balanced'"
	} 

	idx = 1
	for key, value in terms.items():
		print("%d. %s:\n\t%s" % (idx,key,value))
		idx += 1

	print("\n\n\n")


def _is_balanced(source,average,inclusive=True):
	"""Checks if a workload is balanced by ensuring all the processing elements are locally balanced"""
	size = len(source)
	for i in range(size):
		left_diff = source[i] - source[(i - 1) % size] ## Difference in load between left connected processor and current processor
		right_diff = source[i] - source[(i + 1) % size] ## Difference in load between right connected processor and current processor

		## The system is not balanced if any processor is not 'locally balanced'
		if left_diff > 1 or right_diff > 1 or left_diff < -1 or right_diff < -1:
			return False
	return True

def _is_fully_balanced(source,average,inclusive=True):
	"""Check if a workload is fully balanced by ensuring that the load is spread and distributed evenly among all processors. 
	This means every processor must have load value that is an 'average' value."""
	bounds = []
	if inclusive:
		bounds = [average,average+1]
	else:
		bounds = [average,average]

	## The system is not balanced if any processor's load is not within any accept average range (bounds) 
	for load in source:
		if load not in bounds:
			return False
	return True


def balance_node(source,left,node,right):
	"""Perform a full load balance operation on a set of three nodes"""
	total = source[left] + source[node] + source[right] ## Get total load of all 3 nodes
	balanced_load = total / 3 ## Get adjusted balanced load value.

	## Reassign the 3 processors to be have the adjusted load value
	## Leaving the 3 processors to be balanced with respect to the other

	if total % 3 == 1:
		source[left],source[node],source[right] = (balanced_load,balanced_load+1,balanced_load)
	elif total % 3 == 2:
		source[left],source[node],source[right] = (balanced_load,balanced_load+1,balanced_load+1)
	else:
		source[left],source[node],source[right] = (balanced_load,balanced_load,balanced_load)


def test_balance(source,left,node,right):
	"""Test if a processor needs to be load balanced"""
	left_diff = source[node] - source[left] ## Difference in load between current processor and left processor
	right_diff = source[node] - source[right] ## Difference in load between current processor and left processor

	## Load Balancing is only performed when the current processor
	## has more than 1 or more load units than either of its neighbors (left or right)

	if left_diff > 1 and right_diff > 1:
		balance_node(source,left,node,right) ## Perform an optimized balance if the current processor has more load than both of its neighbors
	elif left_diff > 1:
		total = source[node] + source[left] ## When the current processor has more load than only its left processor neighbor
		avg = total / 2 ## Get adjusted load value to assign to the current processor and its neighbor
		
		if total % 2 == 0:
			source[left],source[node] = (avg,avg)
		else:
			source[left],source[node] = (avg,avg + 1)
	elif right_diff > 1:
		total = source[node] + source[right] ## When the current processor has more load than only its right processor neighbor
		avg = total / 2 ## Get adjusted load value to assign to the current processor and its neighbor
		
		if total % 2 == 0:
			source[node],source[right] = (avg,avg)
		else:
			source[node],source[right] = (avg,avg + 1)


def balance_cycle(workloads,balance_op):
	"""Performs a local Load Balance Operation on all processors in a workload system."""
	size = len(workloads)
	for i in range(size):
		balance_op(workloads,((i - 1) % size),i,((i + 1) % size)) ## Perform a load balance on every processors with its left and right neighbor


def distributed_balance(workloads,balance_condition,balance_op):
	num_cycles = 0
	size = len(workloads)

	total = sum(workloads)
	average = total / size
	precise_average = float(total) / size

	inclusive = average != precise_average

	## Assign a cycle in which each processor will balance its load among its neighbors
	next_balance_cycles = [random.randint(MIN_TICK_INTERVAL,MAX_TICK_INTERVAL+1) for i in range(size)]

	## Continue until all the processors are 'locally balanced'
	while balance_condition(workloads,average,inclusive) == False and num_cycles < MAX_CYCLES:
		for i in range(size):
			if num_cycles >= next_balance_cycles[i]: ## If it is time for a processor to perform a local load balance
				next_balance_cycles[i] += random.randint(MIN_TICK_INTERVAL,MAX_TICK_INTERVAL+1) ## Set cycle for another load balance

				balance_op(workloads,((i - 1) % size),i,((i + 1) % size))
		num_cycles += 1

	return num_cycles, workloads




def centralized_balance(workloads,balance_condition,balance_op):
	num_cycles = 0
	size = len(workloads)

	total = sum(workloads)
	average = total / size
	precise_average = float(total) / size

	inclusive = average != precise_average

	## Continue until all the processors are considered 'balanced'
	while balance_condition(workloads,average,inclusive) == False and num_cycles < MAX_CYCLES:
		balance_cycle(workloads,balance_op) ## Balance the load of all processors in the multiprocessor system with its left and right neighbors
		num_cycles += 1

	return num_cycles, workloads


def simulate(file,workloads,balance_condition,balance_op,processors=MAX_PROCESSORS,trials=TRIAL_COUNT):
	workload_copy = workloads[:]

	print("Initial Workload:\n{}".format(workload_copy))
	print("Using load balancing distribution strategy mentioned in the write up for HW7")



	## Perform Load Balancing and calculate amount of cycles it took
	## Calculate number of Cycles it takes to balance the workload for 'n' processors
	num_cycles, adjusted_workload = distributed_balance(workload_copy,balance_condition,balance_op)

	print("Completed load balancing for %d processors after %d cycles" % (processors,num_cycles))

	
	return num_cycles, adjusted_workload


if __name__ == "__main__":
	print("Executing Program: %s" % (sys.argv[0]))

	show_definitions() 

	## Number of trials simulations for each processor configuration
	trials = TRIAL_COUNT

	if len(sys.argv) > 1:
		try:
			trials = int(sys.argv[1])
			if trials <= 0:
				trials = TRIAL_COUNT
		except ValueError:
			print("Error processing argument %s" % (sys.argv[1]))

	## Processor Count Configurations
	processor_configs = [5,10,15,25,50,75,MAX_PROCESSORS]

	file = open("data/initial-readings.csv","w") ## Open data source for later spreadsheet processing
	file.write("num_processors,cycle-time\n") ## Write .csv headers

	## Do n trial simulations for every processor configuration and calculate the average amount of 
	## cycles it takes to load balance  a workload for 'm' processors
	for config in processor_configs:
		cycle_total = 0 
		workloads =  [random.randint(MIN_LOAD,MAX_LOAD) for i in range(config)] ## Create an initial workload for the processor configuration
		for i in range(trials):
			num_cycles, w = simulate(file,workloads,_is_balanced,test_balance,config,trials) ## Calculate number of cycles it took to load balance in this simulation
			cycle_total += num_cycles ## Add to the average
		avg = float(cycle_total) / trials ## Calculate average
		file.write("%d,%f\n" % (config,avg)) ## Write processor configuration and average cycles to load balance to *.csv file.
	file.close() ## Close *.csv file

	sys.exit()
