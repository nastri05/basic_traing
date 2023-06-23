#all: 
#	cd ..
	# The cd above does not affect this line, because each command is effectively run in a new shell
#	echo `pwd`

	# This cd command affects the next because they are on the same line
#	cd ..;echo `pwd`

	# Same as above
#	cd ..; \
#	echo `pwd`
#make_var = I am a make variable
#all:
	# Same as running "sh_var='I am a shell variable'; echo $sh_var" in the shell
#	sh_var='I am a shell variable'; echo $$sh_var

	# Same as running "echo I am a make variable" in the shell
#	echo $(make_var)
new_contents = "hello:\n\ttouch inside_file"
all:
	mkdir -p subdir
	printf $(new_contents) | sed -e 's/^ //' > subdir/makefile
	cd subdir && $(MAKE)

clean:
	rm -rf subdir