#include <iostream>
#include <string>
#include <ctime>
#include "./include/Manager.h"

using namespace std;

int main(){
    clock_t start,end;
    start = clock();
    Manager manager;
    end = clock();
    cout << "Build : " << (double)(end - start)/CLOCKS_PER_SEC * 1000.0 << "ms." << endl;

    //work
    start = clock();
    manager.NormalPolicy();
    end = clock();
    cout << "Manager1 : " << (double)(end - start)/CLOCKS_PER_SEC * 1000.0 << "ms." << endl;

    start = clock();
    manager.AdvancedPolicy();
    end = clock();
    cout << "Manager2 : " << (double)(end - start)/CLOCKS_PER_SEC * 1000.0 << "ms." << endl;
    
    return 0;
}