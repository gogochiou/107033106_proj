#ifndef _WORKDATA_H_
#define _WORKDATA_H_
/* for restoring ondoing-work data (manager2) */

#include <iostream>
#include <string>

using namespace std;

struct WaitData{
    int userID;
    int c_time; // real rent time (complete time)
    WaitData *next;
};

class WaitList{
public:
    WaitData *first;

    WaitList();
    ~WaitList(){};
    void push(int, int);
    void pop();
    WaitData* find_prior(int);
};

struct TransferData{
    int start;
    int end;
    string bike_type;
    int bike_num;
    int* idList;
    int c_time; // complete time
    TransferData *next;
};

class TransferList{
public:
    TransferData *first;
    
    TransferList();
    ~TransferList(){};
    void push(int, int, string, int, int*, int);
    void pop();
    void setData(TransferData**, int, int, string,
                 int, int*, int, TransferData*);
    TransferData* find_prior(int);
    int searchTransfer(int, string); //check if have transfer with particular condition, return c_time 
    void print_list();
};

struct FakeReturnData{ //cause by waiting
    int userID;
    int r_station; //return station
    string bike_type;
    int bikeID;
    int c_time;
    FakeReturnData *next;
};

class FakeReturnList{
public:
    FakeReturnData *first;

    FakeReturnList();
    ~FakeReturnList(){};
    void push(int, int, string, int, int);
    void pop();  
    FakeReturnData* find_prior(int);
};


#endif //_WORKDATA_H_