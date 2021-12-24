#include "./Station.h"

Station::Station(){}

Station::Station(int _num, int _elec, int _lady, int _road){
    s_num = _num;
    elec_num = _elec;
    lady_num = _lady;
    road_num = _road;
}

void Station::Init(string *input){
    s_num = stoi(input[0]);
    elec_num = stoi(input[1]);
    lady_num = stoi(input[2]);
    road_num = stoi(input[3]);

    elec_list = new BikeList();
    for(int i=0; i<elec_num; i++)
        elec_list->push(s_num, i);

    lady_list = new BikeList();
    for(int i=0; i<lady_num; i++)
        lady_list->push(s_num, i);

    road_list = new BikeList();
    for(int i=0; i<road_num; i++)
        road_list->push(s_num, i);
}

bool Station::if_haveBike(string type){
    if(type == "electric"){
        if(elec_num!=0)
            return true;
    }
    else if(type == "lady"){
        if(lady_num!=0)
            return true;
    }
    else if(type == "road"){
        if(road_num!=0)
            return true;
    }
    return false;
}

int Station::rent_bike(string type){
    if(type == "electric"){
        elec_num = elec_num-1;
        return elec_list->pop();
    }
    else if(type == "lady"){
        lady_num = lady_num-1;
        return lady_list->pop();
    }
    else if(type == "road"){
        road_num = road_num-1;
        return road_list->pop();
    }
    return -1; // failure situation
}

void Station::return_bike(string type, int b_id){
    int s_num = b_id/100;
    int b_num = b_id%100;
    if(type == "electric"){
        elec_num = elec_num+1;
        elec_list->push(s_num, b_num);
    }
    else if(type == "lady"){
        lady_num = lady_num+1;
        lady_list->push(s_num, b_num);
    }
    else if(type == "road"){
        road_num = road_num+1;
        road_list->push(s_num, b_num);
    }
}

void Station::show(){
    cout << "Station : " << s_num << endl;
    cout << "1.elec_num - " << elec_num <<endl;
    cout << "  ";
    elec_list->print_list();
    cout << "2.lady_num - " << lady_num <<endl;
    cout << "  ";
    lady_list->print_list(); 
    cout << "3.road_num - " << road_num <<endl;
    cout << "  ";
    road_list->print_list();  
}
