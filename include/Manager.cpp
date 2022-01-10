#include "./Manager.h"
#include <math.h>

#define limit_num 0
#define INF 1000000

/* transfer */
#define max_reject_per 0.3
#define min_bike 1
#define min_store_bike 1
#define max_borrow_rate 0.35

Manager::Manager(){
    cout<< "setting Manager !"<<endl;
    /* init param */
    n_revenue = 0;
    a_revenue = 0;
    s_quantity = 0;

    last_borrow_time = 1440;

    /* init class */
    readfile();

}

void Manager::readfile(){
    /* 1.station read file */
    stationList = NULL;//init stationList as NULL
    readstation(false);
    str_data = new StrategyData(s_quantity); //init strategy data 
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

    bool check = false;
    
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
            /* token : command, stationID, bikeType, userID, time */
            int userID = stoi(token[3]);
            if( stationList[s_num-1].if_haveBike(token[2])!=0 ){
                normal_res<< "accept" << endl;
                int rent_bikeID = stationList[s_num-1].rent_bike(token[2]);
                int rest_bike = stationList[s_num-1].if_haveBike(token[2]);
                user_list->giveResponse(userID, "accept", token[2], 
                                        rent_bikeID, 0, rest_bike);
                
                //write str_data real rent
                str_data->writedata(s_num, token[2], "accept", 1);
                if(rest_bike <= limit_num){
                    str_data->writedata(s_num, token[2], "limit_rent", 1);
                }
            }
            else{
                normal_res << "reject" << endl;
                user_list->giveResponse(userID, "reject", " ", -1, 0, -1);
                str_data->writedata(s_num, token[2], "reject", 1);
            }
            last_borrow_time = stoi(token[4]);
        }
        else if(token[0]=="return"){
            /* token : command, stationID, userID, time */
            int userID = stoi(token[2]);
            string b_type = user_list->returnBikeType(userID, "response");
            int b_id = user_list->returnBikeID(userID);
            if(b_id != -1){
                check = false;
                /* cause b_type will be nothing if we did not borrow,
                   so return_bike will still refuse the rent command
                   in old version(without this conditional expressions) */
                stationList[s_num-1].return_bike(b_type, b_id);
                str_data->writedata(s_num, b_type, "return", 1);
            }
            
            string d_b_type = user_list->returnBikeType(userID, "request");
            str_data->writedata(s_num, d_b_type, "d_return", 1);
            // stationList[s_num-1].show(b_type);
            n_revenue += calculateRevenue(userID, 0, false, "response");
            // normal_res << "Current revenue : " << n_revenue <<endl;
        }
    }
    input_file.close();
    normal_res.close();

    status_output("Normal");
    cout <<"normal policy revenue : "<< n_revenue << endl;

    // str_data->showdata();
}

