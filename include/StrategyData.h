#ifndef _STRATEGYDATA_H_
#define _STRATEGYDATA_H_

#include <iostream>
#include <string>

using namespace std;

class Manager;

struct FutureData{ // manager1 record data, which is also future data for manager2
    int rent_num;          //rest rent quantity (accept)
    int return_num;        //rest return quantity
    int d_return_num;      //rest desire rent quantity
    int reject_num;        //rest r_reject time
    int limit_rent;  //total num of limit rent condition
    float Reject_Rent;
};

struct StationFutureData{ // each type of bike
    FutureData elec_fd;
    FutureData lady_fd;
    FutureData road_fd;
};

class StrategyData{
private:
    int s_quantity;
    StationFutureData *s_futureList;
    
public:
    friend class Manager;
    StrategyData(int);
    ~StrategyData(){};

    void writedata(int, string, string, int);
    void showdata();

};

#endif //_STRATEGYDATA_H_