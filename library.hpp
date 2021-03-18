#ifndef _LIBRARY_HXX_
#define _LIBRARY_HXX_

#include <iostream>
#include <string>   //library for string variables
#include <regex>    // regular expressions
#include <fstream>  // file text scanner
#include <vector>   // vectors library
#include <deque>    // deque library


class Global{

    public:
        //string to lower case
        std::string toLower(std::string s);
        //to string
        unsigned long sToll(std::string s);

        //this is a string printer printer
        template <typename... Ts>
        void println(Ts const & ... vals){
            using unused = int[];
            std::vector<std::string> args;
            (void)unused { 0, (args.push_back(vals), 0)... };

            std::string start = "";
            std::string end = "";

            if(args.size() > 1){
                std::string colors [] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
                for (unsigned int i=0; i< sizeof(colors)/sizeof(*colors); i++){
                    if(toLower(args[args.size()-1]) == colors[i]){
                        start = "\033[1;3"+std::to_string(i)+"m";
                        end = "\033[0m";
                    }
                }
            }

            std::string print = "";
            unsigned int size = start == ""? args.size(): args.size()-1;
            for(unsigned int i=0; i< size; i++){
                print +=args[i];
            }
            
            std::cout << start+print+end << std::endl;
        }

        //the holly strings splitter
        std::vector<std::string> split(std::string s, std::string delim);
};


class Books: public Global {

    private:
        std::string title;
        std::string author;
        unsigned long isbn;
        unsigned int qty;

    public:
        Books(std::string t, std::string a, std::string i, std::string q);
        ~Books();

        std::string getTitle();
        std::string getAuthor();
        unsigned long getId();
        unsigned int getQty();
        unsigned int modifyQty(int qty, bool mode);
};
        


class Collection: public Global {

    protected: 
        std::deque<Books> data;

    public:
        //erasing book collection
        void collectionClear();
        //remove a book
        void removeBook(int index);
        //add a book
        void addBook(Books book);
        //get a book
        Books getBook(int index);

};



class Operations: public Collection{

    public:
        Operations(){};
        ~Operations(){};

        //file reader
        int reader(std::string fileName);
        //main menu
        int options();
        //find a book
        int findBook();
        //adding a new book
        int addNewBook();
};


#endif