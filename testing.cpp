//To Configure catch to use your main, and not its own: #define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_MAIN // let Catch.hpp supply main() and handle configuring itself from the command line.
#include "catch.hpp"
#include "operations.cpp"

Operations ope;

//testing Global methods
TEST_CASE("Global functions", "[bool]"){

    // function to get string to lower case
    SECTION("string to lower case"){
        CHECK(ope.toLower("TeSt") == "test");
    }
}

//testing Operations class methods
TEST_CASE("Operations class", "[bool]"){
    
    //this check if a class is correctly defined
    REQUIRE(std::is_class<Operations>::value);
    
    //the function tested returns an int
    SECTION("Testing the File reader"){
        #include <sstream>
        //simulating user entering input    
        std::istringstream iss("0 1");
        std::cin.rdbuf(iss.rdbuf());

        std::string testFile = "test";
        //checking if the reader manages to exit/terminate
        CHECK(ope.reader(testFile) == 0);
        //checking if capable to make and read dummy files
        CHECK_FALSE(ope.reader(testFile) == 0);
        std::cin.clear();

        //removing the testing file just created
        if( remove( (testFile).c_str() ) == 0 ){
            std::cout << ( "\""+testFile+"\" File successfully deleted\r\n" ) << std::endl;
        }
    }
    SECTION("Testing the tring splitter"){
        //the function tested returns a string value
        std::vector<std::string> vec = ope.split("hello world", " "); 
        for(unsigned int i=0; i<vec.size() ;i++){
            //checking the splitting efficiency
            CHECK_FALSE(vec[i] == "");
        }
    }
}

//this instruction was previously used for minGW: g++ -std=c++17 -o test testing.cpp