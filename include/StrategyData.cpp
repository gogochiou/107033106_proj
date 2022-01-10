#include "./StrategyData.h"

StrategyData::StrategyData(int station_num){
    s_quantity = station_num;
    s_futureList = new StationFutureData[s_quantity];

    for(int i=0; i < s_quantity; i++){
        s_futureList[i].elec_fd.rent_num = 0;
        s_futureList[i].elec_fd.return_num = 0;
        s_futureList[i].elec_fd.d_return_num = 0;
        s_futureList[i].elec_fd.reject_num = 0;
        s_futureList[i].elec_fd.limit_rent = 0;
        s_futureList[i].elec_fd.Reject_Rent = 0.0;

        s_futureList[i].lady_fd.rent_num = 0;
        s_futureList[i].lady_fd.return_num = 0;
        s_futureList[i].lady_fd.d_return_num = 0;
        s_futureList[i].lady_fd.reject_num = 0;
        s_futureList[i].lady_fd.limit_rent = 0;
        s_futureList[i].lady_fd.Reject_Rent = 0.0;

        s_futureList[i].road_fd.rent_num = 0;
        s_futureList[i].road_fd.return_num = 0;
        s_futureList[i].road_fd.d_return_num = 0;
        s_futureList[i].road_fd.reject_num = 0;
        s_futureList[i].road_fd.limit_rent = 0;
        s_futureList[i].road_fd.Reject_Rent = 0.0;
    }
}

void StrategyData::writedata(int station, string bike_type, string command, int add){
    if(bike_type == "electric"){
        if(command == "accept")
            s_futureList[station-1].elec_fd.rent_num += add;
        else if(command == "reject")
            s_futureList[station-1].elec_fd.reject_num += add;
        else if(command == "return")
            s_futureList[station-1].elec_fd.return_num += add;
        else if(command == "d_return"){
            s_futureList[station-1].elec_fd.d_return_num += add;
        }
        else if(command == "limit_rent")
            s_futureList[station-1].elec_fd.limit_rent += add;

        s_futureList[station-1].elec_fd.Reject_Rent = 
            float(s_futureList[station-1].elec_fd.rent_num) /
            float(s_futureList[station-1].elec_fd.reject_num);
    }
    else if(bike_type == "lady"){
        if(command == "accept")
            s_futureList[station-1].lady_fd.rent_num += add;
        else if(command == "reject")
            s_futureList[station-1].lady_fd.reject_num += add;
        else if(command == "return")
            s_futureList[station-1].lady_fd.return_num += add;
        else if(command == "d_return")
            s_futureList[station-1].lady_fd.d_return_num += add;
        else if(command == "limit_rent")
            s_futureList[station-1].lady_fd.limit_rent += add;

        s_futureList[station-1].lady_fd.Reject_Rent = 
            float(s_futureList[station-1].lady_fd.rent_num) /
            float(s_futureList[station-1].lady_fd.reject_num);
    }
    else if(bike_type == "road"){
        if(command == "accept")
            s_futureList[station-1].road_fd.rent_num += add;
        else if(command == "reject")
            s_futureList[station-1].road_fd.reject_num += add;
        else if(command == "return")
            s_futureList[station-1].road_fd.return_num += add;
        else if(command == "d_return")
            s_futureList[station-1].road_fd.d_return_num += add;
        else if(command == "limit_rent")
            s_futureList[station-1].road_fd.limit_rent += add;

        s_futureList[station-1].road_fd.Reject_Rent = 
            float(s_futureList[station-1].road_fd.rent_num) /
            float(s_futureList[station-1].road_fd.reject_num);
    }
}

void StrategyData::showdata(){
    for(int i=0; i < s_quantity; i++){
        cout << "Station " << i+1 << endl;
        cout << " ELEC : "
             << s_futureList[i].elec_fd.rent_num << ", "
             << s_futureList[i].elec_fd.return_num << ", "
             << s_futureList[i].elec_fd.d_return_num << ", "
             << s_futureList[i].elec_fd.reject_num << ", "
             << s_futureList[i].elec_fd.limit_rent << endl;
        cout << " LADY : "
             << s_futureList[i].lady_fd.rent_num << ", "
             << s_futureList[i].lady_fd.return_num << ", "
             << s_futureList[i].lady_fd.d_return_num << ", "
             << s_futureList[i].lady_fd.reject_num << ", "
             << s_futureList[i].lady_fd.limit_rent << endl;
        cout << " ROAD : "
             << s_futureList[i].road_fd.rent_num << ", "
             << s_futureList[i].road_fd.return_num << ", "
             << s_futureList[i].road_fd.d_return_num << ", "
             << s_futureList[i].road_fd.reject_num << ", "
             << s_futureList[i].road_fd.limit_rent << endl;
    }
}