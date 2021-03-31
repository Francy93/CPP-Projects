#include "library.hpp"


//quick-sort
void Collection::quicksort(std::deque<Books*>& arr, long long l, long long r, unsigned int titleIndex){

    //higher scope variables not recursively defined
    std::string pivot="";
    long long newIndex = 0;
    //prevent possible out-of-bonds
    long long aSize = arr.size();
    r = aSize > r? r: aSize-1;
    if(titleIndex < 0){ titleIndex =  0; }

    // lambda function to get the word at a specific index (this has to be very light and performing)
    std::function<std::string(long long index)> getString = [&](long long index){
        std::vector<std::string>arrWords = split((*arr[index]).getTitle(), " ");
        std::string lrElem = "";
        if(titleIndex < arrWords.size()){
            for (std::vector<std::string>::const_iterator it = arrWords.begin()+titleIndex; it != arrWords.end(); ++it){
                lrElem += " "+*it;
            }
        }
        return toLower(lrElem);
    };

    //recursive lambda function
    std::function<void(long long l, long long r)> recur = [&](long long left, long long right){

        long long i = left, j = right;
        newIndex = (left + right) / 2;

        //getting the pivot string
        pivot = getString(newIndex);
        
        // partition 
        while (i <= j) {
            std::string rightElem = "", leftElem = "";
            i--; j++;

            // loop left index if the word is lower than expetd
            do{ if(++i>= left){ leftElem = getString(i); }else{ i--; } }
            while (leftElem  < pivot);

            // loop right index if the word is higher than expetd
            do{ if(--j <= right){ rightElem = getString(j); }else{ j++; } }
            while (rightElem > pivot);

            if (i <= j) {
                Books** tmp = new Books*(arr[i]);
                arr[i] = arr[j];
                arr[j] = *tmp;
                i++; j--;
                delete tmp; //deleting the temp pointer pointing to another pinter (pointer of ponter **)
            }
        }
        // recursion 
        if (left < j ){ recur(left, j ); }
        if (i < right){ recur(i, right); }
    };

    //if the array entered is the default dummy data
    if(&arr == &data){
        //if books are sorted, do not further sort them
        if(titleIndex > 0 || l > 0 || r != aSize-1){
            recur(l,r);
            booksSorted = false;
        }else if(!booksSorted){
            recur(l,r);
            booksSorted = true;
        }
    }else{ recur(l,r); }
}













//the binary search algorithm
bool Collection::binarySearch(std::deque<Books*> &array,std::string word){
    word = toLower(word);

    unsigned long long arrSize = array.size(), left=0, right= arrSize > 0? arrSize-1: 0;
    long long firstMatches = 0;
    unsigned int index = -1;
    bool end;
    std::string iterTitle = "";
    std::unordered_map<std::string, Books*> found;
    std::deque<Books*> *a = &array;
    unsigned int sortedDataInMemorySIZE = sortedDataInMemory.size();

    // lambda function to get the word at a specific index (this has to be very light and performing)
    std::function<std::string(std::deque<Books*> &arr, long long mid, unsigned int index)> getWord = [&](std::deque<Books*> &arr, long long mid, unsigned int index){
        std::vector<std::string> splitted = split((*arr[mid]).getTitle(), " ");
        std::string titleWord = "";
        unsigned int splitSize = splitted.size();
        if(index < splitSize){
            for (std::vector<std::string>::const_iterator it = splitted.begin()+index; it != splitted.end(); ++it){
                titleWord += (*it) + " ";
            }
            //condition to terminate the while cycle of the DEEP search
            if(index+1 < splitSize){ end = false; }
        }
        return toLower(titleWord.substr(0, word.size()));
    };

    //while cycle intended for "deep" search purposes
    while(++index >= 0){
        //boolean witch determine the end of the DEEPER search
        end = true;

        //sorting the arr
        if(index < sortedDataInMemorySIZE){
            a = &sortedDataInMemory[index];
            arrSize = (*a).size();
            right = arrSize > 0? arrSize-1: 0;

        }else if(!(index == 0 && booksSorted)){
            a = &array;
            arrSize = (*a).size();
            right = arrSize > 0? arrSize-1: 0;

            //performin a shuffle to prevent cases of quadratic time scenario
            shuffle(*a);
            //sorting the array
            quicksort(*a, left, right, index);
        }

        //higher scope variables reset without a new recursive redefinition
        long long l=left, r= right, mid;
        


        
        //recursive lambda function (!! SEARCHING CORE RECURSION !!)
        std::function<bool(std::deque<Books*> &arr)> recur = [&](std::deque<Books*> &arr){
            // calculating the mid point
            mid = l + (r - l) / 2;
            

            if (r >= l) {
                iterTitle = getWord(arr, mid, index);
               

                //if a match has been found
                if (iterTitle == word){
                    double increaseMid = mid, decreaseMid = mid;

                    // checking if the next title matches
                    while(right >= ++increaseMid){ if(getWord(arr, increaseMid, index) != word){ break; } }
                    // checking if the previous title matches
                    while(left <= --decreaseMid ){ if(getWord(arr, decreaseMid, index) != word){ break; } }
                    decreaseMid++;


                    //iterating over the found matches indexes
                    for (auto it = arr.begin()+decreaseMid; it != arr.begin()+increaseMid; it++){
                        found[(**it).getId()] = *it;
                    }
                    //get quantity of books found in the first seach
                    if(index == 0){ firstMatches = found.size(); }
                    return true;
                }
                //end of "found" condition



                // If element is smaller than mid, then it can only be present in left subarray 
                if (iterTitle > word){
                    r = mid -1;
                    return recur(arr); 
                }
                // Else the element can only be present in right subarray
                l = mid +1;

                
                return recur(arr); 
            }

            // end of searching recursion if nothig found
            return false;
        };




        
        // We reach here right before the actual search starts (recur())
        if(!recur(*a) && index == 0 && !end){
            println("NOTHING FOUND, AT A GLANCE! ", "yellow");

            //processing the choice
            switch(navChoice({"Perform a DEEPER search?"}, 8)){
                case -1: return false; //terminatign this function and close the program 
                case  0: return true ; //exiting the main loop and terminate this function
            }
  
        }else if(found.size() > 0){

            if(index == 0 && !end){
                for (auto f = found.begin(); f != found.end(); f++){
                    std::cout << "FOUND: -----> " << (*(f->second)).getTitle() << std::endl;
                }
                
                println("\r\n", "YES, FOUND: ", std::to_string(firstMatches), "\r\n", "green");
                std::cout << navOptions({"Select one of these books","PERFORM A DEEPER SEARCH"}, 10) << std::endl;

            }else if(end){
                if(found.size() > firstMatches){
                    //printing the matches
                    for (auto f = found.begin(); f != found.end(); f++){
                        std::cout << "FOUND: -----> " << (*(f->second)).getTitle() << std::endl;
                    }
                    //printing the message
                    std::string* lastMatches = new std::string("");
                    if(firstMatches != 0){ *lastMatches += " MORE OUT OF " + std::to_string(found.size()); }
                    println("\r\n", "FOUND ", std::to_string(found.size()-firstMatches), *lastMatches, "\r\n", "green");
                    delete lastMatches;
                    std::cout << navOptions({"Select a book"}, 10) << std::endl;
                }else{ 
                    println("\r\n", "NO FURTHER MATCHING FOUND!");
                    if(&array == &data && sortedDataInMemory.size() > 0 && !booksSorted){
                        array = sortedDataInMemory[0];
                        booksSorted = true;
                    }else if(&array == &data && !booksSorted){ quicksort(array, left, right, 0); }
                    
                    return true; 
                }
            }

            if(index == 0 || end ){
                //user choice
                std::string choice="";
                while(true){
                    std::cout << "Enter a choice here :> ";
                    choice = cinln();
                    if((choice != "1" && choice != "0" && choice != "00" && choice != "2") || (choice == "2" && end)){
                        println("WRONG SELECTION! Try again.", "yellow"); 
                    }else{ break; }
                }

                if(choice == "00"){ return false; }  //terminatign this function and close the program 
                else if(choice == "0"){ return true; } //exiting the main loop and terminate this function
                else if(choice == "1"){ 
                    std::deque<Books*>HF;
                    for (auto f = found.begin(); f != found.end(); f++){
                        HF.push_back(f->second);
                    }
                    return booksChoice(HF);
                }
            }

        }else if(end){
            println("DEFINITELY NOT FOUND!\r\n", "red");
            return true;
        }
    }

    return false;
} 














