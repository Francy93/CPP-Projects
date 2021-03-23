#include "library.hpp"

// -----------------   global functions   ------------------
//to string
long long Global::sToll(std::string s){
    //checking if string is a digit
    if(std::regex_match (s, std::regex("[-|+]{0,1}[0-9]+") )){
        return stoull(s);
    }
    return 0;
}

std::string Global::toLower(std::string s){
    #include <cctype>
    
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return s;
}

//the strings splitter
std::vector<std::string> Global::split(std::string s, std::string delim){
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
//an enanched and actually workin cin
std::string Global::cinln(){
    std::string input;

    fflush(stdin);
    std::cin.clear();
    std::getline(std::cin, input);
    std::cout << std::endl;
    fflush(stdin);
    std::cin.clear();

    return input;
}
// table generator
std::string Global::tableMaker(std::deque<std::deque<std::string>> &allData, std::vector<unsigned int> longest){
        
    std::string table = "";
    std::string border = "";
    for(unsigned long long i=0; i<allData[0].size(); i++){
        //creating the row
        std::string symbol = "|";
        std::string delimiter ="\033[1;35m"+symbol+"\033[0m";
        std::string row = "";
        for(unsigned int j=0; j<allData.size(); j++){
            auto allD = allData[j][i];
            unsigned long long allDsize = allD.size();
            int leng = longest[j] - allDsize < 1? 0: longest[j] - allDsize;
            std::string elem ="";
            std::string spaces = leng > 0? std::string(leng, ' '): "";

            std::string color = "";
            std::string start = "";
            std::string end   = "";
            if(i == 0 || j == 0){
                color = "3";
                if(i == 0){
                    color = "6";
                }
                start = "\033[1;3"+color+"m";
                end   = "\033[0m";
            }
            elem += start+ allD + spaces + end;
            row += delimiter+" " + elem + " ";
        }
        row += delimiter;

        if(i == 0){
            //creating the border
            int spaces = row.size()-((delimiter.size()-symbol.size())*2*allData.size()+delimiter.size()-symbol.size());
            border += "\033[1;35m"+std::string(spaces, '-')+"\033[0m";
            table += border+"\r\n"+row+"\r\n"+border+"\r\n";
        }else{
            table += row+"\r\n";
        }
    }

    return table+border+"\r\n";
}












// -----------------   class Collection   ------------------

Books Collection::getBook(int index){
    if((unsigned int)index < data.size()){
        return data[index];
    }else{
        println("\r\nOut of Bonds! Returned last index element.\r\n", "red");
        return data[data.size()-1];
    }
    
}
void Collection::addBook(Books book){
    data.push_back(book);
}
bool Collection::removeBook(double index){
    if(index < data.size() && index >=0){
        data.erase(data.begin() + index);
        return true;
    }else{
        println("\r\nERROR! Out of Bonds. No book removed.\r\n", "yellow");
    }
    return false;
}
//get book index
double Collection::bookIndex(Books book){
        long long i = 0;
        for (auto it = data.cbegin(); it != data.cend(); ++it){
            i++;
            Books current = *it;
            if(book.getId() == current.getId()){
                return i;
            }
        }
    return -1;
}
void Collection::collectionClear(){
    data.clear();
}

//adding a new book
int Collection::addNewBook(){

    std::vector<std::string> wizard = {"Enter here the title","Now enter the author","Include an ISBN","Specify a quantity"};
    std::vector<std::string> bookData;
    
    std::cin.ignore();
    for(unsigned int i=0; i< wizard.size(); i++){
        println("\r\n",wizard[i],"magenta");
        //getting user input
        std::cout << "\r\nEnter data here :> ";
        std::string choice;
        std::getline(std::cin, choice);
        std::cout << std::endl;

        if(i > 1){
            if(sToll(choice) == 0){
                println("\r\nInput has to be a valid numeric! Try again.","yellow");
                --i;
            }else{ bookData.push_back(choice); }
        }else{ bookData.push_back(choice); }
    }
    
    //creating book
    Books book(bookData[0], bookData[1], bookData[2], bookData[3]);
    //adding the book
    addBook(book);

    println("\r\nBook successfully added!","green");
    return 1;
}


//building booksTable
std::string Collection::booksTable(std::deque<Books> &books){
    //the instruction to append at the begining would be like: a.insert(a.begin(), b.begin(), b.end());
    std::deque<std::deque<std::string>> allData = {{"No."}, {"Title(s)"}};
    std::vector<unsigned int> longest = {allData[0][0].size(), allData[1][0].size()};

    //longest detector
    unsigned long long i=0;
    for (auto it = books.begin(); it != books.end(); it++){
        std::string title = (*it).getTitle();
        //adding data
        allData[0].push_back(std::to_string(++i));
        allData[1].push_back(title);
        
        //calculating longest
        unsigned int iSize = std::to_string(i).size(), titleSize = title.size();
        longest[0] = longest[0] < iSize? iSize: longest[0];
        longest[1] = longest[1] < titleSize? titleSize: longest[1];
    }

    return tableMaker(allData, longest);
}

void Collection::printCollection(){
    std::cout << booksTable(data) << std::endl;
}

//find a book
int Collection::findBook(){
    
    std::cout << "Enter here below a book title to search" << std::endl;
    std::cout << "Go back..........1 " << std::endl;
    std::cout << "Exit.............0 " << std::endl;

    //getting user input
    std::cout << "\r\nEnter data here :> ";
    std::string choice = cinln();
    std::cout << std::endl;

    if(choice == "0"){
        return 0;
    }else if(choice == "1"){
        return 1;
    }else if(choice == "" || choice == " "){
        println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
        return findBook();
    }else{
        std::deque<Books> books =  binarySearch(data, choice);
        //if any book was found than do..
        if(books.size() != 0){
            if(booksChoice(books) == 0){
                return 0;
            }
            return findBook();
        }else{
            println("\r\n", "No books found! Try again.", "\r\n", "cyan");
            return findBook();
        }

    }
    return 0;
}
//select a book from a given list
int Collection::booksChoice(std::deque<Books> books){
    //if the books list is not empty
    if(books.size() > 0){
        std::cout << booksTable(books) << std::endl;

        while(true){
            std::cout << "Enter here below the number of the book to show" << std::endl;
            std::cout << "Go back...........0 " << std::endl;
            std::cout << "Exit.............00 " << std::endl;

            //getting user input
            std::cout << "\r\nEnter number here :> ";
            std::string choice; std::cin >> choice;
            std::cout << std::endl;

            if(choice == "0"){
                return 1;
            }else if(choice == "00"){
                return 0;
            }else if(sToll(choice) > 0 && sToll(choice) <= books.size()){
                
                int result = books[(sToll(choice)-1)].bookManager();
                if( result == 0){
                    return 0;
                }else if(result == 2){
                    //book removing
                    if(removeBook(bookIndex(books[(sToll(choice)-1)]))){
                        println("\r\n", "Book successfully removed!.", "\r\n", "green");
                        books.erase(books.begin() + sToll(choice)-1);
                    }
                    return booksChoice(books);
                }else{ return booksChoice(books); }
            }else{ println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
        }
    }else{ 
        println("\r\n", "No more Books to show!", "\r\n", "\r\n", "yellow");
        return 1;
    }
    return 0;
}

//data shuffle
void Collection::shuffle(std::deque<Books> &data){
    #include <algorithm>
    //performin a shuffle to prevent cases of quadratic time scenario
    std::random_shuffle(data.begin(), data.end());
}
/* //quick-sort
void Collection::quicksort(std::deque<Books>& a, int l, int r){
    #include <algorithm>
    std::random_shuffle(a.begin(), a.end());
    //recursive lambda function
    std::function<void(std::deque<Books>& a, int l, int r)> recur = [&](std::deque<Books>& arr, int left, int right){

        int i = left, j = right;
        Books pivot = arr[(left + right) / 2];

        // partition
        while (i <= j) {
            while (arr[i].getTitle() < pivot.getTitle()) { i++; }
            while (arr[j].getTitle() > pivot.getTitle()) { j--; }
            if (i <= j) {
                Books tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i++;
                j--;
            }
        }
        // recursion 
        if (left < j){ recur(arr, left, j); }
        if (i < right){ recur(arr, i, right); }
    };
    recur(a,l,r);
} */












// -----------------   class Book   ------------------
//constructor
Books::Books(std::string t, std::string a, std::string i, std::string q){
    title  = t;
    author = a;
    isbn   = stoull(i);
    qty    = stoul(q);
};
//decontructor
Books::~Books(){};

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
            println("\r\nWARNING! Are you sure you wanna remove this book from the library?\r\n", "yellow");
            std::cout << "Confirm.............2" << std::endl;
            std::cout << "Abort...............1" << std::endl;
            std::cout << "Exit................0" << std::endl;
            std::cout << "\r\nEnter a choice here :> ";
            //getting user input
            std::string choice; std::cin >> choice;
            if(choice == "2"){
                return 2;
            }else if(choice == "1"){
                return 1;
            }else if(choice == "0"){
                return 0;
            }else{
                println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
                return setQty(qty, mode);
            }
            

        }else if((int)Books::qty-qty < 0){
            println("\r\nERROR! Not so many copies to be decreased.", "red");
            std::cout << "Try again..\r\n" << std::endl;
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
    
    //println(getTitle(),"_", getAuthor(),"_", std::to_string(getId()),"_", std::to_string(getQty()),"red");

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
        std::cout << "\r\nEdit the quantity...2" << std::endl;
        std::cout << "Go back.............1" << std::endl;
        std::cout << "Exit................0" << std::endl;

        //getting user input
        std::cout << "\r\nEnter a choice here :> ";
        std::string choice; std::cin >> choice;
        std::cout << std::endl;
        
        if(choice == "0"){
            return 0;
        }else if(choice == "1"){
            return 1;
        }else if(choice == "2"){
            while(true){
                std::cout << "\r\nNow enter a quantity to be summed (e.g 1, -1, 5, -18)" << std::endl;
                std::cout << "Go back.............0" << std::endl;

                //getting user input
                std::cout << "\r\nEnter a choice here :> ";
                std::string choice; std::cin >> choice;
                std::cout << std::endl;

                if(choice == "0"){
                    return bookManager();
                }else if(sToll(choice) != 0){
                    long *q = new long(sToll(choice));
                    int result = setQty(*q, *q>0);
                    if(result == 0){
                        return 0;
                    }else if(result == 1){
                        return bookManager();
                    }else{
                        return 2;
                    }
                    delete q;
                    break; //----------------------------------------------------------book remouving if decrese to 0 (TO DO)
                }else { println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
            }

            return bookManager();
        }else{ println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow"); }
    }
    return 0;
}










// -----------------   class Operations   ------------------

//file reader
int Operations::reader(std::string fileName){
    
    std::ifstream file(fileName);
    bool newOpen = false;
    collectionClear();

    while(true){
        if (file.is_open() || newOpen) {
            std::string line;
            int corruptedCounter = 0;
            while (std::getline(file, line)) {
                line.c_str();

                if(line != "" && line != "1" && line != "0"){
                    //splitting the string by delimiter "tab" (ascii code 9)
                    std::vector<std::string> elements = split(line, std::string(1, 9));
                    if(elements.size() > 3 && elements.size() < 6){
                        if(sToll(elements[3]) != 0 && sToll(elements[2]) != 0){
                        //Making the book object
                        Books book(elements[0], elements[1], elements[2], elements[3]);
                            //storing the book object
                            addBook(book);
                        }
                    }else{ corruptedCounter++; }
                }
            }
            //closing the file scanner
            file.close();
            // displaying amount of corrupted data if any
            if(corruptedCounter != 0){
                println("\r\n" +std::to_string(corruptedCounter)+ " book(s) data was corrupted!\r\n", "yellow");
            }
            //sorting elements
            shuffle(data);
            quicksort(data, 0, data.size() - 1, 0);
            return 1;
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
                    return 0;
                }else{
                    println("\r\nWrong selection!\r\n", "yellow");
                }
            }
        }
    }
}

int Operations::options(){
    
    std::string *border = new std::string("----------------------");
    println("\r\n", *border, "blue");
    std::cout << "| Add a new Book...2 |" << std::endl;
    std::cout << "| Find a Book......3 |" << std::endl;
    std::cout << "| Show collection..4 |" << std::endl;
    println(*border, "blue");
    std::cout << "| Go BACK..........1 |" << std::endl;
    std::cout << "| EXIT.............0 |" << std::endl;
    println(*border,"\r\n", "blue");
    delete border;

    //user input
    std::cout << "\r\nEnter a choice here :> ";
    std::string choice; std::cin >> choice;
    std::cout << std::endl;

    if(choice == "2"){
        int nav = addNewBook();
        if(nav == 0){
            return 0;
        } else {  return options(); }
    }else if(choice == "3"){
        int nav = findBook();
        if(nav == 0){
            return 0;
        } else {  return options(); }
    }else if(choice == "4"){
        if(booksChoice(data) == 0){
            return 0;
        }
        return options();
    }else if(choice == "1"){
        return 1;
    }else if(choice == "0"){
        return 0;
    }else{
        println("\r\nWrong selection!\r\n", "yellow");
        return options();
    }

    return 0;
}