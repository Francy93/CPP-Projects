#include "library.hpp"
Operations ope;


std::string *intro = new std::string(R"(
                                                                                                            
8 8888          8 8888 8 888888888o   8 888888888o.            .8.          8 888888888o. `8.`8888.      ,8'
8 8888          8 8888 8 8888    `88. 8 8888    `88.          .888.         8 8888    `88. `8.`8888.    ,8' 
8 8888          8 8888 8 8888     `88 8 8888     `88         :88888.        8 8888     `88  `8.`8888.  ,8'  
8 8888          8 8888 8 8888     ,88 8 8888     ,88        . `88888.       8 8888     ,88   `8.`8888.,8'   
8 8888          8 8888 8 8888.   ,88' 8 8888.   ,88'       .8. `88888.      8 8888.   ,88'    `8.`88888'    
8 8888          8 8888 8 8888888888   8 888888888P'       .8`8. `88888.     8 888888888P'      `8. 8888     
8 8888          8 8888 8 8888    `88. 8 8888`8b          .8' `8. `88888.    8 8888`8b           `8 8888     
8 8888          8 8888 8 8888      88 8 8888 `8b.       .8'   `8. `88888.   8 8888 `8b.          8 8888     
8 8888          8 8888 8 8888    ,88' 8 8888   `8b.    .888888888. `88888.  8 8888   `8b.        8 8888     
8 888888888888  8 8888 8 888888888P   8 8888     `88. .8'       `8. `88888. 8 8888     `88.      8 8888     
                                                                                       .         .          
   d888888o.  `8.`8888.      ,8' d888888o. 8888888 8888888888 8 8888888888            ,8.       ,8.         
 .`8888:' `88. `8.`8888.    ,8'.`8888:' `88.     8 8888       8 8888                 ,888.     ,888.        
 8.`8888.   Y8  `8.`8888.  ,8' 8.`8888.   Y8     8 8888       8 8888                .`8888.   .`8888.       
 `8.`8888.       `8.`8888.,8'  `8.`8888.         8 8888       8 8888               ,8.`8888. ,8.`8888.      
  `8.`8888.       `8.`88888'    `8.`8888.        8 8888       8 888888888888      ,8'8.`8888,8^8.`8888.     
   `8.`8888.       `8. 8888      `8.`8888.       8 8888       8 8888             ,8' `8.`8888' `8.`8888.    
    `8.`8888.       `8 8888       `8.`8888.      8 8888       8 8888            ,8'   `8.`88'   `8.`8888.   
8b   `8.`8888.       8 8888   8b   `8.`8888.     8 8888       8 8888           ,8'     `8.`'     `8.`8888.  
`8b.  ;8.`8888       8 8888   `8b.  ;8.`8888     8 8888       8 8888          ,8'       `8        `8.`8888. 
 `Y8888P ,88P'       8 8888    `Y8888P ,88P'     8 8888       8 888888888888 ,8'         `         `8.`8888.)");





int main(){
    //setting the color mode
    ope.setColor(false);

    //intro title
    ope.println(*intro, "\r\n", "yellow");
    delete intro;
    //printing the welcome
    std::string *title = new std::string("  WELCOME TO THE LIBRARY SYSTEM  ");
    std::string *titleColor = new std::string("\033[1;43m "+std::string((*title).size(), '.')+" \033[0m");
    ope.println("\r\n\r\n\r\n", *titleColor, "yellow");
    ope.println("\033[1;43m ", *title," \033[0m", "black");
    ope.println(*titleColor, "\r\n", "yellow");

    delete titleColor;
    delete title;


    //starting the main while loop
    while(true){
        std::string *border = new std::string("----------------------------------");
        
        ope.println("\r\n", *border, "blue");
        ope.println("|       DUMMY FILE LOADER        |", "cyan");
        ope.println(*border, "blue");
        std::cout << "| Enter here below the file name |" << std::endl;
        std::cout << "| Press  1  to set color ON/OFF  |" << std::endl;
        std::cout << "| Otherwise enter  00  to EXIT   |" << std::endl;
        ope.println(*border, "blue");
        delete border;

        std::cout << "\r\nEnter a choice here :> ";
        std::string fileName; std::cin >> fileName;

        if(fileName != "00"){
            if(fileName == "1"){
                ope.setColor(!ope.getCstate());
            }else if(ope.reader(fileName)){
                if(!ope.options()){  break; }
            }
        }else{ break; }
    }

 
    ope.println("\r\nSuccessfully Exited!\r\n", "green");
    std::exit(1);
    return 0;
}