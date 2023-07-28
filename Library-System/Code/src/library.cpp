/**
 * @file library.cpp
 * @author Francy (fa1033@live.mdx.ac.uk)
 * @brief final project of Software engineering and management
 * @version 0.1
 * @date 2021-04-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/library.hpp"


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





int main(int argc, char *argv[]){
    // geting main arguments (file name to read)
    std::string fileName = "";
    if(argc > 1){
        for (int i = 1; i < argc; ++i) {
            fileName += argv[i];
            if(i < argc-1) fileName += " ";
        }
    }

    Operations ope;
    
    //colored intro title
    Util::println(*intro, "\r\n", "green");
    delete intro;

    //printing the welcome
    std::string *title = new std::string("  WELCOME TO THE LIBRARY SYSTEM  ");
    std::string *titleColor = new std::string("\033[1;43m "+std::string(title->size(), '.')+" \033[0m");
    Util::println("\r\n\r\n\r\n", *titleColor, "yellow");
    Util::println("\033[1;43m ", *title," \033[0m", "black");
    Util::println(*titleColor, "\r\n", "yellow");


    delete titleColor;
    delete title;



    //starting the main while loop
    while(true){
        std::deque<std::deque<std::string>> * loader = new std::deque<std::deque<std::string>>{};
        *loader={{"      DUMMY FILE LOADER","Enter here below the file name","Press  1  to set color ON/OFF","Otherwise enter  00  to EXIT"}};
        std::cout << Util::tableMaker(*loader, "blue")+"\n" << std::endl;
        delete loader;

        //getting file name to read
        std::cout << "Enter a choice here :> ";
        if(fileName == "") fileName = Util::cinln();
        else std::cout << fileName << std::endl;


        if(fileName != "00"){
            if(fileName == "1") Util::setColor(!Util::getColorState());
            else if(ope.reader(fileName)){
                Util::println("File: \"", fileName, "\" successfully loaded!\r\n", "green");
                Util::println("\r\nBefore starting! Would you like to sort data so as to experience faster performances?", "yellow");
                const long long nav = Util::navChoice({"Yes, get faster!    (RAM consumption)", "No, go normal speed (CPU consumption)"}, 40);

                if(nav == 2){ if(!ope.options()) break; }
                else if(nav == 1){ ope.sortDataInMemory(); if(!ope.options()) break; }
                else if(nav == -1) break;
            }
        }else break;

        //resetting the fileName
        fileName = "";
    }

 
    Util::println("\r\nSuccessfully Exited!\r\n", "green");
    std::exit(0);
    return 0;
}