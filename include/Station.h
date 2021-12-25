#ifndef _STATION_H_
#define _STATION_H_

#include <iostream>
#include <string>

#include "./Bike.h"

using namespace std;

class Station{
private:
    int s_num;        // station number
    int elec_num;     // quantity of electric
    int lady_num;     // quantity of lady
    int road_num;     // quantity of road
    BikeList* elec_list;
    BikeList* lady_list;
    BikeList* road_list;
    
public:
    Station();
    Station(int,int,int,int);
    ~Station(){};

    void Init(string *); //initialize the station
    int if_haveBike(string); //return bike_num(quantity)
    int rent_bike(string);
    void return_bike(string, int);
    string* output_list(int); // int for count : 0 is elec, 1 is lady, 2 is road
    void show(string);
};

#endif //_STATION_H_