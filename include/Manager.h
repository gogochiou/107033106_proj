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
    Station* stationList; //store with starting from 0
    Fee fee;
    userList* user_list;

    /* output use */
    ofstream normal_res; //part1_response.txt output
    ofstream status_file; //status.txt output
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
    void status_output(string);
};

#endif //_MANAGER_H_