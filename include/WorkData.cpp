#include "./WorkData.h"

/* -------------------------------------------------------------------------- */
WaitList::WaitList(){
    first = NULL;
}

void WaitList::push(int ID, int time){
    if(first==NULL){
        first = new WaitData;
        first->userID = ID;
        first->c_time = time;
        first->next = NULL;
    }
    else{
        // add as first
        if(time < first->c_time){
            WaitData *new_data = new WaitData;
            new_data->userID = ID;
            new_data->c_time = time;
            new_data->next = first;
            first = new_data;
            new_data = NULL;
            return;
        }
        // add at other position
        WaitData* current = find_prior(time);
        WaitData* new_data = new WaitData;
        new_data->userID = ID;
        new_data->c_time = time;
        WaitData* temp = current->next;
        new_data->next = temp;
        current->next = new_data;
        temp = NULL;
        new_data = NULL;
    }
}

void WaitList::pop(){
    if(first == NULL)
        return;

    WaitData* temp = first;
    first = first->next;
    delete temp;
    temp = NULL;
}

WaitData* WaitList::find_prior(int time){
    WaitData* current = first;
    bool comparing = true;
    while(comparing && current->next!=NULL){
        if(time > current->next->c_time)
            current = current->next;
        else
            comparing=false;
    }
    return current;
}

/* -------------------------------------------------------------------------- */
TransferList::TransferList(){
    first = NULL;
}

void TransferList::push(int start_, int end_, string type, int num, int* idList_, int time){
    if(first==NULL){
        first = new TransferData;
        setData(&first, start_, end_, type, num, idList_, time, NULL);
    }
    else{
        // add as first
        if(time < first->c_time){
            TransferData* new_data = new TransferData;
            setData(&new_data, start_, end_, type, num, idList_, time, first);
            first = new_data;
            new_data = NULL;
            return;
        }
        // add at other position
        TransferData* current = find_prior(time);
        TransferData* new_data = new TransferData;
        setData(&new_data, start_, end_, type, num, idList_, time, NULL);
        TransferData* temp = current->next;
        new_data->next = temp;
        current->next = new_data;
        temp = NULL;
        new_data = NULL;
    }
}

void TransferList::pop(){
    if(first == NULL)
        return;

    TransferData* temp = first;
    first = first->next;
    delete temp;
    temp = NULL;
}

void TransferList::setData(TransferData** current, int start_, int end_, string type,
                           int num, int* idList_, int time, TransferData* next){
    (*current)->start = start_;
    (*current)->end = end_;
    (*current)->bike_type = type;
    (*current)->bike_num = num;
    (*current)->idList = new int[num];
    for(int i=0; i < num; i++)
        (*current)->idList[i] = idList_[i];
    (*current)->c_time = time;
    (*current)->next = next;
}

TransferData* TransferList::find_prior(int time){
    TransferData* current = first;
    bool comparing = true;
    while(comparing && current->next!=NULL){
        if(time > current->next->c_time)
            current = current->next;
        else
            comparing=false;
    }
    return current;
}

int TransferList::searchTransfer(int station, string b_type){
    if(first==NULL)
        return -1;

    TransferData* current = first;
    while(current!=NULL){
        if( (station == current->end) && (b_type == current->bike_type) )
            return current->c_time;
        current = current->next;
    }
    return -1;    
}

void TransferList::print_list(){
    cout << "Show Transfer list :" <<endl;
    if(first==NULL){
        cout << " List is empty"<<endl;
        return;
    }

    TransferData* current = first;
    while(current!=NULL){
        cout << " " << current->start << ", " << current->end << ", "
             << current->bike_type << ", " << current->bike_num << endl;
        current = current->next;
    }
}

/* -------------------------------------------------------------------------- */
FakeReturnList::FakeReturnList(){
    first = NULL;
}

void FakeReturnList::push(int user, int return_station, string type, int ID, int time){
    if(first==NULL){
        first = new FakeReturnData;
        first->userID = user;
        first->r_station = return_station;
        first->bike_type = type;
        first->bikeID = ID;
        first->c_time = time;
        first->next = NULL;
    }
    else{
        // add as first
        if(time < first->c_time){
            FakeReturnData *new_data = new FakeReturnData;
            new_data->userID = user;
            new_data->r_station = return_station;
            new_data->bike_type = type;
            new_data->bikeID = ID;
            new_data->c_time = time;
            new_data->next = first;
            first = new_data;
            new_data = NULL;
            return;
        }
        // add at other position
        FakeReturnData* current = find_prior(time);
        FakeReturnData* new_data = new FakeReturnData;
        new_data->userID = user;
        new_data->r_station = return_station;
        new_data->bike_type = type;
        new_data->bikeID = ID;
        new_data->c_time = time;
        FakeReturnData* temp = current->next;
        new_data->next = temp;
        current->next = new_data;
        temp = NULL;
        new_data = NULL;
    }
}

void FakeReturnList::pop(){
    if(first == NULL)
        return;

    FakeReturnData* temp = first;
    first = first->next;
    delete temp;
    temp = NULL;
}

FakeReturnData* FakeReturnList::find_prior(int time){
    FakeReturnData* current = first;
    bool comparing = true;
    while(comparing && current->next!=NULL){
        if(time > current->next->c_time)
            current = current->next;
        else
            comparing=false;
    }
    return current;
}