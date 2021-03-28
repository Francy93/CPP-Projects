#creating the g++ variable containing the compiler instruction
CXX = g++ -std=c++2a
#creating the -c -Wall variable containing compilation parameters
CXXFLAGS = -c -Wall
#variable containing the final.exe file name
TARGET = librarySystem
#variable containing the testing.exe file name
TESTS = tests
#header varable
HEADER = library.hpp

#Making all executable
all: final #tests final
#compiling the two files to get the final one which would be main.cpp and components.cpp
final: algo.o operations.o library.o
	$(CXX) algo.o operations.o library.o -o $(TARGET)
#compiling the testing unit
#tests: testing.o
#	$(CXX) testing.o -o $(TESTS)


#compiling just the main.cpp file even though main,h is not compulsory, it is just for precaution
library.o: library.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) library.cpp

#compiling just the operations.cpp file
operations.o: operations.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) operations.cpp

#compiling just the algo.cpp file
algo.o: algo.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) algo.cpp

#compiling just the testing.cpp file
testing.o: testing.cpp catch.hpp $(HEADER)
	$(CXX) $(CXXFLAGS) testing.cpp


#clearing all copild files
clean:
	rm *.o $(TARGET) $(TESTS)


#following instruction was used previously used for minGW: mingw32-make -f makefile clear