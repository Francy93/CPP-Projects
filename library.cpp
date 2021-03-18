#include "library.hpp"
Operations ope;

int main(){
    while(true){
        std::string *border = new std::string("----------------------------------");
        
        ope.println("\r\n", *border, "blue");
        std::cout << "| Enter here below the file name |" << std::endl;
        std::cout << "| Otherwise enter 0 to exit      |" << std::endl;
        ope.println(*border, "blue");
        delete border;

        std::cout << "\r\nEnter a choice here :> ";
        std::string fileName; std::cin >> fileName;

        if (fileName != "0"){
            if(ope.reader(fileName) == 1){
                if(ope.options() == 0){
                    break;
                }
            }
        }
    }

    ope.println("\r\nSuccessfully Exited!\r\n", "green");
    std::exit(1);
    return 0;
}