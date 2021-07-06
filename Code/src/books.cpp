#include "../include/library.hpp"


// -----------------   class Book   ------------------
/**
 * @brief Construct a new Books:: Books object
 * 
 * @param t 
 * @param a 
 * @param i 
 * @param q 
 */
Books::Books(std::string t, std::string a, std::string i, std::string q) : title(t), author(a), isbn(i) {
    qty = (unsigned long) Util::sTod(q);
    titleSplitter();
}

/**
 * @brief Destroy the Books:: Books object
 * 
 */
Books::~Books(){
    std::cout << "Deleted: " << isbn << std::endl;
}

 /**
  * @brief function to split title by indexes
  * 
  */
void Books::titleSplitter(){
    const std::vector<std::string> splitted(Util::split(Util::toLower(title), " ") );
    for(unsigned int i=0; i< splitted.size(); i++){
        std::string titleWord = "";
        for (std::vector<std::string>::const_iterator it = splitted.begin()+i; it != splitted.end(); ++it){
           titleWord += (*it) + " ";
        }
        
        splittedTitle.push_back(titleWord.substr(0, titleWord.size()-1));
    }
}

std::string Books::getTitle(){
    return title;
}
std::string Books::getAuthor(){
    return author;
}
std::string Books::getId(){
    return isbn;
}
unsigned long Books::getQty(){
    return qty;
}
std::string Books::getSplittedT(unsigned long index){
    if(index >= splittedTitle.size()) return "";
    return splittedTitle[index];
}
unsigned long Books::getSTsize(){
    return splittedTitle.size();
}

/**
 * @brief modifing book quantity
 * 
 * @param qty 
 * @param mode 
 * @return unsigned int 
 */
unsigned short Books::setQty(long q, bool mode){

    if(mode && q > 0) Books::qty = getQty()+(unsigned long)q;
    else{
        //inverting sign
        q = q<0? q*-1: q;
        if(Books::qty-(unsigned long)q == 0){
            Util::println("\r\nWARNING! Are you sure you wanna remove this title from the library?\r\n", "yellow");
            //processing the choice
            switch(Util::navChoice({"Confirm"}, 20)){
                case -1: return 0; //terminatign this function and close the program
                case  0: return 1; //exiting the main loop and terminate this function
                case  1: return 2;
            }

        }else if((long long)Books::qty-(long long)q > 0) Books::qty -= (unsigned long)q;
        else{
            Util::println("\r\nERROR! Not so many copies to be decreased.", "red");
            std::cout << "Try again..\r\n" << std::endl;
        }
    }
    return 1;
}

/**
 * @brief checking wether a book is empty or not
 * 
 * @return true 
 * @return false 
 */
bool Books::emptyCheck(){
    if((getTitle() == "" && getAuthor() == "" && getId() == "") || getQty() <= 0) return false;
    return true;
}

/**
 * @brief printing book values
 * 
 * @return std::string 
 */
std::string Books::bookPrint(){

    std::deque<std::deque<std::string>> allData =   {{ "ATTRIBUTES", "Title", "Authors", "ISBN", "Quantity" },
                    { "VALUES", getTitle(), getAuthor(), getId(), std::to_string(getQty()) }};

    std::vector<unsigned long> longest = {allData[0][0].size(), allData[1][0].size()};


    //longest detector
    for(unsigned int i=0; i<longest.size() ;i++){
        for(unsigned int j=0; j<allData[0].size(); j++){
            const std::string attr = allData[i][j];
            //calculating longest
            longest[i] = longest[i] < attr.size()? attr.size(): longest[i];
        }
    }
    return Util::tableMaker(allData, longest,100,"magenta")+"\n";
}

/**
 * @brief book dashboard
 * 
 * @return int 
 */
int Books::bookManager(){
    std::cout << bookPrint();

    while(true){
        std::cout << std::endl;
        //processing the choice
        switch(Util::navChoice({"Edit the quantity"}, 40)){
            case -1: return 0; //terminatign this function and close the program 
            case  0: return 1; //exiting the main loop and terminate this function
            case  1:
        
                while(true){
                    std::cout << "\r\nNow enter a quantity to be summed (e.g 1, -1, 5, -18)" << std::endl;
                    Util::navOptions({}, 20, "yellow",true);

                    //getting user input
                    std::cout << "\r\nEnter a choice here :> ";
                    std::string choice; std::cin >> choice;
                    std::cout << std::endl;

                    if(choice == "0") return bookManager();
                    else if(choice == "00") return 0;
                    else if(Util::isNumber(choice)){
                        long *q = new long((long)Util::sTod(choice));
                        unsigned int *result = new unsigned int(setQty(*q, *q>0));
                        delete q;

                        if(*result == 0){
                            delete result;
                            return 0;
                        }else if(*result == 1){
                            delete result;
                            return bookManager();
                        }else return 2;
                        break; //----------------------------------------------------------book remouving if decrese to 0 (TO DO)
                    }else Util::println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
                }

            return bookManager();
        }
    }
    return 0;
}