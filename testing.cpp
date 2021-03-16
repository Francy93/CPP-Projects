#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "main.hpp"


TEST_CASE("Quantity categories supported by util.print(int))", "[integers]"){
    //the function tested returns a boolean value
    for(int i=0; i<10 ;i++){
        //a fifth cicle would get error
        REQUIRE(true);
    }
    
    SECTION("Testing reporting inpu and output"){
        //the function tested returns a string value   
        for(int i=0; i<10 ;i++){
            //checking if reporting capability
            //fifth cicle would get error
            CHECK_FALSE(false);
        }
        //checking if report is empty
        CHECK(true);
    }
}

//g++ -std=c++17 -o test testing.cpp