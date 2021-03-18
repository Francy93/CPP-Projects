#include "library.hpp"

// -----------------   global functions   ------------------
//to string
unsigned long Global::sToll(std::string s){
    //checking if string is a digit
    if(std::regex_match (s, std::regex("[0-9]+") )){
        return stoull(s);
    }
    return 0;
}

std::string Global::toLower(std::string s){
    #include <algorithm>
    #include <cctype>
    
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return s;
}

//the strings splitter
std::vector<std::string> Global::split(std::string s, std::string delim){
    std::vector<std::string> vec;
    
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        vec.push_back(token);
        s.erase(0, pos + delim.length());
    }
    vec.push_back(s);
    return vec;
}



// -----------------   class Collection   ------------------

Books Collection::getBook(int index){
    if((unsigned int)index < data.size()){
        return data[index];
    }else{
        println("\r\nOut of Bonds! Returned last index element.\r\n", "red");
        return data[data.size()-1];
    }
    
}
void Collection::addBook(Books book){
    data.push_back(book);
}
void Collection::removeBook(int index){
    if((unsigned int)index < data.size()){
        data.erase(data.begin() + index);
    }else{
        println("\r\nOut of Bonds! No book removed.\r\n", "yellow");
    }
}
void Collection::collectionClear(){
    data.clear();
}


// -----------------   class Book   ------------------

Books::Books(std::string t, std::string a, std::string i, std::string q){
    title  = t;
    author = a;
    isbn   = stoull(i);
    qty    = stoul(q);
};
Books::~Books(){};

std::string Books::getTitle(){
    return title;
}
std::string Books::getAuthor(){
    return author;
}
unsigned long Books::getId(){
    return isbn;
}
unsigned int Books::getQty(){
    return qty;
}
unsigned int Books::modifyQty(int qty, bool mode){

    if(mode){
        Books::qty = Books::qty+qty; 
    }else{
        
        if(Books::qty-qty == 0){
            delete this;
        }else if(Books::qty-qty < 0){
            println("\r\nERROR! Not so many copies to be deleted.", "red");
            std::cout << "Try again..\r\n" << std::endl;
        }else{
            Books::qty = Books::qty-qty;
        }
    }
    return getQty();
}


// -----------------   class Operations   ------------------

//file reader
int Operations::reader(std::string fileName){
    
    std::ifstream file(fileName);
    bool newOpen = false;
    collectionClear();

    while(true){
        if (file.is_open() || newOpen) {
            std::string line;
            int corruptedCounter = 0;
            while (std::getline(file, line)) {
                line.c_str();

                if(line != "" && line != "1" && line != "0"){
                    //splitting the string by delimiter "tab" (ascii code 9)
                    std::vector<std::string> elements = split(line, std::string(1, 9));
                    if(elements.size() > 3 && elements.size() < 6){
                        if(sToll(elements[3]) != 0 && sToll(elements[2]) != 0){
                        //Making the book object
                        Books book(elements[0], elements[1], elements[2], elements[3]);
                            //storing the book object
                            addBook(book);
                        }
                    }else{ corruptedCounter++; }
                }
            }
            //closing the file scanner
            file.close();
            // displaying amount of corrupted data if any
            if(corruptedCounter != 0){
                println("\r\n" +std::to_string(corruptedCounter)+ " book(s) data was corrupted!\r\n", "yellow");
            }
            return 1;
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
                    return 0;
                }else{
                    println("\r\nWrong selection!\r\n", "yellow");
                }
            }
        }
    }
}

int Operations::options(){
    
    std::string *border = new std::string("----------------------");
    println("\r\n", *border, "blue");
    std::cout << "| Add a new Book...1 |" << std::endl;
    std::cout << "| Find a Book......2 |" << std::endl;
    std::cout << "| Go back..........3 |" << std::endl;
    std::cout << "| Exit.............0 |" << std::endl;
    println(*border,"\r\n", "blue");
    delete border;

    std::cout << "\r\nEnter a choice here :> ";
    std::string choice; std::cin >> choice;
    std::cout << std::endl;

    if(choice == "1"){
        int nav = addNewBook();
        if(nav == 0){
            return 0;
        } else {  return options(); }
    }else if(choice == "2"){

    }else if(choice == "3"){
        return 1;
    }else if(choice == "0"){
        return 0;
    }else{
        println("\r\nWrong selection!\r\n", "yellow");
        return options();
    }

    return 0;
}

int Operations::addNewBook(){

    std::vector<std::string> wizard = {"Enter here the title","Now enter the author","Include an ISBN","Specify a quantity"};
    std::vector<std::string> bookData;
    
    std::cin.ignore();
    for(unsigned int i=0; i< wizard.size(); i++){
        println("\r\n",wizard[i],"magenta");
        //getting user input
        std::cout << "\r\nEnter data here :> ";
        std::string choice;
        std::getline(std::cin, choice);
        std::cout << std::endl;

        if(i > 1){
            if(sToll(choice) == 0){
                println("\r\nInput has to be numeric! Try again.","yellow");
                --i;
            }else{ bookData.push_back(choice); }
        }else{ bookData.push_back(choice); }
    }
    
    //creating book
    Books book(bookData[0], bookData[1], bookData[2], bookData[3]);
    //adding the book
    addBook(book);

    println("\r\nBook successfully added!","green");
    return 1;
}