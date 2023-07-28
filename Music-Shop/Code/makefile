#creating the g++ variable containing the compiler instruction
CC = g++
#creating the -c -Wall variable containing compilation parameters
CFLAGS = -std=c++17  -c -Wall

#compiling the two files to get the final one which would be musicShop.cpp and components.cpp
final: util.o components.o operations.o musicShop.o
	$(CC)  musicShop.o -o final 
#util.o components.o operations.o

#compiling just the musicShop.cpp file even though musicShop,h is not compulsory, it is just for precaution
musicShop.o: musicShop.cpp musicShop.hpp
	$(CC) $(CFLAGS) musicShop.cpp

util.o: util.cpp musicShop.hpp
	$(CC) $(CFLAGS) util.cpp

#compiling just the components.cpp file
components.o: components.cpp musicShop.hpp
	$(CC) $(CFLAGS) components.cpp

#compiling just the operations.cpp file
operations.o: operations.cpp musicShop.hpp
	$(CC) $(CFLAGS) operations.cpp

#clearing all copild files
clean:
	rm *.o final


#mingw32-make -f makefile clear