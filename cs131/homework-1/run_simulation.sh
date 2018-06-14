#!/bin/bash

SEED=1
UNIFORM_LOG=logs/uniformLogs.csv
GAUSSIAN_LOG=logs/gaussianLogs.csv

PYTHON_FILE=plotter.py

LOG_DIR=logs/
PLOTS_DIR=plots/


NUM_REQUESTS=100
MODULE_COUNT=2048

CREATE_GRAPHS="${1:-no}"
CREATE_GRAPHS=`echo $CREATE_GRAPHS | tr "[:upper:]" "[:lower:]"`

echo "Compiling source code."
make clean
make

echo "Performing simulation with $NUM_REQUESTS requests and up to $MODULE_COUNTS memory modules."
./main $UNIFORM_LOG $GAUSSIAN_LOG $SEED

echo "Simulation done. Data stored in $LOG_DIR directory."

if [ "$CREATE_GRAPHS" == "yes" ];then
	echo "Preparing plots. Plot images will be store in $PLOT_DIR directory."
	echo "Will be using Python 2.7, numpy, and matplotlib to create graphs"
	echo "Ensure you have these packages installed and have write permissions in the $PWD directory."

	echo "To install you need pip and sudo privileges."
	echo "sudo pip install numpy matplotlib --upgrade"
	python $PYTHON_FILE
	echo "Plots successfully made!"
fi

echo "Cleaning directory."
make clean
