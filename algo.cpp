#include "library.hpp"


//quick-sort
void Collection::quicksort(std::deque<Books*>& arr, long long l, long long r, unsigned int titleIndex){

    //higher scope variables not recursively defined
    std::string pivot="";
    long long newIndex = 0;

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
    recur(l,r);
}

bool Collection::binarySearch(std::deque<Books*> &arr,std::string word){
    word = toLower(word);

    long long arrSize = arr.size(), left=0, right=arrSize-1, firstMatches = 0;
    unsigned int index = -1;
    bool end;
    std::unordered_map<unsigned long long, Books*> found;

    while(++index >= 0){
        
        //higher scope variables not recursively defined
        long long l=left, r= right;
        //boolean witch determine the end of the DEEPER search
        end = true;
        //performin a shuffle to prevent cases of quadratic time scenario
        shuffle(arr);
        //sorting the array
        quicksort(arr, left, right, index);
        long long mid;
        
        //recursive lambda function (searching core)
        std::function<bool()> recur = [&](){

            mid = l + (r - l) / 2;
            mid = mid < arrSize? mid: arrSize-1;
            //Books** book = new Books*(arr[mid]);
            //Books *book = arr[mid];
            std::vector<std::string> *splitted = new std::vector<std::string>{split((*arr[mid]).getTitle(), " ")};
            std::string *titleWord = new std::string((*splitted).size() > index? (*splitted)[index]: "");
            *titleWord = toLower(*titleWord);
            end = *titleWord != ""? false: end;

            if (r >= l) {
        
                // If the element is present at the middle itself 
                if (*titleWord == word){
                    long long increaseMid = mid, decreaseMid = mid;

                    while(right >= ++increaseMid){
                        *splitted = split((**(arr.begin()+ increaseMid)).getTitle(), " ");
                        std::string newWord = (*splitted).size() > index? (*splitted)[index]: "";

                        if(toLower(newWord) != word){ break; }
                    }
                    while(left <= --decreaseMid){
                        *splitted = split((**(arr.begin()+ decreaseMid)).getTitle(), " ");
                        std::string newWord = (*splitted).size() > index? (*splitted)[index]: "";

                        if(toLower(newWord) != word){ break; decreaseMid++; }
                    }

                    //iterating ofer the found matches indexes
                    for (auto it = arr.begin()+decreaseMid; it != arr.begin()+increaseMid; it++){
                        found[(**it).getId()] = *it;
                    }
                    //destroying pointers
                    delete splitted;
                    delete titleWord;
                    
                    //get quantity of books found in the first seach
                    firstMatches = index == 0? found.size(): firstMatches;
                    return true;
                }
                // If element is smaller than mid, then it can only be present in left subarray 
                if (*titleWord > word){
                    r = mid -1;
                    return recur(); 
                }
                // Else the element can only be present in right subarray
                l = mid +1;
                return recur(); 
            }
            //destroying pointers
            delete splitted;
            delete titleWord;

            return false;
        };
        
        // We reach here right before the actual search starts (recur())
        if(!recur() && index == 0 && !end){
            println("NOTHING FOUND, AT A GLANCE! ", "yellow");
            std::cout << navOptions({"Perform a DEEPER search?"}, 5) << std::endl;

            //checking the choice
            std::string choice="";
            while(true){
                std::cout << "Enter a choice here :> ";
                choice = cinln();
                if((choice != "1" && choice != "0" && choice != "00")){
                    println("WRONG SELECTION! Try again.", "yellow"); 
                }else{ break; }
            }
            //processing the choice
            if(choice == "00"){ return false; }  //terminatign this function and close the program 
            else if(choice == "0"){ return true; } //exiting the main loop and terminate this function
  
        }else if(found.size() > 0){

            if(index == 0){
                unsigned long long i = 0;
                for (auto f = found.begin(); f != found.end(); f++){
                    std::cout << " |" << ++i << " | "<< (*(f->second)).getTitle() << std::endl;
                }
                
                println("\r\n", "YES, FOUND: ", std::to_string(firstMatches), "\r\n", "green");
                std::cout << navOptions({"Select a book","Perform a DEEPER search"}, 10) << std::endl;

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
                }else{ println("\r\n", "NOT FURTHER MATCHING FOUND!"); break; }
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