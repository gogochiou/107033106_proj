#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Map{
private:
    int station_num;
    int **min_time; // min time(distance) between station (start from 0)
    
public:
    Map(int);
    ~Map(){};

    void show();
    void FloydWarshall(); // find min dist
    int get_minTime(int, int);
};

#endif //_MAP_H_