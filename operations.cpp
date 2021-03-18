#include "library.hpp"

// -----------------   global functions   ------------------
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



// -----------------   class Book   ------------------

Books::Books(std::string t, std::string a, unsigned long i, unsigned int q){
    title  = t;
    author = a;
    isbn   = i;
    qty    = q;
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
//contructor
Operations::Operations(){

}
//file reader
int Operations::reader(std::string fileName){
    
    std::ifstream file(fileName);
    
    bool newOpen = false;
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
                        //Making the book object
                        Books book(elements[0], elements[1], std::stoull(elements[2]), std::stoul(elements[3]));
                        if(stoi(elements[3]) != 0){
                            //storing the book object
                            addBook(book);
                            //std::cout << "Added! Title: " << elements[0] << std::endl;
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
                
                std::cout << "Create a new one....1" << std::endl;
                std::cout << "Try again...........0" << std::endl;

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