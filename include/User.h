#ifndef _USER_H_
#define _USER_H_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class userList;

struct Request{
    string b_type; //bike type
    int rent_time;
    int return_time;
};

struct Response{
    string command; // accept, wait, change, reject
    string b_type;
    int b_id; // given bike id
    int rent_time;
    int return_time;
};

class User{
private:
    int userID;
    Request req;
    Response res;
    int riding_time;
    int start_station;
    int end_station;

public:
    friend userList;
    User();
    ~User(){};
};

class userList{
private:
    User* users;
    int user_num;
    int script_count; // how many line in user.txt
public:
    userList();
    ~userList(){};

    void writeUser(string);
    void addRenter(string*);
    void addReturn(string*);
    void giveResponse(int, string, string, int, int);
    string returnBikeType(int);
    int returnBikeID(int);
    int returnRideTime(int);
    int* returnSandE(int); // return start and end station num
    void showInfo(int);
};

#endif //_USER_H_