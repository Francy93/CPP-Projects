#include "../include/library.hpp"



// -----------------   class Collection   ------------------

Books* Collection::getBook(unsigned long long index){
    if(index < data.size()) return data[index];
    else{
        Util::println("\r\nOut of Bonds! Returned last index element.\r\n", "red");
        return data[data.size()-1];
    }
    
}

/**
 * @brief adding the new book to the data structure
 * 
 * @param book 
 */
void Collection::addBook(Books *book){
    //adding the new book to the primary data structure
    data.push_back(book);
    shuffle(data);
    quicksort(data, 0, data.size()-1, 0);
    
    const unsigned long titleSize = book->getSTsize();

    //cicling over the title length of the new book
    for(unsigned long i=0; i<titleSize; i++){
        if(i >= sortedDataInMemory.size()){
            //if iterating the first index the basic ordered one
            if(i == 0){
                //defining a nex index into sortedDataInMemory
                sortedDataInMemory.push_back(data);
            }else{
                //declaring a new empty index into sortedDataInMemory
                sortedDataInMemory.push_back({});

                //cicling over the whole data
                for(unsigned long long j=0; j<data.size(); j++){
                    const unsigned long iterTsize = data[j]->getSTsize();

                    if(iterTsize > i) sortedDataInMemory[i].push_back(data[j]);
                }
                //sorting the deque just made at a specific index of the words (i)
                quicksort(sortedDataInMemory[i], 0, sortedDataInMemory[i].size()-1, i);
            }
        }else{
            //insert the new book into the proper sorted index of the "sortedDataInMemory"
            sortedDataInMemory.at(i).insert(sortedDataInMemory.at(i).begin() + (long)bookSearch(sortedDataInMemory.at(i), book, i)[1], book);
        }
        //printing the loading bar
        Util::loading(titleSize, i+1);
    }
}

/**
 * @brief book remover
 * 
 * @param indexes 
 * @return true 
 * @return false 
 */
bool Collection::removeBook(std::vector<double> indexes){
    bool removed = true;
    Books *b = nullptr;

    for(unsigned int i=0; i<indexes.size(); i++){
        if(indexes[i] != -1){
            if(i == 0){
                //removing the book from the default data
                if(indexes[i] < (double)data.size() && indexes[i] >= 0){
                    b = &(*data[(unsigned long)indexes[i]]);
                    data.erase(data.begin() + (long)indexes[i]);
                }else{ 
                    Util::println("\r\nERROR! Out of Bonds. No book removed at index ", std::to_string(indexes[i])," of default Data.", "\r\n", "yellow");
                    removed = false;
                }
            }else{
                //removing the book from the sortedDataInMemory
                if(i-1 <= sortedDataInMemory.size()){
                    if((unsigned long)indexes[i] < sortedDataInMemory[i-1].size() && indexes[i] >= 0){
                        sortedDataInMemory[i-1].erase(sortedDataInMemory[i-1].begin() + (long)indexes[i]);
                    }else{ 
                        Util::println("\r\nERROR! Out of Bonds at index ", std::to_string(i-1)," of sortedDataInMemory.", "\r\n", "yellow");
                        removed = false;
                    }
                }else{ 
                    Util::println("\r\nERROR! Out of Bonds. No book removed at index ", std::to_string(i-1)," of sortedDataInMemory.", "\r\n", "yellow");
                    removed = false;
                }
            }
        }else{
            std::string d = i<1? "default Data": "sortedDataInMemory";
            Util::println("\r\nERROR! No book found at ", d, "\r\n", "yellow");
            removed = false;
        }
    }
    if(removed) delete b;
    return removed;
}

/**
 * @brief get book index
 * 
 * @param book 
 * @return std::vector<double> 
 */
std::vector<double> Collection::bookIndexes(Books *book){
    std::vector<double> foundIndexes;

    //getting index from default data
    const std::vector<unsigned long long> resultData = bookSearch(data, book, 0);
    if(resultData[0] != 0) foundIndexes.push_back((double)resultData[1]);
    else{
         foundIndexes.push_back(-1);
         Util::println("No book found in standard data", "yellow"); 
    }

    //getting the maximum index iterable
    const unsigned long tLength = book->getSTsize();
    const unsigned long sdimMaxSize = tLength > sortedDataInMemory.size()? sortedDataInMemory.size(): tLength;

    //getting indexes from the sortedDataInMemory
    for(unsigned int i=0; i<sdimMaxSize; i++){
        const std::vector<unsigned long long> result = bookSearch(sortedDataInMemory[i], book, i);
        if(result[0] != 0) foundIndexes.push_back((double)result[1]);
        else{
            foundIndexes.push_back(-1);
            Util::println("ERROR occurred! No book found in sortedDataInMemory at index: ", std::to_string(i), "red");
        }
    }
    return foundIndexes;
}

/**
 * @brief erase entire collection
 * 
 */
void Collection::collectionClear(){
    data.clear();
    sortedDataInMemory.clear();
}

/**
 * @brief adding a new book
 * 
 */
void Collection::addNewBook(){

    const std::vector<std::string> wizard = {"Enter here the title","Now enter the author","Include an ISBN","Specify a quantity"};
    std::vector<std::string> bookData;
    
    //cicling over all the required filds
    for(unsigned int i=0; i< wizard.size(); i++){
        Util::println("\r\n",wizard[i],"magenta");
        //getting user input
        std::cout << "\r\nEnter data here :> ";
        const std::string choice = Util::cinln();

        if(i > 1){
            if(!Util::isNumber(choice) || (i > 2 && Util::sTod(choice) < 1)){
                Util::println("\r\nInput has to be a valid numeric! Try again.","yellow");
                --i;
            }else bookData.push_back(choice);
        }else bookData.push_back(choice);
    }
    
    //creating book
    Books *b = new Books(bookData[0], bookData[1], bookData[2], bookData[3]);
    //adding the book
    addBook( b );

    Util::println("\r\nBook successfully added!","green");
}

