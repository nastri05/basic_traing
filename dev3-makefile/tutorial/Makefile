#hello:
	#echo "Hello, World"
	#echo "This line will always print, because the file hello does not exist."
blah: blah.o
		cc blah.o -o blah 
#Runs third
blah.o: blah.c
	cc -c blah.c -o blah.o 
# Runs second

# Typically blah.c would already exist, but I want to limit any additional required files
blah.c:
	echo "int main() { return 0; }" > blah.c # Runs first
