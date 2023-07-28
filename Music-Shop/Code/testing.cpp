#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "testingLib.cpp"


Util util2;
vector<string> idsCat = {"D","C","M","B"};


TEST_CASE("Quantity categories supported by util.print(int))", "[integers]"){
    //the function tested returns a boolean value
    for(int i=0; i< idsCat.size() ;i++){
        //a fifth cicle would get error
        REQUIRE(util2.print(i));
    }
    
    SECTION("Testing reporting inpu and output"){
        //the function tested returns a string value   
        for(int i=0; i<idsCat.size() ;i++){
            //checking if reporting capability
            //fifth cicle would get error
            CHECK_FALSE(util2.reporting(idsCat[i], 0, "testing") == "");
        }
        //checking if report is empty
        CHECK(util2.reportDisplay());
    }
}

TEST_CASE("String generators", "[strings]"){

    //this function returns the currenet date as a string value
    REQUIRE(util2.dateGen() != "");
    //this function has to return a string valaue
    CHECK(util2.IntToString(1) == "1");
}

TEST_CASE("Searching alorithms testing", "[ID]"){

        //this function returns a string vector
    for(int i=0; i<idsCat.size() ;i++){
        string ID = idsCat[i]+"000";
        REQUIRE_FALSE(util2.getById(ID)[0] == "");
    }

    //this function returns an integer
    for(int i=0; i<idsCat.size() ;i++){
        string ID = idsCat[i]+"000";
        REQUIRE(util2.getIndex(0, ID));
    }
}
//g++ -std=c++17 -o test testing.cpp