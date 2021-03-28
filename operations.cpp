#include "library.hpp"

// -----------------   global functions   ------------------


void Global::setColor(bool b){
    colorMode = b;
}
bool Global::getCstate(){
    return colorMode;
}



//making strings lower case
std::string Global::toLower(std::string s){
    #include <cctype>
    
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return s;
}

std::string Global::color(std::string c){
    std::string colors [] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
    std::string color = "";
    if(getCstate()){
        for (unsigned int i=0; i< sizeof(colors)/sizeof(*colors); i++){
            if(toLower(c) == colors[i]){
                color = "\033[1;3"+std::to_string(i)+"m";
            }
        }
    }
    return color;
}
std::string Global::colorReset(){
    std::string reset = "";
    if(getCstate()){
        reset = "\033[0m";
    }
    return reset;
}

std::string Global::loading(unsigned long long size, unsigned long long index){
    if (size > 0){
        static int i = 0;
        int percent = index * 100 / size;
        int maxToken = percent/5;
        
        if(maxToken != i){
            i = maxToken;
        
            if(percent != 100 && size > index){
                std::string status = std::string(maxToken*2, (char)219);
                return color("green")+status+" "+std::to_string(percent)+"%"+colorReset()+"\r";
            }
            return "\33[2K";
        }
    }
    return "";
}

//returns a string of numerate options
std::string Global::navOptions(std::vector<std::string> options, int minimum){
    std::string list="";

    int oSize = options.size();
    int iSize = 2;

    int i = 0;
    int longest = 7;
    for(auto o: options){
        int strSize = o.size();
        longest = strSize > longest? strSize: longest;
        if(++i == oSize){

        iSize = (int)std::to_string(i).size() > iSize? std::to_string(i).size(): iSize;
        }
    }
    
    std::string cStart = color("yellow"); //yellow corresponds to: "\033[1;35m"
    std::string cEnd = colorReset();      //reset  corresponds to: "\033[0m"

    longest += minimum > 3? minimum: 3;
    i = 0;
    for(auto o: options){
        int currenISize = iSize - std::to_string(i).size();
        int dotSize = longest - o.size();
        std::string index = cStart + std::to_string(++i) + cEnd;
        
        list += o + std::string(dotSize+currenISize,'.') + index + "\r\n";
    }

    list += "Go back" + std::string(longest-7+iSize-1,'.') + cStart+"0"+cEnd  + "\r\n";
    list += "Exit"    + std::string(longest-4+iSize-2,'.') + cStart+"00"+cEnd + "\r\n";
    return list;
}

//string to long long
long long Global::sToll(std::string s){
    //checking if string is a digit
    // sToll() method uses the regex library: #include <regex>
    if(std::regex_match (s, std::regex("[-|+]{0,1}[0-9]+") )){
        return stoull(s);
    }
    println("WARNING! Prevented a crash at sToll(). (Non numeric value enterd).", "yellow");
    return 0;
}

//the strings splitter
std::vector<std::string> Global::split(std::string s, std::string delim){
    std::vector<std::string> vec;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        
        if(pos != 0){
            token = s.substr(0, pos);
            vec.push_back(token);
        }
        s.erase(0, pos + delim.length());
    }
    if(s.size() > 0){
        vec.push_back(s);
    }
    return vec;
}
//an enanched and actually workin cin
std::string Global::cinln(){
    std::string input;

    fflush(stdin);
    std::cin.clear();
    std::getline(std::cin, input);
    std::cout << std::endl;
    fflush(stdin);
    std::cin.clear();

    return input;
}
// table generator
std::string Global::tableMaker(std::deque<std::deque<std::string>> &allData, std::vector<unsigned int> longest){
        
    std::string table = "";
    std::string border = "";
    for(unsigned long long i=0; i<allData[0].size(); i++){
        //creating the row
        std::string colorStart = color("magenta");
        std::string colorEnd = colorReset();

        std::string symbol = "|";
        std::string delimiter =colorStart+symbol+colorEnd;
        std::string row = "";
        for(unsigned int j=0; j<allData.size(); j++){
            auto allD = allData[j][i];
            unsigned long long allDsize = allD.size();
            int leng = longest[j] - allDsize < 1? 0: longest[j] - allDsize;
            std::string elem ="";
            std::string spaces = leng > 0? std::string(leng, ' '): "";

            std::string color = "";
            std::string start = "";
            std::string end   = "";
            if(i == 0 || j == 0){
                color = "3";
                if(i == 0){
                    color = "6";
                }
                start = "\033[1;3"+color+"m";
                end   = colorEnd;
            }
            if(!getCstate()){ start =""; }
            elem += start+ allD + spaces + end;
            row += delimiter+" " + elem + " ";
        }
        row += delimiter;

        if(i == 0){
            //creating the border
            int spaces = row.size()-((delimiter.size()-symbol.size())*2*allData.size()+delimiter.size()-symbol.size());
            border += colorStart+std::string(spaces, '-')+colorEnd;
            table += border+"\r\n"+row+"\r\n"+border+"\r\n";
        }else{
            table += row+"\r\n";
        }
    }

    return table+border+"\r\n";
}

