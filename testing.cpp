//To Configure catch to use your main, and not its own: #define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_MAIN // let Catch.hpp supply main() and handle configuring itself from the command line.
#include "catch.hpp"
#include "algo.cpp"
#include "operations.cpp"

//#include "operations.cpp"
#include <sstream>

Operations ope;

//testing Global methods
TEST_CASE("Global methods", "[bool]"){

    // function to get string to lower case
    SECTION("string to lower case"){
        CHECK(ope.toLower("TeSt") == "test");
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
        CHECK(ope.reader(testFile) == 0);
        //checking if capable to make and read dummy files
        CHECK_FALSE(ope.reader(testFile) == 0);
        std::cin.clear();

        //removing the testing file just created
        if( remove( (testFile).c_str() ) == 0 ){
            std::cout << ( "\""+testFile+"\" File successfully deleted\r\n" ) << std::endl;
        }
    }
    SECTION("Testing the main menu"){
        //simulating user entering input  
        std::istringstream iss("0 3");
        std::cin.rdbuf(iss.rdbuf());

        //checking if the manu manage to successfully exits
        CHECK(ope.options() == 0);
        //checking if the manu manage to successfully goes back
        CHECK(ope.options() == 1);

        std::cin.clear();
    }
}

//this instruction was previously used for minGW: g++ -std=c++17 -o test testing.cpp


/* 
class Global{

    private:
        inline static bool colorMode = true;
    public:

        
        //get color
        std::string color(std::string);
        //get color reset
        std::string colorReset();
        //print vavigation and get choice
        std::string navOptions(std::vector<std::string> options, int minimum);
        //string to long long
        long long sToll(std::string s);

       

        //the holly strings splitter
        std::vector<std::string> split(std::string s, std::string delim);
        //an enanched and actually workin cin
        std::string cinln();
        //table generator
        std::string tableMaker(std::deque<std::deque<std::string>> &allData, std::vector<unsigned int> longest);
        //get user choice
        int getChoice(int options);
        // display options and get choice
        int navChoice(std::vector<std::string> options, int minimum);
};


class Books: public Global {

    private:
        std::string title;
        std::string author;
        unsigned long long isbn;
        unsigned int qty;

    public:
        //Books constructor
        Books(){}
        Books(std::string t, std::string a, std::string i, std::string q);
        ~Books();

        // book title getter
        std::string getTitle();
        // book author getter
        std::string getAuthor();
        // book id getter
        unsigned long long getId();
        // to get the book quantity
        unsigned int getQty();
        // modifing book quantity attribute
        unsigned int setQty(int qty, bool mode);
        //checking wether a book is empty or not
        bool emptyCheck();
        // printing book values
        std::string bookPrint();
        // book dashboard
        int bookManager();
};
        


class Collection: public Global {

    protected:
        //great dummy data
        std::deque<Books*> data;
        //mark if data is sorted
        bool booksSorted = false;

    public:
        //erasing book collection
        void collectionClear();
        //remove a book
        bool removeBook(double index);
        //get book index
        double bookIndex(Books *book);
        //add a book
        void addBook(Books *book);
        //get a book
        Books* getBook(unsigned long long index);
                

        //find a book
        bool findBook();
        //adding a new book
        void addNewBook();
        //building booksTable
        std::string booksTable(std::deque<Books*> &books);
        //print all books
        void printCollection();
        //select book from table
        int booksChoice(std::deque<Books*> &books);
        //quick-sort
        void quicksort(std::deque<Books*>& a, long long l, long long r, unsigned int titleIndex);
        //binary-search
        bool binarySearch(std::deque<Books*> &arr,std::string word);
        //data shuffle       
        void shuffle(std::deque<Books*> &data);

};



class Operations: public Collection{

    public:
        Operations();
        ~Operations(){};

        //file reader
        bool reader(std::string fileName);
        //main menu
        bool options();
}; */