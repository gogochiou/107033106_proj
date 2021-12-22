#include "./Manager.h"

Manager::Manager(){
    cout<< "setting Manager !"<<endl;
    readfile();

}

void Manager::readfile(){
    //map read file
    // map = new Map();

    /* 2.station read file */
    //relative path is decided by where(path in terminal) you execute your code 
    ifstream s_file("./test_case/station.txt",ifstream::in);
    string s;
    string delimiter=" ";
    string token[4];
    size_t pos=0;
    cout << "readfile..." << endl;
    if (s_file.is_open())
        cout << "open file"<<endl;
    else
        cout << "failed to open file"<<endl;

    /* 2-1.get last line to know how many station */
    s_file.seekg(-1,ios_base::end); // go to one spot before EOF
    bool keepLooking = true;
    while(keepLooking){
        char ch;
        s_file.get(ch);
        if((int)s_file.tellg() <= 1) {             // If the data was at or before the 0th byte
            s_file.seekg(0);                       // The first line is the last line
            keepLooking = false;                   // So stop there
        }
        else if (ch == '\n'){
            keepLooking = false;
        }
        else
            s_file.seekg(-2, ios_base::cur);
    }
    getline(s_file, s);
    if( (pos = s.find(delimiter))!=string::npos){
        token[0] = s.substr(0, pos);
        stationList = new Station[stoi(token[0])];//init station list
    }
    s_file.clear(); // important to let us read file again
    s_file.seekg(0, ios::beg); // make cursor back to begin

    while (getline(s_file, s)){
        for(int i=0; i<4; i++){ //each line, we need read in 4 data
            if ((pos = s.find(delimiter))!=string::npos){
                token[i] = s.substr(0, pos);
                s.erase(0, pos+delimiter.size());
            }
            else
                token[i] = s;//final words
        }
        stationList[stoi(token[0])-1].Init(token); // initial each station
    }
    s_file.close();
}