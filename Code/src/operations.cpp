#include "../include/library.hpp"


// -----------------   class Operations   ------------------

/**
 * @brief Construct a new Operations:: Operations object
 * 
 */
Operations::Operations(){ int s = system(""); s=s; };

/**
 * @brief file scanner
 * 
 * @param fileName 
 * @return true 
 * @return false 
 */
bool Operations::reader(std::string fileName){
    //getting the file lines amount
    std::ifstream count(fileName);
    const unsigned long long fileSize = std::count(std::istreambuf_iterator<char>(count),  std::istreambuf_iterator<char>(), '\n');
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
                line.c_str();
                //splitting the string by delimiter "tab" (ascii code 9)
                const std::vector<std::string> elements = Util::split(line, std::string(1, 9));

                if(elements.size() > 3){
                    if(Util::sTod(elements[3]) != 0){
                            //storing the book object
                            Books *b = new Books(elements[0], elements[1], elements[2], elements[3]);
                            data.push_back( b );
                    }else corruptedCounter++;
                }
                //printing the loading bar
                Util::loading(fileSize, ++index);
            }
            //closing the file scanner
            file.close();
            // displaying amount of corrupted data if any
            if(corruptedCounter != 0){
                const char s = corruptedCounter != 1? 's': '\0';
                Util::println("\r\n" +std::to_string(corruptedCounter)+ " book"+s+" data was corrupted!\r\n", "yellow");
            }
            //sorting elements
            shuffle(data);
            return true;
        }else if(file.fail()){
            Util::println("\r\nDummy file missing! Do you want to create one?", "cyan");

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
                    file.open(fileName, std::ios::in);
                    if (file.is_open()) { 
                        Util::println("File: \"" + fileName + "\"" + " has been successfully created!\r\n", "green");
                        newOpen = true; 
                    }else Util::println("Something went wrong while creating the new file.\r\n", "red");
                    break;
                }else if(choice == "0") return false;
                else Util::println("\r\nWrong selection!\r\n", "yellow");
            }
        }
    }
}

/**
 * @brief main menu
 * 
 * @return true 
 * @return false 
 */
bool Operations::options(){
    
    std::deque<std::deque<std::string>> * menu = new std::deque<std::deque<std::string>>{};
    *menu = {{"         MAIN MENU", "Add a new Book...........1", "Find a Book..............2", "Show collection..........3"}}; 
    std::cout << Util::tableMaker(*menu, "blue")+"\r";
    *menu = {Util::navOptions({},Util::getLongest(*menu)[0])};
    std::cout << Util::tableMaker(*menu, "blue", false)+"\n" << std::endl;
    delete menu;

    //sorting the default data deque
    if(!booksSorted){ shuffle(data); quicksort(data, 0, data.size()-1, 0); }

    //user input
    switch(Util::getChoice(3)){
        case -1: return false;
        case  0: return true;
        case  1: addNewBook(); return options();
        case  2:  
            if(!findBook()) return false;
            else return options();
        case  3:
            if(booksChoice(data) == 0) return false;
            else return options();
    }
    return false;
}