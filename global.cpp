#include "library.hpp"


// -----------------   global functions   ------------------

void Global::setColor(bool b){
    colorMode = b;
}
bool Global::getCstate(){
    return colorMode;
}



//making strings lower case
std::string Global::toLower(std::string s){
    #include <cctype>
    
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return s;
}

std::string Global::color(std::string c){
    std::string colors [] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
    std::string color = "";
    if(getCstate()){
        for (unsigned int i=0; i< sizeof(colors)/sizeof(*colors); i++){
            if(toLower(c) == colors[i]){
                color = "\033[1;3"+std::to_string(i)+"m";
            }
        }
    }
    return color;
}
std::string Global::colorReset(){
    std::string reset = "";
    if(getCstate()){
        reset = "\033[0m";
    }
    return reset;
}

std::string Global::loading(unsigned long long size, unsigned long long index){
    if (size > 0){
        static int i = 0;
        int percent = index * 100 / size;
        int maxToken = percent/5;
        
        if(maxToken != i){
            i = maxToken;
        
            if(percent != 100 && size > index){
                std::string status = std::string(maxToken*2, (char)219);
                return color("green")+status+" "+std::to_string(percent)+"%"+colorReset()+"\r";
            }
            return "\33[2K";
        }
    }
    return "";
}

//returns a string of numerate options
std::string Global::navOptions(std::vector<std::string> options, int minimum){
    std::string list="";

    int oSize = options.size();
    int iSize = 2;

    int i = 0;
    int longest = 7;
    for(auto o: options){
        int strSize = o.size();
        longest = strSize > longest? strSize: longest;
        if(++i == oSize){

        iSize = (int)std::to_string(i).size() > iSize? std::to_string(i).size(): iSize;
        }
    }
    
    std::string cStart = color("yellow"); //yellow corresponds to: "\033[1;35m"
    std::string cEnd = colorReset();      //reset  corresponds to: "\033[0m"

    longest += minimum > 3? minimum: 3;
    i = 0;
    for(auto o: options){
        int currenISize = iSize - std::to_string(i).size();
        int dotSize = longest - o.size();
        std::string index = cStart + std::to_string(++i) + cEnd;
        
        list += o + std::string(dotSize+currenISize,'.') + index + "\r\n";
    }

    list += "Go back" + std::string(longest-7+iSize-1,'.') + cStart+"0"+cEnd  + "\r\n";
    list += "Exit"    + std::string(longest-4+iSize-2,'.') + cStart+"00"+cEnd + "\r\n";
    return list;
}

//string to long long
long long Global::sToll(std::string s){
    //checking if string is a digit
    // sToll() method uses the regex library: #include <regex>
    if(std::regex_match (s, std::regex("[-|+]{0,1}[0-9]+") )){
        return stoull(s);
    }
    return 0;
}

//the strings splitter
std::vector<std::string> Global::split(std::string s, std::string delim){
    std::vector<std::string> vec;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        
        if(pos != 0){
            token = s.substr(0, pos);
            vec.push_back(token);
        }
        s.erase(0, pos + delim.length());
    }
    if(s.size() > 0){
        vec.push_back(s);
    }
    return vec;
}
//an enanched and actually working cin
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
        std::string colorStart = color("magenta");
        std::string colorEnd = colorReset();

        std::string symbol = "|";
        std::string delimiter =colorStart+symbol+colorEnd;
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
                end   = colorEnd;
            }
            if(!getCstate()){ start =""; }
            elem += start+ allD + spaces + end;
            row += delimiter+" " + elem + " ";
        }
        row += delimiter;

        if(i == 0){
            //creating the border
            int spaces = row.size()-((delimiter.size()-symbol.size())*2*allData.size()+delimiter.size()-symbol.size());
            border += colorStart+std::string(spaces, '-')+colorEnd;
            table += border+"\r\n"+row+"\r\n"+border+"\r\n";
        }else{
            table += row+"\r\n";
        }
    }

    return table+border+"\r\n";
}

//get user choice
int Global::getChoice(int options){

    //checking the choice
    int choice=0;
    std::string input ="";
    while(true){
        std::cout << "Enter a choice here :> ";
        input = cinln();
        if(input != "0" && input != "00"){
            for(int i=1; i<=options; i++){
                if(input == std::to_string(i)){
                    return i;
                }
            }
            println("WRONG SELECTION! Try again.", "yellow"); 
        }else if(input == "0"){ return 0; }
        else { return -1; }
    }
    return choice;
}

int Global::navChoice(std::vector<std::string> options, int minimum){
    //displaying options
    std::cout << navOptions(options, minimum) << std::endl;
    //getting the choice
    return getChoice(options.size());
}