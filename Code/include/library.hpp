#ifndef _LIBRARY_HXX_
#define _LIBRARY_HXX_

/**
 * @file library.hpp
 * @author Francy (fa1033@live.mdx.ac.uk)
 * @brief header
 * @version 0.1
 * @date 2021-04-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>      // input output
#include <string>        // library for string variables
#include <regex>         // regular expressions
#include <fstream>       // file text scanner
#include <vector>        // vectors library
#include <deque>         // deque library
#include <unordered_map> // unordered hash table read/write: O(1)
#include <algorithm>     // for toLower() and the shuffle()
#include "../lib/util.h"




class Books {

    private:
        
        const std::string title;
        const std::string author;
        const std::string isbn;
        unsigned long qty;
        std::vector<std::string> splittedTitle;

    public:
        //Books constructor
        Books() : title(""), author(""), isbn(""), qty(0), splittedTitle({})  {}
        Books(std::string t, std::string a, std::string i, std::string q);
        ~Books();

    private:
        // to split the title by indexes
        void titleSplitter();

    public:
        // book title getter
        std::string getTitle();
        // book author getter
        std::string getAuthor();
        // book id getter
        std::string getId();
        // to get the book quantity
        unsigned long getQty();
        //get splitted title
        std::string getSplittedT(unsigned long index);
        //get size of vector splittedTitle
        unsigned long getSTsize();
        // modifing book quantity attribute
        unsigned short setQty(long qty, bool mode);
        //checking wether a book is empty or not
        bool emptyCheck();
        // printing book values
        std::string bookPrint();
        // book dashboard
        int bookManager();
};
        


class Collection {

    protected:
        //great dummy data
        std::deque<Books*> data;
        //sorted data in memory
        std::deque<std::deque<Books*>> sortedDataInMemory;
        //mark if data is sorted
        bool booksSorted = false;

    private:
        //remove a book
        bool removeBook(std::vector<double> indexes);
        //get book index
        std::vector<double> bookIndexes(Books *book);
        //add a book
        void addBook(Books *book);
        //get a book
        Books* getBook(unsigned long long index);
        //building booksTable
        std::string booksTable(std::deque<Books*> &books);
        //binary-search
        bool binarySearch(std::deque<Books*> &arr,std::string word);
        // entire books search
        std::vector<unsigned long long> bookSearch(std::deque<Books*>& arr, Books* book, unsigned int index);

    public:
        //find a book
        bool findBook();
        //adding a new book
        void addNewBook();
        //print all books
        void printCollection();
        //improved performance sorting data in memory
        void sortDataInMemory();
        //erasing book collection
        void collectionClear();
        //data shuffle       
        void shuffle(std::deque<Books*> &data);
        //select book from table
        int booksChoice(std::deque<Books*> &books);
        //quick-sort
        void quicksort(std::deque<Books*>& a, long long l, long long r, unsigned long titleIndex);
};



class Operations: public Collection{

    public:
        Operations();
        ~Operations(){};

        //file reader
        bool reader(std::string fileName);
        //main menu
        bool options();
};


#endif