void Manager::AdvancedPolicy(){
    working = true;
    work_time = 0;
    state_change = true;
    readComplete = false;
    /* 1.rebuild station */
    readstation(true);
    // cout << "------------------------------------------" <<endl;
    // for(int i=0; i<s_quantity; i++)
    //     stationList[i].show("all");
    // cout << "------------------------------------------" <<endl;

    /* 2.open file of user and start */
    ifstream input_file("./test_case/user.txt",ifstream::in);
    advanced_res.open("./part2_response.txt",ios::out|ios::trunc);

    /* 3.read fisrt data */
    string u_data[5];
    // first data always be "rent"
    /* u_data : command, stationID, bikeType, userID, time */
    input_file >> u_data[0] >> u_data[1] >> u_data[2] >> u_data[3] >> u_data[4];
    while(working){
        if(work_time <= 1440){
            working = true;
        }
        else{
            working = false;
            break;
        }

        // if(transferList.first==NULL)
        //     state_change = true;

        if( state_change && (work_time < (last_borrow_time - map->get_mapMaxTime())) ){
            cout << "Set balance !" << endl;
            stationBalance(work_time);
            transferList.print_list();
            state_change = false;
            cout << "Set balance over !" << endl;
        }

        while( (transferList.first!=NULL) &&
               (work_time == transferList.first->c_time) ){
            // pop and return bike
            // cout << "Transfer !" << endl;
            int s_num = transferList.first->end;
            string b_type = transferList.first->bike_type;
            for(int i=0; i < transferList.first->bike_num; i++)
                stationList[s_num-1].return_bike(b_type, transferList.first->idList[i]);
            
            transferList.pop();
            // state_change = true;
            // cout << "Transfer over !" << endl;
        }

        while( (waitList.first!=NULL) &&
               (work_time == waitList.first->c_time) ){
            // pop and rent bike
            // cout << "Waiting list !" << endl;
            int userID = waitList.first->userID;
            int *SandE = user_list->returnSandE(userID);
            string b_type = user_list->returnBikeType(userID, "response");
            int w_time = user_list->returnWaitTime(userID);

            int rent_bikeID = stationList[SandE[0]-1].rent_bike(b_type);
            user_list->giveResponse(userID, "wait", b_type, 
                                    rent_bikeID, w_time);

            if(assignBike[userID]){
                int return_time = user_list->returnReturnTime(userID, "response");
                fake_rList.push(userID, SandE[1], b_type, rent_bikeID, return_time);
            }

            waitList.pop();
            // state_change = true;
            // cout << "Wating list over !" << endl;
        }

        while( (fake_rList.first!=NULL) &&
               (work_time == fake_rList.first->c_time) ){
            // pop and return bike
            // cout << "Fake return !" << endl;
            int s_num = fake_rList.first->r_station;
            string b_type = fake_rList.first->bike_type;
            int userID = fake_rList.first->userID;
            stationList[s_num-1].return_bike(b_type, fake_rList.first->bikeID);
            
            int w_time = user_list->returnWaitTime(userID);
            a_revenue += calculateRevenue(userID, w_time, false, "response");

            fake_rList.pop();
            // state_change = true;
            // cout << "Fake return over !" << endl;
        }

        while( work_time == stoi(u_data[4]) && !readComplete){
            // cout << "Strategy choose !" << endl;
            /* u_data : command, stationID, bikeType, userID, time */
            int s_num = stoi(u_data[1]);
            int userID = stoi(u_data[3]);
            refresh_StrData(userID, u_data[0]);

            if( u_data[0] == "rent"){
                advanced_res << u_data[0] << " " << u_data[1] << " " << u_data[2]
                            << " " << u_data[3] << " " << u_data[4] << endl;
                if( stationList[s_num-1].if_haveBike(u_data[2])!=0 ){
                    advanced_res << "accept" << endl;
                    int rent_bikeID = stationList[s_num-1].rent_bike(u_data[2]);
                    user_list->giveResponse(userID, "accept", u_data[2], 
                                            rent_bikeID, 0);
                }
                else{
                    shortageStrategy(s_num, u_data[2], userID, work_time);
                }
                // state_change = true;
            }
            else if( u_data[0] == "return"){
                advanced_res << u_data[0] << " " << u_data[1] << " "
                            << u_data[3] << " " << u_data[4] << endl;
                string b_type = user_list->returnBikeType(userID, "response");
                string user_state = user_list->returnCommand(userID);
                int b_id = user_list->returnBikeID(userID);
                if(user_state=="wait"){
                    if(b_id==-1){
                        // when we want return at origin time, but still not rent bike to user
                        assignBike[userID] = true;
                    }
                    else{
                        int return_time = user_list->returnReturnTime(userID, "response");
                        fake_rList.push(userID,s_num, b_type, b_id, return_time);
                    }
                }
                else if(user_state=="switch"){
                    stationList[s_num-1].return_bike(b_type, b_id);
                    a_revenue += calculateRevenue(userID, 0, true, "response");
                    // state_change = true;
                }
                else if(user_state=="accept"){
                    stationList[s_num-1].return_bike(b_type, b_id);
                    a_revenue += calculateRevenue(userID, 0, false, "response");
                    // state_change = true;
                }
            }
            // cout << "Strategy choose over !" << endl;

            // read next data
            // cout << "Read data !" << endl;
            if(input_file >> u_data[0]){
                readComplete = false;
            }
            else
                readComplete = true;
            if(u_data[0] == "rent")
                input_file >> u_data[1] >> u_data[2] >> u_data[3] >> u_data[4];
            else if(u_data[0] == "return")
                input_file >> u_data[1] >> u_data[3] >> u_data[4];
                
            // cout << "Read data over !" << endl;
        }
        work_time ++;
    }
    input_file.close();
    advanced_res.close();

    status_output("Advanced");
    cout << "a_revenue : " << a_revenue << endl;
}

