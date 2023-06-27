#!/bin/bash


# Author : hxtuan
# Copyright (c) Tutorialspoint.com
# Script follows here:
HELLO="Xin chào, "
HELLO=$(printf "%s %s" "$HELLO" "$(whoami)" "!")
DAY="Hôm nay là ngày "
DAY=$(printf "%s %s" "$DAY" "$(date)")
echo $HELLO
echo $DAY
