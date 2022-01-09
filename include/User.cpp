#include "./User.h"

User::User(){
    req.rent_time = -1;
    req.return_time = 1441;
    res.b_id = -1;
}

userList::userList (){
    users = new User[99999];
    user_num = 0;
    script_count = 0;
    
    /* 1.read file */
    ifstream u_file("./test_case/user.txt",ifstream::in);
    string data;
    if (u_file.is_open())
        cout << "open user file"<<endl;
    else
        cout << "failed to open user file"<<endl;

    while(getline(u_file, data)){
        writeUser(data);
        script_count++;
    }
    u_file.close();
}

void userList::writeUser(string d){
    string delimiter=" ";
    string token[5];
    size_t pos=0;
    int d_count=0;
    while((pos = d.find(delimiter))!=string::npos){
        token[d_count] = d.substr(0, pos);
        d.erase(0, pos+delimiter.size());
        d_count++;
    }
    token[d_count] = d;
    d_count++;

    if(token[0]=="rent"){
        user_num++;
        addRenter(token);
    }
    else if(token[0]=="return"){
        addReturn(token);
    }
    else
        cout << "Reading wrong data in user.txt - "<<token[0];

}

void userList::addRenter(string* input){
    int id = stoi(input[3]);
    users[id].start_station = stoi(input[1]);
    users[id].req.b_type = input[2];
    users[id].userID = id;
    users[id].req.rent_time = stoi(input[4]);
    users[id].res.b_id = -1;
}

void userList::addReturn(string* input){
    int id = stoi(input[2]);
    users[id].end_station = stoi(input[1]);
    users[id].req.return_time = stoi(input[3]);

    users[id].riding_time = users[id].req.return_time-users[id].req.rent_time;
}

void userList::giveResponse(int user_id, string command, string bike_type,
                            int bike_id, int wait_time){// response renting bike id and type + wait_time
    users[user_id].res.command = command;
    users[user_id].res.b_type = bike_type;
    users[user_id].res.b_id = bike_id;
    users[user_id].res.rent_time = users[user_id].req.rent_time + wait_time;
    users[user_id].res.return_time = users[user_id].res.return_time + wait_time;
}

string userList::returnBikeType(int user_id){
    return users[user_id].res.b_type;
}

int userList::returnBikeID(int user_id){
    return users[user_id].res.b_id;
}

int userList::returnRideTime(int user_id){
    return users[user_id].riding_time;
}

int* userList::returnSandE(int user_id){
    int *SandE = new int[2];
    SandE[0] = users[user_id].start_station;
    SandE[1] = users[user_id].end_station;
    return SandE;
}

void userList::showInfo(int id){
    cout << "User ID : "<< users[id].userID << endl;
    cout << "bike : "<< users[id].req.b_type << endl;
    cout << "rent : "<< users[id].start_station << " " << users[id].req.rent_time << endl;
    cout << "return : "<< users[id].end_station << " " << users[id].req.return_time << endl;
}