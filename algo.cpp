#include "library.hpp"


//quick-sort
void Collection::quicksort(std::deque<Books>& arr, long long l, long long r, unsigned int titleIndex){

    //higher scope variables
    std::string pivot="";
    long long newIndex = 0;

    //recursive lambda function
    std::function<void(long long l, long long r)> recur = [&](long long left, long long right){

        long long i = left, j = right;
        newIndex = (left + right) / 2 ;
        pivot="";
        //getting the pivot string
        std::vector<std::string>arrWords = split((*(arr.begin()+newIndex)).getTitle(), " ");
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
                    std::vector<std::string>arrLeft = split(arr[i].getTitle(), " ");
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
                    std::vector<std::string>arrRight = split(arr[j].getTitle(), " ");
                    if(titleIndex < arrRight.size()){
                        for (std::vector<std::string>::const_iterator itR = arrRight.begin()+titleIndex; itR != arrRight.end(); ++itR){
                            rightElem += " "+*itR;
                        }
                    }
                }else{ j++; }
            }
            while (toLower(rightElem) > pivot);

            if (i <= j) {
                Books *tmp = new Books(arr[i]);
                //Books tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = *tmp;
                i++; j--;
                delete tmp;
            }
        }
        // recursion 
        if (left < j ){ recur(left, j ); }
        if (i < right){ recur(i, right); }
    };
    recur(l,r);
}

std::deque<Books> Collection::binarySearch(std::deque<Books> &arr,std::string word){
    word = toLower(word);
    long long arrSize = arr.size(), left=0, right=arrSize-1, cyclesFound = 0;
    unsigned int index = -1;
    int end = (int)index;
    std::unordered_map<unsigned long long, Books> found;
    /* std::deque<Books> found; */

    while(++index >= 0){
        
        long long l=left, r= right;
        end = -1;
        //performin a shuffle to prevent cases of quadratic time scenario
        shuffle(arr);
        //sorting the array
        quicksort(arr, left, right, index);

        long long mid;

        std::function<bool()> recur = [&](){

            mid = l + (r - l) / 2;
            mid = mid < arrSize? mid: arrSize-1;
            Books *book = new Books(arr[mid]);
            std::vector<std::string> *splitted = new std::vector<std::string>{split((*book).getTitle(), " ")};
            std::string *titleWord = new std::string((*splitted).size() > index? (*splitted)[index]: "");
            *titleWord = toLower(*titleWord);
            end = *titleWord != ""? index: end;

            if (r >= l) {
        
                // If the element is present at the middle itself 
                if (*titleWord == word){
                    long long increaseMid = mid, decreaseMid = mid;

                    while(right >= ++increaseMid){
                        *splitted = split((*(arr.begin()+ increaseMid)).getTitle(), " ");
                        std::string newWord = (*splitted).size() > index? (*splitted)[index]: "";

                        if(toLower(newWord) != word){ break; }
                    }
                    while(left <= --decreaseMid){
                        *splitted = split((*(arr.begin()+ decreaseMid)).getTitle(), " ");
                        std::string newWord = (*splitted).size() > index? (*splitted)[index]: "";

                        if(toLower(newWord) != word){ break; }
                    }
                    increaseMid--;
                    decreaseMid++;


                    /* long long i =0;
                    for (auto it = arr.begin()+decreaseMid; it != arr.begin()+increaseMid; it++){
                        found.push_back(*it);
                    } */
                    /* for(unsigned long i= decreaseMid; i<=increaseMid; i++){
                        if(i == mid){
                            found.push_back(*book);
                        }else{found.push_back(arr[i]); }
                    } */
                    for(unsigned long i= decreaseMid; i<=increaseMid; i++){
                        //Books *b = &(arr[i]);
                        unsigned long long id = arr[i].getId();
                        if(found.find(id) == found.end()){
                            found[id] = arr[i];
                            //delete b;
                        }
                    }
                    //destroying pointers
                    delete splitted;
                    delete titleWord;
                    delete book;
                    
                    //get quantity of books found in the first seach
                    cyclesFound = index == 0? found.size(): cyclesFound;
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
            delete book;

            return false;
        };
        
        // We reach here when search terminates
        //bool result = recur();
        if(!recur() && index == 0 && end == 0){
            println("NOT FOUND, AT A GLANCE! ", "yellow");
            println("Search better...1");
            println("Exit............0");

            //user choice
            std::string choice="";
            while(choice != "1" && choice != "0"){
                choice = cinln();
                if(choice != "1" && choice != "0"){
                    println("WRONG SELECTION! Try again.", "yellow"); 
                }
            }
            if(choice == "0"){ break; }
        }else if(found.size() > 0){
            if(index == 0){
                println("YES, FOUND: ", std::to_string(cyclesFound), "green");
            
                println("\r\n", "NOT WHAT YOU WERE LOOKING FOR?");
                println("Deep search.....1");
                println("Exit............0");
                
                //user choice
                std::string choice="";
                while(choice != "1" && choice != "0"){
                    choice = cinln();
                    if(choice != "1" && choice != "0"){
                        println("WRONG SELECTION! Try again.", "yellow"); 
                    }
                }
                if(choice == "0"){ break; }
            }else if(end == -1){
                if(found.size() > cyclesFound){
                    println("\r\n", "FOUND ", std::to_string(found.size()-cyclesFound), " MORE", "\r\n", "green");
                }else{ println("\r\n", "NOT FURTHER MATCHING FOUND!"); }
                break;
            }
        }else if(end == -1){
            println("DEFINITELY NOT FOUND! ","red");
            break;
        }
    }


    std::deque<Books>HF;
     for (auto f = found.begin(); f != found.end(); f++){
        HF.push_back(f->second);
    }
    return HF;
    /* return found; */
} 
