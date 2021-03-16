#include "main.hpp"


// -----------------   class Collection   ------------------

Books Collection::getBook(int index){
    if((unsigned int)index < data.size()){
        return data[index];
    }else{
        std::cout << "\r\nOut of Bonds! Returned last index element.\r\n" << std::endl;
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
        std::cout << "\r\nOut of Bonds! No book removed.\r\n" << std::endl;
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


// -----------------   class Operations   ------------------

//the strings splitter
std::vector<std::string> Operations::split(std::string s, std::string delim){
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
                std::cout << "\r\n" << corruptedCounter << " book(s) data was corrupted!\r\n" << std::endl;
            }
            return 1;
        }else if(file.fail()){
            std::cout << "\r\nDump file missing! Do you want to create one?" << std::endl;

            while(true){
                
                std::cout << "Create a new one....1" << std::endl;
                std::cout << "Try again...........0" << std::endl;

                std::cout << "\r\nEnter a choice herer :> ";
                std::string choice; std::cin >> choice;
                if (choice == "1"){
                    std::fstream file2;
                    file2.open(fileName, std::ios::out);
                    file2 << std::fflush(stdin);
                    file2.close();
                    file2.open(fileName, std::ios::in);
                    if (file2.is_open()) { 
                        std::cout << "\"" << fileName << "\"" << " has been successfully created!\r\n\r\n\r\n";
                        newOpen = true; 
                    }else{ std::cout << "Something went wrong while creating the new file.\r\n" << std::endl; }
                    break;
                } else if(choice == "0"){
                    return 0;
                }else{
                    std::cout << "\r\nWrong selection!\r\n" << std::endl;
                }
            }
        }
    }
}