//get user choice
int Global::getChoice(int options){

    //checking the choice
    int choice=0;
    std::string input ="";
    while(true){
        std::cout << "Enter a choice here :> ";
        input = cinln();
        if(input != "0" && input != "00"){
            for(int i=1; i<=options; i++){
                if(input == std::to_string(i)){
                    return i;
                }
            }
            println("WRONG SELECTION! Try again.", "yellow"); 
        }else if(input == "0"){ return 0; }
        else { return -1; }
    }
    return choice;
}

int Global::navChoice(std::vector<std::string> options, int minimum){
    //displaying options
    std::cout << navOptions(options, minimum) << std::endl;
    //getting the choice
    return getChoice(options.size());
}








// -----------------   class Collection   ------------------

Books* Collection::getBook(unsigned long long index){
    if(index < data.size()){
        return data[index];
    }else{
        println("\r\nOut of Bonds! Returned last index element.\r\n", "red");
        return data[data.size()-1];
    }
    
}
void Collection::addBook(Books *book){
    data.push_back(book);
    shuffle(data);
    unsigned int titleSize = split((*book).getTitle(), " ").size();
    unsigned int bigDataSize = sortedDataInMemory.size();
    unsigned int iterations = bigDataSize > titleSize? bigDataSize: titleSize;

    for(unsigned int i=0; i<iterations; i++){
        if(i >= sortedDataInMemory.size()){
            sortedDataInMemory.push_back(data);
            quicksort(sortedDataInMemory[i], 0, data.size()-1, i);
        }else{
            sortedDataInMemory[i].insert(sortedDataInMemory[i].begin() + bookSearch(sortedDataInMemory[i], book, i)[1], book);
        }
        //printing the loading bar
        std::cout << loading(iterations, i+1);
    }
    
}
//book remover
bool Collection::removeBook(double index){
    
    if(index < data.size() && index >=0){
        delete data[index];
        data.erase(data.begin() + index);
        return true;
    }
    println("\r\nERROR! Out of Bonds. No book removed.\r\n", "yellow");
    return false;
}
//get book index  (THIS MIGHT BE IMPROVED WITH a BINARY SEARCH)
double Collection::bookIndex(Books *book){
    auto it = find(begin(data), end(data), book);
    if(it != data.end()){
        return it - data.begin();
    }
    println("\r\nERROR! Out of Bonds. No book found.\r\n", "red");
    return -1;
}
void Collection::collectionClear(){
    data.clear();
    sortedDataInMemory.clear();
}

//adding a new book
void Collection::addNewBook(){

    std::vector<std::string> wizard = {"Enter here the title","Now enter the author","Include an ISBN","Specify a quantity"};
    std::vector<std::string> bookData;
    
    //std::cin.ignore();
    for(unsigned int i=0; i< wizard.size(); i++){
        println("\r\n",wizard[i],"magenta");
        //getting user input
        std::cout << "\r\nEnter data here :> ";
        std::string choice = cinln();

        if(i > 1){
            if(sToll(choice) == 0){
                println("\r\nInput has to be a valid numeric! Try again.","yellow");
                --i;
            }else{ bookData.push_back(choice); }
        }else{ bookData.push_back(choice); }
    }
    
    //creating book
    Books *b = new Books(bookData[0], bookData[1], bookData[2], bookData[3]);
    //adding the book
    addBook( b );

    println("\r\nBook successfully added!","green");
}


