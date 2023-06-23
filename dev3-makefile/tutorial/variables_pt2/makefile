# Recursive variable. This will print "later" below
#one = one ${later_variable}
# Simply expanded variable. This will not print "later" below
#two := two ${later_variable}

#later_variable = later

#all: 
#	echo $(one)
#	echo $(two)
#one = hello
# one gets defined as a simply expanded variable (:=) and thus can handle appending
#one := ${one} there

#all: 
#	echo $(one)
one = hello
one ?= will not be set
two ?= will be set

all: 
	echo $(one)
	echo $(two)