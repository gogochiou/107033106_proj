#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "./Station.h"
#include "./Map.h"
#include "./User.h"
#include "./StrategyData.h"
#include "./WorkData.h"

using namespace std;

struct Rate{
    int discount;
    int regular;
};

struct Fee{
    Rate elec;
    Rate lady;
    Rate road;
    int waiting;
    double switching;
    int transferring;
};

class Manager{
private:
    /* input data */
    Map* map;
    Station* stationList; //store with starting from 0!!!!
    Fee fee;
    userList* user_list;

    /* output use */
    ofstream normal_res; //part1_response.txt output
    ofstream status_file; //status.txt output
    ofstream advanced_res; //part2_response.txt output
    int n_revenue;
    int a_revenue;

    /* common param */
    int s_quantity;

    /* determine strategy use data */
    StrategyData *str_data;
    FakeReturnList fake_rList;
    TransferList transferList;
    WaitList waitList;

    /* advanced strategy */
    int last_borrow_time;
    bool working;
    int work_time;
    bool state_change; // station have new bike in or out

    bool assignBike[99999] = {0}; // if origin return time < wait then rent time -> set to true
    bool readComplete;

public:
    Manager(/* args */);
    ~Manager(){};
    void readfile();
    void readstation(bool);
    void writeFee( Rate&, ifstream&);

    void NormalPolicy();
    void AdvancedPolicy(); 
    int calculateRevenue(int, int, bool, string);
    void status_output(string);

    /* advanced strategy */
    void stationBalance(int); // if need transfer bike
    void refresh_StrData(int, string);
    void shortageStrategy(int, string, int, int);
};

#endif //_MANAGER_H_