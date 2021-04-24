#creating the g++ variable containing the compiler instruction
CXX = g++ -std=c++2a
#creating the -c -Wall variable containing compilation parameters
CXXFLAGS = -g -Wall -Wextra
#variable containing the final.exe file name
TARGET = librarySystem
#variable containing the testing.exe file name
TESTS = tests
#header varable
HEADER = library.hpp
#files list
FILES = global.o collection.o books.o algo.o operations.o library.o
#exes list
ALLEXE = $(TESTS) $(TARGET)


.PHONY : all
#Making all executable
all: $(ALLEXE)
#compiling the two files to get the final one which would be main.cpp and components.cpp
$(TARGET): $(FILES)
	$(CXX) $^ -o $@
#compiling the testing unit
$(TESTS): testing.o
	$(CXX) $^ -o $@



#compiling just the library.cpp file. library.hpp is not compulsory, it is just for precaution
library.o: library.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

#compiling just the global.cpp file
global.o: global.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

#compiling just the collection.cpp file
collection.o: collection.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

#compiling just the books.cpp file
books.o: books.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

#compiling just the algo.cpp file
algo.o: algo.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<

#compiling just the operations.cpp file
operations.o: operations.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $<


#compiling just the testing.cpp file
testing.o: testing.cpp catch.hpp $(HEADER)
	$(CXX) $(CXXFLAGS) -Wa,-mbig-obj -c $<



.PHONY : clean
#clearing all copild files
clean:
	rm *.o $(ALLEXE)


#the following instruction was previously used to test Makefile with minGW: mingw32-make -f makefile clean