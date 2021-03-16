#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <string>   //library for string variables
#include <regex>    // regular expressions
#include <fstream>  // file text scanner
#include <vector>   // vectors library



class Books {

    private:
        std::string title;
        std::string author;
        unsigned long isbn;
        unsigned int qty;

    public:
        std::string getTitle();
        std::string getAuthor();
        unsigned long getId();
        unsigned int getQty();

        Books(std::string t, std::string a, unsigned long i, unsigned int q);
        ~Books();
};
        


class Collection {

    protected:  
        std::vector<Books> data;

    public:
        void removeBook(int index);
        void addBook(Books book);
        Books getBook(int index);
};



class Operations: public Collection{

    public:
        Operations(){};
        ~Operations(){};
        //the holly strings splitter
        std::vector<std::string> split(std::string s, std::string delim);
        std::vector<Books> collections;

        //file reader
        int reader(std::string fileName);

};


#endif