/**
 * @brief building booksTable
 * 
 * @param books 
 * @return std::string 
 */
std::string Collection::booksTable(std::deque<Books*> &books){
    //the instruction to append at the begining would be like: a.insert(a.begin(), b.begin(), b.end());
    std::deque<std::deque<std::string>> allData = {{"No."}, {"Title(s)"}};
    std::vector<unsigned long> longest = {(unsigned int)allData[0][0].size(), (unsigned int)allData[1][0].size()};

    //longest detector
    unsigned long long i=0;
    for (auto it = books.begin(); it != books.end(); it++){
        const std::string title = (**it).getTitle();
        //adding data
        allData[0].push_back(std::to_string(++i));
        allData[1].push_back(title);
        
        //calculating longest
        const unsigned long iSize = std::to_string(i).size(), titleSize = title.size();
        longest[0] = longest[0] < iSize? iSize: longest[0];
        longest[1] = longest[1] < titleSize? titleSize: longest[1];
    }

    return Util::tableMaker(allData, longest,100,"magenta")+"\n";
}

/**
 * @brief printing the whole collection is O(n*2) time complexity
 * 
 */
void Collection::printCollection(){
    std::cout << booksTable(data) << std::endl;
}

/**
 * @brief find a book
 * 
 * @return true 
 * @return false 
 */
bool Collection::findBook(){
    
    std::cout << "Enter here below a book title to search" << std::endl;
    Util::navOptions({}, 20, "yellow", true);

    //getting user input
    std::cout << "\r\nEnter data here :> ";
    const std::string choice = Util::cinln();
    std::cout << std::endl;

    if     (choice == "00") return false;
    else if(choice == "0" ) return true;
    else if(choice == "" || choice == " "){
        Util::println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
        return findBook();
    }else{
        //if any book was found than do..
        if(!binarySearch(data, choice)) return false;
        else return findBook();
    }
    return false;
}

/**
 * @brief select a book from a given list
 * 
 * @param books 
 * @return int 
 */
int Collection::booksChoice(std::deque<Books*> &books){
    //if the books list is not empty
    
    if(books.size() > 0){
        std::cout << booksTable(books) << std::endl;

        while(true){
            std::cout << "Enter here below the number of the book to show" << std::endl;
            Util::navOptions({}, 20, "yellow", true);

            //getting user input
            std::cout << "\r\nEnter number here :> ";
            const std::string choice = Util::cinln();

            if     (choice == "0" ) return 1;
            else if(choice == "00") return 0;
            else if(Util::isNumber(choice) && Util::sTod(choice)>0 && Util::sTod(choice) <= (double)books.size()){
                
                const double result = books[(unsigned long)(Util::sTod(choice)-1)]->bookManager();
                
                if     (result == 0) return 0;
                else if(result == 2){
                    //book removing
                    if(&books == &data){
                        // if the deque "books" corispond to the default "data"
                        if(removeBook(bookIndexes(books[(unsigned long)Util::sTod(choice)-1]))){
                            Util::println("\r\n", "Book successfully removed!.", "\r\n", "green");
                        }else Util::println("\r\n", "ERROR removing the book! Try again.", "\r\n", "red");
                        
                    }else{
                        if(removeBook(bookIndexes(books[(unsigned long)Util::sTod(choice)-1]))){
                            books.erase(books.begin() + (long)Util::sTod(choice)-1);
                            Util::println("\r\n", "Book successfully removed!.", "\r\n", "green");
                        
                        }else Util::println("\r\n", "ERROR removing the book! Try again.", "\r\n", "red");
                    }
                    return booksChoice(books);
                }else return booksChoice(books);
            }else Util::println("\r\n", "Wrong selection! Try again.", "\r\n", "yellow");
        }
    }else{ 
        Util::println("\r\n", "No more Books to show!", "\r\n", "\r\n", "yellow");
        return 1;
    }
    return 0;
}

/**
 * @brief data shuffle
 * 
 * @param d 
 */
void Collection::shuffle(std::deque<Books*> &d){
    //this function requires: #include <algorithm> and #include <regex>
    std::random_shuffle(d.begin(), d.end());
    if(&d == &data)  booksSorted = false;
}

/**
 * @brief coverting data to sortedDataInMemory
 * 
 */
void Collection::sortDataInMemory(){

    unsigned long long dataSize = data.size();
    quicksort(data, 0, dataSize-1, 0);

    // if the sortedDataInMemory is empty then fill it
    if(sortedDataInMemory.size() == 0){
        //filing the index 0
        sortedDataInMemory.push_back(data);
    
        //filling the sortedDataInMemory from the index 1 on
        for(unsigned long long i=0; i<dataSize; i++){
            unsigned long titleSize = Util::split(data[i]->getTitle(), " ").size();

            for(unsigned int j=1; j<titleSize; j++){
                if(j < sortedDataInMemory.size()) sortedDataInMemory[j].push_back(data[i]);
                else sortedDataInMemory.push_back({data[i]});
            }
            //printing the loading bar
            Util::loading(dataSize*2, i);
        }

        //sorting data of "sortedDataInMemory" from index 1 on
        unsigned long BIGsize = sortedDataInMemory.size();
        for(unsigned int i=1; i<BIGsize; i++){
            shuffle(sortedDataInMemory[i]);
            quicksort(sortedDataInMemory[i], 0, sortedDataInMemory[i].size()-1, i);

            //printing the loading bar
            Util::loading(BIGsize*2, BIGsize+i+1);
        }
    }
    std::cout << std::endl;
}