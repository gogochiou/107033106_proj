#include "./Manager.h"

Manager::Manager(){
    cout<< "setting Manager !"<<endl;
    /* init param */
    n_revenue = 0;
    a_revenue = 0;
    s_quantity = 0;

    /* init class */
    readfile();

}

void Manager::readfile(){
    /* 1.station read file */
    //relative path is decided by where(path in terminal) you execute your code 
    ifstream s_file("./test_case/station.txt",ifstream::in);
    string s;
    string delimiter=" ";
    string token[4];
    size_t pos=0;
    if (s_file.is_open())
        cout << "open station file"<<endl;
    else
        cout << "failed to open station file"<<endl;

    /* 1-1.get last line to know how many station */
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
        s_quantity = stoi(s.substr(0, pos));
        stationList = new Station[s_quantity];//init station list
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
    cout << endl;

    /* 2.map read file */
    map = new Map(s_quantity);
    map->show();
    cout << endl;

    /* 3.read fee */
    ifstream f_file("./test_case/fee.txt",ifstream::in);
    if (f_file.is_open())
        cout << "open fee file"<<endl;
    else
        cout << "failed to open fee file"<<endl;

    writeFee(fee.elec, f_file);
    writeFee(fee.lady, f_file);
    writeFee(fee.road, f_file);
    getline(f_file, s);
    fee.waiting = stod(s);
    getline(f_file, s);
    fee.switching = stod(s);
    getline(f_file, s);
    fee.transferring = stod(s);
    f_file.close();
    // cout << fee.waiting << " " << fee.switching << " " << fee.transferring<< endl;

    /* 4.setting user */
    user_list = new userList();
    // user_list->showInfo(99);
}

void Manager::writeFee(Rate& rate, ifstream& file){
    string s;
    string delimiter=" ";
    size_t pos=0;
    getline(file, s);
    pos = s.find(delimiter);
    s.erase(0, pos+delimiter.size());
    pos = s.find(delimiter);
    rate.discount = stoi(s.substr(0, pos));
    s.erase(0, pos+delimiter.size());
    rate.regular = stoi(s);
    // cout << rate.discount << " " << rate.regular << endl;
}

void Manager::NormalPolicy(){
    ifstream input_file("./test_case/user.txt",ifstream::in);
    normal_res.open("./part1_response.txt",ios::out|ios::trunc); //trunc clear exist text in file

    string input;
    string delimiter=" ";
    string token[5];
    size_t pos=0;
    

    while(getline(input_file, input)){
        normal_res << input << endl;

        /* seperate data */
        int in_count=0;
        while((pos = input.find(delimiter))!=string::npos){
            token[in_count] = input.substr(0, pos);
            input.erase(0, pos+delimiter.size());
            in_count++;
        }
        token[in_count] = input;
        in_count++;

        int s_num = stoi(token[1]); //rent or return station number
        if(token[0]=="rent"){
            // check if have bike
            cout << "Rent by " << stoi(token[3])<<endl;
            if(stationList[s_num-1].if_haveBike(token[2])){
                normal_res<< "accept" << endl;
                cout << "type : "<<token[2]<<endl;
                int rent_bikeID = stationList[s_num-1].rent_bike(token[2]);
                int userID = stoi(token[3]);
                user_list->giveResponse(userID, token[2], 
                                        rent_bikeID, 0);
            }
            else
                normal_res << "reject" << endl;
        }
        else if(token[0]=="return"){
            cout << "Return by " << stoi(token[2])<<endl;
            int userID = stoi(token[2]);
            string b_type = user_list->returnBikeType(userID);
            int b_id = user_list->returnBikeID(userID);
            stationList[s_num-1].return_bike(b_type, b_id);
            // calculateRevenue(userID, 0);
        }
        cout << " mission complete "<<endl;
    }
    input_file.close();
    normal_res.close();
    cout <<"normal policy revenue : "<< n_revenue << endl;
    
}

void Manager::calculateRevenue(int userID, int waiting_time){
    double fee_rate = 0.0;
    /* check if use regular or discount */
    int riding_time = user_list->returnRideTime(userID);
    int* SandE = user_list->returnSandE(userID);//start and end station num
    bool use_regular = true;
    if( riding_time == map->get_minTime(SandE[0],SandE[1]) )
        use_regular = false;

    /* check whick bike type and fee */
    string b_type = user_list->returnBikeType(userID);
    if(b_type == "electric"){
        if(use_regular)
            fee_rate = fee.elec.regular;
        else
            fee_rate = fee.elec.discount;
    }
    else if(b_type == "lady"){
        if(use_regular)
            fee_rate = fee.lady.regular;
        else
            fee_rate = fee.lady.discount;
    }
    else if(b_type == "road"){
        if(use_regular)
            fee_rate = fee.road.regular;
        else
            fee_rate = fee.road.discount;
    }

    /* calculate */
    n_revenue = n_revenue + fee_rate*riding_time - waiting_time*fee.waiting;

}