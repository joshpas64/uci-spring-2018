#!/bin/bash

BASE=`basename $PWD`
ZIP_FILE="$BASE.zip"

rm -f plots/*
cd ../ && zip -r $ZIP_FILE $BASE/*
