#include "library.hpp"


// -----------------   class Book   ------------------
//constructor
Books::Books(std::string t, std::string a, std::string i, std::string q){
    title  = t;
    author = a;
    isbn   = stoull(i);
    qty    = stoul(q);
};
//decontructor
Books::~Books(){
    std::cout << "Deleted: " << isbn << std::endl;
};

std::string Books::getTitle(){
    return title;
}
std::string Books::getAuthor(){
    return author;
}
unsigned long long Books::getId(){
    return isbn;
}
unsigned int Books::getQty(){
    return qty;
}
// modifing book quantity
unsigned int Books::setQty(int qty, bool mode){

    if(mode && qty > 0){
        Books::qty = getQty()+qty;
    }else{
        //inverting sign
        qty = qty<0? qty*-1: qty;
        if(Books::qty-qty == 0){
            println("\r\nWARNING! Are you sure you wanna remove this title from the library?\r\n", "yellow");
            //processing the choice
            switch(navChoice({"Confirm"}, 10)){
                case -1: return 0; //terminatign this function and close the program
                case  0: return 1; //exiting the main loop and terminate this function
                case  1: return 2;
            }

        }else if((int)Books::qty-qty < 0){
            println("\r\nERROR! Not so many copies to be decreased.", "red");
            std::cout << "Try again..\r\n" << std::endl;
        }else{
            Books::qty -= qty;
        }
    }
    return 1;
}
//checking wether a book is empty or not
bool Books::emptyCheck(){
    if((getTitle() == "0" && getAuthor() == "0" && getId() == 0) || getQty() <= 0){
        return false;
    }
    return true;
}
//printing book values
std::string Books::bookPrint(){

    std::deque<std::deque<std::string>> allData =   {{ "ATTRIBUTES", "Title", "Authors", "ISBN", "Quantity" },
                    { "VALUES", getTitle(), getAuthor(), std::to_string(getId()), std::to_string(getQty()) }};

    std::vector<unsigned int> longest = {allData[0][0].size(), allData[1][0].size()};


    //longest detector
    for(unsigned int i=0; i<longest.size() ;i++){
        for(unsigned int j=0; j<allData[0].size(); j++){
            std::string attr = allData[i][j];
            //calculating longest
            longest[i] = longest[i] < attr.size()? attr.size(): longest[i];
        }
    }
    return tableMaker(allData, longest);
}
//book dashboard
int Books::bookManager(){
    std::cout << bookPrint() <<std::endl;

    while(true){
        std::cout << std::endl;
        //processing the choice
        switch(navChoice({"Edit the quantity"}, 10)){
            case -1: return 0; //terminatign this function and close the program 
            case  0: return 1; //exiting the main loop and terminate this function
            case  1:
        
                while(true){
                    std::cout << "\r\nNow enter a quantity to be summed (e.g 1, -1, 5, -18)" << std::endl;
                    std::cout << navOptions({}, 10) << std::endl;

                    //getting user input
                    std::cout << "\r\nEnter a choice here :> ";
                    std::string choice; std::cin >> choice;
                    std::cout << std::endl;

                    if(choice == "0"){
                        return bookManager();
                    }else if(choice == "00"){
                        return 0;
                    }else if(sToll(choice) != 0){
                        long *q = new long(sToll(choice));
                        int *result = new int(setQty(*q, *q>0));
                        delete q;

                        if(*result == 0){
                            delete result;
                            return 0;
                        }else if(*result == 1){
                            delete result;
                            return bookManager();
                        }else{
                            return 2;
                        }
                        break; //----------------------------------------------------------book remouving if decrese to 0 (TO DO)
                    }else { println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
                }

            return bookManager();
        }
    }
    return 0;
}