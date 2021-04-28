#include "library.hpp"


// -----------------   global functions   ------------------

void Global::setColor(bool b){
    colorMode = b;
}
bool Global::getCstate(){
    return colorMode;
}


/**
 * @brief making strings lower case
 * 
 * @param s 
 * @return std::string 
 */
std::string Global::toLower(std::string s){
    #include <cctype>
    
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return s;
}

/**
 * @brief this is a string printer
 * 
 * @param args 
 */
void Global::printLn(std::vector<std::string> args){
    
    std::string start = "";
    std::string end = "";

    if(args.size() > 1){
        std::string colors [] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
        for (unsigned int i=0; i< sizeof(colors)/sizeof(*colors); i++){
            if(toLower(args.back()) == colors[i]){
                start = "\033[1;3"+std::to_string(i)+"m";
                end = "\033[0m";
                args.erase(args.end());
            }
        }
    }
    
    std::string print = "";
    for(unsigned int i=0; i< args.size(); i++){
        print +=args[i];
    }
    
    if(!getCstate()){
        start = end = "";
    }
    std::cout << start+print+end << std::endl;
}

/**
 * @brief getting color ANSI escape code
 * 
 * @param c 
 * @return std::string 
 */
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

/**
 * @brief check whether a string is numeric
 * 
 * @param str 
 * @return true 
 * @return false 
 */
bool Global::isNumber(const std::string str){
    bool symbol = true;
    for (char const &c : str) {
        if (std::isdigit(c) == 0){
            if(!symbol && (c == '-' || c == '+')){ symbol = true; }
            else{ return false; }
        }else if(symbol){ symbol = false; }
    }
    return !symbol;
}

/**
 * @brief loading bar
 * 
 * @param size 
 * @param index 
 * @return std::string 
 */
std::string Global::loading(unsigned long long size, unsigned long long index){
    if (size > 0){
        static int i = 0;
        int percent = index * 100 / size;
        int maxToken = percent/5;
        std::string colors;
        
        if(maxToken != i){
            i = maxToken;
        
            if(percent != 100 && size > index){
                std::string status = std::string(maxToken*2, (char)219); //219 is the ascii code for the square symbol
                if(percent < 33){ colors = "red"; }
                else if(percent < 66){ colors = "yellow"; }
                else{ colors = "green"; }
                
                return color(colors)+status+" "+std::to_string(percent)+"%"+colorReset()+"\r";
            }
            return "\33[2K";
        }
    }
    return "";
}

/**
 * @brief returns a string of numerate options
 * 
 * @param options 
 * @param minimum 
 * @return std::string 
 */
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

/**
 * @brief string to long long
 * 
 * @param s 
 * @return long long 
 */
long long Global::sToll(std::string s){
    //checking if string is a digit
    // sToll() method uses the regex library: #include <regex>
    if(std::regex_match (s, std::regex("[-|+]{0,1}[0-9]+") )){
        return stoull(s);
    }
    return 0;
}

/**
 * @brief the strings splitter
 * 
 * @param s 
 * @param delim 
 * @return std::vector<std::string> 
 */
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

/**
 * @brief an enanched and actually working cin
 * 
 * @return std::string 
 */
std::string Global::cinln(){
    std::string input;

    while(input.empty()){
        fflush(stdin);
        std::cin.clear();
        std::getline(std::cin, input);   
    }
    std::cout << "\n";
    fflush(stdin);
    std::cin.clear();

    return input;
}

/**
 * @brief table generator
 * 
 * @param allData 
 * @param longest 
 * @return std::string 
 */
std::string Global::tableMaker(std::deque<std::deque<std::string>> &allData, std::vector<unsigned int> longest){
    //maximum of characters allowed per string
    const short maxLength = 100;
    //setting the max length of the rows
    for(unsigned int i = 0; i< longest.size(); i++){
        longest[i] = longest[i] > maxLength? maxLength: longest[i];
    }
        
    std::string table = "";
    std::string border = "";
    for(unsigned long long i=0; i<allData[0].size(); i++){
        //creating the row
        std::string colorStart = color("magenta");
        std::string colorEnd = colorReset();

        std::string symbol = "|";
        std::string delimiter =colorStart+symbol+colorEnd;
        std::string row = "";
        
        //cycling over the columns
        for(unsigned int j=0; j<allData.size(); j++){

            std::string str = allData[j][i];
            if(str.size() > maxLength){ str = str.substr(0, maxLength-3)+"..."; }
            const short strSize = str.size();

            int leng = longest[j] - strSize < 1? 0: longest[j] - strSize;
            std::string elem ="";
            std::string spaces = leng > 0? std::string(leng, ' '): "";

            std::string start = "";
            std::string end   = "";
            if(i == 0 || j == 0){
                start = color("yellow");
                if(i == 0){ start = color("cyan"); }
                end   = colorEnd;
            }
            elem += start+ str + spaces + end;
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

/**
 * @brief get user choice
 * 
 * @param options 
 * @return int 
 */
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

/**
 * @brief display options and return choice
 * 
 * @param options 
 * @param minimum 
 * @return int 
 */
int Global::navChoice(std::vector<std::string> options, int minimum){
    //displaying options
    std::cout << navOptions(options, minimum) << std::endl;
    //getting the choice
    return getChoice(options.size());
}
