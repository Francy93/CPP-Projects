#include "library.hpp"


//quick-sort
void Collection::quicksort(std::deque<Books*>& arr, long long l, long long r, unsigned int titleIndex){

    //higher scope variables not recursively defined
    std::string pivot="";
    long long newIndex = 0;
    //prevent possible out-of-bonds
    long long aSize = arr.size();
    r = aSize > r? r: aSize-1;
    titleIndex = titleIndex < 0? 0: titleIndex;

    //recursive lambda function
    std::function<void(long long l, long long r)> recur = [&](long long left, long long right){

        long long i = left, j = right;
        newIndex = (left + right) / 2 ;
        pivot="";
        //getting the pivot string
        std::vector<std::string>arrWords = split((**(arr.begin()+newIndex)).getTitle(), " ");
        if(titleIndex < arrWords.size()){
            for (std::vector<std::string>::const_iterator it = arrWords.begin()+titleIndex; it != arrWords.end(); ++it){
                pivot += " "+*it;
            } 
            pivot = toLower(pivot);
        }
        
        // partition 
        while (i <= j) {
            std::string rightElem = "", leftElem = "";
            i--; j++;
            do{
                if(++i>= left){
                    leftElem = "";
                    std::vector<std::string>arrLeft = split((*arr[i]).getTitle(), " ");
                    if(titleIndex < arrLeft.size()){
                        for (std::vector<std::string>::const_iterator itL = arrLeft.begin()+titleIndex; itL != arrLeft.end(); ++itL){
                            leftElem += " "+*itL;
                        }
                    }
                }else{ i--; }
            }
            while (toLower(leftElem)  < pivot);
            do{
                if(--j <= right){
                    rightElem = "";
                    std::vector<std::string>arrRight = split((*arr[j]).getTitle(), " ");
                    if(titleIndex < arrRight.size()){
                        for (std::vector<std::string>::const_iterator itR = arrRight.begin()+titleIndex; itR != arrRight.end(); ++itR){
                            rightElem += " "+*itR;
                        }
                    }
                }else{ j++; }
            }
            while (toLower(rightElem) > pivot);

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

    //if the array entered is the big dummy data
    if(arr == data){ 
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
bool Collection::binarySearch(std::deque<Books*> &arr,std::string word){
    word = toLower(word);

    const long long arrSize = arr.size(), left=0, right=arrSize-1;
    long long firstMatches = 0;
    unsigned int index = -1, splittedSize = 0;
    bool end;
    std::string titleWord = "", shrinkedTitle = "";
    std::unordered_map<unsigned long long, Books*> found;

    // lambda function to get the word at a specific index (this has to be very light and performing)
    std::function<std::string(long long mid, unsigned int index)> getWord = [&](long long mid, unsigned int index){
        std::vector<std::string> splitted = split((*arr[mid]).getTitle(), " ");
        std::string titleWord = "";
        if(index < (splitted).size()){
            for (std::vector<std::string>::const_iterator it = (splitted).begin()+index; it != (splitted).end(); ++it){
                titleWord += (*it) + " ";
            }
        }
        return toLower(titleWord.substr(0, word.size()));
    };


    while(++index >= 0){
        
        //higher scope variables not recursively defined
        long long l=left, r= right, mid;
        //boolean witch determine the end of the DEEPER search
        end = true;
        //sorting the arr
        if(!(index == 0 && booksSorted)){ 
            //performin a shuffle to prevent cases of quadratic time scenario
            shuffle(arr);
            //sorting the array
            quicksort(arr, left, right, index);
        }
        
        //recursive lambda function (!! SEARCHING CORE RECURSION !!)
        std::function<bool()> recur = [&](){
            
            mid = l + (r - l) / 2;
            mid = mid < arrSize? mid: arrSize-1;
            
 

            if (r >= l) {
                std::vector<std::string> *splitted = new std::vector<std::string>{split((*arr[mid]).getTitle(), " ")};
                shrinkedTitle = titleWord = "";
                splittedSize = (*splitted).size();
                if(index < splittedSize){
                    for (std::vector<std::string>::const_iterator it = (*splitted).begin()+index; it != (*splitted).end(); ++it){
                        titleWord += (*it) + " ";
                    }
                    titleWord = toLower(titleWord);
                    shrinkedTitle = (titleWord).substr(0, word.size());
                }
                //destroying pointer
                delete splitted;
                //condition to exit the "title index" while loop
                end = index+1 < splittedSize? false: end;


                if (shrinkedTitle == word){
                    long long increaseMid = mid, decreaseMid = mid;


                    while(right >= ++increaseMid){
                        // checking if the next title matches
                        if(getWord(increaseMid, index) != word){ break; }
                    }
                    while(left <= --decreaseMid){
                        // checking if the previous title matches
                        if(getWord(decreaseMid, index) != word){ break; }
                    }
                    decreaseMid++;


                    //iterating over the found matches indexes
                    for (auto it = arr.begin()+decreaseMid; it != arr.begin()+increaseMid; it++){
                        found[(**it).getId()] = *it;
                    }

                    //get quantity of books found in the first seach
                    firstMatches = index == 0? found.size(): firstMatches;
                    return true;
                }
                //end of "found" condition



                // If element is smaller than mid, then it can only be present in left subarray 
                if (titleWord > word){
                    r = mid -1;
                    return recur(); 
                }
                // Else the element can only be present in right subarray
                l = mid +1;

                
                return recur(); 
            }


            // end of searching recursion
            return false;
        };




        
        // We reach here right before the actual search starts (recur())
        if(!recur() && index == 0 && !end){
            println("NOTHING FOUND, AT A GLANCE! ", "yellow");
            
            //processing the choice
            switch(navChoice({"Perform a DEEPER search?"}, 8)){
                case -1: return false; //terminatign this function and close the program 
                case  0: return true ; //exiting the main loop and terminate this function
            }
  
        }else if(found.size() > 0){

            if(index == 0){
                for (auto f = found.begin(); f != found.end(); f++){
                    std::cout << "FOUND: -----> " << (*(f->second)).getTitle() << std::endl;
                }
                
                println("\r\n", "YES, FOUND: ", std::to_string(firstMatches), "\r\n", "green");
                std::cout << navOptions({"Select a book","PERFORM A DEEPER SEARCH"}, 10) << std::endl;

            }else if(end ){
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
                    quicksort(arr, left, right, 0);
                return true; }
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