//searching a book
std::vector<unsigned long long> Collection::bookSearch(std::deque<Books*>& arr, Books* book, unsigned int index){
    const long long arrSize = arr.size(), left=0, right=arrSize-1;
    std::string iterTitle = "";
    // store the final index
    unsigned long long result;

    // lambda function to get the word at a specific index 
    std::function<std::string(Books *b)> indexedTitle = [&](Books *b){
        std::vector<std::string> splitted = split((*b).getTitle(), " ");
        std::string titleWord = "";
        if(index < (splitted).size()){
            for (std::vector<std::string>::const_iterator it = (splitted).begin()+index; it != (splitted).end(); ++it){
                titleWord += (*it) + " ";
            }
        }
        return toLower(titleWord);
    };
    //getting the indexed title of the books to be searched
    std::string targetTitle = indexedTitle(book);


    
    //higher scope variables not recursively defined
    long long l=left, r= right, mid;
    //sorting the arr
    if(&arr == &data && !booksSorted){ 
        //performin a shuffle to prevent cases of quadratic time scenario
        shuffle(arr);
        //sorting the array
        quicksort(arr, left, right, index);
    }
    
    //recursive lambda function (!! SEARCHING CORE RECURSION !!)
    std::function<unsigned int()> search = [&](){
        // calculating the mid point
        mid = l + (r - l) / 2;
        


        if (r >= l){
            iterTitle = indexedTitle(arr[mid]);

            //if a match has been found
            if (iterTitle == targetTitle){

                //if the searched book is not actually the one just found
                if(book != arr[mid]){
                    unsigned long long i = mid;
                    
                    while(targetTitle == (*arr[++i]).getTitle()){
                        if(arr[i] == book){ result = i; return (unsigned int)1; }
                    }
                    i = mid;
                    while(targetTitle == (*arr[--i]).getTitle()){
                        if(arr[i] == book){ result = i; return (unsigned int)1; }
                    }
                    result = mid;
                    return (unsigned int)0;

                }else{
                    result = mid;
                    return (unsigned int)1;
                }
            }

            // If element is smaller than mid, then it can only be present in left subarray 
            if (iterTitle > targetTitle){
                r = mid -1;
                return search(); 
            }
            // Else the element can only be present in right subarray
            l = mid +1;

            return search(); 
        }
        result = mid;
        // end of searching recursion if nothig found
        return  (unsigned int)0;
    };

    // returning the result
    return {search(), result};
}