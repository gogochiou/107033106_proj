#include "./Bike.h"

Bike::Bike(int station_num, int bike_num, Bike* next_bike=NULL){
    s_num = station_num;
    b_num = bike_num;
    ID = station_num*100+bike_num;
    next = next_bike;
}

BikeList::BikeList(){
    first = NULL;
}

void BikeList::push(int station_num, int bike_num){
    int id = station_num*100+bike_num;
    if(first==NULL)
        first = new Bike(station_num, bike_num, NULL);
    else{
        // add as first
        if(id < first->ID){
            Bike* new_bike = new Bike(station_num, bike_num, first);
            first = new_bike;
            delete new_bike;
            return;
        }
        // add at other position
        Bike* current = find_prior(id);
        Bike* new_bike = new Bike(station_num, bike_num, NULL);
        Bike* temp = current->next;
        new_bike->next = temp;
        current->next = new_bike;
        delete temp, new_bike;
    }
}

int BikeList::pop(){
    if(first == NULL)
        return 0;
    int id = first->ID;
    Bike* temp = first;
    first = first->next;
    delete temp;
    return id;
}

Bike* BikeList::find_prior(int id){
    Bike* current = first;
    bool comparing = true;
    while(comparing && current->next!=NULL){
        if(id > current->next->ID)
            current = current->next;
        else
            comparing=false;
    }
    return current;
}

void BikeList::print_list(){
    if(first==NULL){
        cout << "List is empty"<<endl;
        return;
    }

    Bike* current = first;
    while(current!=NULL){
        if(current->next==NULL)
            cout << current->ID << endl;
        else
            cout << current->ID << "-->";
        current = current->next;
    }
}