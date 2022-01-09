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
    Station* stationList; //store with starting from 0
    Fee fee;
    userList* user_list;

    /* output use */
    ofstream normal_res; //part1_response.txt output
    ofstream status_file; //status.txt output
    int n_revenue;
    int a_revenue;

    /* determine strategy use data */
    int s_quantity;

public:
    Manager(/* args */);
    ~Manager(){};
    void readfile();
    void readstation(bool);
    void writeFee( Rate&, ifstream&);

    void NormalPolicy();
    void AdvancedPolicy(); 
    void calculateRevenue(int, int);
    void status_output(string);
};

#endif //_MANAGER_H_