int Manager::calculateRevenue(int userID, int waiting_time, bool switching, string condition){
    int fee_rate = 0;
    /* check if use regular or discount */
    int riding_time = user_list->returnRideTime(userID);
    int* SandE = user_list->returnSandE(userID);//start and end station num
    bool use_regular = true;
    if( riding_time == map->get_minTime(SandE[0],SandE[1]) )
        use_regular = false;

    /* check whick bike type and fee */
    string b_type = user_list->returnBikeType(userID, condition);
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

    if(switching)
        fee_rate = round(fee_rate*fee.switching);
    // else
    //     cout << "wrong name : " << b_type << "  QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ!" <<endl;

    /* calculate */
    return fee_rate*riding_time - waiting_time*fee.waiting;
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

void Manager::stationBalance(int cur_time){
    int requirement[s_quantity][3];

    //check each station requirement
    for(int i=0; i < s_quantity; i++){
        for(int j=0; j < 3; j++)
            requirement[i][j] = 0; // set default
        
        int elec_num = stationList[i].if_haveBike("electric");
        int lady_num = stationList[i].if_haveBike("lady");
        int road_num = stationList[i].if_haveBike("road");

        // elec
        if(str_data->s_futureList[i].elec_fd.Reject_Rent >= max_reject_per
           && elec_num < min_bike ){
            requirement[i][0] = -(str_data->s_futureList[i].elec_fd.reject_num);
        }
        else if(str_data->s_futureList[i].elec_fd.reject_num == 0
                && str_data->s_futureList[i].elec_fd.limit_rent == 0
                && elec_num > min_store_bike){
            int max_rent = str_data->s_futureList[i].elec_fd.return_num + 
                           elec_num - str_data->s_futureList[i].elec_fd.rent_num - 
                           min_store_bike;
            if(max_rent < 0)
                max_rent = 0;
            else if( max_rent > int(elec_num*max_borrow_rate) )
                max_rent = int(elec_num*max_borrow_rate);
            requirement[i][0] = min(elec_num-min_store_bike,max_rent);
        }

        // lady
        if(str_data->s_futureList[i].lady_fd.Reject_Rent >= max_reject_per
           && lady_num < min_bike ){
            requirement[i][1] = -(str_data->s_futureList[i].lady_fd.reject_num);;
        }
        else if(str_data->s_futureList[i].lady_fd.reject_num == 0
                && str_data->s_futureList[i].lady_fd.limit_rent == 0){
            int max_rent = str_data->s_futureList[i].lady_fd.return_num + 
                           lady_num - str_data->s_futureList[i].lady_fd.rent_num - 
                           min_store_bike;
            if(max_rent < 0)
                max_rent = 0;
            else if( max_rent > int(lady_num*max_borrow_rate) )
                max_rent = int(lady_num*max_borrow_rate);
            requirement[i][1] = min(lady_num-min_store_bike,max_rent);
        }

        // road
        if(str_data->s_futureList[i].road_fd.Reject_Rent >= max_reject_per
           && road_num < min_bike ){
            requirement[i][2] = -(str_data->s_futureList[i].road_fd.reject_num);;
        }
        else if(str_data->s_futureList[i].road_fd.reject_num == 0
                && str_data->s_futureList[i].road_fd.limit_rent == 0){
            int max_rent = str_data->s_futureList[i].road_fd.return_num + 
                           road_num - str_data->s_futureList[i].road_fd.rent_num - 
                           min_store_bike;
            if(max_rent < 0)
                max_rent = 0;
            else if( max_rent > int(road_num*max_borrow_rate) )
                max_rent = int(road_num*max_borrow_rate);
            requirement[i][0] = min(road_num-min_store_bike,max_rent);
        }     
    }

    // set transfer 
    for(int i=0; i < 3; i++){
        string _bike_type;
        if(i == 0)
            _bike_type = "electric";
        else if(i == 1)
            _bike_type = "lady";
        else if(i == 2)
            _bike_type = "road";

        for(int j=0; j < s_quantity; j++){
            // if(transferList.searchTransfer(j+1, _bike_type)!=-1 ){
            //     continue;
            // }
            if(requirement[j][i] < 0){
                int borrow_station = -1; //real station num (start from 1)
                int min_cost = INF;
                for(int k=0; k < s_quantity; k++){
                    if( requirement[k][i] > 0 && k!=j){
                        int temp_cost = map->get_minTime(k+1, j+1);
                        if( (temp_cost < min_cost) &&
                            ( (cur_time+temp_cost) <= last_borrow_time) ){
                            min_cost = temp_cost;
                            borrow_station = k+1;
                        }
                    }
                }
                if(borrow_station != -1){
                    a_revenue -= min_cost*fee.transferring;
                    int rent_num = min(abs(requirement[j][i]),requirement[borrow_station-1][i]);
                    int id_array[rent_num];
                    for(int r=0; r < rent_num; r++)
                        id_array[r] = stationList[borrow_station-1].rent_bike(_bike_type);

                    transferList.push( j+1, borrow_station, _bike_type, rent_num
                                       , id_array, cur_time+min_cost);
                    advanced_res << "transfer " << borrow_station << " " << j+1 << " "
                                << _bike_type << " " << rent_num << " " << cur_time << endl;
                    requirement[borrow_station][i] = 0;
                }
            }
        }
    }
}

void Manager::refresh_StrData(int user_id, string RentOrReturn){
    string command = user_list->returnCommand(user_id);
    int* SandE = user_list->returnSandE(user_id);
    string bike_type = user_list->returnBikeType(user_id, "request");
    int rest_bike = user_list->returnRestbike(user_id);

    if(RentOrReturn == "rent"){
        if(command == "accept"){
            str_data->writedata(SandE[0], bike_type, "accept", -1);
            if(rest_bike <= limit_num)
                str_data->writedata(SandE[0], bike_type, "limit_rent", -1);
        }
        else if(command == "reject")
            str_data->writedata(SandE[0], bike_type, "reject", -1);
    }
    else if(RentOrReturn == "return"){
        if(rest_bike != -1){
            str_data->writedata(SandE[0], bike_type, "return", -1);
        }
        else{
            str_data->writedata(SandE[0], bike_type, "return", -1);
        }
    }
}

void Manager::shortageStrategy(int stationID, string bikeType, int userID, int cur_time){
    
    string result = "reject";
    // if accept, how much we earn
    int earn = calculateRevenue(userID, 0, false, "request");

    // waiting condition
    int wait_cost = INF;
    int wait_time = INF;
    int wait_earn = -1;
    int feasible_time = transferList.searchTransfer(stationID, bikeType);
    if(feasible_time!=-1){
        int final_time =  feasible_time + user_list->returnRideTime(userID); // return time
        if( final_time <= 1440 ){
            wait_time = feasible_time - cur_time;
            wait_cost = fee.waiting*wait_time;
            wait_earn = earn-wait_cost;
            result = "wait";
        }
    }

    //switch condition
    string switch_bike_type = "no";
    int switch_earn = -1;
    // 1.elec
    int elec_earn = 0;
    int elec_rest = stationList[stationID-1].if_haveBike("electric");
    if(elec_rest > min_store_bike){
        user_list->giveResponse(userID, "temp", "electric", -1, 0);
        elec_earn = calculateRevenue(userID, 0, true, "response");
        if(elec_earn > switch_earn){
            switch_bike_type = "electric";
            switch_earn = elec_earn;
        }
    }
    // 2.lady
    int lady_earn = 0;
    int lady_rest = stationList[stationID-1].if_haveBike("lady");
    if(lady_rest > min_store_bike){
        user_list->giveResponse(userID, "temp", "lady", -1, 0);
        lady_earn = calculateRevenue(userID, 0, true, "response");
        if( (lady_earn == switch_earn) && switch_bike_type=="electric"){
            if(lady_rest > elec_rest){
                switch_bike_type = "lady";
                switch_earn = lady_earn;
            }
        }
        else if(lady_earn > switch_earn){
            switch_bike_type = "lady";
            switch_earn = lady_earn;
        }
    }
    // 3.road
    int road_earn = 0;
    int road_rest = stationList[stationID-1].if_haveBike("road");
    if(road_rest > min_store_bike){
        user_list->giveResponse(userID, "temp", "road", -1, 0);
        road_earn = calculateRevenue(userID, 0, true, "response");
        if( road_earn == switch_earn){
            if(switch_bike_type == "electric"){
                if( road_rest > elec_rest){
                    switch_bike_type = "road";
                    switch_earn = road_earn;
                }
            }
            else if(switch_bike_type == "lady"){
                if( road_rest > lady_rest){
                    switch_bike_type = "road";
                    switch_earn = road_earn;
                }
            }
        }
        else if(road_earn > switch_earn){
            switch_bike_type = "road";
            switch_earn = road_earn;
        }
    }

    if( (switch_earn!=-1) && (switch_earn >= wait_earn))
        result = "switch";

    // implement strategy
    if(result == "reject"){
        advanced_res << "reject" << endl;
        user_list->giveResponse(userID, "reject", " ", -1, 0);
    }
    else if(result == "wait"){
        advanced_res << "wait" << endl;
        user_list->giveResponse(userID, "wait", bikeType, -1, wait_time);
        waitList.push(userID, feasible_time);
    }
    else if(result == "switch"){
        advanced_res << "discount " << switch_bike_type << endl;
        int rent_bikeID = stationList[stationID-1].rent_bike(switch_bike_type);
        user_list->giveResponse(userID, "switch", switch_bike_type, rent_bikeID, 0);
    }
    
}