#creating the g++ variable containing the compiler instruction
CXX = g++
#creating the -c -Wall variable containing compilation parameters
CXXFLAGS = -std=c++17  -c -Wall
#variable containing the .exe file name
FINAL = librarySystem

#compiling the two files to get the final one which would be main.cpp and components.cpp
final: util.o components.o operations.o main.o
	$(CXX)  main.o operations.o -o $(FINAL)

#compiling just the main.cpp file even though main,h is not compulsory, it is just for precaution
main.o: main.cpp main.hpp
	$(CXX) $(CXXFLAGS) main.cpp

#compiling just the operations.cpp file
operations.o: operations.cpp main.hpp
	$(CXX) $(CXXFLAGS) operations.cpp

#clearing all copild files
clean:
	rm *.o $(FINAL)


#mingw32-make -f makefile clear