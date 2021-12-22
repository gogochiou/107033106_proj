#include "./Station.h"

Station::Station(){}

Station::Station(int _num, int _elec, int _lady, int _road){
    s_num = _num;
    electric = _elec;
    lady = _lady;
    road = _road;
}

void Station::Init(string *input){
    s_num = stoi(input[0]);
    electric = stoi(input[1]);
    lady = stoi(input[2]);
    road = stoi(input[3]);
}
