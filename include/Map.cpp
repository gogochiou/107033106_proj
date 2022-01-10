#include "./Map.h"

#define INF 99999

Map::Map(int vertix){
    map_min_t = INF;
    map_max_t = -1;

    /* 1.init min_dis graph */
    station_num = vertix;
    min_time = new int*[station_num];
    for(int i=0; i<station_num; i++)
        min_time[i] = new int[station_num];
    //set 0 for self to self
    for(int i=0; i<station_num; i++){
        for(int j=i; j<station_num; j++){
            min_time[i][j] = INF;
        }
        min_time[i][i] = 0;
    }

    /* 2.open file to know distance */
    ifstream m_file("./test_case/map.txt"); //map file
    string data;
    string delimiter=" ";
    size_t pos=0;

    if (m_file.is_open())
        cout << "open map file"<<endl;
    else
        cout << "failed to open map file"<<endl;

    while(getline(m_file, data)){
        int start;
        int end;
        pos = data.find(delimiter);
        start = stoi(data.substr(0,pos));
        data.erase(0, pos+delimiter.size());
        pos = data.find(delimiter);
        end = stoi(data.substr(0,pos));
        data.erase(0, pos+delimiter.size());
        min_time[start-1][end-1] = stoi(data);
        min_time[end-1][start-1] = stoi(data);
    }
    m_file.close();
    // cout << "Print min_time :" << endl;
    // show();

    /* 3.find min dist for each point by using Floyd Warshall Algorithm */
    FloydWarshall();
    // cout << "Calculate min_time result"<<endl;
    // show();
}

void Map::show(){
    for(int i=0; i<station_num; i++){
        for(int j=0; j<station_num; j++){
            cout<< " " << min_time[i][j] ;
        }
        cout << endl;
    }
}

void Map::FloydWarshall(){
    for(int k=0; k < station_num; k++){
        for(int i=0; i < station_num; i++){
            for(int j=0; j < station_num; j++){
                if(min_time[i][j] > (min_time[i][k]+min_time[k][j]) 
                   &&(min_time[i][k]!=INF && min_time[k][j]!=INF) ){
                    min_time[i][j] = min_time[i][k]+min_time[k][j];
                }
            }
        }
    }

    /* find min and max of all min_time map */
    for(int i=0; i < station_num; i++){
        for(int j=0; j < station_num; j++){
            if(min_time[i][j] < map_min_t){
                map_min_t = min_time[i][j];
            }
            if(min_time[i][j] > map_max_t){
                map_max_t = min_time[i][j];
            }
        }
    }
}

int Map::get_minTime(int station_A, int station_B){
    return min_time[station_A-1][station_B-1];
}

int Map::get_mapMinTime(){
    return map_min_t;
}

int Map::get_mapMaxTime(){
    return map_max_t;
}