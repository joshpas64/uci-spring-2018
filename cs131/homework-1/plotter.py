from __future__ import (absolute_import, division,
                        print_function, unicode_literals)
import sys, csv
import matplotlib
matplotlib.use('agg')

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.legend_handler import (HandlerLineCollection,
                                       HandlerTuple)
import matplotlib.collections as mcol
from matplotlib.lines import Line2D

from collections import defaultdict


def np_translate(dataset):
	"""Converts all the lists or iterable data structures in the simulation dataset into numpy arrays"""
	for key,axis in dataset.items():
		for inner_key in axis:
			## Get the x and y arrays for each processor configurations and convert them into numpy arrays
			dataset[key][inner_key] = np.array(dataset[key][inner_key])

def display_dataset(dataset):
	"""Output all the data used to plot a dataset (uniform or gaussian) and direct it to stdout"""
	for key in dataset:
		print("Processor %s" % (key))
		print("Data")
		print("X-axis",dataset[key]["x"])
		print("Y-axis",dataset[key]["y"])


def create_graph(dataset,outfile,title):
	"""Create a multiline graph plot from a simulation dataset (using Gaussian or uniform distribution) and save it to an output file."""
	
	## To make our plot easy to read we will need a legend so we know which line corresponds to which processor configuration.
	## We will need to map each line to a label that will show
	
	legend_keys = [] ## This stores the labels for the lines in our legend
	legend_data = [] ## This will store the lines that will be used by our labels in the legend

	##Color keys for the different processor counts
	legend_colors = ['g','r','c','m','y','k']

	# idx = 0
	figure,ax = plt.subplots()

	## Sort the keys (the processor configurations) in increasing order {2,4,8,...}
	sorted_keys = sorted(dataset.keys(),key=lambda item: int(item))
	
	for idx,key in enumerate(sorted_keys):
		## Create a for a given processor configuration and choose its color
		line, = ax.plot(dataset[key]["x"],dataset[key]["y"],linewidth=0.5,color=legend_colors[idx])

		legend_keys.append(key) ##Create a label for this line
		legend_data.append(line) ## Add this line to the legend.

	## Create a legend for our plot
	ax.legend(legend_data,legend_keys,loc='upper right',shadow=True)

	## Set X and Y labels as well as the title.
	ax.set_xlabel('# of Memory Modules')
	ax.set_ylabel('Average Wait Time (Memory Cycles)')
	ax.set_title(title)

	## Save the plots into the project directory as images.
	plt.savefig(outfile,dpi=500)

if __name__ == "__main__":
	## Default data source files for the uniform and gaussian distribution graphs / plots.
	uniform_file = "logs/uniformLogs.csv"
	gaussian_file = "logs/gaussianLogs.csv"

	delimiter = ","
	delimiter = str(delimiter).encode('utf-8')

	## Override the default data source files if there are alternative files specified on the command-line
	if len(sys.argv) > 1:
		uniform_file = sys.argv[1]
	if len(sys.argv) > 2:
		gaussian_file = sys.argv[2]

	## To create a plot with multiple lines on the graph an easy approach is to just create a nested dictionary

	## In the plot, there will be a line for each processor configuration (2 processors, 4 processsors, etc)

	## The first key will be for the different count of processors.
	## This key's value will be a two-item dictionary for storing the x and y data points for that processor configuration's line.
	## Ex: uniform_data[2] = {"x":[1,2,3],"y":[4.3,5.6,6.7]}
	## Example data source for the uniform distribution graph: 
	## uniform_data = {
	# 	2 : {"x": [1,2,3], "y":[4,5,6]},
	# 	4: {"x":[1,2,3],"y":[4,5,6]}
	# }


	## There will be two nested dictionaries, one for containing all the data for the simulations that utilized a uniform distribution,
	## another for one that utilized a gaussian distribution
	processor_data_uniform = defaultdict(lambda:defaultdict(list))
	processor_data_gaussian = defaultdict(lambda: defaultdict(list))

	##First we will get all the data from the uniform simulation and create data points.
	## This will consist of an 'x' array and a 'y' array to make the points for each line.
	with open(uniform_file,'rb') as uniform_csv:
		## Open the csv files and read their data line-by-line; column-by-column
		reader = csv.reader(uniform_csv,delimiter=delimiter)
		reader.next()

		##Go through the file row-by-row and create a data point for a certain line in our graph
		for row in reader:
			## A row in the *.csv file will contain three items in this order.
			## 1. The number of processor that was used in that simulation cycle.
			## 2. The number of memory modules that was used in that simulation cycle.
			## 3. The average time a processor had to wait to access a memory module during the simulation (in cycles)
			processors,memory_modules,waitTime = row

			## In the graph the x-axis will be number of memory modules for a given simulation configuration
			## The y-axis will be the average wait time a processor has for a given simulation configuration

			## Each row will form a point for a given line in our graph.
			## There will be a line for each 
			processor_data_uniform[processors]["x"].append(float(memory_modules)) ##Add memory module count as x-coordinate
			processor_data_uniform[processors]["y"].append(float(waitTime)) ## Add average wait-time as y-coordinate



	## Repeat the same process for the gaussian data-source. 
	## All steps will be the same as when reading through the csv file that had the data for uniform request simulations.
	with open(gaussian_file,'rb') as gaussian_csv:
		reader = csv.reader(gaussian_csv,delimiter=delimiter)
		reader.next()

		for row in reader:
			processors,memory_modules,waitTime = row

			processor_data_gaussian[processors]["x"].append(float(memory_modules))
			processor_data_gaussian[processors]["y"].append(float(waitTime))


	## Convert all arrays for that uniform and gaussian data-sources into numpy arrays
	np_translate(processor_data_uniform)
	np_translate(processor_data_gaussian)

	## Create a plot for gaussian simulations and uniform simulations and save the into *.jpg files.
	create_graph(processor_data_uniform,'plots/uniform.jpg','Uniform Distribution')
	create_graph(processor_data_gaussian,'plots/gaussian.jpg','Gaussian Distribution')

