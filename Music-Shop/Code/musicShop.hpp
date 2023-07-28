#ifndef _MUSICSHOP_H_
#define _MUSICSHOP_H_


#include <iostream>
#include <string>  //library for string variables
#include <ctime>   //library for data and time
#include <sstream> //library for intToString converter
#include <vector> //library for vectors

#include <fstream>  // file text scanner
#include <algorithm>// std::find
#include <cctype>   // remove white spaces

#include <regex>    // regular expressions

#include <memory>

using namespace std;
//declaring all the classes that I are going to use


//____________________________File util.cpp_____________________________

class Util {

    public:
        bool saved = true;
        //categories matrix
        vector<vector<string> > DVDs;
        vector<vector<string> > CDs;
        vector<vector<string> > Magazines;
        vector<vector<string> > Books;
        
        vector<vector<string> > SessionDB;
        vector<string> reportVec;

    private:
        string delimiter = " :: ";
        string fname = "DataBase.txt";

        //file reader
        vector<string> importer(std::string cat);

        //data exporter
        void exporter();

        //importing data from HardDisk
        vector<vector<string> > dbQuery(bool InOut);

    public:
        bool reportDisplay();

        string reporting(std::string id, int qty, std::string user);

        //the holly strings splitter *.*
        vector<string> split(std::string s, std::string delim);

    private:
        //fetching imported data, from sessionDB (ram) and splitting into categories
        vector<vector<string> > catSplit(std::string category);
        
    public:
        //constructor method to initialize DataBase vectors
        Util(){ 
            SessionDB = dbQuery(false); //selfrunnin dbQuery in output mode
            DVDs      = catSplit("D");
            CDs       = catSplit("C");
            Magazines = catSplit("M");
            Books     = catSplit("B");
        }

        void updater();
        string IntToString(int a);

        //current date generator
        string dateGen();
        vector<vector<string> > catSelector(std::string id);

        // Function to print the index of an element
        int getIndex(int cat, std::string k);
        vector<string> getById(std::string id);
        bool print(int cat);
};


//____________________________File components.cpp_____________________________

//Making the dataBase object/struct
Util util;

class Categories{

    private:
        string name = "";
        string date;
        string price = "";
        int amount  = -1;

    protected:
        string id;
        int catIndex;
        string idCalc(int cat);
        void dataToVec();
            
    public:
        void setName(std::string n);
        void setPrice(std::string p);
        void setAmount(int a);

        string getID();

        string getName();
        string getPrice();
        int getAmount();

        void propSetter();
        //constructor
        Categories(){
            date = util.dateGen();
        }
};

class DVD : public Categories {

    private:
        string cat = "D";
        int index = 0;
    public:
        DVD(std::string ID){ id = ID;  catIndex = index;  propSetter(); }
        DVD(){
            string n = idCalc(index);
            id = cat+"00"+n;
        }
};

class CD : public Categories {

     private:
        string cat = "C";
        int index = 1;
    public:
        CD(std::string ID){ id = ID;  catIndex = index;  propSetter(); }
        CD(){
            string n = idCalc(index);
            id = cat+"00"+n;
        }
};

class Magazine : public Categories {

    private:
        string cat = "M";
        int index = 2;
    public:
        Magazine(std::string ID){ id = ID;  catIndex = index;  propSetter(); }
        Magazine(){
            string n = idCalc(index);
            id = cat+"00"+n;
        }
};

class Book : public Categories {

    private:
        string cat = "B";
        int index = 3;
    public:
        Book(std::string ID){ id = ID;  catIndex = index;  propSetter(); }
        Book(){
            string n = idCalc(index);
            id = cat+"00"+n;
        }
};

class Logistic {
    private:
        //Util util;
        int chooseCat(bool IorO);
        bool amountEditing(std::string id, int num);

        vector<char> catChar = {'D', 'C', 'M', 'B'};

    public:
        string user; //this variable shows in the report which user operated

        bool update();
        bool restock(bool exit);
        bool newItem(bool exit);
        bool sale(bool exit);
        bool report();

};


#endif