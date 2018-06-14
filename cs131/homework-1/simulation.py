#!/usr/bin/env python

import sys
import math
import numpy as np
import random

PROCESSOR_CONFIGS = [2**i for i in range(1,7)]
DEFAULT_SEED = 1

DISTRIBUTION_TYPES = ["UNIFORM","GAUSSIAN"]

def random_uniform(start,stop,moduleCount):
	return int(np.random.uniform(start,stop,1)[0]) % moduleCount

def random_gaussian(mean,sigma,moduleCount)
	return int(np.random.normal(mean,sigma,1)[0]) % moduleCount


def run_simulation_instance(requests,distribution=DISTRIBUTION_TYPES[0]):
	pass

def run_session():
	pass	

if __name__ == "__main__":
	print("Executing Program %s" % (sys.argv[0]))
	
	seed = DEFAULT_SEED
	if len(sys.argv) > 1:
		seed = int(sys.argv[1])

	np.random.seed(seed)

	sys.exit()