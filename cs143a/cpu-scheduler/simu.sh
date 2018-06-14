#!/bin/bash

EXEC="java"
FILE="CPUSchedulingSimulator.jar"

if ! >&2 type -p $EXEC; then
	>&2 echo "JVM is not found."
	>&2 echo "Please install Java Development Kit (JDK) first."
	exit -1
fi

if [ ! -f "$FILE" ]; then
	>&2 echo "Runnable jar for the simulation is not found."
	>&2 echo "Please compile your project first."
	exit -2
fi

$EXEC -jar $FILE "$@"

