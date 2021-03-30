#include "library.hpp"


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
                if(line != " " && line != " " && line != "" && line != "1" && line != "0"){
                    line.c_str();

                    //splitting the string by delimiter "tab" (ascii code 9)
                    std::vector<std::string> elements = split(line, std::string(1, 9));
                    if(elements.size() > 3 && sToll(elements[3]) != 0){
                            //storing the book object
                            Books *b = new Books(elements[0], elements[1], elements[2], elements[3]);
                            data.push_back( b );
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

    //sorting the default data deque
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