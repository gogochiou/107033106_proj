#include "./Manager.h"
#include <math.h>

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
    stationList = NULL;//init stationList as NULL
    readstation(false); 
    // cout << endl;

    /* 2.map read file */
    map = new Map(s_quantity);
    // map->show();
    // cout << endl;

    /* 3.read fee */
    ifstream f_file("./test_case/fee.txt",ifstream::in);
    string s;
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
    // user_list->showInfo(450);
}

void Manager::readstation(bool rebuild_){
    //relative path is decided by where(path in terminal) you execute your code 
    ifstream s_file("./test_case/station.txt",ifstream::in);
    string s;
    string delimiter=" ";
    size_t pos=0;
    string s_data[4]; // id, elec, lady, road
    if (s_file.is_open())
        cout << "open station file"<<endl;
    else
        cout << "failed to open station file"<<endl;

    /* 1-1.get last line to know how many station*/
    if(!rebuild_){
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
        }
        s_file.clear(); // important to let us read file again
        s_file.seekg(0, ios::beg); // make cursor back to begin
    }

    delete[] stationList;
    stationList = new Station[s_quantity];//init station list

    while(s_file >> s_data[0] >> s_data[1] >> s_data[2] >> s_data[3]){
        stationList[stoi(s_data[0])-1].Init(s_data);
    }
    
    s_file.close();

    /* old version of read file way */
    // while (getline(s_file, s)){
    //     for(int i=0; i<4; i++){ //each line, we need read in 4 data
    //         if ((pos = s.find(delimiter))!=string::npos){
    //             token[i] = s.substr(0, pos);
    //             s.erase(0, pos+delimiter.size());
    //         }
    //         else
    //             token[i] = s;//final words
    //     }
    //     stationList[stoi(token[0])-1].Init(token); // initial each station
    // }
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
            int userID = stoi(token[3]);
            if( stationList[s_num-1].if_haveBike(token[2])!=0 ){
                normal_res<< "accept" << endl;
                int rent_bikeID = stationList[s_num-1].rent_bike(token[2]);
                user_list->giveResponse(userID, "accept", token[2], 
                                        rent_bikeID, 0);
            }
            else{
                normal_res << "reject" << endl;
                user_list->giveResponse(userID, "accept", "no", -1, 0);
            }
        }
        else if(token[0]=="return"){
            int userID = stoi(token[2]);
            string b_type = user_list->returnBikeType(userID);
            int b_id = user_list->returnBikeID(userID);
            if(b_id == -1){
                /* cause b_type will be nothing if we did not borrow,
                   so return_bike will still refuse the rent command
                   in old version(without this conditional expressions) */
                cout << " We did not borrow bike to user " << b_type <<endl;
            }
            else{
                stationList[s_num-1].return_bike(b_type, b_id);
            }
            // stationList[s_num-1].show(b_type);
            calculateRevenue(userID, 0);
            // normal_res << "Current revenue : " << n_revenue <<endl;
        }
    }
    input_file.close();
    normal_res.close();

    status_output("Normal");
    cout <<"normal policy revenue : "<< n_revenue << endl;
}

void Manager::AdvancedPolicy(){
    /* 1.rebuild station */
    readstation(true);
    // cout << "------------------------------------------" <<endl;
    // for(int i=0; i<s_quantity; i++)
    //     stationList[i].show("all");
    // cout << "------------------------------------------" <<endl;

    /* 2.open file of user and start */

}

void Manager::calculateRevenue(int userID, int waiting_time){
    int fee_rate = 0;
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
    // else
    //     cout << "wrong name : " << b_type << "  QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ!" <<endl;

    /* calculate */
    n_revenue = n_revenue + fee_rate*riding_time - waiting_time*fee.waiting;
    // cout << "fee rate : " << fee_rate << " ; riding time : " << riding_time << endl;
    // cout << "now money : " <<  n_revenue << endl;
}

void Manager::status_output(string policy_){
    string* output;
    int output_revenue = -1;
    if(policy_ == "Normal"){
        output_revenue = n_revenue;
        status_file.open("./part1_status.txt",ios::out|ios::trunc);
    }
    else if(policy_ == "Advanced"){
        output_revenue = a_revenue;
        status_file.open("./part2_status.txt",ios::out|ios::trunc);
    }
    else{
        cout << "Wrong command for writing status file !"<<endl;
        return;
    }

    for(int i=0; i < s_quantity; i++){
        status_file << i+1 << ":" <<endl;
        for(int j=0; j < 3; j++){
            int bike_quantity=0;
            if(j==0){
                status_file << "electric:";
                bike_quantity = stationList[i].if_haveBike("electric");
            }
            else if(j==1){
                status_file << "lady:";
                bike_quantity = stationList[i].if_haveBike("lady");
            }
            else{
                status_file << "road:";
                bike_quantity = stationList[i].if_haveBike("road");
            }
            output = stationList[i].output_list(j);
            // int bike_quantity = sizeof(output)/sizeof(output[0]); //output is a pointer, so it will return sizeof a pointer
            for(int k=0; k < bike_quantity; k++){
                status_file << " " << output[k];
            }
            status_file << endl;
        }
    }
    status_file << output_revenue;
    status_file.close();
}