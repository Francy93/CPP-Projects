
#include <string>   // strings library
#include <iostream> // general library
#include <sstream>  // library for intoToString converter
#include <ctime>    // library for data and time
#include <vector>   // vectors library
#include <fstream>  // file text scanner
#include <algorithm>// std::find
#include <cctype>   // remove white spaces

using namespace std;


class Util {

    public:
        bool saved = true;
        //categories matrix
        vector<vector<string>> DVDs;
        vector<vector<string>> CDs;
        vector<vector<string>> Magazines;
        vector<vector<string>> Books;
        
        vector<vector<string>> SessionDB;
        vector<string> reportVec;

    private:
        string delimiter = " :: ";
        string fname = "DataBase.txt";

        //file reader
        vector<string> importer(std::string cat){
            vector<string> DB;
            
            ifstream file(fname);
            
            bool newOpen = false;
            while(true){
                if (file.is_open() || newOpen) {
                    string line;
                    while (std::getline(file, line)) {
                        line.c_str();
                        if(line != "" && line != "1" && line.at(0) == cat.at(0)){
                            DB.push_back(line);
                        }
                    }
                    file.close();
                    break;
                }else if(file.fail()){
                    cout << "DataBase missing. Creating one..." << endl;
                    fstream file;
                    file.open(fname, std::ios::out);
                    file << fflush;
                    file.close();
                    file.open(fname, std::ios::in);
                    if (file.is_open()) { cout << "Created!\r\n\r\n\r\n"; newOpen = true; }
                }
            }
            return DB;
        }

        //data exporter
        void exporter(){
            vector<vector<vector<string>>> all({DVDs, CDs, Magazines, Books});
            vector<int> caracters(5);
            int leng;
            bool mode = false;

            fstream file;
            file.open(fname, std::ios::out);

            //writing cicle
            while(true){
                for(vector<vector<string>> cat: all){
                    for(vector<string> item: cat){ int i = -1;
                        for(string data: item){        i ++;
                            //removing white spaces
                            data.erase(std::remove_if(data.begin(), data.end(), ::isspace), data.end());
                            leng = data.length();

                            if(!mode){
                                caracters[i] = caracters[i] < leng ? leng : caracters[i];
                            }else{
                                string spaces = "";
                                for(int j=0; j<caracters[i]-leng ;j++){ spaces += " "; }
                                if(data != item[(item.size())-1]){ data += spaces + delimiter; }
                                file << data;
                            }
                        }
                        if(mode){ file << endl; }
                    }
                }
                if (mode){ break; } //exiting main loop
                mode = true;
            }
            //closing the file
            file.close();
            saved = true;
        }

        //importing data from HardDisk
        vector<vector<string>> dbQuery(bool InOut){
            
            vector<vector<string>> categories;
            vector<string> cat = {"D", "C", "M", "B"};

            if(!InOut){   
                for(string c: cat){
                    categories.push_back(importer(c));
                }
            }else{ exporter(); }

            return categories;
        }

    public:
        bool reportDisplay(){
            bool full = false;

            for(string line: reportVec){
                full = true;
                cout << line << endl;
            }

            return full;
        }

        string reporting(std::string id, int qty, std::string user){
            string line;
            for(int i=0 ; i< getById(id).size() ; i++){
                switch(i){
                    case 0: line += id + delimiter;
                        break;
                    case 3: line += IntToString(qty) + delimiter;
                            line += user + delimiter;
                        break;
                    default: 
                        if(getById(id).size()-1 > i) { line += getById(id)[i] + delimiter; }
                        else{ line += getById(id)[i]; }
                        break;
                }
            }
            reportVec.push_back(line);
            return line;
        }

        //the holly strings splitter *.*
        vector<string> split(std::string s, std::string delim){
            vector<string> vec;
            
            size_t pos = 0;
            string token;
            while ((pos = s.find(delim)) != string::npos) {
                token = s.substr(0, pos);
                vec.push_back(token);
                s.erase(0, pos + delim.length());
            }
            vec.push_back(s);
            return vec;
        }

