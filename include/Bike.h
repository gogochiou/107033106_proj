#ifndef _BIKE_H_
#define _BIKE_H_

#include <iostream>
#include <string>

using namespace std;

class BikeList;

class Bike{
private:
    int s_num;
    int b_num;
    int ID;
    Bike *next;
public:
    friend BikeList;
    Bike(int, int, Bike*);
    ~Bike(){};
};

class BikeList{
private:
    Bike *first;
public:
    BikeList();
    ~BikeList(){};
    void push(int, int);
    int pop(); // return bike id
    Bike* find_prior(int);

    string* output_bike(int);
    void print_list();
};

#endif //_BIKE_H_