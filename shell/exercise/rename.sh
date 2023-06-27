#!/bin/bash


# Author : hxtuan
# Copyright (c) Tutorialspoint.com
# Script follows here:

mkdir CAT
mkdir DOG

for i in {1..50}
do
	if [ -e oneflie/STUPID_CAT_$i.txt ]
	then
		mv oneflie/CAT_$i.txt oneflie/STUPID_CAT_NEW_$i.txt
	else
		mv oneflie/CAT_$i.txt oneflie/STUPID_CAT_$i.txt
	fi
	if [ -e oneflie/STUPID_DOG_$i.txt ]
	then
		mv oneflie/DOG_$i.txt oneflie/STUPID_DOG_NEW_$i.txt
	else
		mv oneflie/DOG_$i.txt oneflie/STUPID_DOG_$i.txt
	fi
done