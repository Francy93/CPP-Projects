#include "main.hpp"

// -----------------   class Book   ------------------

Books::Books(std::string t, std::string a, std::string i, int q){
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
std::string Books::getId(){
    return isbn;
}
int Books::getQty(){
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
void Operations::reader(std::string fileName){
    
    std::ifstream file(fileName);
    
    bool newOpen = false;
    while(true){
        if (file.is_open() || newOpen) {
            std::string line;
            while (std::getline(file, line)) {
                line.c_str();
                if(line != "" && line != "1"){
                    //splitting the string by delimiter "tab" (ascii code 9)
                    std::vector<std::string> data = split(line, std::string(1, 9));
                    Books book(data[0], data[1], data[2], std::stoi(data[3]));
                

                }
            }
            file.close();
            break;
        }else if(file.fail()){
            std::cout << "DataBase missing. Creating one..." << std::endl;
            std::fstream file;
            file.open(fileName, std::ios::out);
            file << fflush;
            file.close();
            file.open(fileName, std::ios::in);
            if (file.is_open()) { std::cout << "Created!\r\n\r\n\r\n"; newOpen = true; }
        }
    }
}