//building booksTable
std::string Collection::booksTable(std::deque<Books*> &books){
    //the instruction to append at the begining would be like: a.insert(a.begin(), b.begin(), b.end());
    std::deque<std::deque<std::string>> allData = {{"No."}, {"Title(s)"}};
    std::vector<unsigned int> longest = {allData[0][0].size(), allData[1][0].size()};

    //longest detector
    unsigned long long i=0;
    for (auto it = books.begin(); it != books.end(); it++){
        std::string title = (**it).getTitle();
        //adding data
        allData[0].push_back(std::to_string(++i));
        allData[1].push_back(title);
        
        //calculating longest
        unsigned int iSize = std::to_string(i).size(), titleSize = title.size();
        longest[0] = longest[0] < iSize? iSize: longest[0];
        longest[1] = longest[1] < titleSize? titleSize: longest[1];
    }

    return tableMaker(allData, longest);
}
//printing the whole collection is O(n*2) time complexity
void Collection::printCollection(){
    std::cout << booksTable(data) << std::endl;
}

//find a book
bool Collection::findBook(){
    
    std::cout << "Enter here below a book title to search" << std::endl;
    std::cout << navOptions({}, 10) << std::endl;


    //getting user input
    std::cout << "\r\nEnter data here :> ";
    std::string choice = cinln();
    std::cout << std::endl;

    if(choice == "00"){
        return false;
    }else if(choice == "0"){
        return true;
    }else if(choice == "" || choice == " "){
        println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
        return findBook();
    }else{
        //if any book was found than do..
        if(!binarySearch(data, choice)){
            return false;
        }else{ return findBook(); }
    }
    return false;
}
//select a book from a given list
int Collection::booksChoice(std::deque<Books*> &books){
    //if the books list is not empty
    
    if(books.size() > 0){
        std::cout << booksTable(books) << std::endl;

        while(true){
            std::cout << "Enter here below the number of the book to show" << std::endl;
            std::cout << navOptions({}, 10) << std::endl;

            //getting user input
            std::cout << "\r\nEnter number here :> ";
            std::string choice; std::cin >> choice;
            std::cout << std::endl;

            if(choice == "0"){
                return 1;
            }else if(choice == "00"){
                return 0;
            }else if(sToll(choice) > 0 && sToll(choice) <= books.size()){
                
                int result = (*books[(sToll(choice)-1)]).bookManager();
                
                if( result == 0){
                    return 0;
                }else if(result == 2){
                    //book removing
                    if(books == data){
                        // if the deque "books" corispond to the main big deque "data"
                        if(removeBook(sToll(choice)-1)){
                            println("\r\n", "Book successfully removed!.", "\r\n", "green");
                        } else{ println("\r\n", "ERROR removing the book! Try again.", "\r\n", "red"); }
                        
                    }else{
                        if(removeBook(bookIndex(books[(sToll(choice)-1)]))){
                            books.erase(books.begin() + sToll(choice)-1);
                            println("\r\n", "Book successfully removed!.", "\r\n", "green");
                        
                        }else {println("\r\n", "ERROR removing the book! Try again.", "\r\n", "red"); }
                    }
                    return booksChoice(books);
                }else{ return booksChoice(books); }
            }else{ println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
        }
    }else{ 
        println("\r\n", "No more Books to show!", "\r\n", "\r\n", "yellow");
        return 1;
    }
    return 0;
}

//data shuffle
void Collection::shuffle(std::deque<Books*> &data){
    //this function requires: #include <algorithm> and #include <regex>
    std::random_shuffle(data.begin(), data.end());
    if(data == Collection::data){
        booksSorted = false;

    }
}

//coverting data to sortedDataInMemory data with time complexity: O(n+t+(n*2*t))
void Collection::sortDataInMemory(){
    unsigned int length = 0;
    unsigned int longest = 0;
    unsigned long long dataSize = data.size(), index = 0;

    //getting the longest title

    for (auto it = data.begin(); it != data.end(); it++){
        length = split((**it).getTitle(), " ").size();
        if(longest<length){ longest=length; }
        
        //printing the loading bar
        std::cout << loading(dataSize, ++index);
    }

    unsigned int bigDataSize = sortedDataInMemory.size();
    unsigned int iterations = bigDataSize > longest? bigDataSize: longest;
    //loading bar title
    println("Creating ", std::to_string(iterations), " copies of de dummy data", "green");

    //filling the sortedDataInMemory
    for(unsigned int i=0; i<iterations; i++){
        if(i >= sortedDataInMemory.size()){
            sortedDataInMemory.push_back(data);
            quicksort(sortedDataInMemory[i], 0, data.size()-1, i);
        }
        //printing the loading bar
        std::cout << loading(iterations, i+1);
    }
    //removing loading bar title
    std::cout << "\r\e[4K";
}












// -----------------   class Book   ------------------
//constructor
Books::Books(std::string t, std::string a, std::string i, std::string q){
    title  = t;
    author = a;
    isbn   = stoull(i);
    qty    = stoul(q);
};
//decontructor
Books::~Books(){
    std::cout << "Deleted: " << isbn << std::endl;
};

std::string Books::getTitle(){
    return title;
}
std::string Books::getAuthor(){
    return author;
}
unsigned long long Books::getId(){
    return isbn;
}
unsigned int Books::getQty(){
    return qty;
}
// modifing book quantity
unsigned int Books::setQty(int qty, bool mode){

    if(mode && qty > 0){
        Books::qty = getQty()+qty;
    }else{
        //inverting sign
        qty = qty<0? qty*-1: qty;
        if(Books::qty-qty == 0){
            println("\r\nWARNING! Are you sure you wanna remove this title from the library?\r\n", "yellow");
            //processing the choice
            switch(navChoice({"Confirm"}, 10)){
                case -1: return 0; //terminatign this function and close the program
                case  0: return 1; //exiting the main loop and terminate this function
                case  1: return 2;
            }

        }else if((int)Books::qty-qty < 0){
            println("\r\nERROR! Not so many copies to be decreased.", "red");
            std::cout << "Try again..\r\n" << std::endl;
        }else{
            Books::qty -= qty;
        }
    }
    return 1;
}
//checking wether a book is empty or not
bool Books::emptyCheck(){
    if((getTitle() == "0" && getAuthor() == "0" && getId() == 0) || getQty() <= 0){
        return false;
    }
    return true;
}
//printing book values
std::string Books::bookPrint(){

    std::deque<std::deque<std::string>> allData =   {{ "ATTRIBUTES", "Title", "Authors", "ISBN", "Quantity" },
                    { "VALUES", getTitle(), getAuthor(), std::to_string(getId()), std::to_string(getQty()) }};

    std::vector<unsigned int> longest = {allData[0][0].size(), allData[1][0].size()};


    //longest detector
    for(unsigned int i=0; i<longest.size() ;i++){
        for(unsigned int j=0; j<allData[0].size(); j++){
            std::string attr = allData[i][j];
            //calculating longest
            longest[i] = longest[i] < attr.size()? attr.size(): longest[i];
        }
    }
    return tableMaker(allData, longest);
}
//book dashboard
int Books::bookManager(){
    std::cout << bookPrint() <<std::endl;

    while(true){
        std::cout << std::endl;
        //processing the choice
        switch(navChoice({"Edit the quantity"}, 10)){
            case -1: return 0; //terminatign this function and close the program 
            case  0: return 1; //exiting the main loop and terminate this function
            case  1:
        
                while(true){
                    std::cout << "\r\nNow enter a quantity to be summed (e.g 1, -1, 5, -18)" << std::endl;
                    std::cout << navOptions({}, 10) << std::endl;

                    //getting user input
                    std::cout << "\r\nEnter a choice here :> ";
                    std::string choice; std::cin >> choice;
                    std::cout << std::endl;

                    if(choice == "0"){
                        return bookManager();
                    }else if(choice == "00"){
                        return 0;
                    }else if(sToll(choice) != 0){
                        long *q = new long(sToll(choice));
                        int *result = new int(setQty(*q, *q>0));
                        delete q;

                        if(*result == 0){
                            delete result;
                            return 0;
                        }else if(*result == 1){
                            delete result;
                            return bookManager();
                        }else{
                            return 2;
                        }
                        break; //----------------------------------------------------------book remouving if decrese to 0 (TO DO)
                    }else { println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
                }

            return bookManager();
        }
    }
    return 0;
}










// -----------------   class Operations   ------------------

Operations::Operations(){ 
    //getting console in fullScreen mode uses the library: #include <windows.h>
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    //setting system base color;
    system("Color 0F"); // white text mode
};

//file reader
bool Operations::reader(std::string fileName){
    //getting the file lines amount
    std::ifstream count(fileName);
    unsigned long long fileSize = std::count(std::istreambuf_iterator<char>(count),  std::istreambuf_iterator<char>(), '\n');
    count.close();
    //opening back the file
    std::ifstream file(fileName);

    bool newOpen = false;
    collectionClear();

    while(true){
        if (file.is_open() || newOpen) {
            std::string line;
            unsigned long long corruptedCounter = 0, index = 0;

            while (std::getline(file, line)) {
                index++;
                if(line != "" && line != "1" && line != "0"){
                    line.c_str();

                    //splitting the string by delimiter "tab" (ascii code 9)
                    std::vector<std::string> elements = split(line, std::string(1, 9));
                    if(elements.size() > 3 && elements.size() < 6){
                        if(sToll(elements[3]) != 0 && sToll(elements[2]) != 0){
                            //storing the book object
                            Books *b = new Books(elements[0], elements[1], elements[2], elements[3]);
                            data.push_back( b );
                        }
                    }else{ corruptedCounter++; }
                }
                //printing the loading bar
                std::cout << loading(fileSize, index);
            }
            //closing the file scanner
            file.close();
            // displaying amount of corrupted data if any
            if(corruptedCounter != 0){
                println("\r\n" +std::to_string(corruptedCounter)+ " book(s) data was corrupted!\r\n", "yellow");
            }
            //sorting elements
            shuffle(data);
            return true;
        }else if(file.fail()){
            println("\r\nDummy file missing! Do you want to create one?", "cyan");

            while(true){
                
                std::cout << "Create a new one........1" << std::endl;
                std::cout << "Go back and Try again...0" << std::endl;

                std::cout << "\r\nEnter a choice herer :> ";
                std::string choice; std::cin >> choice;
                std::cout << std::endl;

                if (choice == "1"){
                    std::fstream file2;
                    file2.open(fileName, std::ios::out);
                    file2 << std::fflush(stdin);
                    file2.close();
                    file2.open(fileName, std::ios::in);
                    if (file2.is_open()) { 
                        println("\"" + fileName + "\"" + " has been successfully created!\r\n\r\n\r\n", "green");
                        newOpen = true; 
                    }else{ println("Something went wrong while creating the new file.\r\n", "red"); }
                    break;
                } else if(choice == "0"){
                    return false;
                }else{
                    println("\r\nWrong selection!\r\n", "yellow");
                }
            }
        }
    }
}

bool Operations::options(){
    
    std::string *border = new std::string("---------------------------");
    println("\r\n", *border, "blue");
    println("|        MAIN MENU        |", "cyan");
    println(*border, "blue");
    std::cout << "| Add a new Book........1 |" << std::endl;
    std::cout << "| Find a Book...........2 |" << std::endl;
    std::cout << "| Show collection.......3 |" << std::endl;
    println(*border, "blue");
    std::cout << "| Go BACK...............0 |" << std::endl;
    std::cout << "| EXIT.................00 |" << std::endl;
    println(*border,"\r\n", "blue");
    delete border;

    //sorting the big data deque
    if(!booksSorted){ shuffle(data); quicksort(data, 0, data.size()-1, 0); }

    //user input
    switch(getChoice(3)){
        case -1: return false;
        case  0: return true;
        case  1: addNewBook(); return options();
        case  2:  
            if(!findBook()){
                return false;
            } else {  return options(); }
        case  3:
            if(booksChoice(data) == 0){
                return false;
            }else{ return options(); }
    }
    return false;
}