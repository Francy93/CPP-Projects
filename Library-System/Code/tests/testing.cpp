//To Configure catch to use your main, and not its own: #define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_MAIN   //let Catch.hpp supply main() and handle configuring itself from the command line.
#include "catch.hpp"
#define private public      //making private methos public in order to allow testing
#define protected public    //making protected methos public in order to allow testing
#include "../src/operations.cpp"
#include "../src/collection.cpp"
#include "../src/algo.cpp"
#include "../src/books.cpp"

#include <sstream>

Operations test;

//testing util.h library methods
TEST_CASE("Global methods", "[bool]"){

    // function to get string to lower case
    SECTION("string to lower case"){
        CHECK(Util::toLower("TeSt") == "test");
    }
    
    SECTION("Testing the tring splitter"){
        //the function tested returns a string value
        std::vector<std::string> vec = Util::split("hello world", " "); 
        for(unsigned int i=0; i<vec.size() ;i++){
            //checking the splitting efficiency
            CHECK_FALSE(vec[i] == "");
        }
        CHECK(vec.size() == 2);
    }

    SECTION("Testing the color switcher"){
        Util::setColor(false);
        CHECK_FALSE(Util::getColorState());
        CHECK(Util::colorReset()   == "");
        CHECK(Util::color("green") == "");

        Util::setColor(true);
        CHECK(Util::getColorState());
        CHECK(Util::colorReset()   != "");
        CHECK(Util::color("green") != "");
    }

    SECTION("Testing the string parser to long long"){
        CHECK(Util::sTod("abc") ==   0);
        CHECK(Util::sTod("0")   ==   0);
        CHECK(Util::sTod("12-3") ==  9);
    }
}

//testing Operations class methods
TEST_CASE("Operations class", "[bool]"){
    
    //this check if a class is correctly defined
    REQUIRE(std::is_class<Operations>::value);
    
    //the function tested returns an int
    SECTION("Testing the File reader"){
        //simulating user entering input    
        std::istringstream iss("0 1");
        std::cin.rdbuf(iss.rdbuf());

        std::string testFile = "test";
        //checking if the reader manages to exit/terminate
        CHECK(test.reader(testFile) == 0);
        //checking if capable to make and read dummy files
        CHECK_FALSE(test.reader(testFile) == 0);
        std::cin.clear();

        //removing the testing file just created
        if( remove( (testFile).c_str() ) == 0 ){
            std::cout << ( "\""+testFile+"\" File successfully deleted\r\n" ) << std::endl;
        }
    }
}

//testing Collection class methods
TEST_CASE("Collection class", "[bool]"){
    SECTION("Testing book creation and insertion into the data structure"){
        //creating a new book
        Books *book1 = new Books("test11 test12 test13", "author1", "12345", "1");
        Books *book2 = new Books("test21 test22", "author2", "54321", "2");
        //add a book
        test.addBook(book1);
        test.addBook(book2);
        //get a book
        CHECK((*test.getBook(0)).getId() == "12345");
        CHECK((*test.getBook(1)).getId() == "54321");
    }
    SECTION("Testing the quicksort, bookSearch, bookIndexes and removeBook"){
        //get book index
        std::vector<double> BI0 =  test.bookIndexes(test.getBook(0));
        //remove a book
        CHECK(test.removeBook(BI0));

        //remove last book
        std::vector<double> BI1 =  test.bookIndexes(test.getBook(0));
        CHECK(test.removeBook(BI1));
    }
}

//testing Books class methods
TEST_CASE("Books class", "[bool]"){
    //this check if a class is correctly defined
    REQUIRE(std::is_class<Books>::value);

    //checking wether a book is empty or not
    Books b;
    CHECK_FALSE(b.emptyCheck());
}


//this instruction was previously used to compile this test file with minGW: g++ -std=c++17 -o test testing.cpp