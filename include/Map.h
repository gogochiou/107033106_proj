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
    int map_min_t;
    int map_max_t;

public:
    Map(int);
    ~Map(){};

    void show();
    void FloydWarshall(); // find min dist
    int get_minTime(int, int);
    int get_mapMinTime();
    int get_mapMaxTime();
};

#endif //_MAP_H_