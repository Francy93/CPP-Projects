#include "library.hpp"
Operations ope;


int main(){
    ope.println("\r\n\r\n", "WELCOME TO THE LIBRARY SYSTEM", "magenta");

    while(true){
        std::string *border = new std::string("----------------------------------");
        
        ope.println("\r\n", *border, "blue");
        ope.println("|       DUMMY FILE LOADER        |", "cyan");
        ope.println(*border, "blue");
        std::cout << "| Enter here below the file name |" << std::endl;
        std::cout << "| Otherwise enter 00 to EXIT     |" << std::endl;
        ope.println(*border, "blue");
        delete border;

        std::cout << "\r\nEnter a choice here :> ";
        std::string fileName; std::cin >> fileName;

        if (fileName != "00"){
            if(ope.reader(fileName) == 1){
                if(ope.options() == 0){
                    break;
                }
            }
        }else{ break; }
    }

 
    ope.println("\r\nSuccessfully Exited!\r\n", "green");
    std::exit(1);
    return 0;
}