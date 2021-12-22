#ifndef _STATION_H_
#define _STATION_H_

#include <iostream>
#include <string>

using namespace std;

class Station{
private:
    int s_num;    // station number
    int electric; // quantity of electric
    int lady;     // quantity of lady
    int road;     // quantity of road
    
public:
    Station();
    Station(int,int,int,int);
    ~Station(){};

    void Init(string *); //initialize the station
};

#endif //_STATION_H_