    private:
        //fetching imported data, from sessionDB (ram) and splitting into categories
        vector<vector<string>> catSplit(std::string category){
            vector<vector<string>> products;

            if(category == "D"){
                vector<string> DVDs ({SessionDB[0]});
                for (string item: DVDs){
                    products.push_back(split(item, delimiter));
                }
            }else if(category == "C"){
                vector<string> CDs ({SessionDB[1]});
                for (string item: CDs){
                    products.push_back(split(item, delimiter));
                }
            }else if(category == "M"){
                vector<string> Magazines ({SessionDB[2]});
                for (string item: Magazines){
                    products.push_back(split(item, delimiter));
                }
            }else if(category == "B"){
                vector<string> Books ({SessionDB[3]});
                for (string item: Books){
                    products.push_back(split(item, delimiter));
                }
            }else{ cout << "System error! Util::catSplit: (Wrong category)" << endl; }

            return products;
        }
        
    public:
        //constructor method to initialize DataBase vectors
        Util(){ 
            SessionDB = dbQuery(false); //selfrunnin dbQuery in output mode
            DVDs      = catSplit("D");
            CDs       = catSplit("C");
            Magazines = catSplit("M");
            Books     = catSplit("B");
        }

        void updater(){
            dbQuery(true); //runnin dbQuery in input mode
            SessionDB = dbQuery(false); //runnin dbQuery in output mode
        }

        string IntToString(int a){
            ostringstream temp;
            temp << a;
            return temp.str();
        }

        //current date generator
        string dateGen(){
            time_t curr_time;
            curr_time = time(NULL);
            tm *tm_local = localtime(&curr_time);

            int H = tm_local->tm_hour;
            string h = IntToString(H);  //converting hour to string
            int MI = tm_local->tm_min;
            string mi = IntToString(MI);//converting minutes to string
            int S = tm_local->tm_sec;
            string s = IntToString(S);  //converting seconds to string
            
            int D = tm_local->tm_mday;
            string d = IntToString(D);  //converting day to string
            int MO = tm_local->tm_mon;
            string mo = IntToString(MO);//converting month to string
            int Y = tm_local->tm_year;
            string y = IntToString(Y);  //converting year to string
            mo += y.at(0);  string vs = ""; y = y.at(1) +vs+ y.at(2);

            string time = h+":"+mi+":"+s;
            string date = d+":"+mo+":"+y;

            return date+"/"+time;
        }

        vector<vector<string>> catSelector(std::string id){
            vector<vector<string>> cat({{"void"},{"void"}});

            if     (id.at(0) == 'D' || id.at(0) == '0'){ cat = DVDs;      }
            else if(id.at(0) == 'C' || id.at(0) == '1'){ cat = CDs;       }
            else if(id.at(0) == 'M' || id.at(0) == '2'){ cat = Magazines; }
            else if(id.at(0) == 'B' || id.at(0) == '3'){ cat = Books;     }
            else{ cout << "System error! Util::catSelector(): Wrong ID format\r\n" << endl; }

            return cat;
        }

        // Function to print the index of an element
        int getIndex(int cat, std::string k){
            int index = -1;
            vector<vector<string>> category ({catSelector(k)});

            for (int i = 0; i < category.size(); i++){
                if(category[i][0] == k){ index = i; }
            }

            return index;
        }

        vector<string> getById(std::string id){
            vector<vector<string>> cat({catSelector(id)});
            vector<string> data(5, "");

            for(vector<string> c: cat){
                if(c[0] == id){
                    data = c;
                }
            }
            return data;
        }

        bool print(int cat){
            bool success = true;
            if(cat > SessionDB.size()){
                cout << "System error! Util::print() wrong cateory.\r\n" << endl;
                success = false;
            }else{
                vector<vector<string>> category ({catSelector(IntToString(cat))});

                for (vector<string> product: category){
                    for (string details: product){
                        cout << details << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            return success;
        }
};
