#include "main.hpp"
Operations ope;

int main(){
    while(true){
        std::string *border = new std::string("----------------------------------");

        std::cout << "\r\n"+*border << std::endl;
        std::cout << "| Enter here below the file name |" << std::endl;
        std::cout << "| Otherwise enter 0 to exit      |" << std::endl;
        std::cout << *border << std::endl;
        delete border;

        std::cout << "\r\nEnter a choice here :> ";
        std::string fileName; std::cin >> fileName;
        
        //std::ifstream library(fileName);

        if (fileName == "0"){
            std::cout << "Successfully Exited!\r\n" << std::endl;
            std::exit(1);
            return 0;
        }else{
            if(ope.reader(fileName) == 1){
                break;
            }
        }
    }

    std::cout << ope.getBook(5).getTitle() << std::endl;

    std::cout << "\r\nNext operation is about working in progress!\r\n" << std::endl;

    return 0;
}