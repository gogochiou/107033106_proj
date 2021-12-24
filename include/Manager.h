#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "./Station.h"
#include "./Map.h"
#include "./User.h"

using namespace std;

struct Rate{
    double discount;
    double regular;
};

struct Fee{
    Rate elec;
    Rate lady;
    Rate road;
    double waiting;
    double switching;
    double transferring;
};

class Manager{
private:
    /* data */
    Map* map;
    Station* stationList;
    Fee fee;
    userList* user_list;

    /* output use */
    ofstream normal_res; //part1_response.txt output
    ofstream normal_status; //part1_status.txt output
    double n_revenue;
    double a_revenue;

    int s_quantity;

public:
    Manager(/* args */);
    ~Manager(){};
    void readfile();
    void writeFee( Rate&, ifstream&);

    void NormalPolicy();
    void calculateRevenue(int, int);
};

#endif //_MANAGER_H_