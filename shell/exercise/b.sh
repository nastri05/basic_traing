#!/bin/bash


# Author : hxtuan
# Copyright (c) Tutorialspoint.com
# Script follows here:

mkdir CAT
mkdir DOG

for i in {1..50}
do
	mv oneflie/STUPID_DOG_$i.txt DOG/
	mv oneflie/STUPID_CAT_$i.txt CAT/
done
