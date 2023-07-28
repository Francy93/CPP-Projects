#include "musicShop.hpp"
#include "util.cpp"

#include <regex>    // regular expressions

using namespace std;

//Making the dataBase object/truct
//Util util;


//starting class Categories

string Categories::idCalc(int cat){
    int number = util.SessionDB[cat].size();  //reading the file DataBase
    catIndex = cat;
    return util.IntToString(number);
}
void Categories::dataToVec(){ //storing data to vector only after complete collection of data
    if(price != "" && amount >= 0 && name != ""){
        vector<string> newItem({id, name, price, util.IntToString(amount), date});
        bool found = util.getById(id)[0] == id? true : false;
        int index = util.getIndex(catIndex, id);

        switch(catIndex){
            case 0: 
                if(found){
                    util.DVDs[index] = newItem;
                }else{ util.DVDs.push_back(newItem); }
                util.saved = false;
                //util.updater(); // auto updating the database
                break;
            case 1:  
                if(found){
                    util.CDs[index] = newItem;
                }else{ util.CDs.push_back(newItem); }
                util.saved = false;
                break;
            case 2:  
                if(found){
                    util.Magazines[index] = newItem;
                }else{ util.Magazines.push_back(newItem); }
                util.saved = false;
                break;
            case 3: if(found){
                    util.Books[index] = newItem;
                }else{ util.Books.push_back(newItem); }
                util.saved = false;
                break;
        }
    }
}

void Categories::setName(std::string n) { name = n;   dataToVec(); }
void Categories::setPrice(std::string p){ price = p;  dataToVec(); }
void Categories::setAmount(int a)       { amount = a; dataToVec(); }

string Categories::getID(){ return id; }

string Categories::getName(){ 
    if (name == ""){
        name = util.getById(id)[1];
    }
    return name;
}
string Categories::getPrice(){ 
    if (price == ""){
        price = util.getById(id)[2];
    }
    return price;
}
int Categories::getAmount(){
    if (amount == -1){
        amount = stoi(util.getById(id)[3]);
    }
    return amount; 
}

void Categories::propSetter(){
    getName();
    getPrice();
    getAmount();
}

//ending class Categories


//starting class Logistic

int Logistic::chooseCat(bool IorO){
    int nav = 0;
    cout << "Choose category by entering \"DVD\", \"CD\", \"Magazines\", or \"Books\"" << endl;
    cout << "Alternatively enter \"back\" to abort or \"exit\" to close the program.\r\n" << endl;

    while(nav == 0){
        string choice; cin >> choice;
        if     (choice == "exit"     ){ nav = -2; break; }
        else if(choice == "back"     ){ nav = -1; break; }
        else if(choice == "DVD"      ){ nav =  0;  util.print(nav);  break; }
        else if(choice == "CD"       ){ nav =  1;  util.print(nav);  break; }
        else if(choice == "Magazines"){ nav =  2;  util.print(nav);  break; }
        else if(choice == "Books"    ){ nav =  3;  util.print(nav);  break; }
        else {  cout << "Choice not contemplated" << endl; }
    }
    return nav;
}

bool Logistic::amountEditing(std::string id, int num){
    bool success = true;

    int result = stoi(util.getById(id)[3]) + num; //summing the value entered
    if     (id.at(0) == 'D' || id.at(0) == '0'){ DVD cat(id);      cat.setAmount(result); }
    else if(id.at(0) == 'C' || id.at(0) == '1'){ CD cat(id);       cat.setAmount(result); }
    else if(id.at(0) == 'M' || id.at(0) == '2'){ Magazine cat(id); cat.setAmount(result); }
    else if(id.at(0) == 'B' || id.at(0) == '3'){ Book cat(id);     cat.setAmount(result); }
    else{ cout << "System error! Logistic::restock(): Wrong ID format\r\n" << endl; success = false; }
    return success;
}

bool Logistic::update(){
    //function to update
    util.updater();

    cout << "UPDATED!" << endl;
    return false;
}

bool Logistic::restock(bool exit){
    bool back = false;
    cout << "RESTOCKING!\r\n"<< endl;
    
    int nav = chooseCat(false);  //this will print the product list
    if(nav == -2){ exit = true; }
    else if(nav == -1){ back = true; }
    else{
        cout << "\r\nNow enter an ID followed by the amount to be summed like \"-2\", \"5\" up to 2147483647"<< endl;
        cout << "Alternatively enter \"back\" to abort or \"exit\" to close the program.\r\n" << endl;
    }
    
    while(!exit && !back){
        string id; cin >> id;
        if(id == "exit"){
            exit = true;
            break;
        }else if(id == "back"){
            back = true;
            exit = restock(exit);
            break;
        }else try{
            string amount; cin >> amount;
            int num = stoi( amount );
            vector<string> detailes({util.getById(id)});
            
            if(detailes[0] == ""){
                cout << "Wrong ID. Try again!\r\n" << endl;
            }else if(detailes[0].at(0) != catChar[nav]){
                cout << "Wrong category. Try again!\r\n" << endl;
            }else if(amountEditing(id, num)){
                // function to summ restock amount
                cout << "Stock updated!" << endl;
                cout << "Restoking one more product? Enter \"yes\" or \"no\".\r\n" << endl;
            
                while(true){
                    string choice; cin >> choice;
                    if(choice == "yes"){ exit = restock(exit);  break; }
                    else if(choice == "no"){ break; }
                    else if(choice == "exit"){ exit = true;  break; }
                    else { cout << "Choice not contemplated. Try again!" << endl; }
                }
                break;
            }
        }catch (...) {
            cout << "Wrong amount. Try again!\r\n" << endl;
        }
    }
    return exit;
}

