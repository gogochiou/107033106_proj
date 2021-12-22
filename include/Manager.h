#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "./Station.h"
// #include "./Map.h"

using namespace std;

class Manager{
private:
    /* data */
    // Map *map;
    Station *stationList;


public:
    Manager(/* args */);
    ~Manager(){};
    void readfile();
};

#endif //_MANAGER_H_