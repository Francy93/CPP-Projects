#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <string>   //library for string variables
#include <regex>    // regular expressions
#include <fstream>  // file text scanner
#include <vector>   // vectors library



class Books {

    private:
        std::string title  = "";
        std::string author = "";
        std::string isbn   = "";
        int qty            = 0 ;

    public:
        std::string getTitle();
        std::string getAuthor();
        std::string getId();
        int getQty();

        Books(std::string t, std::string a, std::string i, int q);
        ~Books();
};
        


class Operations {

    private:
        

    public:
       
        //the holly strings splitter
        std::vector<std::string> Operations::split(std::string s, std::string delim);

        //file reader
        void reader(std::string fileName);

};

#endif