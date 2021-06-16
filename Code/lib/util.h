#ifndef _UTIL_H_
#define _UTIL_H_

/**
 * @file util.h
 * @author Francy (fa1033@live.mdx.ac.uk)
 * @brief 
 * @version 0.1
 * @date 2021-04-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <string>
#include <vector>
#include <deque>     // for the tableMaker() method
#include <algorithm> // "trasform" in toLower()
//#include <cctype>  // this library is already included in "<algorithm>"


class Util{

    private:
        inline static bool colorMode = true;
        // std color text mode
        const int systemColor = system("");

    public:
        // turning on off methods color
        static void setColor(bool b){ colorMode = b; }
        // getting the state of the class color
        static bool getColorState(){ return colorMode; }


        /**
         * @brief making strings lower case
         * 
         * @param s 
         * @return std::string 
         */
        static std::string toLower(std::string s){
            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c){ return std::tolower(c); });
            
            return s;
        }

        /**
         * @brief this is a string printer
         * 
         * @param args 
         */
        static void printLn(std::vector<std::string> args){            
            std::string start = "", end = "";

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
            for(unsigned int i=0; i< args.size(); i++) print +=args[i];
            
            if(!getColorState()) start = end = "";
            std::cout << start+print+end << std::endl;
        }

        
        //allowing polimorphic inputs
        template <typename T>
        static typename std::enable_if<false == std::is_convertible<T, std::string>::value,
            std::string>::type toStr (T const & val){ return std::to_string(val); }
        static std::string toStr (std::string const & val) { return val; }
        static std::string toStr (char const & val) { return std::string(1,val); }
        //this is a function to get template inputs
        template <typename... Ts>
        static void println(Ts const & ... vals){
            using unused = int[];
            std::vector<std::string> args;
            (void)unused { 0, (args.push_back(toStr(vals)), 0)... };
            printLn(args);
        }


        /**
         * @brief getting color ANSI escape code
         * 
         * @param c 
         * @return std::string 
         */
        static std::string color(std::string c){
            std::string colors [] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
            std::string color = "";
            if(getColorState()){
                for (unsigned int i=0; i< sizeof(colors)/sizeof(*colors); i++){
                    if(toLower(c) == colors[i]) color = "\033[1;3"+std::to_string(i)+"m";
                }
            }
            return color;
        }

        // setting back the standard terminal color
        static std::string colorReset(){
            std::string reset = getColorState()? "\033[0m": "";
            return reset;
        }


        /**
         * @brief check wether a string is numeric
         * 
         * @param str 
         * @return true 
         * @return false 
         */
        static bool isNumber(const std::string str){
            bool symbol = str.size() == 0;

            for (char const &c : str) {
                if (std::isdigit(c) == 0){
                    if(!symbol && (c == '-' || c == '+')) symbol = true;
                    else return false;
                }else symbol = false;
            }

            return !symbol;
        }

        
        /**
         * @brief loading bar
         * 
         * @param size 
         * @param index 
         * @param barLength (facultative)
         * @param updates (facultative)
         * @return std::string 
         */
        static void loading(const unsigned long long size, const unsigned long long index){ loadingBar(size,index,0,0); }
        static void loading(const unsigned long long size, const unsigned long long index ,unsigned short barLength){ loadingBar(size,index,barLength,0); }
        static void loading(const unsigned long long size, const unsigned long long index ,unsigned short barLength, unsigned short updates){ loadingBar(size,index,barLength,updates); }
        static void loadingBar(const unsigned long long size, const unsigned long long index, unsigned short barLength, unsigned short updates){

            if (size > 0 && index <= size){
                //bar standard parameters
                barLength   = barLength > 0? barLength: 50;
                updates     = updates > 0? updates: 100;

                //calculating loading bar
                static short i  = -1;
                updates = updates > 99? 100: updates < 1? 0: updates;
                const unsigned short barPercent   = (unsigned short)(index * updates / size);
                const unsigned short tokens       = (unsigned short)((float)barLength / updates * barPercent);
                
                if(tokens != i){
                    // "i" determines when to print the status bar
                    i = tokens;
                    const unsigned short percent = (unsigned short)(index * 100 / size);
                    std::string block,dotted;
                    #if defined(_WIN32)
                        block   = std::string(1,(char)219), dotted  = std::string(1,(char)176);
                    #else
                        block   = "█",dotted = "░";
                    #endif
                    
                    if(percent != 100 && size > index){
                        std::string statusFull = "", statusVoid = "", colors; //219 is the ascii code for the square symbol
                        for(unsigned long j=0;j<tokens;j++) statusFull += block;
                        for(unsigned long j=0;j<barLength-(unsigned long)tokens;j++) statusVoid += dotted;
                        if      (percent < 33)  colors = "red";
                        else if (percent < 66)  colors = "yellow";
                        else                    colors = "green";
                        
                        std::string outcome = color(colors)+statusFull+colorReset()+statusVoid+" "+color(colors)+std::to_string(percent)+"%"+colorReset()+"\r";
                        std::cout << std::flush << outcome << std::flush;
                    }else {
                        i = -1;
                        std::cout << "\33[2K" << std::flush;
                    }
                }
            }
        }


        /**
         * @brief string to double
         * 
         * @param s 
         * @return double
         */
        static double sTod(const std::string str){

            //grouping digits
            std::string tempNum = "";
            std::vector<std::string>strNum;
            for(unsigned int i=0; i< str.size(); i++){
                char s = str.at(i);

                if(i < str.size()-1 && (s == '+' || s == '-' || s == '*' || s == '/')){
                    if(tempNum != ""){ 
                        strNum.push_back(tempNum); 
                        tempNum ="";
                    }
                    strNum.push_back(std::string(1, s));
                }else if(std::isdigit(s)){
                    tempNum += std::string(1, s);
                    if(i == str.size()-1) strNum.push_back(tempNum);
                }
            }

            //grouping numbers with their symbols
            std::vector<std::string>strNumSign;
            for(unsigned int i=0; i< strNum.size(); i++){
                std::string sn = strNum.at(i);

                if(i < str.size()-1 && (sn == "+" || sn == "-")){
                    strNumSign.push_back(sn + strNum.at(i+1));
                    i++;
                }else if(i > 0 && i < str.size()-1 && (sn == "*" || sn == "/")){
                    strNumSign.push_back(sn);
                }else if(sn != "*" && sn != "/"){
                    std::string symbol = sn.at(0) == '-' || sn.at(0) == '+'? "": "+";
                    strNumSign.push_back(symbol+sn);
                }
            }

            //summing numbers with given operators
            double sum = 0;
            tempNum = "";
            for(unsigned int i=0; i< strNumSign.size(); i++){
                std::string sns = strNumSign.at(i);  
                static std::string symbol = "";

                if(i < str.size()-1 && (sns == "+" || sns == "-"))      symbol = sns;
                else if(i < strNumSign.size()-1 && (strNumSign.at(i+1) == "*" || strNumSign.at(i+1) == "/")){
                    if(std::isdigit(sns.at(0)) || sns.at(0) == '+' || sns.at(0) == '-'){
                        tempNum = strNumSign.at(i+1) + sns;
                        i++;
                    }
                }else if((sns.at(0) != '*' && sns.at(0) != '/') || (i == strNumSign.size()-1 && (sns.at(0) == '*' || sns.at(0) == '/'))){
                    
                    if(i == strNumSign.size()-1 && (sns.at(0) == '*' || sns.at(0) == '/')){
                        sns = tempNum.substr(1, tempNum.size());
                        tempNum = "";
                    }
                    if(tempNum != ""){
                        if(tempNum.at(0) == '*')    sum += std::stod(tempNum.substr(1, tempNum.size())) * std::stod(sns);
                        else                        sum += std::stod(tempNum.substr(1, tempNum.size())) / std::stod(sns);
                    }else {
                        sum += std::stod(symbol + std::to_string(std::stod(sns)));
                        symbol = "";
                    }
                }
            }
            
            return sum;
        }


        /**
         * @brief the strings splitter
         * 
         * @param s 
         * @param delim 
         * @return std::vector<std::string> 
         */
        static std::vector<std::string> split(std::string s, std::string delim){
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
            if(s.size() > 0) vec.push_back(s);
            return vec;
        }

        /**
         * @brief an enanched and actually working cin
         * 
         * @return std::string 
         */
        static std::string cinln(){
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
        static std::string tableMaker(std::deque<std::deque<std::string>> &allData, std::vector<unsigned long> longest){
            // style parameters
            const short maxLength         = 100;              // maximum of characters allowed per string
            const std::string columnDelim = "|";              // columns delimiter
            const char rowDelim           = '-';              // rows delimiter
            // color paramenters
            const std::string colorStart  = color("magenta"); // the table frame color
            const std::string colorEnd    = colorReset();     // resetting the color after use


            //setting the max length of the rows
            for(unsigned int i = 0; i< longest.size(); i++){
                longest[i] = longest[i] > maxLength? maxLength: longest[i];
            }
            // getting the size of the longest column
            unsigned long long longestColumn = 0;
            for(unsigned int i = 0; i< allData.size(); i++){
                longestColumn = allData[i].size() > longestColumn? allData[i].size(): longestColumn;
            }
                
            std::string table = "", border = "";
            for(unsigned long long i=0; i<longestColumn; i++){
                //creating the row
                std::string delimiter =colorStart+columnDelim+colorEnd;
                std::string row = "";
                
                //cycling over the columns
                for(unsigned int j=0; j<allData.size(); j++){

                    std::string str = allData[j].size() > i? allData[j][i]: " ";
                    if(str.size() > maxLength) str = str.substr(0, maxLength-3)+"...";
                    const unsigned long strSize = str.size();

                    const unsigned long leng = longest[j] - strSize < 1? 0: longest[j] - strSize;
                    std::string elem ="";
                    std::string spaces = leng > 0? std::string(leng, ' '): "";

                    std::string start = "",end   = "";
                    if(i == 0 || j == 0){
                        start = color("yellow");
                        if(i == 0) start = color("cyan");
                        end   = colorEnd;
                    }
                    elem += start+ str + spaces + end;
                    row += delimiter+" " + elem + " ";
                }
                row += delimiter;

                if(i == 0){
                    //creating the border
                    const unsigned long spaces = row.size()-((delimiter.size()-columnDelim.size())*2*allData.size()+delimiter.size()-columnDelim.size());
                    border += colorStart+std::string(spaces, rowDelim)+colorEnd;
                    table += border+"\r\n"+row+"\r\n"+border+"\r\n";
                }else table += row+"\r\n";
            }

            return table+border+"\r\n";
        }


        /**
         * @brief returns a string of numerate options
         * 
         * @param options 
         * @param minimum 
         * @return std::string 
         */
        static std::string navOptions(std::vector<std::string> options, const unsigned long minimum){
            std::string list="";

            const unsigned long oSize = options.size();
            unsigned long iSize = 2, i = 0, longest = 7;

            for(std::string o: options){
                const unsigned long strSize = o.size();
                longest = strSize > longest? strSize: longest;
                if(++i == oSize){
                    iSize = std::to_string(i).size() > iSize? std::to_string(i).size(): iSize;
                }
            }
            
            std::string cStart = color("yellow"); //yellow corresponds to: "\033[1;35m"
            std::string cEnd = colorReset();      //reset  corresponds to: "\033[0m"

            longest += minimum > 3? minimum: 3;
            i = 0;
            for(auto o: options){
                const unsigned long currentSize = iSize - std::to_string(i).size();
                const unsigned long dotSize = longest - o.size();
                std::string index = cStart + std::to_string(++i) + cEnd;
                
                list += o + std::string(dotSize+currentSize,'.') + index + "\r\n";
            }

            list += "Go back" + std::string(longest-7+iSize-1,'.') + cStart+"0"+cEnd  + "\r\n";
            list += "Exit"    + std::string(longest-4+iSize-2,'.') + cStart+"00"+cEnd + "\r\n";
            return list;
        }


        /**
         * @brief get user choice
         * 
         * @param options 
         * @return int 
         */
        static long long getChoice(const unsigned long options){

            //checking the choice
            unsigned long choice=0;
            std::string input ="";
            while(true){
                std::cout << "Enter a choice here :> ";
                input = cinln();
                if(input != "0" && input != "00"){
                    for(unsigned long i=1; i<=options; i++){
                        if(input == std::to_string(i)) return (long long)i;
                    }
                    println("WRONG SELECTION! Try again.", "yellow"); 
                }else if(input == "0") return 0;
                else return -1;
            }
            return (long long)choice;
        }


        /**
         * @brief display options and return choice
         * 
         * @param options 
         * @param minimum 
         * @return int 
         */
        static long long navChoice(std::vector<std::string> options, unsigned long minimum){
            //displaying options
            std::cout << navOptions(options, minimum) << std::endl;
            //getting the choice
            return getChoice(options.size());
        }

                
        /**
         * @brief extracting digits and symbols from a string
         * 
         * @param std::string str 
         * @return std::string 
         */
        static std::string numFilter(std::string str){
            std::string numStr = "";
            bool symbol = false;

            for(unsigned int i=0; i < str.size(); i++){
                const char c = str.at(i);

                if(std::isdigit(c)){ 
                    numStr += std::string(1, c);
                    symbol = false;
                }else if(!symbol && i<str.size()-1 && (c == '-' || c == '+')){
                    numStr += std::string(1, c);
                    symbol = true;
                }else if(!symbol && i>0 && i<str.size()-1 && (c == '*' || c == '/')){
                    numStr += std::string(1, c);
                    symbol = true;
                }else if(!symbol && i>0 && i<str.size()-1 && c == '.'){
                    numStr += std::string(1, c);
                    symbol = true;
                }
            }

            return numStr;
        }
};

#endif