bool Logistic::newItem(bool exit){
    bool back = false;
    cout << "ADDING A NEW PRODUCT\r\n" << endl;

    int amount;
    string price;
    string name;

    int nav = chooseCat(true); //this will print the product list
    if(nav == -2){ exit = true; }
    else if(nav == -1){ back = true; }
    else{
        cout << "Please, enter a Name, a Price and a Quantity (e.g \"name\" \"10.90\" \"30\")" << endl;
        cout << "Alternatively enter \"back\" to abort or \"exit\" to close the program.\r\n" << endl;
        
        regex price_expr("-?[0-9]+([.][0-9]+)?"); //price regular expression validation
        regex amount_expr("-?[0-9]+([0-9]+)?");   //amount regular expression validation
        while(nav != -1 || nav != -2){
            string amInt;
            cin >> name;
            if     (name == "exit"){ nav = -2;  exit = true;  break; }
            else if(name == "back"){ nav = -1; break; }
            cin >> price;
            cin >> amInt;
            if(regex_match(price, price_expr) && regex_match(amInt, amount_expr)){
                amount = stoi(amInt);
                price += "$"; 
                break;
            }else{
                cout << "Wrong input! Make sure \"price\" and \"amount\" are numbers." << endl;
                cout << "Try again!\r\n" << endl;
            }
        }
    }

    DVD dvd;
    CD cd;
    Magazine magazine;
    Book book;

    switch(nav){
        case -2: exit = true;
            break;
        case -1:
            break;
        case  0: 
            dvd.setName(name);
            dvd.setPrice(price);
            dvd.setAmount(amount);
            cout << "Item successfully added!\r\n" << endl;
            back = true;
            break; 
        case  1: 
            cd.setName(name);
            cd.setPrice(price);
            cd.setAmount(amount);
            cout << "Item successfully added!\r\n" << endl;
            back = true;
            break;
        case  2: 
            magazine.setName(name);
            magazine.setPrice(price);
            magazine.setAmount(amount);
            cout << "Item successfully added!\r\n" << endl;
            back = true;
            break;
        case  3:
            book.setName(name);
            book.setPrice(price);
            book.setAmount(amount);
            cout << "Item successfully added!\r\n" << endl;
            back = true;
            break;
        default: cout << "System error: Wrong output from chooseCat()\r\n" << endl;
            break;
    }

    if(!back && !exit){ exit = newItem(exit); }
    return exit;
}

bool Logistic::sale(bool exit){
    bool back = false;
    cout << "SELLING\r\n";

    int nav = chooseCat(false);
    if(nav == -2){ exit = true; }
    else if(nav == -1){ back = true; }
    
    while(!exit && !back){
        cout << "Now enter the ID of the product to be sold and the quantity.\r\n" << endl;
        string id; cin >> id;

        if(id == "exit"){ exit = true; break; }
        else if(id == "back"){ break; }
        else if(util.getById(id)[0] == ""){ cout << "Wrong ID. Try again!\r\n" << endl; }
        else if(util.getById(id)[0].at(0) != catChar[nav]){
                cout << "Wrong category. Try again!\r\n" << endl;
        }else try{ 
            int qty; cin >> qty;
            //this function below represent the stock updating
            if(amountEditing(id, -qty)){
                util.reporting(id, qty, user);  //creating a report line
                cout << "Successfully sold. Thanks for shopping!\r\n" << endl;
                break;
            }
        }catch(...){ cout << "Wrong input, not a number. Try again!\r\n" << endl; }

    }
    if(!back && !exit){ exit = sale(exit); }
    return exit;    
}

bool Logistic::report(){
    bool exit = false;
    bool back = false;
    cout << "REPORT LIST" << endl;
    cout << "Enter \"back\" to abort or \"exit\" to close the program.\r\n" << endl;
    //function displaying the report list           
    if(!util.reportDisplay()){
        cout << "No report to be shown!\r\n" << endl;
    }

    while(!exit){
        cout << "\r\nEnter \"back\" to abort or \"exit\" to close the program.\r\n" << endl;
        string choice; cin >> choice;

        if(choice == "back"){ break; }
        else if(choice == "exit"){ exit = true; }
        else { cout << "Choice not contemplated" << endl; }
    }

    return exit;    
}

struct User {
    
    protected:
        Logistic logistic;

    public:
        public:
        User(){
            logistic.user = "Employee";
        }
        virtual bool privileges(){ return false; }
        virtual string text(){ return "Sale,\r\nRestock,\r\n"; }
        
        virtual bool selection(std::string choice){
            bool exit = false;

            if(choice == "Sale"){
                exit =logistic.sale(exit);
            }else if (choice == "Restock"){
                exit =logistic.restock(exit);
            }else if (choice == "NewItem" || choice == "Update" || choice == "Report"){
                cout << "Access denied. User not allowed!\r\n" << endl;
            }else { cout << "Error user: wrong operation choice!\r\n" << endl; }

            return exit;
        }
};

struct SuperUser : User{
        
    public:
        SuperUser(){
            logistic.user = "Manager";
        }
        bool privileges() override { return true; }
        string text() override { 
            return "Sale,\r\nRestock,\r\nNewItem,\r\nUpdate,\r\nReport.\r\n";
        }
        
        bool selection(std::string choice) override {
            bool exit = false;

            if(choice == "Sale"){
                exit = logistic.sale(exit);
            }else if (choice == "Restock"){
                exit = logistic.restock(exit);
            }else if(choice == "NewItem"){
                exit = logistic.newItem(exit);
            }else if (choice == "Update"){
                exit = logistic.update();
            }else if (choice == "Report"){
                exit = logistic.report();
            }else { cout << "Error sudo: wrong operation choice!\r\n"; }

            return exit;
        }
};