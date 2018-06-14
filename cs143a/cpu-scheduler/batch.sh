#!/bin/bash

if [ "$#" -ne 1 ] && [ "$#" -ne 2 ] || ! [ -d "$1" ]; then
	>&2 echo "Usage: $0 DIRECTORY [TARGET]"
	exit -1
fi

SIMU="simu.sh"
if ! [ -f "$SIMU" ]; then
	>&2 echo "Cannot find simulation script."
	exit -3
fi

target=$1
if [ "$#" -eq 2 ]; then
	mkdir -p $2
	if ! [ -d "$2" ]; then
		>&2 echo "Cannot create TARGET directory."
		exit -2
	fi
	touch "$2/file"
	if ! [ -f "$2/file" ]; then
		>&2 echo "Cannot access TARGET directory."
		exit -2
	fi
	target=$2
fi

PARS=("FCFS" "SJF" "SJF true" "Priority" "Priority true" "RR 20")
TIMEOUT=10

find "$1" -name "case_*.jobs.txt" | while read line; do
	bn=$(basename "$line")
	fn="${bn%.*}"
	for ((ind=0; ind<${#PARS[@]}; ++ind)); do
		par=${PARS[$ind]}
		cm="bash $SIMU $line $par >$target/$fn.out_$ind 2>$target/$fn.err_$ind"
		>&2 echo $cm
		eval $cm
	done
done

